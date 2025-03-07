/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkRSXform.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkTypeface.h"
#include "src/core/SkFontPriv.h"
#include "src/core/SkGlyphRun.h"
#include "src/core/SkPaintPriv.h"
#include "src/core/SkReadBuffer.h"
#include "src/core/SkSafeMath.h"
#include "src/core/SkTextBlobPriv.h"
#include "src/core/SkTextToPathIter.h"
#include "src/core/SkWriteBuffer.h"

#include <atomic>
#include <limits>
#include <new>

#if SK_SUPPORT_GPU
#include "src/gpu/text/GrTextBlobCache.h"
#endif

namespace {
struct RunFontStorageEquivalent {
    SkScalar fSize, fScaleX;
    void*    fTypeface;
    SkScalar fSkewX;
    uint32_t fFlags;
};
static_assert(sizeof(SkFont) == sizeof(RunFontStorageEquivalent), "runfont_should_stay_packed");
}

size_t SkTextBlob::RunRecord::StorageSize(uint32_t glyphCount, uint32_t textSize,
                                          SkTextBlob::GlyphPositioning positioning,
                                          SkSafeMath* safe) {
    static_assert(SkIsAlign4(sizeof(SkScalar)), "SkScalar size alignment");

    auto glyphSize = safe->mul(glyphCount, sizeof(uint16_t)),
            posSize = safe->mul(PosCount(glyphCount, positioning, safe), sizeof(SkScalar));

    // RunRecord object + (aligned) glyph buffer + position buffer
    auto size = sizeof(SkTextBlob::RunRecord);
    size = safe->add(size, safe->alignUp(glyphSize, 4));
    size = safe->add(size, posSize);

    if (textSize) {  // Extended run.
        size = safe->add(size, sizeof(uint32_t));
        size = safe->add(size, safe->mul(glyphCount, sizeof(uint32_t)));
        size = safe->add(size, textSize);
    }

    return safe->alignUp(size, sizeof(void*));
}

const SkTextBlob::RunRecord* SkTextBlob::RunRecord::First(const SkTextBlob* blob) {
    // The first record (if present) is stored following the blob object.
    // (aligned up to make the RunRecord aligned too)
    return reinterpret_cast<const RunRecord*>(SkAlignPtr((uintptr_t)(blob + 1)));
}

const SkTextBlob::RunRecord* SkTextBlob::RunRecord::Next(const RunRecord* run) {
    return SkToBool(run->fFlags & kLast_Flag) ? nullptr : NextUnchecked(run);
}

namespace {
struct RunRecordStorageEquivalent {
    SkFont   fFont;
    SkPoint  fOffset;
    uint32_t fCount;
    uint32_t fFlags;
    SkDEBUGCODE(unsigned fMagic;)
};
}

void SkTextBlob::RunRecord::validate(const uint8_t* storageTop) const {
    SkASSERT(kRunRecordMagic == fMagic);
    SkASSERT((uint8_t*)NextUnchecked(this) <= storageTop);

    SkASSERT(glyphBuffer() + fCount <= (uint16_t*)posBuffer());
    SkASSERT(posBuffer() + fCount * ScalarsPerGlyph(positioning())
             <= (SkScalar*)NextUnchecked(this));
    if (isExtended()) {
        SkASSERT(textSize() > 0);
        SkASSERT(textSizePtr() < (uint32_t*)NextUnchecked(this));
        SkASSERT(clusterBuffer() < (uint32_t*)NextUnchecked(this));
        SkASSERT(textBuffer() + textSize() <= (char*)NextUnchecked(this));
    }
    static_assert(sizeof(SkTextBlob::RunRecord) == sizeof(RunRecordStorageEquivalent),
                  "runrecord_should_stay_packed");
}

const SkTextBlob::RunRecord* SkTextBlob::RunRecord::NextUnchecked(const RunRecord* run) {
    SkSafeMath safe;
    auto res = reinterpret_cast<const RunRecord*>(
            reinterpret_cast<const uint8_t*>(run)
            + StorageSize(run->glyphCount(), run->textSize(), run->positioning(), &safe));
    SkASSERT(safe);
    return res;
}

size_t SkTextBlob::RunRecord::PosCount(uint32_t glyphCount,
                                       SkTextBlob::GlyphPositioning positioning,
                                       SkSafeMath* safe) {
    return safe->mul(glyphCount, ScalarsPerGlyph(positioning));
}

uint32_t* SkTextBlob::RunRecord::textSizePtr() const {
    // textSize follows the position buffer.
    SkASSERT(isExtended());
    SkSafeMath safe;
    auto res = (uint32_t*)(&this->posBuffer()[PosCount(fCount, positioning(), &safe)]);
    SkASSERT(safe);
    return res;
}

