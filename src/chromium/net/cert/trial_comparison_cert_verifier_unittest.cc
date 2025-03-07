// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/cert/trial_comparison_cert_verifier.h"

#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/test/metrics/histogram_tester.h"
#include "base/threading/sequenced_task_runner_handle.h"
#include "build/build_config.h"
#include "crypto/sha2.h"
#include "net/base/test_completion_callback.h"
#include "net/cert/cert_verify_proc.h"
#include "net/cert/cert_verify_result.h"
#include "net/cert/ev_root_ca_metadata.h"
#include "net/cert/x509_certificate.h"
#include "net/cert/x509_util.h"
#include "net/log/net_log_with_source.h"
#include "net/test/cert_test_util.h"
#include "net/test/gtest_util.h"
#include "net/test/test_data_directory.h"
#include "net/test/test_with_scoped_task_environment.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using net::test::IsError;
using net::test::IsOk;
using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace net {

namespace {

MATCHER_P(CertChainMatches, expected_cert, "") {
  CertificateList actual_certs =
      X509Certificate::CreateCertificateListFromBytes(
          arg.data(), arg.size(), X509Certificate::FORMAT_PEM_CERT_SEQUENCE);
  if (actual_certs.empty()) {
    *result_listener << "failed to parse arg";
    return false;
  }
  std::vector<std::string> actual_der_certs;
  for (const auto& cert : actual_certs) {
    actual_der_certs.emplace_back(
        x509_util::CryptoBufferAsStringPiece(cert->cert_buffer()));
  }

  std::vector<std::string> expected_der_certs;
  expected_der_certs.emplace_back(
      x509_util::CryptoBufferAsStringPiece(expected_cert->cert_buffer()));
  for (const auto& buffer : expected_cert->intermediate_buffers()) {
    expected_der_certs.emplace_back(
        x509_util::CryptoBufferAsStringPiece(buffer.get()));
  }

  return actual_der_certs == expected_der_certs;
}

// Like TestClosure, but handles multiple closure.Run()/WaitForResult()
// calls correctly regardless of ordering.
class RepeatedTestClosure {
 public:
  RepeatedTestClosure()
      : closure_(base::BindRepeating(&RepeatedTestClosure::DidSetResult,
                                     base::Unretained(this))) {}

  const base::RepeatingClosure& closure() const { return closure_; }

  void WaitForResult() {
    DCHECK(!run_loop_);
    if (!have_result_) {
      run_loop_.reset(new base::RunLoop());
      run_loop_->Run();
      run_loop_.reset();
      DCHECK(have_result_);
    }
    have_result_--;  // Auto-reset for next callback.
  }

 private:
  void DidSetResult() {
    have_result_++;
    if (run_loop_)
      run_loop_->Quit();
  }

  // RunLoop.  Only non-NULL during the call to WaitForResult, so the class is
  // reusable.
  std::unique_ptr<base::RunLoop> run_loop_;

  unsigned int have_result_ = 0;

  base::RepeatingClosure closure_;
};

// Fake CertVerifyProc that sets the CertVerifyResult to a given value for
// all certificates that are Verify()'d
class FakeCertVerifyProc : public CertVerifyProc {
 public:
  FakeCertVerifyProc(const int result_error, const CertVerifyResult& result)
      : result_error_(result_error),
        result_(result),
        main_task_runner_(base::SequencedTaskRunnerHandle::Get()) {}

  void WaitForVerifyCall() { verify_called_.WaitForResult(); }

  // CertVerifyProc implementation:
  bool SupportsAdditionalTrustAnchors() const override { return false; }

 protected:
  ~FakeCertVerifyProc() override = default;

 private:
  int VerifyInternal(X509Certificate* cert,
                     const std::string& hostname,
                     const std::string& ocsp_response,
                     int flags,
                     CRLSet* crl_set,
                     const CertificateList& additional_trust_anchors,
                     CertVerifyResult* verify_result) override;

  const int result_error_;
  const CertVerifyResult result_;
  RepeatedTestClosure verify_called_;
  scoped_refptr<base::TaskRunner> main_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(FakeCertVerifyProc);
};

int FakeCertVerifyProc::VerifyInternal(
    X509Certificate* cert,
    const std::string& hostname,
    const std::string& ocsp_response,
    int flags,
    CRLSet* crl_set,
    const CertificateList& additional_trust_anchors,
    CertVerifyResult* verify_result) {
  *verify_result = result_;
  main_task_runner_->PostTask(FROM_HERE, verify_called_.closure());
  return result_error_;
}

// Fake CertVerifyProc that causes a failure if it is called.
class NotCalledCertVerifyProc : public CertVerifyProc {
 public:
  NotCalledCertVerifyProc() = default;

  // CertVerifyProc implementation:
  bool SupportsAdditionalTrustAnchors() const override { return false; }

 protected:
  ~NotCalledCertVerifyProc() override = default;

 private:
  int VerifyInternal(X509Certificate* cert,
                     const std::string& hostname,
                     const std::string& ocsp_response,
                     int flags,
                     CRLSet* crl_set,
                     const CertificateList& additional_trust_anchors,
                     CertVerifyResult* verify_result) override;

  DISALLOW_COPY_AND_ASSIGN(NotCalledCertVerifyProc);
};

int NotCalledCertVerifyProc::VerifyInternal(
    X509Certificate* cert,
    const std::string& hostname,
    const std::string& ocsp_response,
    int flags,
    CRLSet* crl_set,
    const CertificateList& additional_trust_anchors,
    CertVerifyResult* verify_result) {
  ADD_FAILURE() << "NotCalledCertVerifyProc was called!";
  return ERR_UNEXPECTED;
}

void NotCalledCallback(int error) {
  ADD_FAILURE() << "NotCalledCallback was called with error code " << error;
}

class MockCertVerifyProc : public CertVerifyProc {
 public:
  MockCertVerifyProc() = default;
  // CertVerifyProc implementation:
  bool SupportsAdditionalTrustAnchors() const override { return false; }
  MOCK_METHOD7(VerifyInternal,
               int(X509Certificate* cert,
                   const std::string& hostname,
                   const std::string& ocsp_response,
                   int flags,
                   CRLSet* crl_set,
                   const CertificateList& additional_trust_anchors,
                   CertVerifyResult* verify_result));

 protected:
  ~MockCertVerifyProc() override = default;

  DISALLOW_COPY_AND_ASSIGN(MockCertVerifyProc);
};

struct TrialReportInfo {
  TrialReportInfo(const std::string& hostname,
                  const scoped_refptr<X509Certificate>& unverified_cert,
                  bool enable_rev_checking,
                  bool require_rev_checking_local_anchors,
                  bool enable_sha1_local_anchors,
                  bool disable_symantec_enforcement,
                  const CertVerifyResult& primary_result,
                  const CertVerifyResult& trial_result)
      : hostname(hostname),
        unverified_cert(unverified_cert),
        enable_rev_checking(enable_rev_checking),
        require_rev_checking_local_anchors(require_rev_checking_local_anchors),
        enable_sha1_local_anchors(enable_sha1_local_anchors),
        disable_symantec_enforcement(disable_symantec_enforcement),
        primary_result(primary_result),
        trial_result(trial_result) {}

  std::string hostname;
  scoped_refptr<X509Certificate> unverified_cert;
  bool enable_rev_checking;
  bool require_rev_checking_local_anchors;
  bool enable_sha1_local_anchors;
  bool disable_symantec_enforcement;
  CertVerifyResult primary_result;
  CertVerifyResult trial_result;
};

void RecordTrialReport(std::vector<TrialReportInfo>* reports,
                       const std::string& hostname,
                       const scoped_refptr<X509Certificate>& unverified_cert,
                       bool enable_rev_checking,
                       bool require_rev_checking_local_anchors,
                       bool enable_sha1_local_anchors,
                       bool disable_symantec_enforcement,
                       const CertVerifyResult& primary_result,
                       const CertVerifyResult& trial_result) {
  TrialReportInfo report(
      hostname, unverified_cert, enable_rev_checking,
      require_rev_checking_local_anchors, enable_sha1_local_anchors,
      disable_symantec_enforcement, primary_result, trial_result);
  reports->push_back(report);
}

}  // namespace

class TrialComparisonCertVerifierTest : public TestWithScopedTaskEnvironment {
  void SetUp() override {
    cert_chain_1_ = CreateCertificateChainFromFile(
        GetTestCertsDirectory(), "multi-root-chain1.pem",
        X509Certificate::FORMAT_AUTO);
    ASSERT_TRUE(cert_chain_1_);
    leaf_cert_1_ = X509Certificate::CreateFromBuffer(
        bssl::UpRef(cert_chain_1_->cert_buffer()), {});
    ASSERT_TRUE(leaf_cert_1_);
    cert_chain_2_ = CreateCertificateChainFromFile(
        GetTestCertsDirectory(), "multi-root-chain2.pem",
        X509Certificate::FORMAT_AUTO);
    ASSERT_TRUE(cert_chain_2_);
  }

 protected:
  scoped_refptr<X509Certificate> cert_chain_1_;
  scoped_refptr<X509Certificate> cert_chain_2_;
  scoped_refptr<X509Certificate> leaf_cert_1_;
  base::HistogramTester histograms_;
};

TEST_F(TrialComparisonCertVerifierTest, InitiallyDisallowed) {
  CertVerifyResult dummy_result;
  dummy_result.verified_cert = cert_chain_1_;

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      false /* initial_allowed */,
      base::MakeRefCounted<FakeCertVerifyProc>(OK, dummy_result),
      base::MakeRefCounted<NotCalledCertVerifyProc>(),
      base::BindRepeating(&RecordTrialReport, &reports));
  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  // Primary verifier should have ran, trial verifier should not have.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 0);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               0);
  histograms_.ExpectTotalCount("Net.CertVerifier_TrialComparisonResult", 0);
}

