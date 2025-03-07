// Copyright 2015 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <ostream>
#include <vector>

#include "base/command_line.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/test/scoped_task_environment.h"
#include "cobalt/base/cobalt_paths.h"
#include "cobalt/cssom/viewport_size.h"
#include "cobalt/layout_tests/layout_snapshot.h"
#include "cobalt/layout_tests/test_parser.h"
#include "cobalt/layout_tests/test_utils.h"
#include "cobalt/math/size.h"
#include "cobalt/render_tree/animations/animate_node.h"
#include "cobalt/renderer/render_tree_pixel_tester.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using cobalt::cssom::ViewportSize;

namespace cobalt {
namespace layout_tests {

namespace switches {
// If enabled, running the tests will result in the generation of expected
// test output from the actual output to the output directory.
const char kRebaseline[] = "rebaseline";

// If enabled, will run all tests and display which ones passed and which
// ones failed, as usual, however it will also output new expected output
// images for ONLY the tests that failed.
const char kRebaselineFailedTests[] = "rebaseline-failed-tests";

// If enabled, will output details (in the form of files placed in the output
// directory) for all tests that fail.
const char kOutputFailedTestDetails[] = "output-failed-test-details";

// Like kOutputFailedTestDetails, but outputs details for tests that
// succeed as well.
const char kOutputAllTestDetails[] = "output-all-test-details";
}  // namespace switches

namespace {

void ScreenshotFunction(
    scoped_refptr<base::SingleThreadTaskRunner> expected_message_loop,
    renderer::RenderTreePixelTester* pixel_tester,
    const scoped_refptr<render_tree::Node>& node,
    const base::Optional<math::Rect>& clip_rect,
    const dom::ScreenshotManager::OnUnencodedImageCallback& callback) {
  if (base::MessageLoop::current()->task_runner() != expected_message_loop) {
    expected_message_loop->PostTask(
        FROM_HERE, base::Bind(&ScreenshotFunction, expected_message_loop,
                              pixel_tester, node, clip_rect, callback));
    return;
  }
  // The tests only take full-screen screenshots, so |clip_rect| is ignored.
  std::unique_ptr<uint8_t[]> image_data =
      pixel_tester->RasterizeRenderTree(node);
  const math::Size& image_dimensions = pixel_tester->GetTargetSize();
  callback.Run(std::move(image_data), image_dimensions);
}

struct GetTestName {
  std::string operator()(const ::testing::TestParamInfo<TestInfo>& info) const {
    // Only alphanumeric characters and '_' are valid.
    std::string name = info.param.base_file_path.BaseName().value();
    for (size_t i = 0; i < name.size(); ++i) {
      char ch = name[i];
      if (ch >= 'A' && ch <= 'Z') {
        continue;
      }
      if (ch >= 'a' && ch <= 'z') {
        continue;
      }
      if (ch >= '0' && ch <= '9') {
        continue;
      }
      name[i] = '_';
    }
    return name;
  }
};

}  // namespace

class Layout : public ::testing::TestWithParam<TestInfo> {};
TEST_P(Layout, Test) {
  // Output the name of the current input file so that it is visible in test
  // output.
  std::cout << "(" << GetParam() << ")" << std::endl;

  // Setup a message loop for the current thread since we will be constructing
  // a WebModule, which requires a message loop to exist for the current
  // thread.
  base::test::ScopedTaskEnvironment scoped_environment;

  // Setup the pixel tester we will use to perform pixel tests on the render
  // trees output by the web module.
  renderer::RenderTreePixelTester::Options pixel_tester_options;
  pixel_tester_options.output_failed_test_details =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kOutputFailedTestDetails);
  pixel_tester_options.output_all_test_details =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kOutputAllTestDetails);

  // Resolve the viewport size to a default if the test did not explicitly
  // specify a size.  The size of the test viewport tells us how many pixels our
  // layout tests will have available to them.  We must make a trade-off between
  // room for tests to maneuver within and speed at which pixel tests can be
  // done.
  const ViewportSize kDefaultViewportSize(640, 360);
  ViewportSize viewport_size = GetParam().viewport_size
                                   ? *GetParam().viewport_size
                                   : kDefaultViewportSize;

  renderer::RenderTreePixelTester pixel_tester(
      viewport_size.width_height(), GetTestInputRootDirectory(),
      GetTestOutputRootDirectory(), pixel_tester_options);

  browser::WebModule::LayoutResults layout_results = SnapshotURL(
      GetParam().url, viewport_size, pixel_tester.GetResourceProvider(),
      base::Bind(&ScreenshotFunction,
                 base::MessageLoop::current()->task_runner(),
                 base::Unretained(&pixel_tester)));

  scoped_refptr<render_tree::animations::AnimateNode> animate_node =
      new render_tree::animations::AnimateNode(layout_results.render_tree);

  scoped_refptr<render_tree::animations::AnimateNode> animated_node =
      animate_node->Apply(layout_results.layout_time).animated;

