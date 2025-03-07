// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/viz/service/frame_sinks/compositor_frame_sink_support.h"

#include "base/bind.h"
#include "base/stl_util.h"
#include "components/viz/common/frame_sinks/copy_output_request.h"
#include "components/viz/common/frame_sinks/copy_output_result.h"
#include "components/viz/common/quads/compositor_frame.h"
#include "components/viz/common/surfaces/frame_sink_id.h"
#include "components/viz/common/surfaces/surface_id.h"
#include "components/viz/common/surfaces/surface_info.h"
#include "components/viz/service/display_embedder/server_shared_bitmap_manager.h"
#include "components/viz/service/frame_sinks/frame_sink_manager_impl.h"
#include "components/viz/service/surfaces/surface.h"
#include "components/viz/test/begin_frame_args_test.h"
#include "components/viz/test/compositor_frame_helpers.h"
#include "components/viz/test/fake_compositor_frame_sink_client.h"
#include "components/viz/test/fake_external_begin_frame_source.h"
#include "components/viz/test/fake_surface_observer.h"
#include "components/viz/test/mock_compositor_frame_sink_client.h"
#include "services/viz/privileged/interfaces/compositing/frame_sink_manager.mojom.h"
#include "services/viz/public/interfaces/compositing/compositor_frame_sink.mojom.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#if defined(OS_EMSCRIPTEN)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "third_party/khronos/GLES2/gl2.h"
#include "third_party/khronos/GLES2/gl2ext.h"
#endif // OS_EMSCRIPTEN

using testing::UnorderedElementsAre;
using testing::IsEmpty;
using testing::SizeIs;
using testing::Invoke;
using testing::_;
using testing::Eq;

namespace viz {
namespace {

constexpr bool kIsRoot = false;
constexpr bool kNeedsSyncPoints = true;

constexpr FrameSinkId kArbitraryFrameSinkId(1, 1);
constexpr FrameSinkId kAnotherArbitraryFrameSinkId(2, 2);

const base::UnguessableToken kArbitraryToken =
    base::UnguessableToken::Deserialize(1, 2);
const base::UnguessableToken kArbitrarySourceId1 =
    base::UnguessableToken::Deserialize(0xdead, 0xbeef);
const base::UnguessableToken kArbitrarySourceId2 =
    base::UnguessableToken::Deserialize(0xdead, 0xbee0);

// Matches a SurfaceInfo for |surface_id|.
MATCHER_P(SurfaceInfoWithId, surface_id, "") {
  return arg.id() == surface_id;
}

void StubResultCallback(std::unique_ptr<CopyOutputResult> result) {}

gpu::SyncToken GenTestSyncToken(int id) {
  gpu::SyncToken token;
  token.Set(gpu::CommandBufferNamespace::GPU_IO,
            gpu::CommandBufferId::FromUnsafeValue(id), 1);
  return token;
}

bool BeginFrameArgsAreEquivalent(const BeginFrameArgs& first,
                                 const BeginFrameArgs& second) {
  return first.source_id == second.source_id &&
         first.sequence_number == second.sequence_number;
}

}  // namespace

class MockFrameSinkManagerClient : public mojom::FrameSinkManagerClient {
 public:
  MockFrameSinkManagerClient() = default;
  ~MockFrameSinkManagerClient() override = default;

  // mojom::FrameSinkManagerClient:
  MOCK_METHOD1(OnFirstSurfaceActivation, void(const SurfaceInfo&));
  MOCK_METHOD2(OnFrameTokenChanged, void(const FrameSinkId&, uint32_t));
  void OnAggregatedHitTestRegionListUpdated(
      const FrameSinkId& frame_sink_id,
      const std::vector<AggregatedHitTestRegion>& hit_test_data) override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MockFrameSinkManagerClient);
};

class CompositorFrameSinkSupportTest : public testing::Test {
 public:
  CompositorFrameSinkSupportTest()
      : manager_(&shared_bitmap_manager_),
        begin_frame_source_(0.f, false),
        local_surface_id_(3, kArbitraryToken),
        frame_sync_token_(GenTestSyncToken(4)),
        consumer_sync_token_(GenTestSyncToken(5)) {
    manager_.SetLocalClient(&frame_sink_manager_client_);
    manager_.surface_manager()->AddObserver(&surface_observer_);
    manager_.RegisterFrameSinkId(kArbitraryFrameSinkId,
                                 true /* report_activation */);
    support_ = std::make_unique<CompositorFrameSinkSupport>(
        &fake_support_client_, &manager_, kArbitraryFrameSinkId, kIsRoot,
        kNeedsSyncPoints);
    support_->SetBeginFrameSource(&begin_frame_source_);
  }
  ~CompositorFrameSinkSupportTest() override {
    manager_.InvalidateFrameSinkId(kArbitraryFrameSinkId);
    manager_.surface_manager()->RemoveObserver(&surface_observer_);
  }

  void AddResourcesToFrame(CompositorFrame* frame,
                           ResourceId* resource_ids,
                           size_t num_resource_ids) {
    for (size_t i = 0u; i < num_resource_ids; ++i) {
      TransferableResource resource;
      resource.id = resource_ids[i];
      resource.mailbox_holder.texture_target = GL_TEXTURE_2D;
      resource.mailbox_holder.sync_token = frame_sync_token_;
      frame->resource_list.push_back(resource);
    }
  }

  void SubmitCompositorFrameWithResources(ResourceId* resource_ids,
                                          size_t num_resource_ids) {
    auto frame = MakeDefaultCompositorFrame();
    AddResourcesToFrame(&frame, resource_ids, num_resource_ids);
    support_->SubmitCompositorFrame(local_surface_id_, std::move(frame));
    EXPECT_EQ(surface_observer_.last_created_surface_id().local_surface_id(),
              local_surface_id_);
  }

