/*
 * Copyright (c) 2013, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/animation/animation_effect.h"
#include "third_party/blink/renderer/core/animation/timing_calculations.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace blink {

TEST(AnimationTimingCalculationsTest, ActiveTime) {
  Timing timing;

  // calculateActiveTime(
  //     activeDuration, fillMode, localTime, parentPhase, phase, timing)

  // Before Phase
  timing.start_delay = 10;
  EXPECT_TRUE(
      IsNull(CalculateActiveTime(20, Timing::FillMode::FORWARDS, 0,
                                 AnimationEffect::kPhaseBefore, timing)));
  EXPECT_TRUE(IsNull(CalculateActiveTime(
      20, Timing::FillMode::NONE, 0, AnimationEffect::kPhaseBefore, timing)));
  EXPECT_EQ(0, CalculateActiveTime(20, Timing::FillMode::BACKWARDS, 0,
                                   AnimationEffect::kPhaseBefore, timing));
  EXPECT_EQ(0, CalculateActiveTime(20, Timing::FillMode::BOTH, 0,
                                   AnimationEffect::kPhaseBefore, timing));
  timing.start_delay = -10;
  EXPECT_EQ(5, CalculateActiveTime(20, Timing::FillMode::BACKWARDS, -5,
                                   AnimationEffect::kPhaseBefore, timing));

  // Active Phase
  timing.start_delay = 10;
  EXPECT_EQ(5, CalculateActiveTime(20, Timing::FillMode::FORWARDS, 15,
                                   AnimationEffect::kPhaseActive, timing));

  // After Phase
  timing.start_delay = 10;
  EXPECT_EQ(21, CalculateActiveTime(21, Timing::FillMode::FORWARDS, 45,
                                    AnimationEffect::kPhaseAfter, timing));
  EXPECT_EQ(21, CalculateActiveTime(21, Timing::FillMode::BOTH, 45,
                                    AnimationEffect::kPhaseAfter, timing));
  EXPECT_TRUE(
      IsNull(CalculateActiveTime(21, Timing::FillMode::BACKWARDS, 45,
                                 AnimationEffect::kPhaseAfter, timing)));
  EXPECT_TRUE(IsNull(CalculateActiveTime(
      21, Timing::FillMode::NONE, 45, AnimationEffect::kPhaseAfter, timing)));

  // None
  EXPECT_TRUE(
      IsNull(CalculateActiveTime(32, Timing::FillMode::NONE, NullValue(),
                                 AnimationEffect::kPhaseNone, timing)));
}

TEST(AnimationTimingCalculationsTest, OffsetActiveTime) {
  // if the active time is null
  EXPECT_TRUE(IsNull(CalculateOffsetActiveTime(4, NullValue(), 5)));

  // normal case
  EXPECT_EQ(15, CalculateOffsetActiveTime(40, 10, 5));

  // infinte activeTime
  EXPECT_EQ(
      std::numeric_limits<double>::infinity(),
      CalculateOffsetActiveTime(std::numeric_limits<double>::infinity(),
                                std::numeric_limits<double>::infinity(), 0));
}

TEST(AnimationTimingCalculationsTest, IterationTime) {
  Timing timing;

  // calculateIterationTime(
  //     iterationDuration, repeatedDuration, scaledActiveTime, startOffset,
  //     phase, timing)

  // if the scaled active time is null
  EXPECT_TRUE(IsNull(CalculateIterationTime(
      1, 1, NullValue(), 1, AnimationEffect::kPhaseActive, timing)));

  // if (complex-conditions)...
  EXPECT_EQ(12, CalculateIterationTime(12, 12, 12, 0,
                                       AnimationEffect::kPhaseActive, timing));

  // otherwise
  timing.iteration_count = 10;
  EXPECT_EQ(5, CalculateIterationTime(10, 100, 25, 4,
                                      AnimationEffect::kPhaseActive, timing));
  EXPECT_EQ(7, CalculateIterationTime(11, 110, 29, 1,
                                      AnimationEffect::kPhaseActive, timing));
  timing.iteration_start = 1.1;
  EXPECT_EQ(8, CalculateIterationTime(12, 120, 20, 7,
                                      AnimationEffect::kPhaseActive, timing));
}

TEST(AnimationTimingCalculationsTest, OverallProgress) {
  // If the active time is null.
  EXPECT_TRUE(IsNull(CalculateOverallProgress(AnimationEffect::kPhaseAfter,
                                              /*active_time=*/NullValue(),
                                              /*iteration_duration=*/1.0,
                                              /*iteration_count=*/1.0,
                                              /*iteration_start=*/1.0)));

  // If iteration duration is zero, calculate progress based on iteration count.
  EXPECT_EQ(3, CalculateOverallProgress(AnimationEffect::kPhaseActive,
                                        /*active_time=*/3.0,
                                        /*iteration_duration=*/0.0,
                                        /*iteration_count=*/3.0,
                                        /*iteration_start=*/0.0));
  // ...unless in before phase, in which case progress is zero.
  EXPECT_EQ(0, CalculateOverallProgress(AnimationEffect::kPhaseBefore,
                                        /*active_time=*/3.0,
                                        /*iteration_duration=*/0.0,
                                        /*iteration_count=*/3.0,
                                        /*iteration_start=*/0.0));
  // Edge case for duration being within Epsilon of zero.
  // crbug.com/954558
  EXPECT_EQ(1, CalculateOverallProgress(AnimationEffect::kPhaseActive,
                                        /*active_time=*/3.0,
                                        /*iteration_duration=*/1e-18,
                                        /*iteration_count=*/1.0,
                                        /*iteration_start=*/0.0));

  // Otherwise.
  EXPECT_EQ(3.0, CalculateOverallProgress(AnimationEffect::kPhaseAfter,
                                          /*active_time=*/2.5,
                                          /*iteration_duration=*/1.0,
                                          /*iteration_count=*/0.0,
                                          /*iteration_start=*/0.5));
}