TEST_F(TrialComparisonCertVerifierTest, InitiallyDisallowedThenAllowed) {
  // Certificate that has multiple subjectAltName entries. This allows easily
  // confirming which verification attempt the report was generated for without
  // having to mock different CertVerifyProc results for each.
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("verify_certificate_chain_unittest")
          .AppendASCII("many-names");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "ok-all-types.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);
  ASSERT_EQ(2U, cert_chain->intermediate_buffers().size());

  scoped_refptr<X509Certificate> leaf = X509Certificate::CreateFromBuffer(
      bssl::UpRef(cert_chain->cert_buffer()), {});
  ASSERT_TRUE(leaf);

  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier returns an error status.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_DATE_INVALID;
  secondary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      false /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf, "t0.test", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);
  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  // Enable the trial and do another verification.
  verifier.set_trial_allowed(true);
  CertVerifier::RequestParams params2(leaf, "t1.test", 0 /* flags */,
                                      std::string() /* ocsp_response */);
  CertVerifyResult result2;
  TestCompletionCallback callback2;
  std::unique_ptr<CertVerifier::Request> request2;
  error = verifier.Verify(params2, &result2, callback2.callback(), &request2,
                          NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request2);

  error = callback2.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Primary verifier should have run twice, trial verifier should run once.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 2);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryValidSecondaryError, 1);

  // Expect a report from the second verification.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];
  EXPECT_EQ("t1.test", report.hostname);
}

