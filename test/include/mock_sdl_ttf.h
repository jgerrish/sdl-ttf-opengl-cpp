#ifndef _SDL_TTF_OPENGL_CPP_MOCK_TTF_H_
#define _SDL_TTF_OPENGL_CPP_MOCK_TTF_H_

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"
#include "sdl_ttf_base.h"
#include "sdl_ttf_wrapper.h"

#include "gl_context.h"

namespace sdl_ttf_opengl_cpp {

class MockSDLTTFWrapper : public SDLTTFWrapper {
public:
  MockSDLTTFWrapper() : SDLTTFWrapper() {}

  ~MockSDLTTFWrapper(){};

  MOCK_METHOD(int, Init, (), (override));
  MOCK_METHOD(void, Quit, (), (override));

  MOCK_METHOD(TTF_Font *, OpenFont, (const char *file, int ptsize), (override));

  MOCK_METHOD(void, CloseFont, (TTF_Font * font), (override));

  MOCK_METHOD(void, SetFontStyle, (TTF_Font * font, int style), (override));

  MOCK_METHOD(int, FontHeight, (const TTF_Font *font), (override));

  MOCK_METHOD(SDL_Surface *, RenderText_Solid,
              (TTF_Font * font, const char *text, SDL_Color fg), (override));

  MOCK_METHOD(SDL_Surface *, RenderUTF8_Solid,
              (TTF_Font * font, const char *text, SDL_Color fg), (override));

  MOCK_METHOD(SDL_Surface *, RenderUNICODE_Solid,
              (TTF_Font * font, const Uint16 *text, SDL_Color fg), (override));

  MOCK_METHOD(SDL_Surface *, RenderText_Blended,
              (TTF_Font * font, const char *text, SDL_Color fg), (override));

  MOCK_METHOD(SDL_Surface *, RenderUTF8_Blended,
              (TTF_Font * font, const char *text, SDL_Color fg), (override));

  MOCK_METHOD(SDL_Surface *, RenderUNICODE_Blended,
              (TTF_Font * font, const Uint16 *text, SDL_Color fg), (override));

public:
  bool initialized = false;
};

} // namespace sdl_ttf_opengl_cpp

#endif