TEST(AnimationTimingCalculationsTest, CalculateSimpleIterationProgress) {
  // If the overall progress is null.
  EXPECT_TRUE(
      IsNull(CalculateSimpleIterationProgress(AnimationEffect::kPhaseAfter,
                                              /*overall_progress=*/NullValue(),
                                              /*iteration_start=*/1.0,
                                              /*active_time=*/NullValue(),
                                              /*active_duration=*/1.0,
                                              /*iteration_count=*/1.0)));

  // If the overall progress is infinite.
  const double inf = std::numeric_limits<double>::infinity();
  EXPECT_EQ(0.5, CalculateSimpleIterationProgress(AnimationEffect::kPhaseAfter,
                                                  /*overall_progress=*/inf,
                                                  /*iteration_start=*/1.5,
                                                  /*active_time=*/0.0,
                                                  /*active_duration=*/0.0,
                                                  /*iteration_count=*/inf));

  // Precisely on an iteration boundary.
  EXPECT_EQ(1.0, CalculateSimpleIterationProgress(AnimationEffect::kPhaseAfter,
                                                  /*overall_progress=*/3.0,
                                                  /*iteration_start=*/0.0,
                                                  /*active_time=*/3.0,
                                                  /*active_duration=*/3.0,
                                                  /*iteration_count=*/3.0));

  // Otherwise.
  EXPECT_EQ(0.5, CalculateSimpleIterationProgress(AnimationEffect::kPhaseAfter,
                                                  /*overall_progress=*/2.5,
                                                  /*iteration_start=*/0.0,
                                                  /*active_time=*/2.5,
                                                  /*active_duration=*/0.0,
                                                  /*iteration_count=*/0.0));
}

TEST(AnimationTimingCalculationsTest, CurrentIteration) {
  // If the active time is null.
  EXPECT_TRUE(
      IsNull(CalculateCurrentIteration(AnimationEffect::kPhaseAfter,
                                       /*active_time=*/NullValue(),
                                       /*iteration_count=*/1.0,
                                       /*overall_progress=*/NullValue(),
                                       /*simple_iteration_progress=*/0)));

  // If the iteration count is infinite.
  const double inf = std::numeric_limits<double>::infinity();
  EXPECT_EQ(inf, CalculateCurrentIteration(AnimationEffect::kPhaseAfter,
                                           /*active_time=*/1.0,
                                           /*iteration_count=*/inf,
                                           /*overall_progress=*/inf,
                                           /*simple_iteration_progress=*/0.0));

  // Hold the endpoint of the final iteration of ending precisely on an
  // iteration boundary.
  EXPECT_EQ(2, CalculateCurrentIteration(AnimationEffect::kPhaseAfter,
                                         /*active_time=*/3.0,
                                         /*iteration_count=*/3.0,
                                         /*overall_progress=*/3.0,
                                         /*simple_iteration_progress=*/1.0));

  // Edge case for zero-duration animation.
  // crbug.com/954558
  EXPECT_EQ(0, CalculateCurrentIteration(AnimationEffect::kPhaseAfter,
                                         /*active_time=*/0.0,
                                         /*iteration_count=*/1.0,
                                         /*overall_progress=*/0.0,
                                         /*simple_iteration_progress=*/1.0));

  // Otherwise.
  EXPECT_EQ(2, CalculateCurrentIteration(AnimationEffect::kPhaseAfter,
                                         /*active_time=*/2.5,
                                         /*iteration_count=*/0.0,
                                         /*overall_progress=*/2.5,
                                         /*simple_iteration_progress=*/0.5));
}

