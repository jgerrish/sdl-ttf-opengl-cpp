#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/doctest.h>
#include <memory>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "mock_sdl_ttf.h"
#include "opengl.h"
#include "sdl_ttf_base.h"
#include "sdl_ttf_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_ttf_opengl_cpp;

SDLTTFTester::SDLTTFTester(
    const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl,
    const std::shared_ptr<MockSDLTTFWrapper> &mock_sdl_ttf_wrapper) {
  ttf.emplace(SDLTTF(sdl, mock_sdl_ttf_wrapper));
}

bool SDLTTFTester::initialized() const { return ttf->initialized; }

void SDLTTFTester::set_uninitialized_ttf() { ttf->initialized = false; }

int main(int argc, char **argv) {

  doctest::Context context;
  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    return res;
  }

  return res;
}
