# Cobalt Version Changelog

This document records all notable changes made to Cobalt since the last release.

## Version 20
 - **Improvements and Bug Fixes**
  - Fix bug where Cobalt would not refresh the layout when the textContent
    property of a DOM TextNode is modified.

 - **Add support for decoding JPEG images as multi-plane YUV**

   JPEG images are decoded into RGBA in previous versions of Cobalt.  The native
   format of most JPEG images is YV12, which takes only 3/8 of memory compare to
   RGBA.  Now JPEG images are decoded into multi-plane YUV images on platforms
   with "rasterizer_type" set to "direct-gles".  As a result, when decoding to
   multi-plane image is enabled, image cache size set by AutoMem will be reduced
   by half due to the more compact nature of the YUV image format versus RGB.
   This feature can also be enabled/disabled explicitly by passing command line
   parameter "allow_image_decoding_to_multi_plane" to Cobalt with value "true"
   or "false".

 - **Cobalt code assumes that no errors are generated for unused parameters**

   There now exists Cobalt code where input parameters may be unused, and it
   is expected that toolchains will not generate errors in these cases.  You
   may need to adjust your Starboard configuration so that your compiler no
   longer emits this error, e.g. build with the `-Wno-unused-parameter`
   command line flag in GCC.
   `UNREFERENCED_PARAMETER` has been removed, but `SB_UNREFERENCED_PARAMETER`
   will continue to be supported.

## Version 19
 - **Add support for V8 JavaScript Engine**

   Cobalt now supports V8 (in addition to SpiderMonkey) as JavaScript engines.
   V8 can be enabled by setting the gyp variable 'javascript_engine' to 'v8',
   and additionally setting the gyp variable 'cobalt_enable_jit' to 1.  These
   variables should be set from your `gyp_configuration.py` Python file (and
   not for example your `gyp_configuration.gypi` file).

 - **Add support for animated WebP images with transparency**

   This was not originally supported by Cobalt, and any prior use of animated
   WebP with transparency would have resulted in visual artifacts.

 - **Storage format changed from sqlite3 to protobuf**

   Cobalt's internal representation for persistent storage of cookies and local
   storage entries changed from sqlite3 to protobuf. The header of the
   SbStorageRecord blob was updated from SAV0 to SAV1 and all the data will be
   migrated to the new format the next time Cobalt is launched.
   The schema is available at cobalt/storage/store/storage.proto.

 - **IPv4 Preference**

   Cobalt is now configured to prefer an IPv4 address to IPv6 addresses when
   choosing an initial server address from DNS responses. That is, if a
   DNS response contains both IPv4 addresses and IPv6 addresses, Cobalt will
   now explicitly attempt to connect an IPv4 address before possibly moving onto
   IPv6 on connect failures. Unfortunately, we've discovered common scenarios
   where IPv6 addresses were listed in DNS responses but intervening network
   configurations prevented IPv6 use after connect time. This caused the
   previous implementation to appear as if no network was available.

 - **Enable voice interactions**

   A subset of WebRTC APIs has been added. `getUserMedia` can be used to get
   audio input stream from the microphone. The media stream can then be
   connected to `MediaRecorder` to receive the audio data stored as `Blob`
   objects which can later be sent over the network. This allows web apps to
   implement voice-enabled features without relying on the platform's capability
   to perform speech recognition.

 - **Bison 3 required**

   Support for compiling with version 2.x of the GNU bison parser generator
   has been removed. Please update to version 3.x.

 - **Improvements and Bug Fixes**
  - Fix pointer/mouse events not being dispatched to JavaScript in the same
    order that they are generated by Starboard, relative to other input events
    like key presses.
  - Fix issue with CSS media queries not assigning correct rule indices.
    An issue was discovered and fixed where CSS rules defined within `@media`
    rules would be assigned a CSS rule index relative to their position within
    the nested `@media` rule rather than being assigned an index relative to
    their position within the parent CSS file.  As a result, rules may have been
    assigned incorrect precedence during CSS rule matching.
  - Fix issue where the style attribute would not appear in the string output
    of calls to element.outerHTML.