TEST(AnimationTimingCalculationsTest, IsCurrentDirectionForwards) {
  // IsCurrentDirectionForwards(current_iteration,
  //                            direction);

  EXPECT_TRUE(IsCurrentDirectionForwards(0, Timing::PlaybackDirection::NORMAL));
  EXPECT_TRUE(IsCurrentDirectionForwards(1, Timing::PlaybackDirection::NORMAL));
  EXPECT_TRUE(IsCurrentDirectionForwards(
      0, Timing::PlaybackDirection::ALTERNATE_NORMAL));
  EXPECT_TRUE(IsCurrentDirectionForwards(
      1, Timing::PlaybackDirection::ALTERNATE_REVERSE));

  EXPECT_FALSE(
      IsCurrentDirectionForwards(0, Timing::PlaybackDirection::REVERSE));
  EXPECT_FALSE(
      IsCurrentDirectionForwards(1, Timing::PlaybackDirection::REVERSE));
  EXPECT_FALSE(IsCurrentDirectionForwards(
      0, Timing::PlaybackDirection::ALTERNATE_REVERSE));
  EXPECT_FALSE(IsCurrentDirectionForwards(
      1, Timing::PlaybackDirection::ALTERNATE_NORMAL));
}

TEST(AnimationTimingCalculationsTest, CalculateDirectedProgress) {
  // CalculateDirectedProgress(simple_iteration_progress,
  //                           current_iteration,
  //                           direction);

  // if the simple iteration progress is null
  EXPECT_TRUE(IsNull(CalculateDirectedProgress(
      NullValue(), NullValue(), Timing::PlaybackDirection::NORMAL)));

  // forwards
  EXPECT_EQ(0,
            CalculateDirectedProgress(0, 8, Timing::PlaybackDirection::NORMAL));
  EXPECT_EQ(1,
            CalculateDirectedProgress(1, 8, Timing::PlaybackDirection::NORMAL));
  EXPECT_EQ(0,
            CalculateDirectedProgress(0, 9, Timing::PlaybackDirection::NORMAL));
  EXPECT_EQ(1,
            CalculateDirectedProgress(1, 9, Timing::PlaybackDirection::NORMAL));
  EXPECT_EQ(0, CalculateDirectedProgress(
                   0, 8, Timing::PlaybackDirection::ALTERNATE_NORMAL));
  EXPECT_EQ(1, CalculateDirectedProgress(
                   1, 8, Timing::PlaybackDirection::ALTERNATE_NORMAL));
  EXPECT_EQ(0, CalculateDirectedProgress(
                   0, 9, Timing::PlaybackDirection::ALTERNATE_REVERSE));
  EXPECT_EQ(1, CalculateDirectedProgress(
                   1, 9, Timing::PlaybackDirection::ALTERNATE_REVERSE));

  // reverse
  EXPECT_EQ(
      1, CalculateDirectedProgress(0, 8, Timing::PlaybackDirection::REVERSE));
  EXPECT_EQ(
      0, CalculateDirectedProgress(1, 8, Timing::PlaybackDirection::REVERSE));
  EXPECT_EQ(
      1, CalculateDirectedProgress(0, 9, Timing::PlaybackDirection::REVERSE));
  EXPECT_EQ(
      0, CalculateDirectedProgress(1, 9, Timing::PlaybackDirection::REVERSE));
  EXPECT_EQ(1, CalculateDirectedProgress(
                   0, 9, Timing::PlaybackDirection::ALTERNATE_NORMAL));
  EXPECT_EQ(0, CalculateDirectedProgress(
                   1, 9, Timing::PlaybackDirection::ALTERNATE_NORMAL));
  EXPECT_EQ(1, CalculateDirectedProgress(
                   0, 8, Timing::PlaybackDirection::ALTERNATE_REVERSE));
  EXPECT_EQ(0, CalculateDirectedProgress(
                   1, 8, Timing::PlaybackDirection::ALTERNATE_REVERSE));
}

TEST(AnimationTimingCalculationsTest, TransformedProgress) {
  // CalculateTransformedProgress(
  //     directed_progress, iteration_duraction, timing_function)

  scoped_refptr<TimingFunction> timing_function =
      StepsTimingFunction::Create(4, StepsTimingFunction::StepPosition::END);

  // directed_progress is null.
  EXPECT_TRUE(
      IsNull(CalculateTransformedProgress(NullValue(), 1, timing_function)));

  // Otherwise.
  EXPECT_EQ(0, CalculateTransformedProgress(0, 1, timing_function));
  EXPECT_EQ(1, CalculateTransformedProgress(1, 1, timing_function));
}

}  // namespace blink
