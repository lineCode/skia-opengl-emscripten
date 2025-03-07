// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';

goog.require('mojo.internal');







goog.provide('mojoBase.mojom.MemoryAllocatorDumpCrossProcessUidSpec');
/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.MemoryAllocatorDumpCrossProcessUidSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };




mojo.internal.Struct(
    mojoBase.mojom.MemoryAllocatorDumpCrossProcessUidSpec.$,
    'MemoryAllocatorDumpCrossProcessUid',
    8,
    [
      mojo.internal.StructField(
        'value', 0,
        0,
        mojo.internal.Uint64,
        0,
        false /* nullable */),
    ]);

goog.provide('mojoBase.mojom.MemoryAllocatorDumpCrossProcessUid');


/**
 * @typedef { {
 *   value: !number,
 * } }
 */

mojoBase.mojom.MemoryAllocatorDumpCrossProcessUid;