## Version 16
 - **Rebase libwebp to version 1.0.0**

   Update the version of libwebp used by Cobalt from 0.3.1 to 1.0.0.  The new
   version brings with it performance improvements and bug fixes.

 - **Move ``javascript_engine`` and ``cobalt_enable_jit`` build variables**

   Move gyp variables ``javascript_engine`` and ``cobalt_enable_jit``, which
   were previously defined in ``cobalt_configuration.gypi``, to
   ``$PLATFORM/gyp_configuration.py``.  This was done in order to work around
   bindings gyp files' complex usage of gyp variables, which prevented us from
   having a default JavaScript engine at the gyp variable level.  Now, platforms
   will by default use the JavaScript engine selected by
   ``starboard/build/platform_configuration.py``, and can override it by
   providing a different one in their ``GetVariables`` implementation.  See the
   ``linux-x64x11-mozjs`` platform for an override example.

   **IMPORTANT**: While existing gyp files that define ``javascript_engine`` and
   ``cobalt_enable_jit`` may continue to work by chance, it is *strongly*
   preferred to move all declarations of these variables to python instead.

 - **Move test data**

   Static test data is now copied to `content/data/test` instead of
   `content/dir_source_root`.  Tests looking for the path to this data should
   use `BasePathKey::DIR_TEST_DATA` instead of `BasePathKey::DIR_SOURCE_ROOT`.
   Tests in Starboard can find the static data in the `test/` subdirectory of
   `kSbSystemPathContentDirectory`.

 - **Add support for cobalt_media_buffer_max_capacity**

   Allow bounding the max capacity allocated by decoder buffers, by setting the
   gypi variables cobalt_media_buffer_max_capacity_1080p and
   cobalt_media_buffer_max_capacity_4k. 1080p applies to all resolutions 1080p
   and below. Those values default to 0, which imposes no bounds. If non-zero,
   each capacity must be greater than or equal to the sum of the video budget
   and non video budget for that resolution (see
   cobalt_media_buffer_video_buffer_1080p, cobalt_media_buffer_non_video_budget,
   etc.), and the max capacities must be greater than or equal to the
   corresponding initial capacities: cobalt_media_buffer_initial_capacity_1080p
   and cobalt_media_buffer_initial_capacity_4k.

- **Fix issue with CSS animations not working with the 'outline' property**

   There was a bug in previous versions that resulted in incorrect behavior when
   applying a CSS animation or CSS transition to the 'outline' property.  This
   is fixed now.

- **Change default minimum frame time to 16.0ms instead of 16.4ms.**

   In case a platform can only wait on millisecond resolution, we would prefer
   that it wait 16ms instead of 17ms, so we round this down.  Many platforms
   will pace themselves to 16.6ms regardless.

- **Make new rasterizer type "direct-gles" default**

   This is an optimized OpenGLES 2.0 rasterizer that provides a fast path for
   rendering most of the skia primitives that Cobalt uses.  While it was
   available since Version 11, it is now polished and set as the default
   rasterizer.

- **Added support for the fetch and streams Web APIs**

   A subset of ReadableStream and Fetch Web APIs have been implemented. Fetch
   may be used to get progressive results via a ReadableStream, or the full
   result can be accessed as text, from the Response class.

- **HTMLMediaElement::loop is supported**

   Now the video plays in a loop if the loop attribute of the video element is
   set.

- **Add support for MediaDevices.enumerateDevices()**

   It is now possible to enumerate microphone devices from JavaScript via a
   call to navigator.mediaDevices.enumerateDevices().  This returns a promise
   of an array of MediaDeviceInfo objects, each partially implemented to have
   valid `label` and `kind` attributes.

- **Improvements and Bug Fixes**
  - Fix for pseudo elements not visually updating when their CSS is modified
    (e.g. by switching their classes).

## Version 14
 - **Add support for document.hasFocus()**

   While support for firing blur and focus events was implemented, querying
   the current state via document.hasFocus() was not implemented until now.

 - **Implemented Same Origin Policy and removed navigation whitelist**

   - Added Same Origin Policy (SOP) and Cross Origin Resource Sharing (CORS)
     suport to Cobalt.  In particular, it is added to XHR, script elements,
     link elements, style elements, media elements and @font-face CSS rules.
   - Removed hardcoded YouTube navigation whitelist in favor of SOP and CSP.

## Version 12
 - **Add support for touchpad not associated with a pointer**

   This adds support for input of type kSbInputDeviceTypeTouchPad for
   touchpads not associated with a pointer.