  bool SubmitCompositorFrameWithCopyRequest(
      CompositorFrame frame,
      std::unique_ptr<CopyOutputRequest> request) {
    frame.render_pass_list.back()->copy_requests.push_back(std::move(request));
    const auto result = support_->MaybeSubmitCompositorFrame(
        local_surface_id_, std::move(frame), base::nullopt, 0,
        mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
    switch (result) {
      case SubmitResult::ACCEPTED:
        return true;
      case SubmitResult::COPY_OUTPUT_REQUESTS_NOT_ALLOWED:
        return false;
      default:
        ADD_FAILURE()
            << "Test broken; fail result not related to copy requests: "
            << CompositorFrameSinkSupport::GetSubmitResultAsString(result);
        break;
    }
    return false;
  }

  void UnrefResources(ResourceId* ids_to_unref,
                      int* counts_to_unref,
                      size_t num_ids_to_unref) {
    std::vector<ReturnedResource> unref_array;
    for (size_t i = 0; i < num_ids_to_unref; ++i) {
      ReturnedResource resource;
      resource.sync_token = consumer_sync_token_;
      resource.id = ids_to_unref[i];
      resource.count = counts_to_unref[i];
      unref_array.push_back(resource);
    }
    support_->UnrefResources(unref_array);
  }

  void CheckReturnedResourcesMatchExpected(ResourceId* expected_returned_ids,
                                           size_t expected_resources) {
    const std::vector<ReturnedResource>& actual_resources =
        fake_support_client_.returned_resources();
    ASSERT_EQ(expected_resources, actual_resources.size());
    for (size_t i = 0; i < expected_resources; ++i) {
      ReturnedResource resource = actual_resources[i];
      EXPECT_EQ(expected_returned_ids[i], resource.id);
    }
    fake_support_client_.clear_returned_resources();
  }

  void CheckReturnedResourcesMatchExpected(ResourceId* expected_returned_ids,
                                           int* expected_returned_counts,
                                           size_t expected_resources,
                                           gpu::SyncToken expected_sync_token) {
    const std::vector<ReturnedResource>& actual_resources =
        fake_support_client_.returned_resources();
    ASSERT_EQ(expected_resources, actual_resources.size());
    for (size_t i = 0; i < expected_resources; ++i) {
      ReturnedResource resource = actual_resources[i];
      EXPECT_EQ(expected_sync_token, resource.sync_token);
      EXPECT_EQ(expected_returned_ids[i], resource.id);
      EXPECT_EQ(expected_returned_counts[i], resource.count);
    }
    fake_support_client_.clear_returned_resources();
  }

  Surface* GetSurfaceForId(const SurfaceId& id) {
    return manager_.surface_manager()->GetSurfaceForId(id);
  }

  bool HasTemporaryReference(const SurfaceId& id) {
    return manager_.surface_manager()->HasTemporaryReference(id);
  }

  void RefCurrentFrameResources() {
    Surface* surface = GetSurfaceForId(
        SurfaceId(support_->frame_sink_id(), local_surface_id_));
    support_->RefResources(surface->GetActiveFrame().resource_list);
  }

  void ExpireAllTemporaryReferences() {
    // First call marks temporary references as old.
    manager_.surface_manager()->ExpireOldTemporaryReferences();
    // Second call removes the temporary references marked as old.
    manager_.surface_manager()->ExpireOldTemporaryReferences();
  }

  const BeginFrameArgs& GetLastUsedBeginFrameArgs(
      const CompositorFrameSinkSupport* support) const {
    return support->LastUsedBeginFrameArgs();
  }

  void SendPresentationFeedback(CompositorFrameSinkSupport* support,
                                uint32_t frame_token) {
    support->DidPresentCompositorFrame(
        frame_token,
        gfx::PresentationFeedback(base::TimeTicks::Now(),
                                  base::TimeDelta::FromMilliseconds(16),
                                  /*flags=*/0));
  }

 protected:
  ServerSharedBitmapManager shared_bitmap_manager_;
  FrameSinkManagerImpl manager_;
  MockFrameSinkManagerClient frame_sink_manager_client_;
  FakeCompositorFrameSinkClient fake_support_client_;
  FakeExternalBeginFrameSource begin_frame_source_;
  std::unique_ptr<CompositorFrameSinkSupport> support_;
  LocalSurfaceId local_surface_id_;
  FakeSurfaceObserver surface_observer_;

  // This is the sync token submitted with the frame. It should never be
  // returned to the client.
  const gpu::SyncToken frame_sync_token_;

  // This is the sync token returned by the consumer. It should always be
  // returned to the client.
  const gpu::SyncToken consumer_sync_token_;
};

// Tests submitting a frame with resources followed by one with no resources
// with no resource provider action in between.
TEST_F(CompositorFrameSinkSupportTest, ResourceLifetimeSimple) {
  ResourceId first_frame_ids[] = {1, 2, 3};
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // All of the resources submitted in the first frame are still in use at this
  // time by virtue of being in the pending frame, so none can be returned to
  // the client yet.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  // The second frame references no resources of first frame and thus should
  // make all resources of first frame available to be returned.
  SubmitCompositorFrameWithResources(nullptr, 0);

  ResourceId expected_returned_ids[] = {1, 2, 3};
  int expected_returned_counts[] = {1, 1, 1};
  // Resources were never consumed so no sync token should be set.
  CheckReturnedResourcesMatchExpected(
      expected_returned_ids, expected_returned_counts,
      base::size(expected_returned_counts), gpu::SyncToken());

  ResourceId third_frame_ids[] = {4, 5, 6};
  SubmitCompositorFrameWithResources(third_frame_ids,
                                     base::size(third_frame_ids));

  // All of the resources submitted in the third frame are still in use at this
  // time by virtue of being in the pending frame, so none can be returned to
  // the client yet.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  // The forth frame references no resources of third frame and thus should
  // make all resources of third frame available to be returned.
  ResourceId forth_frame_ids[] = {7, 8, 9};
  SubmitCompositorFrameWithResources(forth_frame_ids,
                                     base::size(forth_frame_ids));

  ResourceId forth_expected_returned_ids[] = {4, 5, 6};
  int forth_expected_returned_counts[] = {1, 1, 1};
  // Resources were never consumed so no sync token should be set.
  CheckReturnedResourcesMatchExpected(
      forth_expected_returned_ids, forth_expected_returned_counts,
      base::size(forth_expected_returned_counts), gpu::SyncToken());
}

// Tests submitting a frame with resources followed by one with no resources
// with the resource provider holding everything alive.
TEST_F(CompositorFrameSinkSupportTest,
       ResourceLifetimeSimpleWithProviderHoldingAlive) {
  ResourceId first_frame_ids[] = {1, 2, 3};
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // All of the resources submitted in the first frame are still in use at this
  // time by virtue of being in the pending frame, so none can be returned to
  // the client yet.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  // Hold on to everything.
  RefCurrentFrameResources();

  // The second frame references no resources and thus should make all resources
  // available to be returned as soon as the resource provider releases them.
  SubmitCompositorFrameWithResources(nullptr, 0);

  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  int release_counts[] = {1, 1, 1};
  UnrefResources(first_frame_ids, release_counts, base::size(first_frame_ids));

  // None is returned to the client since DidReceiveCompositorAck is not
  // invoked.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());

  // Submitting an empty frame causes previous resources referenced by the
  // previous frame to be returned to client.
  SubmitCompositorFrameWithResources(nullptr, 0);
  ResourceId expected_returned_ids[] = {1, 2, 3};
  int expected_returned_counts[] = {1, 1, 1};
  CheckReturnedResourcesMatchExpected(
      expected_returned_ids, expected_returned_counts,
      base::size(expected_returned_counts), consumer_sync_token_);
}

// Tests referencing a resource, unref'ing it to zero, then using it again
// before returning it to the client.
TEST_F(CompositorFrameSinkSupportTest, ResourceReusedBeforeReturn) {
  ResourceId first_frame_ids[] = {7};
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // This removes all references to resource id 7.
  SubmitCompositorFrameWithResources(nullptr, 0);

  // This references id 7 again.
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // This removes it again.
  SubmitCompositorFrameWithResources(nullptr, 0);

  // Now it should be returned.
  // We don't care how many entries are in the returned array for 7, so long as
  // the total returned count matches the submitted count.
  const std::vector<ReturnedResource>& returned =
      fake_support_client_.returned_resources();
  size_t return_count = 0;
  for (size_t i = 0; i < returned.size(); ++i) {
    EXPECT_EQ(7u, returned[i].id);
    return_count += returned[i].count;
  }
  EXPECT_EQ(2u, return_count);
}

// Tests having resources referenced multiple times, as if referenced by
// multiple providers.
TEST_F(CompositorFrameSinkSupportTest, ResourceRefMultipleTimes) {
  ResourceId first_frame_ids[] = {3, 4};
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // Ref resources from the first frame twice.
  RefCurrentFrameResources();
  RefCurrentFrameResources();

  ResourceId second_frame_ids[] = {4, 5};
  SubmitCompositorFrameWithResources(second_frame_ids,
                                     base::size(second_frame_ids));

  // Ref resources from the second frame 3 times.
  RefCurrentFrameResources();
  RefCurrentFrameResources();
  RefCurrentFrameResources();

  // Submit a frame with no resources to remove all current frame refs from
  // submitted resources.
  SubmitCompositorFrameWithResources(nullptr, 0);

  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  // Expected current refs:
  //  3 -> 2
  //  4 -> 2 + 3 = 5
  //  5 -> 3
  {
    SCOPED_TRACE("unref all 3");
    ResourceId ids_to_unref[] = {3, 4, 5};
    int counts[] = {1, 1, 1};
    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));

    EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
    fake_support_client_.clear_returned_resources();

    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));
    SubmitCompositorFrameWithResources(nullptr, 0);
    ResourceId expected_returned_ids[] = {3};
    int expected_returned_counts[] = {1};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), consumer_sync_token_);
  }

  // Expected refs remaining:
  //  4 -> 3
  //  5 -> 1
  {
    SCOPED_TRACE("unref 4 and 5");
    ResourceId ids_to_unref[] = {4, 5};
    int counts[] = {1, 1};
    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));
    SubmitCompositorFrameWithResources(nullptr, 0);

    ResourceId expected_returned_ids[] = {5};
    int expected_returned_counts[] = {1};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), consumer_sync_token_);
  }

  // Now, just 2 refs remaining on resource 4. Unref both at once and make sure
  // the returned count is correct.
  {
    SCOPED_TRACE("unref only 4");
    ResourceId ids_to_unref[] = {4};
    int counts[] = {2};
    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));
    SubmitCompositorFrameWithResources(nullptr, 0);

    ResourceId expected_returned_ids[] = {4};
    int expected_returned_counts[] = {2};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), consumer_sync_token_);
  }
}

TEST_F(CompositorFrameSinkSupportTest, ResourceLifetime) {
  ResourceId first_frame_ids[] = {1, 2, 3};
  SubmitCompositorFrameWithResources(first_frame_ids,
                                     base::size(first_frame_ids));

  // All of the resources submitted in the first frame are still in use at this
  // time by virtue of being in the pending frame, so none can be returned to
  // the client yet.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());
  fake_support_client_.clear_returned_resources();

  // The second frame references some of the same resources, but some different
  // ones. We expect to receive back resource 1 with a count of 1 since it was
  // only referenced by the first frame.
  ResourceId second_frame_ids[] = {2, 3, 4};
  SubmitCompositorFrameWithResources(second_frame_ids,
                                     base::size(second_frame_ids));
  {
    SCOPED_TRACE("second frame");
    ResourceId expected_returned_ids[] = {1};
    int expected_returned_counts[] = {1};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), gpu::SyncToken());
  }

  // The third frame references a disjoint set of resources, so we expect to
  // receive back all resources from the first and second frames. Resource IDs 2
  // and 3 will have counts of 2, since they were used in both frames, and
  // resource ID 4 will have a count of 1.
  ResourceId third_frame_ids[] = {10, 11, 12, 13};
  SubmitCompositorFrameWithResources(third_frame_ids,
                                     base::size(third_frame_ids));

  {
    SCOPED_TRACE("third frame");
    ResourceId expected_returned_ids[] = {2, 3, 4};
    int expected_returned_counts[] = {2, 2, 1};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), gpu::SyncToken());
  }

  // Simulate a ResourceProvider taking a ref on all of the resources.
  RefCurrentFrameResources();

  ResourceId fourth_frame_ids[] = {12, 13};
  SubmitCompositorFrameWithResources(fourth_frame_ids,
                                     base::size(fourth_frame_ids));

  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());

  RefCurrentFrameResources();

  // All resources are still being used by the external reference, so none can
  // be returned to the client.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());

  // Release resources associated with the first RefCurrentFrameResources() call
  // first.
  {
    ResourceId ids_to_unref[] = {10, 11, 12, 13};
    int counts[] = {1, 1, 1, 1};
    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));
  }

  // Nothing is returned to the client yet since DidReceiveCompositorFrameAck
  // is not invoked.
  {
    SCOPED_TRACE("fourth frame, first unref");
    CheckReturnedResourcesMatchExpected(nullptr, nullptr, 0,
                                        consumer_sync_token_);
  }

  {
    ResourceId ids_to_unref[] = {12, 13};
    int counts[] = {1, 1};
    UnrefResources(ids_to_unref, counts, base::size(ids_to_unref));
  }

  // Resources 12 and 13 are still in use by the current frame, so they
  // shouldn't be available to be returned.
  EXPECT_EQ(0u, fake_support_client_.returned_resources().size());

  // If we submit an empty frame, however, they should become available.
  // Resources that were previously unref'd also return at this point.
  SubmitCompositorFrameWithResources(nullptr, 0u);

  {
    SCOPED_TRACE("fourth frame, second unref");
    ResourceId expected_returned_ids[] = {10, 11, 12, 13};
    int expected_returned_counts[] = {1, 1, 2, 2};
    CheckReturnedResourcesMatchExpected(
        expected_returned_ids, expected_returned_counts,
        base::size(expected_returned_counts), consumer_sync_token_);
  }
}

