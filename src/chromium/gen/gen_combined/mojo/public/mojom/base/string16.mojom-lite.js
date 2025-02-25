// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';


mojo.internal.exportModule('mojoBase.mojom');







/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.String16Spec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };


/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.BigString16Spec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };




mojo.internal.Struct(
    mojoBase.mojom.String16Spec.$,
    'String16',
    8,
    [
      mojo.internal.StructField(
        'data', 0,
        0,
        mojo.internal.Array(mojo.internal.Uint16, false),
        null,
        false /* nullable */),
    ]);





mojo.internal.Struct(
    mojoBase.mojom.BigString16Spec.$,
    'BigString16',
    16,
    [
      mojo.internal.StructField(
        'data', 0,
        0,
        mojoBase.mojom.BigBufferSpec.$,
        null,
        false /* nullable */),
    ]);



