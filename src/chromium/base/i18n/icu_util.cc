// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/icu_util.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>

#include "base/debug/alias.h"
#include "base/files/file_path.h"
#include "base/files/memory_mapped_file.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/strings/string_util.h"
#include "base/strings/sys_string_conversions.h"
#include "build/build_config.h"
#include "third_party/icu/source/common/unicode/putil.h"
#include "third_party/icu/source/common/unicode/udata.h"
#if (defined(OS_LINUX) && !defined(OS_CHROMEOS)) || defined(OS_ANDROID) || defined(OS_EMSCRIPTEN)
#include "third_party/icu/source/i18n/unicode/timezone.h"
#include "third_party/icu/source/common/unicode/brkiter.h"
#endif

#include "unicode/calendar.h"
#include "unicode/datefmt.h"
#include "unicode/dcfmtsym.h"
#include "unicode/decimfmt.h"
#include "unicode/dtfmtsym.h"
#include "unicode/gregocal.h"
#include "unicode/locid.h"
#include "unicode/numfmt.h"
#include "unicode/strenum.h"
#include "unicode/ubrk.h"
#include "unicode/ucal.h"
#include "unicode/uclean.h"
#include "unicode/ucol.h"
#include "unicode/ucurr.h"
#include "unicode/udat.h"
#include "unicode/uloc.h"
#include "unicode/ustring.h"
#include <unicode/ucnv.h>
#include <unicode/ulocdata.h>
#include "unicode/uchar.h"
#include "unicode/utypes.h"

#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"

#if defined(OS_ANDROID)
#include "base/android/apk_assets.h"
#include "base/android/timezone_utils.h"
#endif

#if defined(OS_IOS)
#include "base/ios/ios_util.h"
#endif

#if defined(OS_MACOSX)
#include "base/mac/foundation_util.h"
#endif

