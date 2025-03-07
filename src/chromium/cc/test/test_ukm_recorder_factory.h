// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_TEST_UKM_RECORDER_FACTORY_H_
#define CC_TEST_TEST_UKM_RECORDER_FACTORY_H_

#if defined(ENABLE_UKM)
#include "cc/trees/ukm_manager.h"

namespace cc {

class TestUkmRecorderFactory : public UkmRecorderFactory {
 public:
  ~TestUkmRecorderFactory() override;

  std::unique_ptr<ukm::UkmRecorder> CreateRecorder() override;
};

}  // namespace cc
#endif // ENABLE_UKM

#endif  // CC_TEST_TEST_UKM_RECORDER_FACTORY_H_
