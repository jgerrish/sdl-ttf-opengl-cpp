#include <string>

#include <stdarg.h>

// #ifndef NO_EXCEPTIONS
// #include "spdlog/spdlog.h"
// #endif

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"
#include "sdl_ttf_wrapper.h"

using namespace sdl_ttf_opengl_cpp;

SDLTTFWrapper::~SDLTTFWrapper() {}

SDLTTFWrapper::SDLTTFWrapper() {}

int SDLTTFWrapper::Init() { return TTF_Init(); }

void SDLTTFWrapper::Quit() { return TTF_Quit(); }

TTF_Font *SDLTTFWrapper::OpenFont(const char *file, int ptsize) {
  return TTF_OpenFont(file, ptsize);
}

void SDLTTFWrapper::CloseFont(TTF_Font *font) { return TTF_CloseFont(font); }

void SDLTTFWrapper::SetFontStyle(TTF_Font *font, int style) {
  return TTF_SetFontStyle(font, style);
}

int SDLTTFWrapper::FontHeight(const TTF_Font *font) {
  return TTF_FontHeight(font);
}

SDL_Surface *SDLTTFWrapper::RenderText_Solid(TTF_Font *font, const char *text,
                                             SDL_Color fg) {
  return TTF_RenderText_Solid(font, text, fg);
}

SDL_Surface *SDLTTFWrapper::RenderUTF8_Solid(TTF_Font *font, const char *text,
                                             SDL_Color fg) {
  return TTF_RenderUTF8_Solid(font, text, fg);
}

SDL_Surface *SDLTTFWrapper::RenderUNICODE_Solid(TTF_Font *font,
                                                const Uint16 *text,
                                                SDL_Color fg) {
  return TTF_RenderUNICODE_Solid(font, text, fg);
}

SDL_Surface *SDLTTFWrapper::RenderText_Blended(TTF_Font *font, const char *text,
                                               SDL_Color fg) {
  return TTF_RenderText_Blended(font, text, fg);
}

SDL_Surface *SDLTTFWrapper::RenderUTF8_Blended(TTF_Font *font, const char *text,
                                               SDL_Color fg) {
  return TTF_RenderUTF8_Blended(font, text, fg);
}

SDL_Surface *SDLTTFWrapper::RenderUNICODE_Blended(TTF_Font *font,
                                                  const Uint16 *text,
                                                  SDL_Color fg) {
  return TTF_RenderUNICODE_Blended(font, text, fg);
}
