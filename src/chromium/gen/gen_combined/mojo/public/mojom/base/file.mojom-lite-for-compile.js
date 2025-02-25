// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';

goog.require('mojo.internal');







goog.provide('mojoBase.mojom.FileSpec');
/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.FileSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };




mojo.internal.Struct(
    mojoBase.mojom.FileSpec.$,
    'File',
    8,
    [
      mojo.internal.StructField(
        'fd', 0,
        0,
        mojo.internal.Handle,
        null,
        false /* nullable */),
      mojo.internal.StructField(
        'async', 4,
        0,
        mojo.internal.Bool,
        false,
        false /* nullable */),
    ]);

goog.provide('mojoBase.mojom.File');


/**
 * @typedef { {
 *   fd: !MojoHandle,
 *   async: !boolean,
 * } }
 */

mojoBase.mojom.File;

