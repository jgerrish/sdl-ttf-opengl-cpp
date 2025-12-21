#ifndef _SDL_TTF_OPENGL_CPP_SDL_TTF_TEST_H_
#define _SDL_TTF_OPENGL_CPP_SDL_TTF_TEST_H_

#include <memory>
#include <optional>

#include "mock_sdl_ttf.h"
#include "sdl_ttf_base.h"
#include "sdl_ttf_wrapper.h"

namespace sdl_ttf_opengl_cpp {

class SDLTTFTester {
public:
  SDLTTFTester(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl,
               const std::shared_ptr<MockSDLTTFWrapper> &mock_sdl_ttf_wrapper);

  // Explicitly delete the generated default copy constructor
  SDLTTFTester(const SDLTTFTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLTTFTester &operator=(const SDLTTFTester &) = delete;

  // move constructor
  SDLTTFTester(SDLTTFTester &&) = delete;

  // move assignment operator
  SDLTTFTester &operator=(SDLTTFTester &&) = delete;

  // Surface the underlying SDL handle so we can test it.
  bool initialized() const;

  void set_uninitialized_ttf();

public:
  std::optional<SDLTTF> ttf = std::nullopt;
};

} // namespace sdl_ttf_opengl_cpp

#endif