void SkTextBlob::RunRecord::grow(uint32_t count) {
    SkScalar* initialPosBuffer = posBuffer();
    uint32_t initialCount = fCount;
    fCount += count;

    // Move the initial pos scalars to their new location.
    size_t copySize = initialCount * sizeof(SkScalar) * ScalarsPerGlyph(positioning());
    SkASSERT((uint8_t*)posBuffer() + copySize <= (uint8_t*)NextUnchecked(this));

    // memmove, as the buffers may overlap
    memmove(posBuffer(), initialPosBuffer, copySize);
}

static int32_t next_id() {
    static std::atomic<int32_t> nextID{1};
    int32_t id;
    do {
        id = nextID++;
    } while (id == SK_InvalidGenID);
    return id;
}

SkTextBlob::SkTextBlob(const SkRect& bounds)
    : fBounds(bounds)
    , fUniqueID(next_id())
    , fCacheID(SK_InvalidUniqueID) {}

SkTextBlob::~SkTextBlob() {
#if SK_SUPPORT_GPU
    if (SK_InvalidUniqueID != fCacheID.load()) {
        GrTextBlobCache::PostPurgeBlobMessage(fUniqueID, fCacheID);
    }
#endif

    const auto* run = RunRecord::First(this);
    do {
        const auto* nextRun = RunRecord::Next(run);
        SkDEBUGCODE(run->validate((uint8_t*)this + fStorageSize);)
        run->~RunRecord();
        run = nextRun;
    } while (run);
}

namespace {

union PositioningAndExtended {
    int32_t intValue;
    struct {
        uint8_t  positioning;
        uint8_t  extended;
        uint16_t padding;
    };
};

static_assert(sizeof(PositioningAndExtended) == sizeof(int32_t), "");

} // namespace

enum SkTextBlob::GlyphPositioning : uint8_t {
    kDefault_Positioning      = 0, // Default glyph advances -- zero scalars per glyph.
    kHorizontal_Positioning   = 1, // Horizontal positioning -- one scalar per glyph.
    kFull_Positioning         = 2, // Point positioning -- two scalars per glyph.
    kRSXform_Positioning      = 3, // RSXform positioning -- four scalars per glyph.
};

unsigned SkTextBlob::ScalarsPerGlyph(GlyphPositioning pos) {
    const uint8_t gScalarsPerPositioning[] = {
        0,  // kDefault_Positioning
        1,  // kHorizontal_Positioning
        2,  // kFull_Positioning
        4,  // kRSXform_Positioning
    };
    SkASSERT((unsigned)pos <= 3);
    return gScalarsPerPositioning[pos];
}

void SkTextBlob::operator delete(void* p) {
    sk_free(p);
}

void* SkTextBlob::operator new(size_t) {
    SK_ABORT("All blobs are created by placement new.");
    return sk_malloc_throw(0);
}

void* SkTextBlob::operator new(size_t, void* p) {
    return p;
}

SkTextBlobRunIterator::SkTextBlobRunIterator(const SkTextBlob* blob)
    : fCurrentRun(SkTextBlob::RunRecord::First(blob)) {
    SkDEBUGCODE(fStorageTop = (uint8_t*)blob + blob->fStorageSize;)
}

void SkTextBlobRunIterator::next() {
    SkASSERT(!this->done());

    if (!this->done()) {
        SkDEBUGCODE(fCurrentRun->validate(fStorageTop);)
        fCurrentRun = SkTextBlob::RunRecord::Next(fCurrentRun);
    }
}

SkTextBlobRunIterator::GlyphPositioning SkTextBlobRunIterator::positioning() const {
    SkASSERT(!this->done());
    static_assert(static_cast<GlyphPositioning>(SkTextBlob::kDefault_Positioning) ==
                  kDefault_Positioning, "");
    static_assert(static_cast<GlyphPositioning>(SkTextBlob::kHorizontal_Positioning) ==
                  kHorizontal_Positioning, "");
    static_assert(static_cast<GlyphPositioning>(SkTextBlob::kFull_Positioning) ==
                  kFull_Positioning, "");
    static_assert(static_cast<GlyphPositioning>(SkTextBlob::kRSXform_Positioning) ==
                  kRSXform_Positioning, "");

    return SkTo<GlyphPositioning>(fCurrentRun->positioning());
}

bool SkTextBlobRunIterator::isLCD() const {
    return fCurrentRun->font().getEdging() == SkFont::Edging::kSubpixelAntiAlias;
}

SkTextBlobBuilder::SkTextBlobBuilder()
    : fStorageSize(0)
    , fStorageUsed(0)
    , fRunCount(0)
    , fDeferredBounds(false)
    , fLastRun(0) {
    fBounds.setEmpty();
}

SkTextBlobBuilder::~SkTextBlobBuilder() {
    if (nullptr != fStorage.get()) {
        // We are abandoning runs and must destruct the associated font data.
        // The easiest way to accomplish that is to use the blob destructor.
        this->make();
    }
}

