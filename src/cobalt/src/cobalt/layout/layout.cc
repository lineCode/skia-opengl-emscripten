// Copyright 2014 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/layout/layout.h"

#include "base/trace_event/trace_event.h"
#include "cobalt/base/stop_watch.h"
#include "cobalt/cssom/computed_style.h"
#include "cobalt/cssom/css_style_declaration.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/html_body_element.h"
#include "cobalt/dom/html_element_context.h"
#include "cobalt/dom/html_html_element.h"
#include "cobalt/layout/benchmark_stat_names.h"
#include "cobalt/layout/box_generator.h"
#include "cobalt/layout/initial_containing_block.h"
#include "cobalt/layout/used_style.h"
#include "cobalt/render_tree/animations/animate_node.h"

namespace cobalt {
namespace layout {

namespace {

class ScopedParagraph {
 public:
  explicit ScopedParagraph(const scoped_refptr<Paragraph>& paragraph)
      : paragraph_(paragraph) {}

  ~ScopedParagraph() { paragraph_->Close(); }

  scoped_refptr<Paragraph>& get() { return paragraph_; }

 private:
  scoped_refptr<Paragraph> paragraph_;
};

}  // namespace

void UpdateComputedStylesAndLayoutBoxTree(
    const icu::Locale& locale, const scoped_refptr<dom::Document>& document,
    int dom_max_element_depth, UsedStyleProvider* used_style_provider,
    LayoutStatTracker* layout_stat_tracker,
    icu::BreakIterator* line_break_iterator,
    icu::BreakIterator* character_break_iterator,
    scoped_refptr<BlockLevelBlockContainerBox>* initial_containing_block,
    bool clear_window_with_background_color) {
//printf("UpdateComputedStylesAndLayoutBoxTree 1\n");
  //DCHECK(line_break_iterator);
  TRACE_EVENT0("cobalt::layout", "UpdateComputedStylesAndLayoutBoxTree()");
  // Layout-related cleanup is performed on the UsedStyleProvider in this
  // object's destructor.
  UsedStyleProviderLayoutScope used_style_provider_layout_scope(
      used_style_provider);

  // Update the computed style of all elements in the DOM, if necessary.
  document->UpdateComputedStyles();
#if !defined(OS_EMSCRIPTEN)
  base::StopWatch stop_watch_layout_box_tree(
      LayoutStatTracker::kStopWatchTypeLayoutBoxTree,
      base::StopWatch::kAutoStartOn, layout_stat_tracker);
#endif
  // Create initial containing block.
  InitialContainingBlockCreationResults
      initial_containing_block_creation_results = CreateInitialContainingBlock(
          *document->initial_computed_style_data(), document,
          used_style_provider, layout_stat_tracker);
  *initial_containing_block = initial_containing_block_creation_results.box;

//printf("UpdateComputedStylesAndLayoutBoxTree 2\n");
  if (clear_window_with_background_color) {
    (*initial_containing_block)->set_blend_background_color(false);
  }

  // Generate boxes.
  if (document->html()) {
    TRACE_EVENT0("cobalt::layout", kBenchmarkStatBoxGeneration);
#if !defined(OS_EMSCRIPTEN)
    base::StopWatch stop_watch_box_generation(
        LayoutStatTracker::kStopWatchTypeBoxGeneration,
        base::StopWatch::kAutoStartOn, layout_stat_tracker);
#endif

  //DCHECK(line_break_iterator);
    ScopedParagraph scoped_paragraph(
        new Paragraph(locale, (*initial_containing_block)->GetBaseDirection(),
                      Paragraph::DirectionalEmbeddingStack(),
                      line_break_iterator, character_break_iterator));
    BoxGenerator::Context context(
        used_style_provider, layout_stat_tracker, line_break_iterator,
        character_break_iterator,
        initial_containing_block_creation_results.background_style_source,
        dom_max_element_depth);
    BoxGenerator root_box_generator(
        (*initial_containing_block)->css_computed_style_declaration(),
        (*initial_containing_block)
            ->css_computed_style_declaration()
            ->animations(),
        &(scoped_paragraph.get()), 1 /* dom_element_depth */, &context);
    document->html()->Accept(&root_box_generator);
    const Boxes& root_boxes = root_box_generator.boxes();
    for (Boxes::const_iterator root_box_iterator = root_boxes.begin();
         root_box_iterator != root_boxes.end(); ++root_box_iterator) {
      (*initial_containing_block)->AddChild(*root_box_iterator);
    }
  }

  // Split bidi level runs.
  // The bidi levels were calculated for the paragraphs during box generation.
  // Now the text boxes are split between level runs, so that they will be
  // reversible during layout without requiring additional run-induced splits.
  {
    TRACE_EVENT0("cobalt::layout", "SplitBidiLevelRuns");
    (*initial_containing_block)->SplitBidiLevelRuns();
  }

  // Layout.
  {
    TRACE_EVENT0("cobalt::layout", kBenchmarkStatUpdateUsedSizes);
#if !defined(OS_EMSCRIPTEN)
    base::StopWatch stop_watch_update_used_sizes(
        LayoutStatTracker::kStopWatchTypeUpdateUsedSizes,
        base::StopWatch::kAutoStartOn, layout_stat_tracker);
#endif

    (*initial_containing_block)->set_left(LayoutUnit());
    (*initial_containing_block)->set_top(LayoutUnit());
    (*initial_containing_block)->UpdateSize(LayoutParams());
  }
//printf("UpdateComputedStylesAndLayoutBoxTree 3\n");
}

scoped_refptr<render_tree::Node> GenerateRenderTreeFromBoxTree(
    UsedStyleProvider* used_style_provider,
    LayoutStatTracker* layout_stat_tracker,
    scoped_refptr<BlockLevelBlockContainerBox>* initial_containing_block) {
  TRACE_EVENT0("cobalt::layout", "GenerateRenderTreeFromBoxTree()");
  render_tree::CompositionNode::Builder render_tree_root_builder;
  {
#if defined(__EMSCRIPTEN__)
  //EM_LOG("GenerateRenderTreeFromBoxTree 1\n");
#endif

    TRACE_EVENT0("cobalt::layout", kBenchmarkStatRenderAndAnimate);
#if !defined(OS_EMSCRIPTEN)
    base::StopWatch stop_watch_render_and_animate(
        LayoutStatTracker::kStopWatchTypeRenderAndAnimate,
        base::StopWatch::kAutoStartOn, layout_stat_tracker);
#endif

#if defined(__EMSCRIPTEN__)
  //EM_LOG("GenerateRenderTreeFromBoxTree 2\n");
#endif

    (*initial_containing_block)
        ->RenderAndAnimate(&render_tree_root_builder, math::Vector2dF(0, 0),
                           (initial_containing_block->get()));
  }

  // During computed style update and RenderAndAnimate, we get the actual images
  // that are linked to their URLs. Now go through them and update the playing
  // status for animated images.
  used_style_provider->UpdateAnimatedImages();

  render_tree::CompositionNode* static_root_node =
      new render_tree::CompositionNode(std::move(render_tree_root_builder));

  // Make it easy to animate the entire tree by placing an AnimateNode at the
  // root to merge any sub-AnimateNodes.
  render_tree::animations::AnimateNode* animate_node =
      new render_tree::animations::AnimateNode(static_root_node);

#if defined(__EMSCRIPTEN__)
  //EM_LOG("GenerateRenderTreeFromBoxTree 3\n");
#endif

  return animate_node;
}

}  // namespace layout
}  // namespace cobalt