## Version 11
 - **Splash Screen Customization**

   The Cobalt splash screen is customizable. Documents may use a link element
   with attribute rel="splashscreen" to reference the splash screen which will
   be cached if local cache is implemented on the platform. Additionally
   fallbacks may be specified via command line parmeter or gypi variable.
   For more information, see [doc/splash_screen.md](doc/splash_screen.md).

 - **Introduce C\+\+11**

   C\+\+11 is now used within Cobalt code, and so Cobalt must be compiled using
   C\+\+11 toolchains.

 - **Update SpiderMonkey from version 24 to 45; support ECMAScript 6**

   The Mozilla SpiderMonkey JavaScript engine is rebased up to version 45, and
   thus ECMAScript 6 is now supported by Cobalt.  You will need to modify your
   `gyp_configuration.gypi` file to set `'javascript_engine': 'mozjs-45'` to
   enable this.

 - **Fetch/Stream API**

   Cobalt now supports the Fetch/Stream API.

 - **On-device Speech-to-Text support**

   Support for utilizing the new Starboard speech recognizer interface in order
   to allow for on-device speech-to-text support is now added.  The Starboard
   interface is defined in
   [starboard/speech_recognizer.h](../starboard/speech_recognizer.h).

 - **Mouse pointer support**

   Cobalt now supports pointer events and will respond to Starboard
   `kSbInputEventTypeMove` events.  These will be passed into the WebModule to
   be processed by JavaScript.

 - **Custom Web Extension Support**

   Cobalt now allows platforms to inject a custom namespace property into
   the JavaScript global `window` object visible to the web apps.  This allows
   for custom web apps to interface with custom C++ code written outside of
   Cobalt common code.  See
   [doc/webapi_extension.md](doc/webapi_extension.md) for more details.

 - **Playback Rate**

   Cobalt now supports adjusting the video playback rate.

 - **AutoMem - Memory Configuration**

   AutoMem has been added which assists developers in tuning the memory
   settings of the Cobalt app. On startup, memory settings are now printed
   for all builds except gold. Memory settings can be altered via the
   command line, build files and in some instances the Starboard API.
   For more information, see [cobalt/doc/memory_tuning.md](doc/memory_tuning.md).

 - **Page Visibility API Support**

   Cobalt now supports the page visibility API, and will signal visibility and
   focus changes to the web app as the process transitions between Starboard
   lifecycle states. See the
   new [Application Lifecycle Integration](doc/lifecycle.md) document for more
   details.

 - **Opus Support**

   Added support for providing Opus audio-specific config to Starboard. Requires
   Starboard 6.

 - **Linux build now supports 360 video**

   The Linux build linux-x64x11 now supports 360 video, and can be used as a
   reference implementation.

 - **Stop the application if not retrying after a connection error**

   A positive response from `kSbSystemPlatformErrorTypeConnectionError` now
   indicates that Cobalt should retry the failed request. Any other response now
   causes Cobalt to call `SbSystemRequestStop`.

 - **Frame rate counter**

   A frame rate counter is now made accessible.  It actually displays frame
   times, the inverse of frame rate.  In this case, 16.6ms corresponds to 60fps.
   It is accessible both as an overlay on the display by the command line
   option, "--fps_overlay".  The data can also be printed to stdout with the
   command line option "--fps_stdout".  The frame rate statistics will be
   updated each time an animation ends, or after 60 frames have been processed.
   Both command line flags are available in Gold builds.

 - **Add support for rendering `kSbDecodeTargetFormat1PlaneUYVY` (YUV 422)**

   Decode targets with the format `kSbDecodeTargetFormat1PlaneUYVY` will now
   be rendered by Cobalt.  This will allow decoders that produce YUV 422 UYVY
   video frames to now efficiently support 360 video.

 - **Preload Support**

   Support for preloading an application with no graphics resources. See the
   new [Application Lifecycle Integration](doc/lifecycle.md) guide for more
   details.

 - **Improvements and Bug Fixes**
   - Fixed position elements given correct draw order without requiring z-index.
   - Overflow-hidden correctly applied when z-index is used.
   - Padding box forms containing block for fixed position elements within
     transforms.
   - Determine whether a font is bold in more cases, so synthetic bolding isn't
     incorrectly applied.
   - Fix bug where decoding images with large headers would fail.
   - Include missing CSS transforms in `GetClientRects()` calculations.
   - CSS Transitions and Animations not resetting properly when adjusting
     `display: none` on an element.
   - `offset_left` and similar properties are no longer cause many expensive
     re-layouts to query.
   - Fix bug where pseudo-elements were "inheriting" the inline style of their
     parent elements as if it were their own inline style.

## Version 9
 - **Improvements and Bug Fixes**
   - Non-fixed position elements given correct draw order without requiring
     z-index.