  // We reapply the animation application with the exact same layout time as
  // before.  This is an extra sanity check to ensure that the animated results
  // are deterministic.
  scoped_refptr<render_tree::animations::AnimateNode> twice_animated_node =
      animated_node->Apply(layout_results.layout_time).animated;
  EXPECT_EQ(animated_node, twice_animated_node);

  scoped_refptr<render_tree::Node> static_render_tree =
      twice_animated_node->source();

  bool results =
      pixel_tester.TestTree(static_render_tree, GetParam().base_file_path);

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kRebaseline) ||
      (!results && base::CommandLine::ForCurrentProcess()->HasSwitch(
                       switches::kRebaselineFailedTests))) {
    pixel_tester.Rebaseline(static_render_tree, GetParam().base_file_path);
  }

  EXPECT_TRUE(results);
}

// Cobalt-specific test cases.
INSTANTIATE_TEST_CASE_P(
    CobaltSpecificLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("cobalt")),
    GetTestName());
// Custom CSS 2.1 (https://www.w3.org/TR/CSS21/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSS21LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css-2-1")),
    GetTestName());
// Custom CSS Background (https://www.w3.org/TR/css3-background/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSSBackground3LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-background")),
    GetTestName());
// Custom CSS Color (https://www.w3.org/TR/css3-color/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSSColor3LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-color")),
    GetTestName());
// Custom CSS Images (https://www.w3.org/TR/css3-images/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSSImages3LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-images")),
    GetTestName());
// Custom CSS Media Queries (https://www.w3.org/TR/css3-mediaqueries/) test
// cases.
INSTANTIATE_TEST_CASE_P(
    CSSMediaQueriesLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-mediaqueries")),
    GetTestName());
// Custom CSS Text (https://www.w3.org/TR/css-text-3/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSSText3LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css-text-3")),
    GetTestName());
// Custom CSS Transform (http://https://www.w3.org/TR/css-transforms/)
// test cases.
INSTANTIATE_TEST_CASE_P(
    CSSTransformsLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css-transforms")),
    GetTestName());
// Custom CSS Transition
// (https://www.w3.org/TR/2013/WD-css3-transitions-20131119/)
// test cases.
INSTANTIATE_TEST_CASE_P(
    CSSTransitionLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-transitions")),
    GetTestName());
// Custom CSS Animation
// (https://www.w3.org/TR/2013/WD-css3-animations-20130219/#animations)
// test cases.
INSTANTIATE_TEST_CASE_P(
    CSSAnimationLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-animations")),
    GetTestName());
// Custom bidi text (http://www.unicode.org/reports/tr9/)
// (https://www.w3.org/TR/CSS21/visuren.html#direction) test cases.
INSTANTIATE_TEST_CASE_P(
    BidiLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("bidi")),
    GetTestName());
// Custom text shaping test cases.
INSTANTIATE_TEST_CASE_P(
    TextShapingLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("text-shaping")),
    GetTestName());
// Custom CSS Conditional (https://www.w3.org/TR/css3-conditional/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSSConditional3LayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-conditional")),
    GetTestName());
// Custom CSS Font (https://www.w3.org/TR/css3-fonts/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSS3FontsLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-fonts")),
    GetTestName());
// Custom CSS Text Decor (https://www.w3.org/TR/css-text-decor-3/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSS3TextDecorLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-text-decor")),
    GetTestName());
// Custom CSS UI (https://www.w3.org/TR/css3-ui/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSS3UILayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-ui")),
    GetTestName());
// Custom CSS Value (https://www.w3.org/TR/css3-values/) test cases.
INSTANTIATE_TEST_CASE_P(
    CSS3ValuesLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("css3-values")),
    GetTestName());
// Custom incremental layout test cases.
INSTANTIATE_TEST_CASE_P(
    IncrementalLayoutLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("incremental-layout")),
    GetTestName());
// Custom CSSOM view (https://www.w3.org/TR/2013/WD-cssom-view-20131217/)
// test cases.
INSTANTIATE_TEST_CASE_P(
    CSSOMViewLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("cssom-view")),
    GetTestName());

// JavaScript HTML5 WebAPIs (https://www.w3.org/TR/html5/webappapis.html) test
// cases.
INSTANTIATE_TEST_CASE_P(
    WebAppAPIsLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("webappapis")),
    GetTestName());

// JavaScript HTML5 APIs that describe requestAnimationFrame().
//   https://www.w3.org/TR/animation-timing/
INSTANTIATE_TEST_CASE_P(
    AnimationTimingAPILayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("animation-timing")),
    GetTestName());

// Problematic test cases found through cluster-fuzz.
INSTANTIATE_TEST_CASE_P(
    ClusterFuzzLayoutTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("cluster-fuzz")),
    GetTestName());

// Disable on Windows until network stack is implemented.
#if !defined(COBALT_WIN)
// Content Security Policy test cases.
INSTANTIATE_TEST_CASE_P(
    ContentSecurityPolicyTests, Layout,
    ::testing::ValuesIn(EnumerateLayoutTests("csp")),
    GetTestName());
#endif  // !defined(COBALT_WIN)

}  // namespace layout_tests
}  // namespace cobalt
