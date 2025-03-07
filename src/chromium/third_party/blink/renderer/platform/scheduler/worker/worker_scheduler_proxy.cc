// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/scheduler/worker/worker_scheduler_proxy.h"

#include "base/bind.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/platform/scheduler/main_thread/frame_scheduler_impl.h"
#include "third_party/blink/renderer/platform/scheduler/public/worker_scheduler.h"
#include "third_party/blink/renderer/platform/scheduler/worker/worker_thread_scheduler.h"

namespace blink {
namespace scheduler {

WorkerSchedulerProxy::WorkerSchedulerProxy(FrameOrWorkerScheduler* scheduler) {
  DCHECK(scheduler);
  throttling_observer_handle_ = scheduler->AddLifecycleObserver(
      FrameOrWorkerScheduler::ObserverType::kWorkerScheduler, this);
  if (FrameScheduler* frame_scheduler = scheduler->ToFrameScheduler()) {
    parent_frame_type_ = GetFrameOriginType(frame_scheduler);
    initial_frame_status_ = GetFrameStatus(frame_scheduler);

#if defined(ENABLE_UKM)
    ukm_source_id_ = frame_scheduler->GetUkmSourceId();
#endif // ENABLE_UKM

#if defined(ENABLE_GNET) && defined(ENABLE_UKM)
    if (ukm_source_id_ != ukm::kInvalidSourceId) {
      // The connector must be cloned because it belongs to the main thread,
      // but we intend to acquire and use it from the worker thread. (It must
      // be cloned on the original owning thread, not the destination thread.)
      connector_ = Platform::Current()->GetConnector()->Clone();
    }
#endif // ENABLE_GNET
  }
}

WorkerSchedulerProxy::~WorkerSchedulerProxy() {
  DETACH_FROM_THREAD(parent_thread_checker_);
}

void WorkerSchedulerProxy::OnWorkerSchedulerCreated(
    base::WeakPtr<WorkerScheduler> worker_scheduler) {
  DCHECK(!IsMainThread())
      << "OnWorkerSchedulerCreated should be called from the worker thread";
  DCHECK(!worker_scheduler_) << "OnWorkerSchedulerCreated is called twice";
  DCHECK(worker_scheduler) << "WorkerScheduler is expected to exist";
  worker_scheduler_ = std::move(worker_scheduler);
  worker_thread_task_runner_ = worker_scheduler_->GetWorkerThreadScheduler()
                                   ->ControlTaskQueue()
                                   ->task_runner();
  initialized_ = true;
}

void WorkerSchedulerProxy::OnLifecycleStateChanged(
    SchedulingLifecycleState lifecycle_state) {
  DCHECK_CALLED_ON_VALID_THREAD(parent_thread_checker_);
  if (lifecycle_state_ == lifecycle_state)
    return;
  lifecycle_state_ = lifecycle_state;

  if (!initialized_)
    return;

  worker_thread_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&WorkerScheduler::OnLifecycleStateChanged,
                                worker_scheduler_, lifecycle_state));
}

}  // namespace scheduler
}  // namespace blink
