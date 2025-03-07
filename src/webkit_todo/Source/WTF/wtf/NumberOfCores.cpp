/*
 * Copyright (C) 2012 University of Szeged. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY UNIVERSITY OF SZEGED ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL UNIVERSITY OF SZEGED OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <wtf/NumberOfCores.h>

#include <cstdio>

#if OS(DARWIN)
#include <sys/param.h>
// sys/types.h must come before sys/sysctl.h because the latter uses
// data types defined in the former. See sysctl(3) and style(9).
#include <sys/types.h>
#include <sys/sysctl.h>
#elif OS(LINUX) || OS(AIX) || OS(OPENBSD) || OS(NETBSD) || OS(FREEBSD)
#include <unistd.h>
#elif OS(WINDOWS)
#include <windows.h>
#endif

// __EMSCRIPTEN_PTHREADS__ can be used to detect whether Emscripten is currently targeting pthreads.
// At runtime, you can use the emscripten_has_threading_support()
// see https://emscripten.org/docs/porting/pthreads.html
#if (defined(OS_EMSCRIPTEN) || defined(__EMSCRIPTEN__)) && defined(__EMSCRIPTEN_PTHREADS__)
#include <emscripten/emscripten.h>
#include <emscripten/threading.h>
#endif // OS_EMSCRIPTEN

namespace WTF {

int numberOfProcessorCores()
{
    const int defaultIfUnavailable = 1;
    static int s_numberOfCores = -1;

    if (s_numberOfCores > 0)
        return s_numberOfCores;
    
    if (const char* coresEnv = getenv("WTF_numberOfProcessorCores")) {
        unsigned numberOfCores;
        if (sscanf(coresEnv, "%u", &numberOfCores) == 1) {
            s_numberOfCores = numberOfCores;
            return s_numberOfCores;
        } else
            fprintf(stderr, "WARNING: failed to parse WTF_numberOfProcessorCores=%s\n", coresEnv);
    }

#if defined(OS_EMSCRIPTEN) || defined(__EMSCRIPTEN__)

  //return 1; // TODO

  // see https://github.com/hongkk/urho/blob/master/Source/Urho3D/Core/ProcessUtils.cpp#L448
  // see https://emscripten.org/docs/porting/pthreads.html
#ifdef __EMSCRIPTEN_PTHREADS__
  if (!emscripten_has_threading_support()) {
    printf("warning: !emscripten_has_threading_support\n");
  }
  return emscripten_num_logical_cores();
#else
  #warning "emscripten built without PTHREADS"
  return 1;
#endif

#elif OS(DARWIN)
    unsigned result;
    size_t length = sizeof(result);
    int name[] = {
            CTL_HW,
            HW_AVAILCPU
    };
    int sysctlResult = sysctl(name, sizeof(name) / sizeof(int), &result, &length, 0, 0);

    s_numberOfCores = sysctlResult < 0 ? defaultIfUnavailable : result;
#elif OS(LINUX) || OS(AIX) || OS(OPENBSD) || OS(NETBSD) || OS(FREEBSD)
    long sysconfResult = sysconf(_SC_NPROCESSORS_ONLN);

    s_numberOfCores = sysconfResult < 0 ? defaultIfUnavailable : static_cast<int>(sysconfResult);
#elif OS(WINDOWS)
    UNUSED_PARAM(defaultIfUnavailable);
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    s_numberOfCores = sysInfo.dwNumberOfProcessors;
#else
    s_numberOfCores = defaultIfUnavailable;
#endif
    return s_numberOfCores;
}

}
