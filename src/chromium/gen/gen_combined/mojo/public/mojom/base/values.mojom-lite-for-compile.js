// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';

goog.require('mojo.internal');







goog.provide('mojoBase.mojom.DictionaryValueSpec');
/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.DictionaryValueSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };

goog.provide('mojoBase.mojom.ListValueSpec');
/**
 * @const { {$:!mojo.internal.MojomType}}
 * @export
 */
mojoBase.mojom.ListValueSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };

goog.provide('mojoBase.mojom.ValueSpec');
/**
 * @const { {$:!mojo.internal.MojomType} }
 * @export
 */
mojoBase.mojom.ValueSpec =
    { $: /** @type {!mojo.internal.MojomType} */ ({}) };




mojo.internal.Struct(
    mojoBase.mojom.DictionaryValueSpec.$,
    'DictionaryValue',
    8,
    [
      mojo.internal.StructField(
        'storage', 0,
        0,
        mojo.internal.Map(mojo.internal.String, mojoBase.mojom.ValueSpec.$, false),
        null,
        false /* nullable */),
    ]);

goog.provide('mojoBase.mojom.DictionaryValue');


/**
 * @typedef { {
 *   storage: !Object<!string, !mojoBase.mojom.Value>,
 * } }
 */

mojoBase.mojom.DictionaryValue;



mojo.internal.Struct(
    mojoBase.mojom.ListValueSpec.$,
    'ListValue',
    8,
    [
      mojo.internal.StructField(
        'storage', 0,
        0,
        mojo.internal.Array(mojoBase.mojom.ValueSpec.$, false),
        null,
        false /* nullable */),
    ]);

goog.provide('mojoBase.mojom.ListValue');


/**
 * @typedef { {
 *   storage: !Array<!mojoBase.mojom.Value>,
 * } }
 */

mojoBase.mojom.ListValue;

goog.provide('mojoBase.mojom.Value');


mojo.internal.Union(
    mojoBase.mojom.ValueSpec.$, 'Value',
    {
      'nullValue': {
        'ordinal': 0,
        'type': mojo.internal.Uint8,
      },
      'boolValue': {
        'ordinal': 1,
        'type': mojo.internal.Bool,
      },
      'intValue': {
        'ordinal': 2,
        'type': mojo.internal.Int32,
      },
      'doubleValue': {
        'ordinal': 3,
        'type': mojo.internal.Double,
      },
      'stringValue': {
        'ordinal': 4,
        'type': mojo.internal.String,
      },
      'binaryValue': {
        'ordinal': 5,
        'type': mojo.internal.Array(mojo.internal.Uint8, false),
      },
      'dictionaryValue': {
        'ordinal': 6,
        'type': mojoBase.mojom.DictionaryValueSpec.$,
      },
      'listValue': {
        'ordinal': 7,
        'type': mojoBase.mojom.ListValueSpec.$,
      },
    });

/**
 * @typedef { {
 *   nullValue: (!number|undefined),
 *   boolValue: (!boolean|undefined),
 *   intValue: (!number|undefined),
 *   doubleValue: (!number|undefined),
 *   stringValue: (!string|undefined),
 *   binaryValue: (!Array<!number>|undefined),
 *   dictionaryValue: (!mojoBase.mojom.DictionaryValue|undefined),
 *   listValue: (!mojoBase.mojom.ListValue|undefined),
 * } }
 */
mojoBase.mojom.Value;
