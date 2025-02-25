// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/heap/blink_gc.h"

#include "base/logging.h"

namespace blink {

const char* BlinkGC::ToString(BlinkGC::GCReason reason) {
  switch (reason) {
    case BlinkGC::GCReason::kPreciseGC:
      return "PreciseGC";
    case BlinkGC::GCReason::kConservativeGC:
      return "ConservativeGC";
    case BlinkGC::GCReason::kForcedGCForTesting:
      return "ForcedGCForTesting";
    case BlinkGC::GCReason::kMemoryPressureGC:
      return "MemoryPressureGC";
    case BlinkGC::GCReason::kThreadTerminationGC:
      return "ThreadTerminationGC";
    case BlinkGC::GCReason::kIncrementalV8FollowupGC:
      return "IncrementalV8FollowupGC";
    case BlinkGC::GCReason::kUnifiedHeapGC:
      return "UnifiedHeapGC";
  }
  IMMEDIATE_CRASH();
}

const char* BlinkGC::ToString(BlinkGC::MarkingType type) {
  switch (type) {
    case BlinkGC::MarkingType::kAtomicMarking:
      return "AtomicMarking";
    case BlinkGC::MarkingType::kIncrementalMarking:
      return "IncrementalMarking";
    case BlinkGC::MarkingType::kTakeSnapshot:
      return "TakeSnapshot";
  }
  IMMEDIATE_CRASH();
}

const char* BlinkGC::ToString(BlinkGC::SweepingType type) {
  switch (type) {
    case BlinkGC::SweepingType::kLazySweeping:
      return "LazySweeping";
    case BlinkGC::SweepingType::kEagerSweeping:
      return "EagerSweeping";
  }
  IMMEDIATE_CRASH();
}

const char* BlinkGC::ToString(BlinkGC::StackState stack_state) {
  switch (stack_state) {
    case BlinkGC::kNoHeapPointersOnStack:
      return "NoHeapPointersOnStack";
    case BlinkGC::kHeapPointersOnStack:
      return "HeapPointersOnStack";
  }
  IMMEDIATE_CRASH();
}

}  // namespace blink
