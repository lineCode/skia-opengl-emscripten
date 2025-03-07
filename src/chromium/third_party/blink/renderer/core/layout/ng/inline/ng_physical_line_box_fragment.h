// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NGPhysicalLineBoxFragment_h
#define NGPhysicalLineBoxFragment_h

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_line_height_metrics.h"
#include "third_party/blink/renderer/core/layout/ng/ng_physical_container_fragment.h"
#include "third_party/blink/renderer/platform/fonts/font_baseline.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace blink {

class NGLineBoxFragmentBuilder;

class CORE_EXPORT NGPhysicalLineBoxFragment final
    : public NGPhysicalContainerFragment {
 public:
  enum NGLineBoxType {
    kNormalLineBox,
    // This is an "empty" line box, or "certain zero-height line boxes":
    // https://drafts.csswg.org/css2/visuren.html#phantom-line-box
    // that are ignored for margin collapsing and for other purposes.
    // https://drafts.csswg.org/css2/box.html#collapsing-margins
    // Also see |NGInlineItem::IsEmptyItem|.
    kEmptyLineBox
  };

  static scoped_refptr<const NGPhysicalLineBoxFragment> Create(
      NGLineBoxFragmentBuilder* builder);

  ~NGPhysicalLineBoxFragment() {
    for (const NGLinkStorage& child : Children())
      child.fragment->Release();
  }

  NGLineBoxType LineBoxType() const {
    return static_cast<NGLineBoxType>(sub_type_);
  }
  bool IsEmptyLineBox() const { return LineBoxType() == kEmptyLineBox; }

  // True if descendants were propagated to outside of this fragment.
  bool HasPropagatedDescendants() const { return has_propagated_descendants_; }

  const ComputedStyle& Style() const { return *style_; }
  const NGLineHeightMetrics& Metrics() const { return metrics_; }

  // The base direction of this line. Also known as the paragraph direction.
  // This may be different from the direction of the container box when
  // first-line style is used, or when 'unicode-bidi: plaintext' is used.
  TextDirection BaseDirection() const {
    return static_cast<TextDirection>(base_direction_);
  }

  // Compute baseline for the specified baseline type.
  NGLineHeightMetrics BaselineMetrics(FontBaseline) const;

  // Scrollable overflow. including contents, in the local coordinate.
  // ScrollableOverflow is not precomputed/cached because it cannot be computed
  // when LineBox is generated because it needs container dimensions to
  // resolve relative position of its children.
  PhysicalRect ScrollableOverflow(const LayoutObject* container,
                                  const ComputedStyle* container_style,
                                  PhysicalSize container_physical_size) const;

  // Returns the first/last leaf fragment in the line in logical order. Returns
  // nullptr if the line box is empty.
  const NGPhysicalFragment* FirstLogicalLeaf() const;
  const NGPhysicalFragment* LastLogicalLeaf() const;

  // Returns a point at the visual start/end of the line.
  // Encapsulates the handling of text direction and writing mode.
  PhysicalOffset LineStartPoint() const;
  PhysicalOffset LineEndPoint() const;

  // Whether the content soft-wraps to the next line.
  bool HasSoftWrapToNextLine() const;

 private:
  NGPhysicalLineBoxFragment(NGLineBoxFragmentBuilder* builder);

  scoped_refptr<const ComputedStyle> style_;
  NGLineHeightMetrics metrics_;
  NGLinkStorage children_[];
};

template <>
struct DowncastTraits<NGPhysicalLineBoxFragment> {
  static bool AllowFrom(const NGPhysicalFragment& fragment) {
    return fragment.Type() == NGPhysicalFragment::kFragmentLineBox;
  }
};

}  // namespace blink

#endif  // NGPhysicalBoxFragment_h