TEST_F(CompositorFrameSinkSupportTest, AddDuringEviction) {
  manager_.RegisterFrameSinkId(kAnotherArbitraryFrameSinkId,
                               true /* report_activation */);
  MockCompositorFrameSinkClient mock_client;
  auto support = std::make_unique<CompositorFrameSinkSupport>(
      &mock_client, &manager_, kAnotherArbitraryFrameSinkId, kIsRoot,
      kNeedsSyncPoints);
  LocalSurfaceId local_surface_id(6, kArbitraryToken);
  support->SubmitCompositorFrame(local_surface_id,
                                 MakeDefaultCompositorFrame());

  SurfaceManager* surface_manager = manager_.surface_manager();

  EXPECT_CALL(mock_client, DidReceiveCompositorFrameAck(_))
      .WillOnce(testing::InvokeWithoutArgs([&]() {
        LocalSurfaceId new_id(7, base::UnguessableToken::Create());
        support->SubmitCompositorFrame(new_id, MakeDefaultCompositorFrame());
        surface_manager->GarbageCollectSurfaces();
      }))
      .WillRepeatedly(testing::Return());
  support->EvictSurface(local_surface_id);
  ExpireAllTemporaryReferences();
  manager_.InvalidateFrameSinkId(kAnotherArbitraryFrameSinkId);
}

// Verifies that only monotonically increasing LocalSurfaceIds are accepted.
TEST_F(CompositorFrameSinkSupportTest, MonotonicallyIncreasingLocalSurfaceIds) {
  manager_.RegisterFrameSinkId(kAnotherArbitraryFrameSinkId,
                               true /* report_activation */);
  MockCompositorFrameSinkClient mock_client;
  auto support = std::make_unique<CompositorFrameSinkSupport>(
      &mock_client, &manager_, kAnotherArbitraryFrameSinkId, kIsRoot,
      kNeedsSyncPoints);
  base::UnguessableToken embed_token = base::UnguessableToken::Create();
  LocalSurfaceId local_surface_id1(6, 1, embed_token);
  LocalSurfaceId local_surface_id2(6, 2, embed_token);
  LocalSurfaceId local_surface_id3(7, 2, embed_token);
  LocalSurfaceId local_surface_id4(5, 3, embed_token);
  LocalSurfaceId local_surface_id5(8, 1, embed_token);
  LocalSurfaceId local_surface_id6(9, 3, embed_token);

  // LocalSurfaceId1(6, 1)
  auto result = support->MaybeSubmitCompositorFrame(
      local_surface_id1, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  // LocalSurfaceId(6, 2): Child-initiated synchronization.
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id2, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  // Since the Surface corresponding to |local_surface_id1| was not a dependency
  // anywhere then the Surface corresponding to |local_surface_id2| will not
  // activate until it becomes a dependency.
  Surface* last_created_surface = support->GetLastCreatedSurfaceForTesting();
  EXPECT_EQ(local_surface_id2,
            last_created_surface->surface_id().local_surface_id());
  EXPECT_FALSE(last_created_surface->HasActiveFrame());

  SurfaceId surface_id2(kAnotherArbitraryFrameSinkId, local_surface_id2);
  auto frame =
      CompositorFrameBuilder()
          .AddDefaultRenderPass()
          .SetActivationDependencies({surface_id2})
          .SetReferencedSurfaces({SurfaceRange(base::nullopt, surface_id2)})
          .Build();
  result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  // Submitting a CompositorFrame to the parent FrameSink with a dependency on
  // |local_surface_id2| causes that Surface's CompositorFrame to activate.
  EXPECT_TRUE(last_created_surface->HasActiveFrame());

  // LocalSurfaceId(7, 2): Parent-initiated synchronization.
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id3, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  // LocalSurfaceId(5, 3): Submit rejected because not monotonically increasing.
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id4, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::SURFACE_ID_DECREASED, result);

  // LocalSurfaceId(8, 1): Submit rejected because not monotonically increasing.
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id5, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::SURFACE_ID_DECREASED, result);

  // LocalSurfaceId(9, 3): Parent AND child-initiated synchronization.
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id6, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  manager_.InvalidateFrameSinkId(kAnotherArbitraryFrameSinkId);
}

// Verifies that CopyOutputRequests submitted by unprivileged clients are
// rejected.
TEST_F(CompositorFrameSinkSupportTest, ProhibitsUnprivilegedCopyRequests) {
  manager_.RegisterFrameSinkId(kAnotherArbitraryFrameSinkId,
                               true /* report_activation */);
  MockCompositorFrameSinkClient mock_client;
  auto support = std::make_unique<CompositorFrameSinkSupport>(
      &mock_client, &manager_, kAnotherArbitraryFrameSinkId,
      false /* not root frame sink */, kNeedsSyncPoints);

  bool did_receive_aborted_copy_result = false;
  auto request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(
          [](bool* got_nothing, std::unique_ptr<CopyOutputResult> result) {
            *got_nothing = result->IsEmpty();
          },
          &did_receive_aborted_copy_result));

  auto frame = MakeDefaultCompositorFrame();
  ResourceId frame_resource_ids[] = {1, 2, 3};
  AddResourcesToFrame(&frame, frame_resource_ids,
                      base::size(frame_resource_ids));

  EXPECT_FALSE(SubmitCompositorFrameWithCopyRequest(std::move(frame),
                                                    std::move(request)));
  EXPECT_TRUE(did_receive_aborted_copy_result);

  // All the resources in the rejected frame should have been returned.
  CheckReturnedResourcesMatchExpected(frame_resource_ids,
                                      base::size(frame_resource_ids));

  manager_.InvalidateFrameSinkId(kAnotherArbitraryFrameSinkId);
}

