// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/scheduler/common/metrics_helper.h"

#include "third_party/blink/renderer/platform/scheduler/common/process_state.h"

namespace blink {
namespace scheduler {

namespace {

// Threshold for discarding ultra-long tasks. It is assumed that ultra-long
// tasks are reporting glitches (e.g. system falling asleep on the middle of the
// task).
constexpr base::TimeDelta kLongTaskDiscardingThreshold =
    base::TimeDelta::FromSeconds(30);

scheduling_metrics::ThreadType ConvertBlinkThreadType(
    WebThreadType thread_type) {
  switch (thread_type) {
    case WebThreadType::kMainThread:
      return scheduling_metrics::ThreadType::kRendererMainThread;
    case WebThreadType::kCompositorThread:
      return scheduling_metrics::ThreadType::kRendererCompositorThread;
    case WebThreadType::kDedicatedWorkerThread:
      return scheduling_metrics::ThreadType::kRendererDedicatedWorkerThread;
    case WebThreadType::kServiceWorkerThread:
      return scheduling_metrics::ThreadType::kRendererServiceWorkerThread;
    case WebThreadType::kAnimationAndPaintWorkletThread:
    case WebThreadType::kAudioWorkletThread:
    case WebThreadType::kDatabaseThread:
    case WebThreadType::kFileThread:
    case WebThreadType::kHRTFDatabaseLoaderThread:
    case WebThreadType::kOfflineAudioRenderThread:
    case WebThreadType::kReverbConvolutionBackgroundThread:
    case WebThreadType::kSharedWorkerThread:
    case WebThreadType::kUnspecifiedWorkerThread:
    case WebThreadType::kWebAudioThread:
    case WebThreadType::kTestThread:
      return scheduling_metrics::ThreadType::kRendererOtherBlinkThread;
    case WebThreadType::kCount:
      NOTREACHED();
      return scheduling_metrics::ThreadType::kCount;
  }
}

}  // namespace

MetricsHelper::MetricsHelper(WebThreadType thread_type,
                             bool has_cpu_timing_for_each_task)
    : thread_type_(thread_type),
      thread_metrics_(ConvertBlinkThreadType(thread_type),
                      has_cpu_timing_for_each_task),
      thread_task_duration_reporter_(
          "RendererScheduler.TaskDurationPerThreadType2"),
      thread_task_cpu_duration_reporter_(
          "RendererScheduler.TaskCPUDurationPerThreadType2"),
      foreground_thread_task_duration_reporter_(
          "RendererScheduler.TaskDurationPerThreadType2.Foreground"),
      foreground_thread_task_cpu_duration_reporter_(
          "RendererScheduler.TaskCPUDurationPerThreadType2.Foreground"),
      background_thread_task_duration_reporter_(
          "RendererScheduler.TaskDurationPerThreadType2.Background"),
      background_thread_task_cpu_duration_reporter_(
          "RendererScheduler.TaskCPUDurationPerThreadType2.Background") {}

MetricsHelper::~MetricsHelper() {}

bool MetricsHelper::ShouldDiscardTask(
    base::sequence_manager::TaskQueue* queue,
    const base::sequence_manager::Task& task,
    const base::sequence_manager::TaskQueue::TaskTiming& task_timing) {
  // TODO(altimin): Investigate the relationship between thread time and
  // wall time for discarded tasks.
  using State = base::sequence_manager ::TaskQueue::TaskTiming::State;
  return task_timing.state() == State::Finished &&
         task_timing.wall_duration() > kLongTaskDiscardingThreshold;
}

void MetricsHelper::RecordCommonTaskMetrics(
    base::sequence_manager::TaskQueue* queue,
    const base::sequence_manager::Task& task,
    const base::sequence_manager::TaskQueue::TaskTiming& task_timing) {
  thread_metrics_.RecordTaskMetrics(queue, task, task_timing);

  thread_task_duration_reporter_.RecordTask(thread_type_,
                                            task_timing.wall_duration());

  bool backgrounded = internal::ProcessState::Get()->is_process_backgrounded;

  if (backgrounded) {
    background_thread_task_duration_reporter_.RecordTask(
        thread_type_, task_timing.wall_duration());
  } else {
    foreground_thread_task_duration_reporter_.RecordTask(
        thread_type_, task_timing.wall_duration());
  }

  if (!task_timing.has_thread_time())
    return;
  thread_task_cpu_duration_reporter_.RecordTask(thread_type_,
                                                task_timing.thread_duration());
  if (backgrounded) {
    background_thread_task_cpu_duration_reporter_.RecordTask(
        thread_type_, task_timing.thread_duration());
  } else {
    foreground_thread_task_cpu_duration_reporter_.RecordTask(
        thread_type_, task_timing.thread_duration());
  }
}

}  // namespace scheduler
}  // namespace blink
