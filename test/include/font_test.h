#ifndef _SDL_TTF_OPENGL_CPP_FONT_TEST_H_
#define _SDL_TTF_OPENGL_CPP_FONT_TEST_H_

#include <memory>
#include <optional>

#include "mock_sdl_ttf.h"
#include "sdl_ttf_base.h"
#include "sdl_ttf_font.h"

namespace sdl_ttf_opengl_cpp {

class FontTester {
public:
  FontTester(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl,
             const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf,
             const std::string &font_fn);
  // FontTester(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl,
  // 	       const std::shared_ptr<MockSDLTTFWrapper> &mock_sdl_ttf_wrapper);

  // Explicitly delete the generated default copy constructor
  FontTester(const FontTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  FontTester &operator=(const FontTester &) = delete;

  // move constructor
  FontTester(FontTester &&) = delete;

  // move assignment operator
  FontTester &operator=(FontTester &&) = delete;

  // Surface the underlying SDL handle so we can test it.
  // bool initialized() const;

  // void set_uninitialized_ttf();

public:
  // std::optional<SDLTTF> ttf = std::nullopt;
  std::optional<Font> font = std::nullopt;
};

} // namespace sdl_ttf_opengl_cpp

#endif