// Tests doing an EvictLastActivatedSurface before shutting down the factory.
TEST_F(CompositorFrameSinkSupportTest, EvictLastActivatedSurface) {
  manager_.RegisterFrameSinkId(kAnotherArbitraryFrameSinkId,
                               true /* report_activation */);
  MockCompositorFrameSinkClient mock_client;
  auto support = std::make_unique<CompositorFrameSinkSupport>(
      &mock_client, &manager_, kAnotherArbitraryFrameSinkId, kIsRoot,
      kNeedsSyncPoints);
  LocalSurfaceId local_surface_id(7, kArbitraryToken);
  SurfaceId id(kAnotherArbitraryFrameSinkId, local_surface_id);

  TransferableResource resource;
  resource.id = 1;
  resource.mailbox_holder.texture_target = GL_TEXTURE_2D;
  auto frame = CompositorFrameBuilder()
                   .AddDefaultRenderPass()
                   .AddTransferableResource(resource)
                   .Build();
  support->SubmitCompositorFrame(local_surface_id, std::move(frame));
  EXPECT_EQ(surface_observer_.last_created_surface_id().local_surface_id(),
            local_surface_id);
  local_surface_id_ = LocalSurfaceId();

  std::vector<ReturnedResource> returned_resources = {
      resource.ToReturnedResource()};
  EXPECT_TRUE(GetSurfaceForId(id));
  EXPECT_CALL(mock_client, DidReceiveCompositorFrameAck(returned_resources))
      .Times(1);
  support->EvictSurface(local_surface_id);
  ExpireAllTemporaryReferences();
  manager_.surface_manager()->GarbageCollectSurfaces();
  EXPECT_FALSE(GetSurfaceForId(id));
  manager_.InvalidateFrameSinkId(kAnotherArbitraryFrameSinkId);
}

// This test checks the case where a client submits a CompositorFrame for a
// SurfaceId that has been evicted. The CompositorFrame must be immediately
// evicted.
TEST_F(CompositorFrameSinkSupportTest, ResurectAndImmediatelyEvict) {
  LocalSurfaceId local_surface_id(1, kArbitraryToken);
  SurfaceId surface_id(kArbitraryFrameSinkId, local_surface_id);

  auto frame = CompositorFrameBuilder().AddDefaultRenderPass().Build();
  support_->SubmitCompositorFrame(local_surface_id, std::move(frame));

  // The surface should be activated now.
  EXPECT_EQ(support_->last_activated_surface_id(), surface_id);

  // Evict the surface. Make surface CompositorFrameSinkSupport reflects this.
  manager_.EvictSurfaces({surface_id});
  EXPECT_FALSE(support_->last_activated_surface_id().is_valid());

  // We don't garbage collect the evicted surface yet because either garbage
  // collection hasn't run or something still has a reference to it.

  // Submit the late CompositorFrame which will resurrect the Surface and
  // trigger another eviction.
  frame = CompositorFrameBuilder().AddDefaultRenderPass().Build();
  support_->SubmitCompositorFrame(local_surface_id, std::move(frame));

  // The Surface should be evicted again immediately after it's submitted.
  EXPECT_FALSE(support_->last_activated_surface_id().is_valid());
}

// Verify that a temporary reference does not block surface eviction.
TEST_F(CompositorFrameSinkSupportTest, EvictSurfaceWithTemporaryReference) {
  constexpr FrameSinkId parent_frame_sink_id(1234, 5678);

  manager_.RegisterFrameSinkId(parent_frame_sink_id,
                               true /* report_activation */);

  const LocalSurfaceId local_surface_id(5, kArbitraryToken);
  const SurfaceId surface_id(support_->frame_sink_id(), local_surface_id);

  // When CompositorFrame is submitted, a temporary reference will be created.
  support_->SubmitCompositorFrame(local_surface_id,
                                  MakeDefaultCompositorFrame());
  EXPECT_TRUE(HasTemporaryReference(surface_id));

  // Verify the temporary reference has not prevented the surface from getting
  // destroyed.
  support_->EvictSurface(local_surface_id);
  manager_.surface_manager()->GarbageCollectSurfaces();
  EXPECT_FALSE(GetSurfaceForId(surface_id));
}

// Verifies that evicting a surface destroys all older surfaces as well.
TEST_F(CompositorFrameSinkSupportTest, EvictOlderSurfaces) {
  constexpr FrameSinkId parent_frame_sink_id(1234, 5678);

  manager_.RegisterFrameSinkId(parent_frame_sink_id,
                               true /* report_activation */);

  const LocalSurfaceId local_surface_id1(5, kArbitraryToken);
  const LocalSurfaceId local_surface_id2(6, kArbitraryToken);
  const SurfaceId surface_id1(support_->frame_sink_id(), local_surface_id1);
  const SurfaceId surface_id2(support_->frame_sink_id(), local_surface_id2);

  // When CompositorFrame is submitted, a temporary reference will be created.
  support_->SubmitCompositorFrame(local_surface_id1,
                                  MakeDefaultCompositorFrame());
  EXPECT_TRUE(HasTemporaryReference(surface_id1));

  // Evict |surface_id2|. |surface_id1| should be evicted too.
  support_->EvictSurface(local_surface_id2);
  manager_.surface_manager()->GarbageCollectSurfaces();
  EXPECT_FALSE(GetSurfaceForId(surface_id1));
}

void CopyRequestTestCallback(bool* called,
                             std::unique_ptr<CopyOutputResult> result) {
  *called = true;
}