TEST_F(TrialComparisonCertVerifierTest, InitiallyAllowedThenDisallowed) {
  // Certificate that has multiple subjectAltName entries. This allows easily
  // confirming which verification attempt the report was generated for without
  // having to mock different CertVerifyProc results for each.
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("verify_certificate_chain_unittest")
          .AppendASCII("many-names");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "ok-all-types.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);
  ASSERT_EQ(2U, cert_chain->intermediate_buffers().size());

  scoped_refptr<X509Certificate> leaf = X509Certificate::CreateFromBuffer(
      bssl::UpRef(cert_chain->cert_buffer()), {});
  ASSERT_TRUE(leaf);

  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier returns an error status.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_DATE_INVALID;
  secondary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf, "t0.test", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);
  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  // Disable the trial and do another verification.
  verifier.set_trial_allowed(false);
  CertVerifier::RequestParams params2(leaf, "t1.test", 0 /* flags */,
                                      std::string() /* ocsp_response */);
  CertVerifyResult result2;
  TestCompletionCallback callback2;
  std::unique_ptr<CertVerifier::Request> request2;
  error = verifier.Verify(params2, &result2, callback2.callback(), &request2,
                          NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request2);

  error = callback2.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Primary verifier should have run twice, trial verifier should run once.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 2);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryValidSecondaryError, 1);

  // Expect a report from the first verification.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];
  EXPECT_EQ("t0.test", report.hostname);
}

