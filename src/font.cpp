#include <memory>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#else
#include "error.h"
#include <iostream>
#endif

#include "sdl_ttf_font.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl;
using namespace sdl_ttf_opengl_cpp;
using namespace sdl_ttf_opengl_cpp::sdl_ttf;

Font::Font(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_,
           const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf_,
           const std::string &fn, const int ptsize)
    : sdl{sdl_}, ttf{ttf_}, filename{fn}, point_size{ptsize} {
  font = ttf->OpenFont(filename.c_str(), point_size);

#ifndef NO_EXCEPTIONS
  if (font == NULL) {
    throw FontOpenError("ERROR::FONT::FONT_OPEN_FAILED");
  }
#else
  if (font == NULL) {
    set_error(std::optional<error>(sdl_opengl_cpp::error::SDLTTFFontOpenError));
    cleanup();
    return;
  }
#endif
}

Font::Font(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_,
           const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf_,
           const std::string &fn, const int ptsize, const render_style rs)
    : sdl{sdl_}, ttf{ttf_}, filename{fn}, point_size{ptsize},
      font_render_style{rs} {
  font = ttf->OpenFont(filename.c_str(), point_size);

#ifndef NO_EXCEPTIONS
  if (font == NULL) {
    throw FontOpenError("ERROR::FONT::FONT_OPEN_FAILED");
  }
#else
  if (font == NULL) {
    set_error(std::optional<error>(sdl_opengl_cpp::error::SDLTTFFontOpenError));
    cleanup();
    return;
  }
#endif

  SetFontStyle(rs);
}

Font::~Font() { cleanup(); }

// move constructor
Font::Font(Font &&f) noexcept
    : sdl{f.sdl}, ttf{f.ttf}, filename{f.filename}, font{f.font},
      point_size{f.point_size} {

#ifdef NO_EXCEPTIONS
  last_operation_failed = f.last_operation_failed;
  last_error = std::move(f.last_error);
#endif

  f.sdl = nullptr;
  f.ttf = nullptr;
  f.font = nullptr;
  f.point_size = 0;
}

// move assignment operator
Font &Font::operator=(Font &&f) noexcept {
  if (&f != this) {
    sdl = f.sdl;
    ttf = f.ttf;
    font = f.font;
    filename = f.filename;
    point_size = f.point_size;

#ifdef NO_EXCEPTIONS
    last_operation_failed = f.last_operation_failed;
    last_error = std::move(f.last_error);
#endif

    f.sdl = nullptr;
    f.ttf = nullptr;
    f.font = nullptr;
    f.point_size = 0;
  }

  return *this;
}

void Font::cleanup() noexcept {
  if ((ttf != nullptr) && (font != nullptr)) {
#ifndef NO_EXCEPTIONS
    try {
      ttf->CloseFont(font);
    } catch (sdl::UnspecifiedStateError &e) {
      // spdlog::error("ERROR::SDL_TTF::CloseFont::{}", e);
    }
#else
    ttf->CloseFont(font);
#endif
  }

  sdl = nullptr;
  ttf = nullptr;
  font = nullptr;
  point_size = 0;
}

int Font::render_style_to_sdl_render_style(const render_style rs) {
  switch (rs) {
  case render_style::NORMAL:
    return TTF_STYLE_NORMAL;
    break;
  case render_style::BOLD:
    return TTF_STYLE_BOLD;
    break;
  case render_style::ITALIC:
    return TTF_STYLE_ITALIC;
    break;
  case render_style::UNDERLINE:
    return TTF_STYLE_UNDERLINE;
    break;
  }

  return TTF_STYLE_NORMAL;
}

void Font::SetFontStyle(const render_style renderstyle) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL TTF Object is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return;
#endif
  }

  ttf->SetFontStyle(font, render_style_to_sdl_render_style(renderstyle));
}

int Font::FontHeight() {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL TTF Object is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return -1;
#endif
  }
  return ttf->FontHeight(font);
}

std::unique_ptr<sdl_opengl_cpp::SDLSurface>
Font::RenderText_Solid(const string &text, SDL_Color fg) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL TTF Object is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return nullptr;
#endif
  }
  return std::make_unique<sdl_opengl_cpp::SDLSurface>(
      sdl, TTF_RenderText_Solid(font, text.c_str(), fg));
}

std::unique_ptr<sdl_opengl_cpp::SDLSurface>
Font::RenderText_Blended(const string &text, SDL_Color fg) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL TTF Object is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return nullptr;
#endif
  }
  return std::make_unique<sdl_opengl_cpp::SDLSurface>(
      sdl, TTF_RenderText_Blended(font, text.c_str(), fg));
}

void Font::dump() {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL TTF Object is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return;
#endif
  }

  SDL_Color white = {0xFF, 0xFF, 0xFF, 0};
  SDL_Color black = {0x00, 0x00, 0x00, 0};
  SDL_Color *forecol;
  SDL_Color *backcol;

  // Default is black and white
  forecol = &black;
  backcol = &white;

  for (int i = 48; i < 123; i++) {
    std::unique_ptr<sdl_opengl_cpp::SDLSurface> glyph = nullptr;

    glyph = std::make_unique<sdl_opengl_cpp::SDLSurface>(
        sdl, TTF_RenderGlyph_Shaded(font, i, *forecol, *backcol));

    if (glyph) {
      char outname[64];
      sprintf(outname, "glyph-%d.bmp", i);
      glyph->SaveBMP(outname);
    }
  }
}

// Implement checking for an unspecified state
bool Font::is_in_unspecified_state() const {
  if ((sdl == nullptr) || (ttf == nullptr) || (font == nullptr))
    return true;
  else
    return false;
}