TEST_F(CompositorFrameSinkSupportTest, DuplicateCopyRequest) {
  const SurfaceId surface_id(support_->frame_sink_id(), local_surface_id_);

  {
    auto frame = CompositorFrameBuilder()
                     .AddDefaultRenderPass()
                     .SetReferencedSurfaces({SurfaceRange(surface_id)})
                     .Build();
    support_->SubmitCompositorFrame(local_surface_id_, std::move(frame));
    EXPECT_EQ(surface_observer_.last_created_surface_id().local_surface_id(),
              local_surface_id_);
  }

  bool called1 = false;
  auto request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(&CopyRequestTestCallback, &called1));
  request->set_source(kArbitrarySourceId1);

  support_->RequestCopyOfOutput(local_surface_id_, std::move(request));
  GetSurfaceForId(surface_id)->TakeCopyOutputRequestsFromClient();
  EXPECT_FALSE(called1);

  bool called2 = false;
  request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(&CopyRequestTestCallback, &called2));
  request->set_source(kArbitrarySourceId2);

  support_->RequestCopyOfOutput(local_surface_id_, std::move(request));
  GetSurfaceForId(surface_id)->TakeCopyOutputRequestsFromClient();
  // Callbacks have different sources so neither should be called.
  EXPECT_FALSE(called1);
  EXPECT_FALSE(called2);

  bool called3 = false;
  request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(&CopyRequestTestCallback, &called3));
  request->set_source(kArbitrarySourceId1);

  support_->RequestCopyOfOutput(local_surface_id_, std::move(request));
  GetSurfaceForId(surface_id)->TakeCopyOutputRequestsFromClient();
  // Two callbacks are from source1, so the first should be called.
  EXPECT_TRUE(called1);
  EXPECT_FALSE(called2);
  EXPECT_FALSE(called3);

  support_->EvictSurface(local_surface_id_);
  ExpireAllTemporaryReferences();
  local_surface_id_ = LocalSurfaceId();
  manager_.surface_manager()->GarbageCollectSurfaces();
  EXPECT_TRUE(called1);
  EXPECT_TRUE(called2);
  EXPECT_TRUE(called3);
}

// Check whether the SurfaceInfo object is created and populated correctly
// after the frame submission.
TEST_F(CompositorFrameSinkSupportTest, SurfaceInfo) {
  auto frame = CompositorFrameBuilder()
                   .AddRenderPass(gfx::Rect(5, 6), gfx::Rect())
                   .AddRenderPass(gfx::Rect(7, 8), gfx::Rect())
                   .SetDeviceScaleFactor(2.5f)
                   .Build();

  support_->SubmitCompositorFrame(local_surface_id_, std::move(frame));
  SurfaceId expected_surface_id(support_->frame_sink_id(), local_surface_id_);
  EXPECT_EQ(expected_surface_id, surface_observer_.last_surface_info().id());
  EXPECT_EQ(2.5f, surface_observer_.last_surface_info().device_scale_factor());
  EXPECT_EQ(gfx::Size(7, 8),
            surface_observer_.last_surface_info().size_in_pixels());
}

// Check that if the size of a CompositorFrame doesn't match the size of the
// Surface it's being submitted to, we skip the frame.
TEST_F(CompositorFrameSinkSupportTest, FrameSizeMismatch) {
  SurfaceId id(support_->frame_sink_id(), local_surface_id_);

  // Submit a frame with size (5,5).
  auto frame = CompositorFrameBuilder()
                   .AddRenderPass(gfx::Rect(5, 5), gfx::Rect())
                   .Build();
  auto result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);
  EXPECT_TRUE(GetSurfaceForId(id));

  // Submit a frame with size (5,4). This frame should be rejected and the
  // surface should be destroyed.
  frame = CompositorFrameBuilder()
              .AddRenderPass(gfx::Rect(5, 4), gfx::Rect())
              .Build();
  ResourceId frame_resource_ids[] = {1, 2, 3};
  AddResourcesToFrame(&frame, frame_resource_ids,
                      base::size(frame_resource_ids));

  result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());

  EXPECT_EQ(SubmitResult::SIZE_MISMATCH, result);

  // All the resources in the rejected frame should have been returned.
  CheckReturnedResourcesMatchExpected(frame_resource_ids,
                                      base::size(frame_resource_ids));
}

// Check that if the device scale factor of a CompositorFrame doesn't match the
// device scale factor of the Surface it's being submitted to, the frame is
// rejected and the surface is destroyed.
TEST_F(CompositorFrameSinkSupportTest, DeviceScaleFactorMismatch) {
  SurfaceId id(support_->frame_sink_id(), local_surface_id_);

  // Submit a frame with device scale factor of 0.5.
  auto frame = CompositorFrameBuilder()
                   .AddDefaultRenderPass()
                   .SetDeviceScaleFactor(0.5f)
                   .Build();
  auto result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);
  EXPECT_TRUE(GetSurfaceForId(id));

  // Submit a frame with device scale factor of 0.4. This frame should be
  // rejected and the surface should be destroyed.
  frame = CompositorFrameBuilder()
              .AddDefaultRenderPass()
              .SetDeviceScaleFactor(0.4f)
              .Build();
  result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::SIZE_MISMATCH, result);
}

TEST_F(CompositorFrameSinkSupportTest, PassesOnBeginFrameAcks) {
  // Request BeginFrames.
  support_->SetNeedsBeginFrame(true);

  // Issue a BeginFrame.
  BeginFrameArgs args =
      CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, 0, 1);
  begin_frame_source_.TestOnBeginFrame(args);

  // Check that the support and SurfaceManager forward the BeginFrameAck
  // attached to a CompositorFrame to the SurfaceObserver.
  BeginFrameAck ack(0, 1, true);
  CompositorFrame frame = CompositorFrameBuilder()
                              .AddDefaultRenderPass()
                              .SetBeginFrameAck(ack)
                              .Build();
  support_->SubmitCompositorFrame(local_surface_id_, std::move(frame));
  EXPECT_EQ(ack, surface_observer_.last_ack());

  // Issue another BeginFrame.
  args = CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, 0, 2);
  begin_frame_source_.TestOnBeginFrame(args);

  // Check that the support and SurfaceManager forward a DidNotProduceFrame ack
  // to the SurfaceObserver.
  BeginFrameAck ack2(0, 2, false);
  support_->DidNotProduceFrame(ack2);
  EXPECT_EQ(ack2, surface_observer_.last_ack());

  support_->SetNeedsBeginFrame(false);
}