TEST_F(TrialComparisonCertVerifierTest,
       ConfigChangedDuringPrimaryVerification) {
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1,
      base::MakeRefCounted<NotCalledCertVerifyProc>(),
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  // Change the verifier config before the primary verification finishes.
  CertVerifier::Config config;
  config.enable_sha1_local_anchors = true;
  verifier.SetConfig(config);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  RunUntilIdle();

  // Since the config changed, trial verifier should not run.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 0);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               0);
  histograms_.ExpectTotalCount("Net.CertVerifier_TrialComparisonResult", 0);

  // Expect no report.
  EXPECT_TRUE(reports.empty());
}

TEST_F(TrialComparisonCertVerifierTest, ConfigChangedDuringTrialVerification) {
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier returns an error status.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_DATE_INVALID;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  // Change the verifier config during the trial verification.
  CertVerifier::Config config;
  config.enable_sha1_local_anchors = true;
  verifier.SetConfig(config);

  RunUntilIdle();

  // Since the config was the same when both primary and trial verification
  // started, the result should still be reported.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  // CertVerifier_Job_Latency_TrialSecondary is not recorded due to
  // MultiThreadedCertVerifier's config_id_ check before calling the
  // verify_complete_callback_.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               0);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryValidSecondaryError, 1);

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(0U, report.primary_result.cert_status);
  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.trial_result.cert_status);
}

TEST_F(TrialComparisonCertVerifierTest, SameResult) {
  CertVerifyResult dummy_result;
  dummy_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, dummy_result);
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, dummy_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample("Net.CertVerifier_TrialComparisonResult",
                                 TrialComparisonCertVerifier::kEqual, 1);
}

TEST_F(TrialComparisonCertVerifierTest, PrimaryVerifierErrorSecondaryOk) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status = CERT_STATUS_DATE_INVALID;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               primary_result);

  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_DATE_INVALID));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.primary_result.cert_status);
  EXPECT_EQ(0U, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  EXPECT_FALSE(report.enable_rev_checking);
  EXPECT_FALSE(report.require_rev_checking_local_anchors);
  EXPECT_FALSE(report.enable_sha1_local_anchors);
  EXPECT_FALSE(report.disable_symantec_enforcement);

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryErrorSecondaryValid, 1);
}

TEST_F(TrialComparisonCertVerifierTest, PrimaryVerifierOkSecondaryError) {
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier returns an error status.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_DATE_INVALID;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(0U, report.primary_result.cert_status);
  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryValidSecondaryError, 1);
}

TEST_F(TrialComparisonCertVerifierTest, BothVerifiersDifferentErrors) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.cert_status = CERT_STATUS_VALIDITY_TOO_LONG;
  primary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_VALIDITY_TOO_LONG,
                                               primary_result);

  // Trial verifier returns a different error status.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_DATE_INVALID;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_VALIDITY_TOO_LONG));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(CERT_STATUS_VALIDITY_TOO_LONG, report.primary_result.cert_status);
  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kBothErrorDifferentDetails, 1);
}

TEST_F(TrialComparisonCertVerifierTest,
       BothVerifiersOkDifferentVerifiedChains) {
  // Primary verifier returns chain1 regardless of arguments.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier returns a different verified cert chain.
  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_2_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(0U, report.primary_result.cert_status);
  EXPECT_EQ(0U, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_2_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  // Main CertVerifier_Job_Latency should have 2 counts since the
  // primary_reverifier was used.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 2);
  // CertVerifier_Job_Latency_TrialPrimary only has 1 count since
  // primary_reverifier doesn't use the same CertVerifier.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kBothValidDifferentDetails, 1);
}

TEST_F(TrialComparisonCertVerifierTest,
       BothVerifiersOkDifferentVerifiedChainsEqualAfterReverification) {
  CertVerifyResult chain1_result;
  chain1_result.verified_cert = cert_chain_1_;
  CertVerifyResult chain2_result;
  chain2_result.verified_cert = cert_chain_2_;

  scoped_refptr<MockCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<MockCertVerifyProc>();
  // Primary verifier returns ok status and chain1 if verifying the leaf alone.
  EXPECT_CALL(*verify_proc1,
              VerifyInternal(leaf_cert_1_.get(), _, _, _, _, _, _))
      .WillRepeatedly(DoAll(SetArgPointee<6>(chain1_result), Return(OK)));
  // Primary verifier returns ok status and chain2 if verifying chain2.
  EXPECT_CALL(*verify_proc1,
              VerifyInternal(cert_chain_2_.get(), _, _, _, _, _, _))
      .WillRepeatedly(DoAll(SetArgPointee<6>(chain2_result), Return(OK)));

  // Trial verifier returns ok status and chain2.
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, chain2_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  // Main CertVerifier_Job_Latency should have 2 counts since the
  // primary_reverifier was used.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 2);
  // CertVerifier_Job_Latency_TrialPrimary only has 1 count since
  // primary_reverifier doesn't use the same CertVerifier.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kIgnoredDifferentPathReVerifiesEquivalent,
      1);
}

