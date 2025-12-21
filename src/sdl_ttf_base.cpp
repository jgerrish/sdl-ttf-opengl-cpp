#include <string>

#include <stdarg.h>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#else
#include "error.h"
#endif

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"
#include "sdl_ttf_base.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl;
using namespace sdl_ttf_opengl_cpp;
using namespace sdl_ttf_opengl_cpp::sdl_ttf;

SDLTTF::SDLTTF(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_) : sdl{sdl_} {
  sdl_ttf_wrapper = std::make_shared<SDLTTFWrapper>();

  build();
}

SDLTTF::SDLTTF(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_,
               const std::shared_ptr<SDLTTFWrapper> &sdl_ttf_wrapper_)
    : sdl{sdl_}, sdl_ttf_wrapper{sdl_ttf_wrapper_} {

  build();
};

void SDLTTF::build() {
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

  int res = sdl_ttf_wrapper->Init();

  if (res < 0) {
    const char *sdl_error_string = sdl->GetError();
    std::string error_string;
    if (sdl_error_string == nullptr) {
      error_string =
          std::string("Couldn't initialize SDL TTF.  TTF_Init failed.");
    } else {
      std::string error_msg = std::string(sdl_error_string);
      error_string =
          std::string("Couldn't initialize SDL TTF.  TTF_Init failed: ") +
          sdl_error_string;
    }

#ifndef NO_EXCEPTIONS
    // spdlog::error(error_string);
    throw SDLTTFInitFailedError(error_string);
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::SDLTTFInitFailedError));
    /* Enable standard application logging */
    sdl->LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    sdl->LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", error_string.c_str());

    return;
#endif
  } else {
    initialized = true;
  }
}

SDLTTF::~SDLTTF() { cleanup(); }

// move constructor
SDLTTF::SDLTTF(SDLTTF &&s) noexcept {
  sdl = s.sdl;
  sdl_ttf_wrapper = s.sdl_ttf_wrapper;
  initialized = s.initialized;

#ifdef NO_EXCEPTIONS
  last_operation_failed = s.last_operation_failed;
  last_error = std::move(s.last_error);
#endif

  s.sdl = nullptr;
  s.sdl_ttf_wrapper = nullptr;
  s.initialized = false;
}

// move assignment operator
SDLTTF &SDLTTF::operator=(SDLTTF &&s) noexcept {
  if (&s != this) {
    sdl = s.sdl;
    sdl_ttf_wrapper = s.sdl_ttf_wrapper;
    initialized = s.initialized;

#ifdef NO_EXCEPTIONS
    last_operation_failed = s.last_operation_failed;
    last_error = s.last_error;
#endif

    s.sdl = nullptr;
    s.sdl_ttf_wrapper = nullptr;
    s.initialized = false;
  }

  return *this;
}

void SDLTTF::cleanup() noexcept {
  if (initialized) {
    sdl_ttf_wrapper->Quit();
  }
  initialized = false;
}

bool SDLTTF::is_in_unspecified_state() const {
  if ((sdl == nullptr) || (sdl_ttf_wrapper == nullptr))
    return true;
  else
    return false;
}

int SDLTTF::Init() {
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

  return sdl_ttf_wrapper->Init();
}

void SDLTTF::Quit() {
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

  return sdl_ttf_wrapper->Quit();
}

TTF_Font *SDLTTF::OpenFont(const char *file, int ptsize) {
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

  return sdl_ttf_wrapper->OpenFont(file, ptsize);
}

void SDLTTF::CloseFont(TTF_Font *font) {
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

  return sdl_ttf_wrapper->CloseFont(font);
}

void SDLTTF::SetFontStyle(TTF_Font *font, int style) {
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

  return sdl_ttf_wrapper->SetFontStyle(font, style);
}

int SDLTTF::FontHeight(const TTF_Font *font) {
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

  return sdl_ttf_wrapper->FontHeight(font);
}

SDL_Surface *SDLTTF::RenderText_Solid(TTF_Font *font, const char *text,
                                      SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderText_Solid(font, text, fg);
}

SDL_Surface *SDLTTF::RenderUTF8_Solid(TTF_Font *font, const char *text,
                                      SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderUTF8_Solid(font, text, fg);
}

SDL_Surface *SDLTTF::RenderUNICODE_Solid(TTF_Font *font, const Uint16 *text,
                                         SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderUNICODE_Solid(font, text, fg);
}

SDL_Surface *SDLTTF::RenderText_Blended(TTF_Font *font, const char *text,
                                        SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderText_Blended(font, text, fg);
}

SDL_Surface *SDLTTF::RenderUTF8_Blended(TTF_Font *font, const char *text,
                                        SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderUTF8_Blended(font, text, fg);
}

SDL_Surface *SDLTTF::RenderUNICODE_Blended(TTF_Font *font, const Uint16 *text,
                                           SDL_Color fg) {
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

  return sdl_ttf_wrapper->RenderUNICODE_Blended(font, text, fg);
}