// Validates that if a client asked to stop receiving begin-frames, then it
// stops receiving begin-frames after receiving the presentation-feedback from
// the last submitted frame.
TEST_F(CompositorFrameSinkSupportTest,
       NeedsBeginFrameResetAfterPresentationFeedback) {
  // Request BeginFrames.
  support_->SetNeedsBeginFrame(true);

  // Issue a BeginFrame. Validate that the client receives the begin-frame.
  BeginFrameArgs args =
      CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, 0, 1);
  begin_frame_source_.TestOnBeginFrame(args);
  BeginFrameArgs received_args = GetLastUsedBeginFrameArgs(support_.get());
  EXPECT_TRUE(BeginFrameArgsAreEquivalent(args, received_args));
  EXPECT_EQ(received_args.type, BeginFrameArgs::NORMAL);

  // Client submits a compositor frame in response.
  BeginFrameAck ack(args, true);
  CompositorFrame frame = CompositorFrameBuilder()
                              .AddDefaultRenderPass()
                              .SetBeginFrameAck(ack)
                              .Build();
  auto token = frame.metadata.frame_token;
  support_->SubmitCompositorFrame(local_surface_id_, std::move(frame));

  // Client stops asking for begin-frames.
  support_->SetNeedsBeginFrame(false);

  // Issue a new BeginFrame. This time, the client should not receive it since
  // it has stopped asking for begin-frames.
  args = CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, 1, 2);
  begin_frame_source_.TestOnBeginFrame(args);
  received_args = GetLastUsedBeginFrameArgs(support_.get());
  EXPECT_FALSE(BeginFrameArgsAreEquivalent(args, received_args));

  // The presentation-feedback from the last submitted frame arrives. This
  // results in the client immediately receiving a MISSED begin-frame.
  SendPresentationFeedback(support_.get(), token);
  received_args = GetLastUsedBeginFrameArgs(support_.get());
  EXPECT_TRUE(BeginFrameArgsAreEquivalent(args, received_args));
  EXPECT_EQ(received_args.type, BeginFrameArgs::MISSED);

  // Issue another begin-frame. This time, the client should not receive it
  // anymore since it has stopped asking for begin-frames, and it has already
  // received the last presentation-feedback.
  args = CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, 2, 3);
  begin_frame_source_.TestOnBeginFrame(args);
  received_args = GetLastUsedBeginFrameArgs(support_.get());
  EXPECT_FALSE(BeginFrameArgsAreEquivalent(args, received_args));
}

TEST_F(CompositorFrameSinkSupportTest, FrameIndexCarriedOverToNewSurface) {
  LocalSurfaceId local_surface_id1(1, kArbitraryToken);
  LocalSurfaceId local_surface_id2(2, kArbitraryToken);
  SurfaceId id1(support_->frame_sink_id(), local_surface_id1);
  SurfaceId id2(support_->frame_sink_id(), local_surface_id2);

  // Submit a frame to |id1| and record the frame index.
  support_->SubmitCompositorFrame(local_surface_id1,
                                  MakeDefaultCompositorFrame());
  Surface* surface1 = GetSurfaceForId(id1);
  uint64_t frame_index = surface1->GetActiveFrameIndex();

  // Submit a frame to |id2| and verify that the new frame index is one more
  // than what we had before.
  support_->SubmitCompositorFrame(local_surface_id2,
                                  MakeDefaultCompositorFrame());
  Surface* surface2 = GetSurfaceForId(id2);
  EXPECT_EQ(frame_index + 1, surface2->GetActiveFrameIndex());
}

// Verifies that CopyOutputRequests made at frame sink level are sent to the
// surface that takes them first. In this test this surface is not the last
// activated surface.
TEST_F(CompositorFrameSinkSupportTest,
       OldSurfaceTakesCopyOutputRequestsFromClient) {
  LocalSurfaceId local_surface_id1(1, kArbitraryToken);
  LocalSurfaceId local_surface_id2(2, kArbitraryToken);
  SurfaceId id1(support_->frame_sink_id(), local_surface_id1);
  SurfaceId id2(support_->frame_sink_id(), local_surface_id2);

  // Create the first surface.
  support_->SubmitCompositorFrame(local_surface_id1,
                                  MakeDefaultCompositorFrame());

  // Create the second surface.
  support_->SubmitCompositorFrame(local_surface_id2,
                                  MakeDefaultCompositorFrame());

  // Send a CopyOutputRequest.
  auto request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(StubResultCallback));
  support_->RequestCopyOfOutput(local_surface_id1, std::move(request));

  // First surface takes CopyOutputRequests from its client. Now only the first
  // surface should report having CopyOutputRequests.
  GetSurfaceForId(id1)->TakeCopyOutputRequestsFromClient();
  EXPECT_TRUE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_FALSE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Calling TakeCopyOutputRequestsFromClient() on the second surface should
  // have no effect.
  GetSurfaceForId(id2)->TakeCopyOutputRequestsFromClient();
  EXPECT_TRUE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_FALSE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Also try TakeCopyOutputRequests, to see if its output is consistent with
  // HasCopyOutputRequests.
  Surface::CopyRequestsMap requests_map;
  GetSurfaceForId(id2)->TakeCopyOutputRequests(&requests_map);
  EXPECT_TRUE(requests_map.empty());
  GetSurfaceForId(id1)->TakeCopyOutputRequests(&requests_map);
  EXPECT_FALSE(requests_map.empty());
}

TEST_F(CompositorFrameSinkSupportTest,
       OldSurfaceDoesNotTakeCopyOutputRequestsFromNewLocalId) {
  LocalSurfaceId local_surface_id1(1, kArbitraryToken);
  LocalSurfaceId local_surface_id2(2, kArbitraryToken);
  SurfaceId id1(support_->frame_sink_id(), local_surface_id1);
  SurfaceId id2(support_->frame_sink_id(), local_surface_id2);

  // Create the first surface.
  support_->SubmitCompositorFrame(local_surface_id1,
                                  MakeDefaultCompositorFrame());

  // Create the second surface.
  support_->SubmitCompositorFrame(local_surface_id2,
                                  MakeDefaultCompositorFrame());

  // Send a CopyOutputRequest.
  auto request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(StubResultCallback));
  support_->RequestCopyOfOutput(local_surface_id2, std::move(request));

  // The first surface doesn't have copy output requests, because it can't
  // satisfy the request that the client has.
  GetSurfaceForId(id1)->TakeCopyOutputRequestsFromClient();
  EXPECT_FALSE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_FALSE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Second surface should succeed at taking the requests.
  GetSurfaceForId(id2)->TakeCopyOutputRequestsFromClient();
  EXPECT_FALSE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_TRUE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Do TakeCopyOutputRequests, to see if its output is consistent with
  // HasCopyOutputRequests.
  Surface::CopyRequestsMap requests_map;
  GetSurfaceForId(id1)->TakeCopyOutputRequests(&requests_map);
  EXPECT_TRUE(requests_map.empty());
  GetSurfaceForId(id2)->TakeCopyOutputRequests(&requests_map);
  EXPECT_FALSE(requests_map.empty());
}