TEST_F(TrialComparisonCertVerifierTest,
       DifferentVerifiedChainsIgnorableDifferenceAfterReverification) {
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("trial_comparison_cert_verifier_unittest")
          .AppendASCII("target-multiple-policies");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "chain.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);
  ASSERT_EQ(2U, cert_chain->intermediate_buffers().size());

  scoped_refptr<X509Certificate> leaf = X509Certificate::CreateFromBuffer(
      bssl::UpRef(cert_chain->cert_buffer()), {});
  ASSERT_TRUE(leaf);

  // Chain with the same leaf and different root. This is not a valid chain, but
  // doesn't matter for the unittest since this uses mock CertVerifyProcs.
  std::vector<bssl::UniquePtr<CRYPTO_BUFFER>> intermediates;
  intermediates.push_back(
      bssl::UpRef(cert_chain_1_->intermediate_buffers().back().get()));
  scoped_refptr<X509Certificate> different_chain =
      X509Certificate::CreateFromBuffer(bssl::UpRef(cert_chain->cert_buffer()),
                                        std::move(intermediates));
  ASSERT_TRUE(different_chain);

  CertVerifyResult different_chain_result;
  different_chain_result.verified_cert = different_chain;

  CertVerifyResult nonev_chain_result;
  nonev_chain_result.verified_cert = cert_chain;

  CertVerifyResult ev_chain_result;
  ev_chain_result.verified_cert = cert_chain;
  ev_chain_result.cert_status =
      CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED;

  SHA256HashValue root_fingerprint;
  crypto::SHA256HashString(x509_util::CryptoBufferAsStringPiece(
                               cert_chain->intermediate_buffers().back().get()),
                           root_fingerprint.data,
                           sizeof(root_fingerprint.data));
  // Both policies in the target are EV policies, but only 1.2.6.7 is valid for
  // the root in cert_chain.
  ScopedTestEVPolicy scoped_ev_policy_1(EVRootCAMetadata::GetInstance(),
                                        root_fingerprint, "1.2.6.7");
  ScopedTestEVPolicy scoped_ev_policy_2(EVRootCAMetadata::GetInstance(),
                                        SHA256HashValue(), "1.2.3.4");

  scoped_refptr<MockCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<MockCertVerifyProc>();
  // Primary verifier returns ok status and different_chain if verifying leaf
  // alone.
  EXPECT_CALL(*verify_proc1, VerifyInternal(leaf.get(), _, _, _, _, _, _))
      .WillRepeatedly(
          DoAll(SetArgPointee<6>(different_chain_result), Return(OK)));
  // Primary verifier returns ok status and nonev_chain_result if verifying
  // cert_chain.
  EXPECT_CALL(*verify_proc1, VerifyInternal(cert_chain.get(), _, _, _, _, _, _))
      .WillRepeatedly(DoAll(SetArgPointee<6>(nonev_chain_result), Return(OK)));

  // Trial verifier returns ok status and ev_chain_result.
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, ev_chain_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf, "test.example", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  // Main CertVerifier_Job_Latency should have 2 counts since the
  // primary_reverifier was used.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 2);
  // CertVerifier_Job_Latency_TrialPrimary only has 1 count since
  // primary_reverifier doesn't use the same CertVerifier.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kIgnoredDifferentPathReVerifiesEquivalent,
      1);
}

TEST_F(TrialComparisonCertVerifierTest, BothVerifiersOkDifferentCertStatus) {
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status =
      CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_1_;
  secondary_result.cert_status = CERT_STATUS_CT_COMPLIANCE_FAILED;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::Config config;
  config.enable_rev_checking = true;
  config.enable_sha1_local_anchors = true;
  verifier.SetConfig(config);

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED,
            report.primary_result.cert_status);
  EXPECT_EQ(CERT_STATUS_CT_COMPLIANCE_FAILED, report.trial_result.cert_status);

  EXPECT_TRUE(report.enable_rev_checking);
  EXPECT_FALSE(report.require_rev_checking_local_anchors);
  EXPECT_TRUE(report.enable_sha1_local_anchors);
  EXPECT_FALSE(report.disable_symantec_enforcement);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kBothValidDifferentDetails, 1);
}