SkRect SkTextBlobBuilder::TightRunBounds(const SkTextBlob::RunRecord& run) {
    //printf("SkTextBlobBuilder::TightRunBounds 1.2.1\n");
    const SkFont& font = run.font();
    SkRect bounds;

    if (SkTextBlob::kDefault_Positioning == run.positioning()) {
        font.measureText(run.glyphBuffer(), run.glyphCount() * sizeof(uint16_t),
                         SkTextEncoding::kGlyphID, &bounds);
        return bounds.makeOffset(run.offset().x(), run.offset().y());
    }

    SkAutoSTArray<16, SkRect> glyphBounds(run.glyphCount());
    font.getBounds(run.glyphBuffer(), run.glyphCount(), glyphBounds.get(), nullptr);

    SkASSERT(SkTextBlob::kFull_Positioning == run.positioning() ||
             SkTextBlob::kHorizontal_Positioning == run.positioning());
    // kFull_Positioning       => [ x, y, x, y... ]
    // kHorizontal_Positioning => [ x, x, x... ]
    //                            (const y applied by runBounds.offset(run->offset()) later)
    const SkScalar horizontalConstY = 0;
    const SkScalar* glyphPosX = run.posBuffer();
    const SkScalar* glyphPosY = (run.positioning() == SkTextBlob::kFull_Positioning) ?
                                                      glyphPosX + 1 : &horizontalConstY;
    const unsigned posXInc = SkTextBlob::ScalarsPerGlyph(run.positioning());
    const unsigned posYInc = (run.positioning() == SkTextBlob::kFull_Positioning) ?
                                                   posXInc : 0;

    bounds.setEmpty();
    for (unsigned i = 0; i < run.glyphCount(); ++i) {
        bounds.join(glyphBounds[i].makeOffset(*glyphPosX, *glyphPosY));
        glyphPosX += posXInc;
        glyphPosY += posYInc;
    }

    SkASSERT((void*)glyphPosX <= SkTextBlob::RunRecord::Next(&run));

    //printf("SkTextBlobBuilder::TightRunBounds 1.2.2\n");
    SkRect res = bounds.makeOffset(run.offset().x(), run.offset().y());
    //printf("SkTextBlobBuilder::TightRunBounds 1.2.3\n");
    return res;
}

static SkRect map_quad_to_rect(const SkRSXform& xform, const SkRect& rect) {
    return SkMatrix().setRSXform(xform).mapRect(rect);
}

SkRect SkTextBlobBuilder::ConservativeRunBounds(const SkTextBlob::RunRecord& run) {
    //printf("SkTextBlobBuilder::ConservativeRunBounds 1.2.1\n");
    // TODO >>>
    SkASSERT(run.glyphCount() > 0);
    SkASSERT(SkTextBlob::kFull_Positioning == run.positioning() ||
             SkTextBlob::kHorizontal_Positioning == run.positioning() ||
             SkTextBlob::kRSXform_Positioning == run.positioning());

    //printf("SkTextBlobBuilder::ConservativeRunBounds 1.2.1.0\n");
    const SkRect fontBounds = SkFontPriv::GetFontBounds(run.font());
    //printf("SkTextBlobBuilder::ConservativeRunBounds 1.2.1.1\n");
    if (fontBounds.isEmpty()) {
        // Empty font bounds are likely a font bug.  TightBounds has a better chance of
        // producing useful results in this case.
        return TightRunBounds(run);
    }

    // Compute the glyph position bbox.
    SkRect bounds;
    switch (run.positioning()) {
    case SkTextBlob::kHorizontal_Positioning: {
        const SkScalar* glyphPos = run.posBuffer();
        SkASSERT((void*)(glyphPos + run.glyphCount()) <= SkTextBlob::RunRecord::Next(&run));

        SkScalar minX = *glyphPos;
        SkScalar maxX = *glyphPos;
        for (unsigned i = 1; i < run.glyphCount(); ++i) {
            SkScalar x = glyphPos[i];
            minX = SkMinScalar(x, minX);
            maxX = SkMaxScalar(x, maxX);
        }

        bounds.setLTRB(minX, 0, maxX, 0);
    } break;
    case SkTextBlob::kFull_Positioning: {
        const SkPoint* glyphPosPts = run.pointBuffer();
        SkASSERT((void*)(glyphPosPts + run.glyphCount()) <= SkTextBlob::RunRecord::Next(&run));

        bounds.setBounds(glyphPosPts, run.glyphCount());
    } break;
    case SkTextBlob::kRSXform_Positioning: {
        const SkRSXform* xform = run.xformBuffer();
        SkASSERT((void*)(xform + run.glyphCount()) <= SkTextBlob::RunRecord::Next(&run));
        bounds = map_quad_to_rect(xform[0], fontBounds);
        for (unsigned i = 1; i < run.glyphCount(); ++i) {
            bounds.join(map_quad_to_rect(xform[i], fontBounds));
        }
    } break;
    default:
        SK_ABORT("unsupported positioning mode");
    }

    if (run.positioning() != SkTextBlob::kRSXform_Positioning) {
        // Expand by typeface glyph bounds.
        bounds.fLeft   += fontBounds.left();
        bounds.fTop    += fontBounds.top();
        bounds.fRight  += fontBounds.right();
        bounds.fBottom += fontBounds.bottom();
    }

    // Offset by run position.
    //printf("SkTextBlobBuilder::ConservativeRunBounds 1.2.2\n");
    SkRect res = bounds.makeOffset(run.offset().x(), run.offset().y());
    //printf("SkTextBlobBuilder::ConservativeRunBounds 1.2.2\n");
    return res;
}