// Verifies that CopyOutputRequests made at frame sink level are sent to the
// surface that takes them first. In this test this surface is the last
// activated surface.
TEST_F(CompositorFrameSinkSupportTest,
       LastSurfaceTakesCopyOutputRequestsFromClient) {
  LocalSurfaceId local_surface_id1(1, kArbitraryToken);
  LocalSurfaceId local_surface_id2(2, kArbitraryToken);
  SurfaceId id1(support_->frame_sink_id(), local_surface_id1);
  SurfaceId id2(support_->frame_sink_id(), local_surface_id2);

  // Create the first surface.
  support_->SubmitCompositorFrame(local_surface_id1,
                                  MakeDefaultCompositorFrame());

  // Send a CopyOutputRequest. Note that the second surface doesn't even exist
  // yet.
  auto request = std::make_unique<CopyOutputRequest>(
      CopyOutputRequest::ResultFormat::RGBA_BITMAP,
      base::BindOnce(StubResultCallback));
  support_->RequestCopyOfOutput(local_surface_id1, std::move(request));

  // Create the second surface.
  support_->SubmitCompositorFrame(local_surface_id2,
                                  MakeDefaultCompositorFrame());

  // Second surface takes CopyOutputRequests from its client. Now only the
  // second surface should report having CopyOutputRequests.
  GetSurfaceForId(id2)->TakeCopyOutputRequestsFromClient();
  EXPECT_FALSE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_TRUE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Calling TakeCopyOutputRequestsFromClient() on the first surface should have
  // no effect.
  GetSurfaceForId(id1)->TakeCopyOutputRequestsFromClient();
  EXPECT_FALSE(GetSurfaceForId(id1)->HasCopyOutputRequests());
  EXPECT_TRUE(GetSurfaceForId(id2)->HasCopyOutputRequests());

  // Also try TakeCopyOutputRequests, to see if its output is consistent with
  // HasCopyOutputRequests.
  Surface::CopyRequestsMap requests_map;
  GetSurfaceForId(id1)->TakeCopyOutputRequests(&requests_map);
  EXPECT_TRUE(requests_map.empty());
  GetSurfaceForId(id2)->TakeCopyOutputRequests(&requests_map);
  EXPECT_FALSE(requests_map.empty());
}

// Verifies that OnFrameTokenUpdate is issued after OnFirstSurfaceActivation.
TEST_F(CompositorFrameSinkSupportTest,
       OnFrameTokenUpdateAfterFirstSurfaceActivation) {
  LocalSurfaceId local_surface_id(1, kArbitraryToken);
  auto frame = CompositorFrameBuilder()
                   .AddDefaultRenderPass()
                   .SetSendFrameTokenToEmbedder(true)
                   .Build();
  uint32_t frame_token = frame.metadata.frame_token;
  ASSERT_NE(frame_token, 0u);

  testing::InSequence sequence;
  EXPECT_CALL(frame_sink_manager_client_, OnFirstSurfaceActivation(_));
  EXPECT_CALL(frame_sink_manager_client_, OnFrameTokenChanged(_, frame_token));
  support_->SubmitCompositorFrame(local_surface_id, std::move(frame));
}

// This test verifies that it is not possible to reuse the same embed token in
// two different frame sinks.
TEST_F(CompositorFrameSinkSupportTest,
       DisallowEmbedTokenReuseAcrossFrameSinks) {
  auto result = support_->MaybeSubmitCompositorFrame(
      local_surface_id_, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  // Create another sink and reuse the same embed token to submit a frame. The
  // frame should be rejected.
  MockCompositorFrameSinkClient mock_client;
  auto support = std::make_unique<CompositorFrameSinkSupport>(
      &mock_client, &manager_, kAnotherArbitraryFrameSinkId,
      false /* not root frame sink */, kNeedsSyncPoints);
  LocalSurfaceId local_surface_id(31232, local_surface_id_.embed_token());
  result = support->MaybeSubmitCompositorFrame(
      local_surface_id, MakeDefaultCompositorFrame(), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::SURFACE_OWNED_BY_ANOTHER_CLIENT, result);
}

// This test verifies that the parent sequence number of the submitted
// CompositorFrames can decrease as long as the embed token changes as well.
TEST_F(CompositorFrameSinkSupportTest, SubmitAfterReparenting) {
  LocalSurfaceId local_surface_id1(2, base::UnguessableToken::Create());
  LocalSurfaceId local_surface_id2(1, base::UnguessableToken::Create());

  ASSERT_NE(local_surface_id1.embed_token(), local_surface_id2.embed_token());

  CompositorFrame frame =
      CompositorFrameBuilder().AddDefaultRenderPass().Build();
  SubmitResult result = support_->MaybeSubmitCompositorFrame(
      local_surface_id1, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());
  EXPECT_EQ(SubmitResult::ACCEPTED, result);

  frame = CompositorFrameBuilder().AddDefaultRenderPass().Build();
  result = support_->MaybeSubmitCompositorFrame(
      local_surface_id2, std::move(frame), base::nullopt, 0,
      mojom::CompositorFrameSink::SubmitCompositorFrameSyncCallback());

  // Even though |local_surface_id2| has a smaller parent sequence number than
  // |local_surface_id1|, the submit should still succeed because it has a
  // different embed token.
  EXPECT_EQ(SubmitResult::ACCEPTED, result);
}

// This test verifies that surfaces created with a new embed token are not
// compared against the evicted parent sequence number of the previous embed
// token.
TEST_F(CompositorFrameSinkSupportTest, EvictThenReparent) {
  LocalSurfaceId local_surface_id1(2, base::UnguessableToken::Create());
  LocalSurfaceId local_surface_id2(1, base::UnguessableToken::Create());

  ASSERT_NE(local_surface_id1.embed_token(), local_surface_id2.embed_token());

  support_->EvictSurface(local_surface_id1);
  CompositorFrame frame =
      CompositorFrameBuilder().AddDefaultRenderPass().Build();
  support_->SubmitCompositorFrame(local_surface_id2, std::move(frame));
  manager_.surface_manager()->GarbageCollectSurfaces();

  // Even though |local_surface_id2| has a smaller parent sequence number than
  // |local_surface_id1|, it should not be evicted because it has a different
  // embed token.
  EXPECT_TRUE(
      GetSurfaceForId(SurfaceId(support_->frame_sink_id(), local_surface_id2)));
}

}  // namespace viz
