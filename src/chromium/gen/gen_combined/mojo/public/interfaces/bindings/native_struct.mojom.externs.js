// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



/** @enum {number} */
mojo.native.SerializedHandleType = {};
mojo.native.SerializedHandleType.MOJO_HANDLE;
mojo.native.SerializedHandleType.PLATFORM_FILE;
mojo.native.SerializedHandleType.WIN_HANDLE;
mojo.native.SerializedHandleType.MACH_PORT;
mojo.native.SerializedHandleType.FUCHSIA_HANDLE;
goog.provide('mojo.native.SerializedHandle');

mojo.native.SerializedHandle = class {
  constructor() {
    /** @type { !MojoHandle } */
    this.theHandle;
    /** @type { !mojo.native.SerializedHandleType } */
    this.type;
  }
};

goog.provide('mojo.native.NativeStruct');

mojo.native.NativeStruct = class {
  constructor() {
    /** @type { !Array<number> } */
    this.data;
    /** @type { Array<mojo.native.SerializedHandle> } */
    this.handles;
  }
};