void SkTextBlobBuilder::updateDeferredBounds() {
    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.1\n");
    SkASSERT(!fDeferredBounds || fRunCount > 0);

    if (!fDeferredBounds) {
        return;
    }
    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.2\n");

    SkASSERT(fLastRun >= SkAlignPtr(sizeof(SkTextBlob)));
    SkTextBlob::RunRecord* run = reinterpret_cast<SkTextBlob::RunRecord*>(fStorage.get() +
                                                                          fLastRun);

    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.3\n");

    // FIXME: we should also use conservative bounds for kDefault_Positioning.
    SkRect runBounds = SkTextBlob::kDefault_Positioning == run->positioning() ?
                       TightRunBounds(*run) : ConservativeRunBounds(*run);
    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.4\n");
    fBounds.join(runBounds);
    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.5\n");
    fDeferredBounds = false;
    //printf("SkTextBlobBuilder::updateDeferredBounds 1.1.6\n");
}

void SkTextBlobBuilder::reserve(size_t size) {
    SkSafeMath safe;

    // We don't currently pre-allocate, but maybe someday...
    if (safe.add(fStorageUsed, size) <= fStorageSize && safe) {
        return;
    }

    if (0 == fRunCount) {
        SkASSERT(nullptr == fStorage.get());
        SkASSERT(0 == fStorageSize);
        SkASSERT(0 == fStorageUsed);

        // the first allocation also includes blob storage
        // aligned up to a pointer alignment so SkTextBlob::RunRecords after it stay aligned.
        fStorageUsed = SkAlignPtr(sizeof(SkTextBlob));
    }

    fStorageSize = safe.add(fStorageUsed, size);

    // FYI: This relies on everything we store being relocatable, particularly SkPaint.
    //      Also, this is counting on the underlying realloc to throw when passed max().
    fStorage.realloc(safe ? fStorageSize : std::numeric_limits<size_t>::max());
}

bool SkTextBlobBuilder::mergeRun(const SkFont& font, SkTextBlob::GlyphPositioning positioning,
                                 uint32_t count, SkPoint offset) {
    if (0 == fLastRun) {
        SkASSERT(0 == fRunCount);
        return false;
    }

    SkASSERT(fLastRun >= SkAlignPtr(sizeof(SkTextBlob)));
    SkTextBlob::RunRecord* run = reinterpret_cast<SkTextBlob::RunRecord*>(fStorage.get() +
                                                                          fLastRun);
    SkASSERT(run->glyphCount() > 0);

    if (run->textSize() != 0) {
        return false;
    }

    if (run->positioning() != positioning
        || run->font() != font
        || (run->glyphCount() + count < run->glyphCount())) {
        return false;
    }

    // we can merge same-font/same-positioning runs in the following cases:
    //   * fully positioned run following another fully positioned run
    //   * horizontally postioned run following another horizontally positioned run with the same
    //     y-offset
    if (SkTextBlob::kFull_Positioning != positioning
        && (SkTextBlob::kHorizontal_Positioning != positioning
            || run->offset().y() != offset.y())) {
        return false;
    }

    SkSafeMath safe;
    size_t sizeDelta =
        SkTextBlob::RunRecord::StorageSize(run->glyphCount() + count, 0, positioning, &safe) -
        SkTextBlob::RunRecord::StorageSize(run->glyphCount()        , 0, positioning, &safe);
    if (!safe) {
        return false;
    }

    this->reserve(sizeDelta);

    // reserve may have realloced
    run = reinterpret_cast<SkTextBlob::RunRecord*>(fStorage.get() + fLastRun);
    uint32_t preMergeCount = run->glyphCount();
    run->grow(count);

    // Callers expect the buffers to point at the newly added slice, ant not at the beginning.
    fCurrentRunBuffer.glyphs = run->glyphBuffer() + preMergeCount;
    fCurrentRunBuffer.pos = run->posBuffer()
                          + preMergeCount * SkTextBlob::ScalarsPerGlyph(positioning);

    fStorageUsed += sizeDelta;

    SkASSERT(fStorageUsed <= fStorageSize);
    run->validate(fStorage.get() + fStorageUsed);

    return true;
}