TEST_F(TrialComparisonCertVerifierTest, Coalescing) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status = CERT_STATUS_DATE_INVALID;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               primary_result);

  // Trial verifier has ok status.
  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);

  // Start first verification request.
  CertVerifyResult result_1;
  std::unique_ptr<CertVerifier::Request> request_1;
  TestCompletionCallback callback_1;
  int error = verifier.Verify(params, &result_1, callback_1.callback(),
                              &request_1, NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request_1);

  // Start second verification request with same params.
  CertVerifyResult result_2;
  std::unique_ptr<CertVerifier::Request> request_2;
  TestCompletionCallback callback_2;
  error = verifier.Verify(params, &result_2, callback_2.callback(), &request_2,
                          NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request_2);

  // Both callbacks should be called with same error code.
  error = callback_1.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_DATE_INVALID));
  error = callback_2.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_DATE_INVALID));

  // Trial verifier should run.
  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a single report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.primary_result.cert_status);
  EXPECT_EQ(0U, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  // Only one verification should be done by primary verifier.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  // Only one verification should be done by secondary verifier.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryErrorSecondaryValid, 1);
}

TEST_F(TrialComparisonCertVerifierTest, CancelledDuringPrimaryVerification) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status = CERT_STATUS_DATE_INVALID;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               primary_result);

  // Trial verifier has ok status.
  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  std::unique_ptr<CertVerifier::Request> request;
  int error =
      verifier.Verify(params, &result, base::BindRepeating(&NotCalledCallback),
                      &request, NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  // Delete the request, cancelling it.
  request.reset();

  // The callback to the main verifier does not run. However, the verification
  // still completes in the background and triggers the trial verification.

  // Trial verifier should still run.
  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());
  const TrialReportInfo& report = reports[0];

  EXPECT_EQ(CERT_STATUS_DATE_INVALID, report.primary_result.cert_status);
  EXPECT_EQ(0U, report.trial_result.cert_status);

  EXPECT_TRUE(report.primary_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.trial_result.verified_cert->EqualsIncludingChain(
      cert_chain_1_.get()));
  EXPECT_TRUE(report.unverified_cert->EqualsIncludingChain(leaf_cert_1_.get()));

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryErrorSecondaryValid, 1);
}

TEST_F(TrialComparisonCertVerifierTest, DeletedDuringPrimaryVerification) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status = CERT_STATUS_DATE_INVALID;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               primary_result);

  std::vector<TrialReportInfo> reports;
  auto verifier = std::make_unique<TrialComparisonCertVerifier>(
      true /* initial_allowed */, verify_proc1,
      base::MakeRefCounted<NotCalledCertVerifyProc>(),
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  std::unique_ptr<CertVerifier::Request> request;
  int error =
      verifier->Verify(params, &result, base::BindRepeating(&NotCalledCallback),
                       &request, NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  // Delete the TrialComparisonCertVerifier.
  verifier.reset();

  // The callback to the main verifier does not run. The verification task
  // still completes in the background, but since the CertVerifier has been
  // deleted, the result is ignored.

  // Wait for any tasks to finish.
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  // Histograms should not be recorded.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 0);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 0);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               0);
  histograms_.ExpectTotalCount("Net.CertVerifier_TrialComparisonResult", 0);
}

TEST_F(TrialComparisonCertVerifierTest, DeletedAfterTrialVerificationStarted) {
  // Primary verifier returns an error status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain_1_;
  primary_result.cert_status = CERT_STATUS_DATE_INVALID;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_DATE_INVALID,
                                               primary_result);

  // Trial verifier has ok status.
  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  auto verifier = std::make_unique<TrialComparisonCertVerifier>(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier->Verify(params, &result, callback.callback(), &request,
                               NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  // Wait for primary verifier to finish.
  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_DATE_INVALID));

  // Delete the TrialComparisonCertVerifier. The trial verification is still
  // running on the task scheduler (or, depending on timing, has posted back
  // to the IO thread after the Quit event).
  verifier.reset();

  // The callback to the trial verifier does not run. The verification task
  // still completes in the background, but since the CertVerifier has been
  // deleted, the result is ignored.

  // Wait for any tasks to finish.
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  // Histograms for trial verifier should not be recorded.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               0);
  histograms_.ExpectTotalCount("Net.CertVerifier_TrialComparisonResult", 0);
}

