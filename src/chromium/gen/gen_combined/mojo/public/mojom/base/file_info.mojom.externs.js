// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



goog.provide('mojoBase.mojom.FileInfo');

mojoBase.mojom.FileInfo = class {
  constructor() {
    /** @type { !number } */
    this.size;
    /** @type { !boolean } */
    this.isDirectory;
    /** @type { !boolean } */
    this.isSymbolicLink;
    /** @type { !mojoBase.mojom.Time } */
    this.lastModified;
    /** @type { !mojoBase.mojom.Time } */
    this.lastAccessed;
    /** @type { !mojoBase.mojom.Time } */
    this.creationTime;
  }
};