void SkTextBlobBuilder::allocInternal(const SkFont& font,
                                      SkTextBlob::GlyphPositioning positioning,
                                      int count, int textSize, SkPoint offset,
                                      const SkRect* bounds) {
    if (count <= 0 || textSize < 0) {
        fCurrentRunBuffer = { nullptr, nullptr, nullptr, nullptr };
        return;
    }

    if (textSize != 0 || !this->mergeRun(font, positioning, count, offset)) {
        this->updateDeferredBounds();

        SkSafeMath safe;
        size_t runSize = SkTextBlob::RunRecord::StorageSize(count, textSize, positioning, &safe);
        if (!safe) {
            fCurrentRunBuffer = { nullptr, nullptr, nullptr, nullptr };
            return;
        }

        this->reserve(runSize);

        SkASSERT(fStorageUsed >= SkAlignPtr(sizeof(SkTextBlob)));
        SkASSERT(fStorageUsed + runSize <= fStorageSize);

        SkTextBlob::RunRecord* run = new (fStorage.get() + fStorageUsed)
            SkTextBlob::RunRecord(count, textSize, offset, font, positioning);
        fCurrentRunBuffer.glyphs = run->glyphBuffer();
        fCurrentRunBuffer.pos = run->posBuffer();
        fCurrentRunBuffer.utf8text = run->textBuffer();
        fCurrentRunBuffer.clusters = run->clusterBuffer();

        fLastRun = fStorageUsed;
        fStorageUsed += runSize;
        fRunCount++;

        SkASSERT(fStorageUsed <= fStorageSize);
        run->validate(fStorage.get() + fStorageUsed);
    }
    SkASSERT(textSize > 0 || nullptr == fCurrentRunBuffer.utf8text);
    SkASSERT(textSize > 0 || nullptr == fCurrentRunBuffer.clusters);
    if (!fDeferredBounds) {
        if (bounds) {
            fBounds.join(*bounds);
        } else {
            fDeferredBounds = true;
        }
    }
}

