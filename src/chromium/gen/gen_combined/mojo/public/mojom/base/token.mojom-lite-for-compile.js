// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';

goog.require('mojo.internal');







goog.provide('mojoBase.mojom.TokenSpec');
/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.TokenSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };




mojo.internal.Struct(
    mojoBase.mojom.TokenSpec.$,
    'Token',
    16,
    [
      mojo.internal.StructField(
        'high', 0,
        0,
        mojo.internal.Uint64,
        0,
        false /* nullable */),
      mojo.internal.StructField(
        'low', 8,
        0,
        mojo.internal.Uint64,
        0,
        false /* nullable */),
    ]);

goog.provide('mojoBase.mojom.Token');


/**
 * @typedef { {
 *   high: !number,
 *   low: !number,
 * } }
 */

mojoBase.mojom.Token;