TEST_F(TrialComparisonCertVerifierTest, MacUndesiredRevocationChecking) {
  CertVerifyResult revoked_result;
  revoked_result.verified_cert = cert_chain_1_;
  revoked_result.cert_status = CERT_STATUS_REVOKED;

  CertVerifyResult ok_result;
  ok_result.verified_cert = cert_chain_1_;

  // Primary verifier returns an error status.
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_REVOKED,
                                               revoked_result);

  scoped_refptr<MockCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<MockCertVerifyProc>();
  // Secondary verifier returns ok status...
  EXPECT_CALL(*verify_proc2, VerifyInternal(_, _, _, _, _, _, _))
      .WillRepeatedly(DoAll(SetArgPointee<6>(ok_result), Return(OK)));
  // ...unless it was called with REV_CHECKING_ENABLED.
  EXPECT_CALL(
      *verify_proc2,
      VerifyInternal(_, _, _, CertVerifyProc::VERIFY_REV_CHECKING_ENABLED, _, _,
                     _))
      .WillRepeatedly(
          DoAll(SetArgPointee<6>(revoked_result), Return(ERR_CERT_REVOKED)));

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_REVOKED));

  RunUntilIdle();

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
#if defined(OS_MACOSX)
  // Expect no report.
  EXPECT_EQ(0U, reports.size());

  // Secondary should have been called twice
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               2);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kIgnoredMacUndesiredRevocationChecking, 1);
#else
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryErrorSecondaryValid, 1);

  // Expect a report.
  EXPECT_EQ(1U, reports.size());
#endif
}

TEST_F(TrialComparisonCertVerifierTest, PrimaryRevokedSecondaryOk) {
  CertVerifyResult revoked_result;
  revoked_result.verified_cert = cert_chain_1_;
  revoked_result.cert_status = CERT_STATUS_REVOKED;

  CertVerifyResult ok_result;
  ok_result.verified_cert = cert_chain_1_;

  // Primary verifier returns an error status.
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_REVOKED,
                                               revoked_result);

  // Secondary verifier returns ok status regardless of whether
  // REV_CHECKING_ENABLED was passed.
  scoped_refptr<MockCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<MockCertVerifyProc>();
  EXPECT_CALL(*verify_proc2, VerifyInternal(_, _, _, _, _, _, _))
      .WillRepeatedly(DoAll(SetArgPointee<6>(ok_result), Return(OK)));

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(ERR_CERT_REVOKED));

  RunUntilIdle();

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
#if defined(OS_MACOSX)
  // Secondary should have been called twice on mac due to attempting the
  // kIgnoredMacUndesiredRevocationChecking workaround.
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               2);
#else
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);

#endif
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kPrimaryErrorSecondaryValid, 1);

  // Expect a report.
  EXPECT_EQ(1U, reports.size());
}

TEST_F(TrialComparisonCertVerifierTest, MultipleEVPolicies) {
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("trial_comparison_cert_verifier_unittest")
          .AppendASCII("target-multiple-policies");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "chain.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);
  ASSERT_EQ(2U, cert_chain->intermediate_buffers().size());

  SHA256HashValue root_fingerprint;
  crypto::SHA256HashString(x509_util::CryptoBufferAsStringPiece(
                               cert_chain->intermediate_buffers().back().get()),
                           root_fingerprint.data,
                           sizeof(root_fingerprint.data));

  // Both policies in the target are EV policies, but only 1.2.6.7 is valid for
  // the root in this chain.
  ScopedTestEVPolicy scoped_ev_policy_1(EVRootCAMetadata::GetInstance(),
                                        root_fingerprint, "1.2.6.7");
  ScopedTestEVPolicy scoped_ev_policy_2(EVRootCAMetadata::GetInstance(),
                                        SHA256HashValue(), "1.2.3.4");

  // Both verifiers return OK, but secondary returns EV status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain;
  secondary_result.cert_status =
      CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kIgnoredMultipleEVPoliciesAndOneMatchesRoot,
      1);
}