// SkFont versions

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRun(const SkFont& font, int count,
                                                                SkScalar x, SkScalar y,
                                                                const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kDefault_Positioning, count, 0, {x, y}, bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunPosH(const SkFont& font, int count,
                                                                    SkScalar y,
                                                                    const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kHorizontal_Positioning, count, 0, {0, y}, bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunPos(const SkFont& font, int count,
                                                                   const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kFull_Positioning, count, 0, {0, 0}, bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer&
SkTextBlobBuilder::allocRunRSXform(const SkFont& font, int count) {
    this->allocInternal(font, SkTextBlob::kRSXform_Positioning, count, 0, {0, 0}, nullptr);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunText(const SkFont& font, int count,
                                                                    SkScalar x, SkScalar y,
                                                                    int textByteCount,
                                                                    SkString lang,
                                                                    const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kDefault_Positioning, count, textByteCount, SkPoint::Make(x, y), bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunTextPosH(const SkFont& font, int count,
                                                                        SkScalar y,
                                                                        int textByteCount,
                                                                        SkString lang,
                                                                        const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kHorizontal_Positioning, count, textByteCount, SkPoint::Make(0, y),
                        bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunTextPos(const SkFont& font, int count,
                                                                       int textByteCount,
                                                                       SkString lang,
                                                                       const SkRect *bounds) {
    this->allocInternal(font, SkTextBlob::kFull_Positioning, count, textByteCount, SkPoint::Make(0, 0), bounds);
    return fCurrentRunBuffer;
}

const SkTextBlobBuilder::RunBuffer& SkTextBlobBuilder::allocRunRSXform(const SkFont& font, int count,
                                                                       int textByteCount,
                                                                       SkString lang,
                                                                       const SkRect* bounds) {
    this->allocInternal(font, SkTextBlob::kRSXform_Positioning, count, textByteCount,
                        {0, 0}, bounds);
    return fCurrentRunBuffer;
}

sk_sp<SkTextBlob> SkTextBlobBuilder::make() {
//#if defined(SK_DEBUG) && defined(__EMSCRIPTEN__)
//#error "SK_DEBUG && __EMSCRIPTEN__"
//#endif

    //printf("SkTextBlobBuilder::make 1\n");
    if (!fRunCount) {
        //printf("SkTextBlobBuilder::make 2\n");
        // We don't instantiate empty blobs.
        SkASSERT(!fStorage.get());
        SkASSERT(fStorageUsed == 0);
        SkASSERT(fStorageSize == 0);
        SkASSERT(fLastRun == 0);
        SkASSERT(fBounds.isEmpty());
        return nullptr;
    }
    //printf("SkTextBlobBuilder::make 3\n");

    this->updateDeferredBounds();
    //printf("SkTextBlobBuilder::make 3.1\n");

    // Tag the last run as such.
    auto* lastRun = reinterpret_cast<SkTextBlob::RunRecord*>(fStorage.get() + fLastRun);
    lastRun->fFlags |= SkTextBlob::RunRecord::kLast_Flag;

    //printf("SkTextBlobBuilder::make 3.2\n");
    SkTextBlob* blob = new (fStorage.release()) SkTextBlob(fBounds);
    SkDEBUGCODE(const_cast<SkTextBlob*>(blob)->fStorageSize = fStorageSize;)

    //printf("SkTextBlobBuilder::make 4\n");

    SkDEBUGCODE(
        SkSafeMath safe;
        size_t validateSize = SkAlignPtr(sizeof(SkTextBlob));
        for (const auto* run = SkTextBlob::RunRecord::First(blob); run;
             run = SkTextBlob::RunRecord::Next(run)) {
            validateSize += SkTextBlob::RunRecord::StorageSize(
                    run->fCount, run->textSize(), run->positioning(), &safe);
            run->validate(reinterpret_cast<const uint8_t*>(blob) + fStorageUsed);
            fRunCount--;
        }
        SkASSERT(validateSize == fStorageUsed);
        SkASSERT(fRunCount == 0);
        SkASSERT(safe);
    )

    fStorageUsed = 0;
    fStorageSize = 0;
    fRunCount = 0;
    fLastRun = 0;
    fBounds.setEmpty();

    //printf("SkTextBlobBuilder::make 5\n");

    return sk_sp<SkTextBlob>(blob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <SkTextInterceptsIter::TextType TextType, typename Func>
int GetTextIntercepts(const SkFont& font, const SkPaint* paint, const SkGlyphID glyphs[],
                      int glyphCount, const SkScalar bounds[2], SkScalar* array, Func posMaker) {
    SkASSERT(glyphCount == 0 || glyphs != nullptr);

    const SkPoint pos0 = posMaker(0);
    SkTextInterceptsIter iter(glyphs, glyphCount, font, paint, bounds, pos0.x(), pos0.y(),
                              TextType);

    int i = 0;
    int count = 0;
    while (iter.next(array, &count)) {
        if (TextType == SkTextInterceptsIter::TextType::kPosText) {
            const SkPoint pos = posMaker(++i);
            iter.setPosition(pos.x(), pos.y());
        }
    }

    return count;
}

int SkTextBlob::getIntercepts(const SkScalar bounds[2], SkScalar intervals[],
                              const SkPaint* paint) const {
    int count = 0;
    SkTextBlobRunIterator it(this);

    while (!it.done()) {
        SkScalar* runIntervals = intervals ? intervals + count : nullptr;
        const SkFont& font = it.font();
        const SkGlyphID* glyphs = it.glyphs();
        const int glyphCount = it.glyphCount();

        switch (it.positioning()) {
            case SkTextBlobRunIterator::kDefault_Positioning: {
                SkPoint loc = it.offset();
                count += GetTextIntercepts<SkTextInterceptsIter::TextType::kText>(
                          font, paint, glyphs, glyphCount, bounds, runIntervals, [loc] (int) {
                    return loc;
                });
            } break;
            case SkTextBlobRunIterator::kHorizontal_Positioning: {
                const SkScalar* xpos = it.pos();
                const SkScalar constY = it.offset().fY;
                count += GetTextIntercepts<SkTextInterceptsIter::TextType::kPosText>(
                 font, paint, glyphs, glyphCount, bounds, runIntervals, [xpos, constY] (int i) {
                    return SkPoint::Make(xpos[i], constY);
                });
            } break;
            case SkTextBlobRunIterator::kFull_Positioning: {
                const SkPoint* pos = reinterpret_cast<const SkPoint*>(it.pos());
                count += GetTextIntercepts<SkTextInterceptsIter::TextType::kPosText>(
                             font, paint, glyphs, glyphCount, bounds, runIntervals, [pos] (int i) {
                    return pos[i];
                });
            } break;
            case SkTextBlobRunIterator::kRSXform_Positioning:
                // Unimplemented for now -- can/should we try to make this work?
                break;
        }

        it.next();
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SkTextBlobPriv::Flatten(const SkTextBlob& blob, SkWriteBuffer& buffer) {
    // seems like we could skip this, and just recompute bounds in unflatten, but
    // some cc_unittests fail if we remove this...
    buffer.writeRect(blob.bounds());

    SkTextBlobRunIterator it(&blob);
    while (!it.done()) {
        SkASSERT(it.glyphCount() > 0);

        buffer.write32(it.glyphCount());
        PositioningAndExtended pe;
        pe.intValue = 0;
        pe.positioning = it.positioning();
        SkASSERT((int32_t)it.positioning() == pe.intValue);  // backwards compat.

        uint32_t textSize = it.textSize();
        pe.extended = textSize > 0;
        buffer.write32(pe.intValue);
        if (pe.extended) {
            buffer.write32(textSize);
        }
        buffer.writePoint(it.offset());

        SkFontPriv::Flatten(it.font(), buffer);

        buffer.writeByteArray(it.glyphs(), it.glyphCount() * sizeof(uint16_t));
        buffer.writeByteArray(it.pos(),
                              it.glyphCount() * sizeof(SkScalar) *
                              SkTextBlob::ScalarsPerGlyph(
                                  SkTo<SkTextBlob::GlyphPositioning>(it.positioning())));
        if (pe.extended) {
            buffer.writeByteArray(it.clusters(), sizeof(uint32_t) * it.glyphCount());
            buffer.writeByteArray(it.text(), it.textSize());
        }

        it.next();
    }

    // Marker for the last run (0 is not a valid glyph count).
    buffer.write32(0);
}

sk_sp<SkTextBlob> SkTextBlobPriv::MakeFromBuffer(SkReadBuffer& reader) {
    SkRect bounds;
    reader.readRect(&bounds);

    SkTextBlobBuilder blobBuilder;
    SkSafeMath safe;
    for (;;) {
        int glyphCount = reader.read32();
        if (glyphCount == 0) {
            // End-of-runs marker.
            break;
        }

        PositioningAndExtended pe;
        pe.intValue = reader.read32();
        const auto pos = SkTo<SkTextBlob::GlyphPositioning>(pe.positioning);
        if (glyphCount <= 0 || pos > SkTextBlob::kRSXform_Positioning) {
            return nullptr;
        }
        int textSize = pe.extended ? reader.read32() : 0;
        if (textSize < 0) {
            return nullptr;
        }

        SkPoint offset;
        reader.readPoint(&offset);
        SkFont font;
        if (reader.isVersionLT(SkReadBuffer::kSerializeFonts_Version)) {
            SkPaint paint;
            reader.readPaint(&paint, &font);
        } else {
            SkFontPriv::Unflatten(&font, reader);
        }

        // Compute the expected size of the buffer and ensure we have enough to deserialize
        // a run before allocating it.
        const size_t glyphSize = safe.mul(glyphCount, sizeof(uint16_t)),
                     posSize =
                             safe.mul(glyphCount, safe.mul(sizeof(SkScalar),
                             SkTextBlob::ScalarsPerGlyph(pos))),
                     clusterSize = pe.extended ? safe.mul(glyphCount, sizeof(uint32_t)) : 0;
        const size_t totalSize =
                safe.add(safe.add(glyphSize, posSize), safe.add(clusterSize, textSize));

        if (!reader.isValid() || !safe || totalSize > reader.available()) {
            return nullptr;
        }

        const SkTextBlobBuilder::RunBuffer* buf = nullptr;
        switch (pos) {
            case SkTextBlob::kDefault_Positioning:
                buf = &blobBuilder.allocRunText(font, glyphCount, offset.x(), offset.y(),
                                                textSize, SkString(), &bounds);
                break;
            case SkTextBlob::kHorizontal_Positioning:
                buf = &blobBuilder.allocRunTextPosH(font, glyphCount, offset.y(),
                                                    textSize, SkString(), &bounds);
                break;
            case SkTextBlob::kFull_Positioning:
                buf = &blobBuilder.allocRunTextPos(font, glyphCount, textSize, SkString(), &bounds);
                break;
            case SkTextBlob::kRSXform_Positioning:
                buf = &blobBuilder.allocRunRSXform(font, glyphCount, textSize, SkString(), &bounds);
                break;
        }

        if (!buf->glyphs ||
            !buf->pos ||
            (pe.extended && (!buf->clusters || !buf->utf8text))) {
            return nullptr;
        }

        if (!reader.readByteArray(buf->glyphs, glyphSize) ||
            !reader.readByteArray(buf->pos, posSize)) {
            return nullptr;
            }

        if (pe.extended) {
            if (!reader.readByteArray(buf->clusters, clusterSize) ||
                !reader.readByteArray(buf->utf8text, textSize)) {
                return nullptr;
            }
        }
    }

    return blobBuilder.make();
}

sk_sp<SkTextBlob> SkTextBlob::MakeFromText(const void* text, size_t byteLength, const SkFont& font,
                                           SkTextEncoding encoding) {
    //printf("SkTextBlob::MakeFromText 1\n");
    // Note: we deliberately promote this to fully positioned blobs, since we'd have to pay the
    // same cost down stream (i.e. computing bounds), so its cheaper to pay the cost once now.
    const int count = font.countText(text, byteLength, encoding);
    SkTextBlobBuilder builder;
    auto buffer = builder.allocRunPos(font, count);
    font.textToGlyphs(text, byteLength, encoding, buffer.glyphs, count);
    font.getPos(buffer.glyphs, count, buffer.points(), {0, 0});
    //printf("SkTextBlob::MakeFromText 2\n");
    sk_sp<SkTextBlob> res = builder.make();
    //printf("SkTextBlob::MakeFromText 3\n");
    return res;
}

sk_sp<SkTextBlob> SkTextBlob::MakeFromPosText(const void* text, size_t byteLength,
                                              const SkPoint pos[], const SkFont& font,
                                              SkTextEncoding encoding) {
    const int count = font.countText(text, byteLength, encoding);
    SkTextBlobBuilder builder;
    auto buffer = builder.allocRunPos(font, count);
    font.textToGlyphs(text, byteLength, encoding, buffer.glyphs, count);
    memcpy(buffer.points(), pos, count * sizeof(SkPoint));
    return builder.make();
}

sk_sp<SkTextBlob> SkTextBlob::MakeFromPosTextH(const void* text, size_t byteLength,
                                               const SkScalar xpos[], SkScalar constY,
                                               const SkFont& font, SkTextEncoding encoding) {
    const int count = font.countText(text, byteLength, encoding);
    SkTextBlobBuilder builder;
    auto buffer = builder.allocRunPosH(font, count, constY);
    font.textToGlyphs(text, byteLength, encoding, buffer.glyphs, count);
    memcpy(buffer.pos, xpos, count * sizeof(SkScalar));
    return builder.make();
}

sk_sp<SkTextBlob> SkTextBlob::MakeFromRSXform(const void* text, size_t byteLength,
                                              const SkRSXform xform[], const SkFont& font,
                                              SkTextEncoding encoding) {
    const int count = font.countText(text, byteLength, encoding);
    SkTextBlobBuilder builder;
    auto buffer = builder.allocRunRSXform(font, count);
    font.textToGlyphs(text, byteLength, encoding, buffer.glyphs, count);
    memcpy(buffer.xforms(), xform, count * sizeof(SkRSXform));
    return builder.make();
}

sk_sp<SkData> SkTextBlob::serialize(const SkSerialProcs& procs) const {
    SkBinaryWriteBuffer buffer;
    buffer.setSerialProcs(procs);
    SkTextBlobPriv::Flatten(*this, buffer);

    size_t total = buffer.bytesWritten();
    sk_sp<SkData> data = SkData::MakeUninitialized(total);
    buffer.writeToMemory(data->writable_data());
    return data;
}

sk_sp<SkTextBlob> SkTextBlob::Deserialize(const void* data, size_t length,
                                          const SkDeserialProcs& procs) {
    SkReadBuffer buffer(data, length);
    buffer.setDeserialProcs(procs);
    return SkTextBlobPriv::MakeFromBuffer(buffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t SkTextBlob::serialize(const SkSerialProcs& procs, void* memory, size_t memory_size) const {
    SkBinaryWriteBuffer buffer(memory, memory_size);
    buffer.setSerialProcs(procs);
    SkTextBlobPriv::Flatten(*this, buffer);
    return buffer.usingInitialStorage() ? buffer.bytesWritten() : 0u;
}

///////////////////////////////////////////////////////////////////////////////

// xyIndex is 0 for fAdvanceX or 1 for fAdvanceY
static SkScalar advance(const SkGlyph& glyph) {
    return SkFloatToScalar(glyph.fAdvanceX);
}

static bool has_thick_frame(const SkPaint& paint) {
    return  paint.getStrokeWidth() > 0 &&
    paint.getStyle() != SkPaint::kFill_Style;
}

SkTextBaseIter::SkTextBaseIter(const SkGlyphID glyphs[], int count, const SkFont& font,
                               const SkPaint* paint) : fFont(font) {
    SkAssertResult(count >= 0);

    if (paint) {
        fPaint = *paint;
    }
    fPaint.setMaskFilter(nullptr);   // don't want this affecting our path-cache lookup

    // can't use our canonical size if we need to apply patheffects
    if (fPaint.getPathEffect() == nullptr) {
        // If the wrong size is going to be used, don't hint anything.
        fFont.setHinting(SkFontHinting::kNone);
        fFont.setSubpixel(true);
        fScale = fFont.getSize() / SkFontPriv::kCanonicalTextSizeForPaths;
        fFont.setSize(SkIntToScalar(SkFontPriv::kCanonicalTextSizeForPaths));
        // Note: fScale can be zero here (even if it wasn't before the divide). It can also
        // be very very small. We call sk_ieee_float_divide below to ensure IEEE divide behavior,
        // since downstream we will check for the resulting coordinates being non-finite anyway.
        // Thus we don't need to check for zero here.
        if (has_thick_frame(fPaint)) {
            fPaint.setStrokeWidth(sk_ieee_float_divide(fPaint.getStrokeWidth(), fScale));
        }
    } else {
        fScale = SK_Scalar1;
    }

    SkPaint::Style prevStyle = fPaint.getStyle();
    auto prevPE = fPaint.refPathEffect();
    auto prevMF = fPaint.refMaskFilter();
    fPaint.setStyle(SkPaint::kFill_Style);
    fPaint.setPathEffect(nullptr);

    fCache = SkStrikeCache::FindOrCreateStrikeWithNoDeviceExclusive(fFont, fPaint);

    fPaint.setStyle(prevStyle);
    fPaint.setPathEffect(std::move(prevPE));
    fPaint.setMaskFilter(std::move(prevMF));

    // now compute fXOffset if needed

    SkScalar xOffset = 0;
    fXPos = xOffset;
    fPrevAdvance = 0;

    fGlyphs = glyphs;
    fStop = glyphs + count;
}

bool SkTextInterceptsIter::next(SkScalar* array, int* count) {
    SkASSERT(fGlyphs < fStop);
    const SkGlyph& glyph = fCache->getGlyphIDMetrics(*fGlyphs++);
    fXPos += fPrevAdvance * fScale;
    fPrevAdvance = advance(glyph);   // + fPaint.getTextTracking();
    if (fCache->findPath(glyph)) {
        fCache->findIntercepts(fBounds, fScale, fXPos, false,
                               const_cast<SkGlyph*>(&glyph), array, count);
    }
    return fGlyphs < fStop;
}