#if defined(OS_FUCHSIA)
#include "base/base_paths_fuchsia.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace base {
namespace i18n {

#if ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_SHARED
#define ICU_UTIL_DATA_SYMBOL "icudt" U_ICU_VERSION_SHORT "_dat"
#if defined(OS_WIN)
#define ICU_UTIL_DATA_SHARED_MODULE_NAME "icudt.dll"
#endif
#endif

//#if defined(OS_EMSCRIPTEN) && ICU_UTIL_DATA_IMPL != ICU_UTIL_DATA_FILE
//#error "set ICU_UTIL_DATA_IMPL=ICU_UTIL_DATA_FILE on wasm"
//#endif

namespace {
#if !defined(OS_NACL)// && !defined(OS_EMSCRIPTEN)
#if DCHECK_IS_ON()
// Assert that we are not called more than once.  Even though calling this
// function isn't harmful (ICU can handle it), being called twice probably
// indicates a programming error.
bool g_check_called_once = true;
bool g_called_once = false;
#endif  // DCHECK_IS_ON()

#if ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE

// To debug http://crbug.com/445616.
int g_debug_icu_last_error;
int g_debug_icu_load;
int g_debug_icu_pf_error_details;
int g_debug_icu_pf_last_error;
#if defined(OS_WIN)
wchar_t g_debug_icu_pf_filename[_MAX_PATH];
#endif  // OS_WIN
// Use an unversioned file name to simplify a icu version update down the road.
// No need to change the filename in multiple places (gyp files, windows
// build pkg configurations, etc). 'l' stands for Little Endian.
// This variable is exported through the header file.
#if defined(OS_EMSCRIPTEN)
// see http://userguide.icu-project.org/icudata
// see http://userguide.icu-project.org/howtouseicu#TOC-C-With-Your-Own-Build-System
// see https://github.com/unicode-org/icu/blob/master/docs/userguide/icu_data/buildtool.md
const char kIcuDataFileName[] = "./resources/icu/icudtl.dat";
#elif defined(OS_LINUX)
const char kIcuDataFileName[] = "./resources/icu/icudtl.dat";
#else
const char kIcuDataFileName[] = "icudtl.dat";
#endif
#if defined(OS_ANDROID)
const char kAndroidAssetsIcuDataFileName[] = "assets/icudtl.dat";
#endif

// File handle intentionally never closed. Not using File here because its
// Windows implementation guards against two instances owning the same
// PlatformFile (which we allow since we know it is never freed).
PlatformFile g_icudtl_pf = kInvalidPlatformFile;
MemoryMappedFile* g_icudtl_mapped_file = nullptr;
MemoryMappedFile::Region g_icudtl_region;

void LazyInitIcuDataFile() {
printf("reading icu data 1...\n");
  if (g_icudtl_pf != kInvalidPlatformFile) {
    return;
  }
printf("reading icu data 1.1...\n");
#if defined(OS_ANDROID)
  int fd =
      android::OpenApkAsset(kAndroidAssetsIcuDataFileName, &g_icudtl_region);
  g_icudtl_pf = fd;
  if (fd != -1) {
    return;
  }
// For unit tests, data file is located on disk, so try there as a fallback.
#endif  // defined(OS_ANDROID)
#if !defined(OS_MACOSX)
  FilePath data_path;
#if !defined(OS_EMSCRIPTEN)
  if (!PathService::Get(DIR_ASSETS, &data_path)) {
    LOG(ERROR) << "Can't find " << kIcuDataFileName;
#if defined(OS_EMSCRIPTEN)
    DCHECK(false);
#endif
    return;
  }
#endif
printf("reading icu data 2...\n");
#if defined(OS_WIN)
  // TODO(brucedawson): http://crbug.com/445616
  wchar_t tmp_buffer[_MAX_PATH] = {0};
  wcscpy_s(tmp_buffer, as_wcstr(data_path.value()));
  debug::Alias(tmp_buffer);
#endif
  data_path = data_path.AppendASCII(kIcuDataFileName);
printf("reading icu data 2.1 %s...\n", data_path.value().c_str());

#if defined(OS_WIN)
  // TODO(brucedawson): http://crbug.com/445616
  wchar_t tmp_buffer2[_MAX_PATH] = {0};
  wcscpy_s(tmp_buffer2, as_wcstr(data_path.value()));
  debug::Alias(tmp_buffer2);
#endif

#else  // !defined(OS_MACOSX)
  // Assume it is in the framework bundle's Resources directory.
  ScopedCFTypeRef<CFStringRef> data_file_name(
      SysUTF8ToCFStringRef(kIcuDataFileName));
  FilePath data_path = mac::PathForFrameworkBundleResource(data_file_name);
#if defined(OS_IOS)
  FilePath override_data_path = ios::FilePathOfEmbeddedICU();
  if (!override_data_path.empty()) {
    data_path = override_data_path;
  }
#endif  // !defined(OS_IOS)
  if (data_path.empty()) {
    LOG(ERROR) << kIcuDataFileName << " not found in bundle";
    return;
  }
#endif  // !defined(OS_MACOSX)
printf("reading icu data 3 %s...\n", data_path.value().c_str());
  File file(data_path, File::FLAG_OPEN | File::FLAG_READ);
  if (file.IsValid()) {
    printf("reading icu data 3.1 %s...\n", data_path.value().c_str());
    // TODO(brucedawson): http://crbug.com/445616.
    g_debug_icu_pf_last_error = 0;
    g_debug_icu_pf_error_details = 0;
#if defined(OS_WIN)
    g_debug_icu_pf_filename[0] = 0;
#endif  // OS_WIN

    g_icudtl_pf = file.TakePlatformFile();
    g_icudtl_region = MemoryMappedFile::Region::kWholeFile;
  }
#if defined(OS_WIN)
  else {
    // TODO(brucedawson): http://crbug.com/445616.
    g_debug_icu_pf_last_error = ::GetLastError();
    g_debug_icu_pf_error_details = file.error_details();
    wcscpy_s(g_debug_icu_pf_filename, as_wcstr(data_path.value()));
  }
#endif  // OS_WIN
printf("reading icu data 4...\n");
}

bool InitializeICUWithFileDescriptorInternal(
    PlatformFile data_fd,
    const MemoryMappedFile::Region& data_region) {
printf("InitializeICUWithFileDescriptorInternal 1\n");
  // This can be called multiple times in tests.
  if (g_icudtl_mapped_file) {
printf("InitializeICUWithFileDescriptorInternal 1.1\n");
    g_debug_icu_load = 0;  // To debug http://crbug.com/445616.
    return true;
  }
  if (data_fd == kInvalidPlatformFile) {
printf("InitializeICUWithFileDescriptorInternal 1.2\n");
    g_debug_icu_load = 1;  // To debug http://crbug.com/445616.
    LOG(ERROR) << "Invalid file descriptor to ICU data received: "  << kIcuDataFileName;
#if defined(OS_EMSCRIPTEN)
    DCHECK(false);
#endif
printf("InitializeICUWithFileDescriptorInternal 2\n");
    return false;
  }

  std::unique_ptr<MemoryMappedFile> icudtl_mapped_file(new MemoryMappedFile());
  //if (!icudtl_mapped_file->Initialize(File(data_fd), data_region)) {
  //const base::string16 str = base::ASCIIToUTF16("data_fd");
  if (!icudtl_mapped_file->Initialize(FilePath(kIcuDataFileName))) {
    g_debug_icu_load = 2;  // To debug http://crbug.com/445616.
    LOG(ERROR) << "Couldn't mmap icu data file: " << kIcuDataFileName;
printf("InitializeICUWithFileDescriptorInternal 2.1\n");
#if defined(OS_EMSCRIPTEN)
    DCHECK(false);
#endif
    return false;
  }
  DCHECK(icudtl_mapped_file->length()>0);
  DCHECK(icudtl_mapped_file->data());
  DCHECK(icudtl_mapped_file->IsValid());
  g_icudtl_mapped_file = icudtl_mapped_file.release();
printf("InitializeICUWithFileDescriptorInternal 3\n");

  UErrorCode err = U_ZERO_ERROR;
  DCHECK(g_icudtl_mapped_file->data());
  DCHECK(g_icudtl_mapped_file->length()>0);
  DCHECK(g_icudtl_mapped_file->IsValid());
printf("InitializeICUWithFileDescriptorInternal 3.0\n");

  /// TODO: wasm alignment fault udata_setCommonData 4 -> udata_checkCommonData
  ///udata_setCommonData(const_cast<uint8_t*>(g_icudtl_mapped_file->data()), &err);


    /// \see "Alignment" at http://userguide.icu-project.org/icudata#TOC-ICU-Data-File-Formats
    /// \see https://github.com/tombo-a2o/Foundation/blob/51e451959cba7eade126e0cb3df28c370def7498/System/CoreFoundation/src/CFRuntime.c#L999
    UErrorCode err2 = U_ZERO_ERROR;
    int icuDataFd = open(kIcuDataFileName, O_RDONLY);
    if (icuDataFd != -1) {
        struct stat stbuf;
        fstat(icuDataFd, &stbuf);
        size_t icuDataLen = stbuf.st_size;

        // void *icuData = mmap(0, icuDataLen, PROT_READ, MAP_SHARED, icuDataFd, 0);
        char *icuData = (char*)malloc(icuDataLen);
        read(icuDataFd, icuData, icuDataLen);
        close(icuDataFd);

        udata_setCommonData(icuData, &err2);
        if (err2 != 0)
        {
            printf("icu initialization failed with error %d\n", (int)err);
        }
    } else {
        printf("No icu data found, using minimal built-in tables\n");
    }

  if (err != U_ZERO_ERROR) {
printf("udata_setCommonData error!\n");
    g_debug_icu_load = 3;  // To debug http://crbug.com/445616.
    g_debug_icu_last_error = err;
  }
printf("InitializeICUWithFileDescriptorInternal 3.1\n");
#if defined(OS_ANDROID)
  else {
    // On Android, we can't leave it up to ICU to set the default timezone
    // because ICU's timezone detection does not work in many timezones (e.g.
    // Australia/Sydney, Asia/Seoul, Europe/Paris ). Use JNI to detect the host
    // timezone and set the ICU default timezone accordingly in advance of
    // actual use. See crbug.com/722821 and
    // https://ssl.icu-project.org/trac/ticket/13208 .
    string16 timezone_id = android::GetDefaultTimeZoneId();
    icu::TimeZone::adoptDefault(icu::TimeZone::createTimeZone(
        icu::UnicodeString(FALSE, timezone_id.data(), timezone_id.length())));
  }
#endif
  // Never try to load ICU data from files.
  ///udata_setFileAccess(UDATA_ONLY_PACKAGES, &err);

  // Tell ICU it can *only* use our memory-mapped data.
  udata_setFileAccess(UDATA_NO_FILES, &err);

printf("InitializeICUWithFileDescriptorInternal 4\n");
//UErrorCode status = U_ZERO_ERROR;
// u_init(&status);
  DCHECK(err == U_ZERO_ERROR);
  return err == U_ZERO_ERROR;
}
#endif  // ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE
#endif  // !defined(OS_NACL)

}  // namespace

#if !defined(OS_NACL)// && !defined(OS_EMSCRIPTEN)
#if ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE
#if defined(OS_ANDROID)
bool InitializeICUWithFileDescriptor(
    PlatformFile data_fd,
    const MemoryMappedFile::Region& data_region) {
#if DCHECK_IS_ON()
  DCHECK(!g_check_called_once || !g_called_once);
  g_called_once = true;
#endif
  return InitializeICUWithFileDescriptorInternal(data_fd, data_region);
}

PlatformFile GetIcuDataFileHandle(MemoryMappedFile::Region* out_region) {
  CHECK_NE(g_icudtl_pf, kInvalidPlatformFile);
  *out_region = g_icudtl_region;
  return g_icudtl_pf;
}
#endif

const uint8_t* GetRawIcuMemory() {
  CHECK(g_icudtl_mapped_file);
  return g_icudtl_mapped_file->data();
}

bool InitializeICUFromRawMemory(const uint8_t* raw_memory) {
#if !defined(COMPONENT_BUILD)
#if DCHECK_IS_ON()
  DCHECK(!g_check_called_once || !g_called_once);
  g_called_once = true;
#endif
  DCHECK(raw_memory);
  UErrorCode err = U_ZERO_ERROR;
  udata_setCommonData(const_cast<uint8_t*>(raw_memory), &err);
  // Never try to load ICU data from files.
  ///udata_setFileAccess(UDATA_ONLY_PACKAGES, &err);

  // Tell ICU it can *only* use our memory-mapped data.
  udata_setFileAccess(UDATA_NO_FILES, &err);

  return err == U_ZERO_ERROR;
#else
  return true;
#endif
}

#endif  // ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE

//  __EMSCRIPTEN__: TODO https://github.com/blockspacer/cobalt-clone-28052019/blob/master/src/base/i18n/icu_util.cc

bool InitializeICU() {
printf("InitializeICU() 1\n");
#if DCHECK_IS_ON()
  DCHECK(!g_check_called_once || !g_called_once);
  g_called_once = true;
#endif

  bool result;
#if (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_SHARED)
  FilePath data_path;
  PathService::Get(DIR_ASSETS, &data_path);
  data_path = data_path.AppendASCII(ICU_UTIL_DATA_SHARED_MODULE_NAME);

  HMODULE module = LoadLibrary(data_path.value().c_str());
  if (!module) {
    LOG(ERROR) << "Failed to load " << ICU_UTIL_DATA_SHARED_MODULE_NAME;
    return false;
  }

  FARPROC addr = GetProcAddress(module, ICU_UTIL_DATA_SYMBOL);
  if (!addr) {
    LOG(ERROR) << ICU_UTIL_DATA_SYMBOL << ": not found in "
               << ICU_UTIL_DATA_SHARED_MODULE_NAME;
    return false;
  }

  UErrorCode err = U_ZERO_ERROR;
  udata_setCommonData(reinterpret_cast<void*>(addr), &err);
  // Never try to load ICU data from files.
  ///udata_setFileAccess(UDATA_ONLY_PACKAGES, &err);

  // Tell ICU it can *only* use our memory-mapped data.
  udata_setFileAccess(UDATA_NO_FILES, &err);
  result = (err == U_ZERO_ERROR);
#elif (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_STATIC)
  // The ICU data is statically linked.
  result = true;
#elif (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE)

  //printf("loading ICU_UTIL_DATA_FILE 1...\n");

  // If the ICU data directory is set, ICU won't actually load the data until
  // it is needed.  This can fail if the process is sandboxed at that time.
  // Instead, we map the file in and hand off the data so the sandbox won't
  // cause any problems.
  LazyInitIcuDataFile();

  //printf("loading ICU_UTIL_DATA_FILE 2...\n");

  result =
      InitializeICUWithFileDescriptorInternal(g_icudtl_pf, g_icudtl_region);

  //printf("loading ICU_UTIL_DATA_FILE 3...\n");

  // TODO: ICU_initLocaleDataImpl https://github.com/PhungXuanAnh91/mza-v3.0-bsp/blob/ddfa13450bb0dfdaf424265d70dfcd7cd28591ba/libcore/luni/src/main/native/libcore_icu_ICU.cpp#L364

#if defined(OS_WIN)
  int debug_icu_load = g_debug_icu_load;
  debug::Alias(&debug_icu_load);
  int debug_icu_last_error = g_debug_icu_last_error;
  debug::Alias(&debug_icu_last_error);
  int debug_icu_pf_last_error = g_debug_icu_pf_last_error;
  debug::Alias(&debug_icu_pf_last_error);
  int debug_icu_pf_error_details = g_debug_icu_pf_error_details;
  debug::Alias(&debug_icu_pf_error_details);
  wchar_t debug_icu_pf_filename[_MAX_PATH] = {0};
  wcscpy_s(debug_icu_pf_filename, g_debug_icu_pf_filename);
  debug::Alias(&debug_icu_pf_filename);
  CHECK(result);  // TODO(brucedawson): http://crbug.com/445616
#endif
#endif

// To respond to the timezone change properly, the default timezone
// cache in ICU has to be populated on starting up.
// TODO(jungshik): Some callers do not care about tz at all. If necessary,
// add a boolean argument to this function to init'd the default tz only
// when requested.
#if !UCONFIG_NO_FORMATTING \
    && ((defined(OS_LINUX) && !defined(OS_CHROMEOS)) \
    || defined(OS_EMSCRIPTEN))
  if (result)
    std::unique_ptr<icu::TimeZone> zone(icu::TimeZone::createDefault());
#endif

  //printf("InitializeICU() 2\n");

  UErrorCode status = U_ZERO_ERROR;

/**
 *  Initialize ICU.
 *
 *  Use of this function is optional.  It is OK to simply use ICU
 *  services and functions without first having initialized
 *  ICU by calling u_init().
 *
 *  u_init() will attempt to load some part of ICU's data, and is
 *  useful as a test for configuration or installation problems that
 *  leave the ICU data inaccessible.  A successful invocation of u_init()
 *  does not, however, guarantee that all ICU data is accessible.
 *
 *  Multiple calls to u_init() cause no harm, aside from the small amount
 *  of time required.
 *
 *  In old versions of ICU, u_init() was required in multi-threaded applications
 *  to ensure the thread safety of ICU.  u_init() is no longer needed for this purpose.
 *
 * @param status An ICU UErrorCode parameter. It must not be <code>NULL</code>.
 *    An Error will be returned if some required part of ICU data can not
 *    be loaded or initialized.
 *    The function returns immediately if the input error code indicates a
 *    failure, as usual.
 *
 * @stable ICU 2.6
 */
 /// \see https://github.com/abergmeier/emscripten-icu/blob/master/readme.html#L1464
 /// \see https://github.com/blockspacer/cobalt-clone-28052019/blob/master/src/third_party/icu/source/common/uinit.cpp
   u_init(&status); /// __TODO__

  //DCHECK((status == U_ZERO_ERROR)); /// __TODO__

  printf("ICU Initialized: u_init() returned %s\n", u_errorName(status));

  //printf("InitializeICU() 3\n");

  //printf("Milliseconds since Epoch: %.0f\n", uprv_getUTCtime());

  union {
      uint8_t byte;
      uint16_t word;
  } u;
  u.word=0x0100;
  if(U_IS_BIG_ENDIAN==u.byte) {
    printf("U_IS_BIG_ENDIAN: %d\n", U_IS_BIG_ENDIAN);
  } else {
      fprintf(stderr, "  error: U_IS_BIG_ENDIAN=%d != %d=actual 'is big endian'\n",
              U_IS_BIG_ENDIAN, u.byte);
      status=U_INTERNAL_PROGRAM_ERROR;
  }

  int charsetFamily;
  if('A'==0x41) {
      charsetFamily=U_ASCII_FAMILY;
  } else if('A'==0xc1) {
      charsetFamily=U_EBCDIC_FAMILY;
  } else {
      charsetFamily=-1;  // unknown
  }

  if(U_CHARSET_FAMILY==charsetFamily) {
    printf("U_CHARSET_FAMILY: %d\n", U_CHARSET_FAMILY);
  } else {
      fprintf(stderr, "  error: U_CHARSET_FAMILY=%d != %d=actual charset family\n",
              U_CHARSET_FAMILY, charsetFamily);
      status=U_INTERNAL_PROGRAM_ERROR;
  }

  return result;
}
#endif  // !defined(OS_NACL)

void AllowMultipleInitializeCallsForTesting() {
#if DCHECK_IS_ON() && !defined(OS_NACL)// && !defined(OS_EMSCRIPTEN)
  g_check_called_once = false;
#endif
}

}  // namespace i18n
}  // namespace base