TEST_F(TrialComparisonCertVerifierTest, MultipleEVPoliciesNoneValidForRoot) {
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("trial_comparison_cert_verifier_unittest")
          .AppendASCII("target-multiple-policies");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "chain.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);

  // Both policies in the target are EV policies, but neither is valid for the
  // root in this chain.
  ScopedTestEVPolicy scoped_ev_policy_1(EVRootCAMetadata::GetInstance(), {1},
                                        "1.2.6.7");
  ScopedTestEVPolicy scoped_ev_policy_2(EVRootCAMetadata::GetInstance(), {2},
                                        "1.2.3.4");

  // Both verifiers return OK, but secondary returns EV status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain;
  secondary_result.cert_status =
      CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kBothValidDifferentDetails, 1);
}

TEST_F(TrialComparisonCertVerifierTest, MultiplePoliciesOnlyOneIsEV) {
  base::FilePath certs_dir =
      GetTestNetDataDirectory()
          .AppendASCII("trial_comparison_cert_verifier_unittest")
          .AppendASCII("target-multiple-policies");
  scoped_refptr<X509Certificate> cert_chain = CreateCertificateChainFromFile(
      certs_dir, "chain.pem", X509Certificate::FORMAT_AUTO);
  ASSERT_TRUE(cert_chain);
  ASSERT_EQ(2U, cert_chain->intermediate_buffers().size());

  SHA256HashValue root_fingerprint;
  crypto::SHA256HashString(x509_util::CryptoBufferAsStringPiece(
                               cert_chain->intermediate_buffers().back().get()),
                           root_fingerprint.data,
                           sizeof(root_fingerprint.data));

  // One policy in the target is an EV policy and is valid for the root.
  ScopedTestEVPolicy scoped_ev_policy_1(EVRootCAMetadata::GetInstance(),
                                        root_fingerprint, "1.2.6.7");

  // Both verifiers return OK, but secondary returns EV status.
  CertVerifyResult primary_result;
  primary_result.verified_cert = cert_chain;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  CertVerifyResult secondary_result;
  secondary_result.verified_cert = cert_chain;
  secondary_result.cert_status =
      CERT_STATUS_IS_EV | CERT_STATUS_REV_CHECKING_ENABLED;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect a report.
  ASSERT_EQ(1U, reports.size());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kBothValidDifferentDetails, 1);
}

TEST_F(TrialComparisonCertVerifierTest, LocallyTrustedLeaf) {
  // Platform verifier verifies the leaf directly.
  CertVerifyResult primary_result;
  primary_result.verified_cert = leaf_cert_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc1 =
      base::MakeRefCounted<FakeCertVerifyProc>(OK, primary_result);

  // Trial verifier does not support directly-trusted leaf certs.
  CertVerifyResult secondary_result;
  secondary_result.cert_status = CERT_STATUS_AUTHORITY_INVALID;
  secondary_result.verified_cert = leaf_cert_1_;
  scoped_refptr<FakeCertVerifyProc> verify_proc2 =
      base::MakeRefCounted<FakeCertVerifyProc>(ERR_CERT_AUTHORITY_INVALID,
                                               secondary_result);

  std::vector<TrialReportInfo> reports;
  TrialComparisonCertVerifier verifier(
      true /* initial_allowed */, verify_proc1, verify_proc2,
      base::BindRepeating(&RecordTrialReport, &reports));

  CertVerifier::RequestParams params(leaf_cert_1_, "127.0.0.1", 0 /* flags */,
                                     std::string() /* ocsp_response */);
  CertVerifyResult result;
  TestCompletionCallback callback;
  std::unique_ptr<CertVerifier::Request> request;
  int error = verifier.Verify(params, &result, callback.callback(), &request,
                              NetLogWithSource());
  ASSERT_THAT(error, IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request);

  error = callback.WaitForResult();
  EXPECT_THAT(error, IsError(OK));

  verify_proc2->WaitForVerifyCall();
  RunUntilIdle();

  // Expect no report.
  EXPECT_TRUE(reports.empty());

  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialPrimary", 1);
  histograms_.ExpectTotalCount("Net.CertVerifier_Job_Latency_TrialSecondary",
                               1);
  histograms_.ExpectUniqueSample(
      "Net.CertVerifier_TrialComparisonResult",
      TrialComparisonCertVerifier::kIgnoredLocallyTrustedLeaf, 1);
}

}  // namespace net
