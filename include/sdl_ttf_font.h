#ifndef _SDL_TTF_OPENGL_CPP_FONT_H_
#define _SDL_TTF_OPENGL_CPP_FONT_H_

#include <memory>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "sdl_base.h"
#include "sdl_surface_base.h"
#include "sdl_ttf_base.h"

#define DEFAULT_PTSIZE 18

namespace sdl_ttf_opengl_cpp {

enum class render_style { NORMAL, BOLD, ITALIC, UNDERLINE };

namespace sdl_ttf {

#ifndef NO_EXCEPTIONS

//! A FontOpenError exception
//!
//! This exception is thrown when opening a font fails
class FontOpenError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

} // namespace sdl_ttf

#ifndef NO_EXCEPTIONS
class Font : private sdl_opengl_cpp::MoveChecker {
#else
class Font : public sdl_opengl_cpp::Errors {
#endif
public:
  Font(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_,
       const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf_,
       const std::string &fn, const int ptsize);
  Font(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl_,
       const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf_,
       const std::string &fn, const int ptsize, const render_style rs);

  ~Font();

  // Explicitly delete the generated default copy constructor
  Font(const Font &) = delete;

  // Explicitly delete the generated default copy assignment
  // operator
  Font &operator=(const Font &) = delete;

  //! move constructor
  Font(Font &&) noexcept;

  //! move assignment operator
  Font &operator=(Font &&) noexcept;

  static int render_style_to_sdl_render_style(const render_style rs);
  void SetFontStyle(const render_style renderstyle);

  bool is_in_unspecified_state() const override;

  int FontHeight();

  std::unique_ptr<sdl_opengl_cpp::SDLSurface>
  RenderText_Solid(const string &text, SDL_Color fg);

  // std::unique_ptr<sdl_opengl_cpp::SDLSurface>
  // RenderUNICODE_Solid(const Uint16 *text, SDL_Color fg);

  std::unique_ptr<sdl_opengl_cpp::SDLSurface>
  RenderText_Blended(const string &text, SDL_Color fg);

  // std::unique_ptr<sdl_opengl_cpp::SDLSurface>
  // RenderUTF8_Blended(const string &text, SDL_Color fg);

  // std::unique_ptr<sdl_opengl_cpp::SDLSurface>
  // // RenderUNICODE_Blended(const Uint16 *text, SDL_Color fg);
  // RenderUNICODE_Blended(const string &text, SDL_Color fg);

  void dump();

private:
  //! The underlying SDL object to use for rendering the font
  std::shared_ptr<sdl_opengl_cpp::SDL> sdl = nullptr;

  //! The underlying SDL object for this window
  std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf = nullptr;

  std::string filename;
  int point_size;
  render_style font_render_style;

  TTF_Font *font;

  void cleanup() noexcept;

  // bool is_in_unspecified_state() const override;
};

} // namespace sdl_ttf_opengl_cpp

#endif
