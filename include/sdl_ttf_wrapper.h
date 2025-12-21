#ifndef _SDL_TTF_OPENGL_CPP_SDL_TTF_WRAPPER_H_
#define _SDL_TTF_OPENGL_CPP_SDL_TTF_WRAPPER_H_

#ifdef NO_EXCEPTIONS
#include <optional>
// #else
// #include "spdlog/spdlog.h"
#endif

#include "SDL.h"
#include "SDL_ttf.h"

using namespace std;

namespace sdl_ttf_opengl_cpp {

class SDLTTFWrapper {
public:
  SDLTTFWrapper();
  SDLTTFWrapper(Uint32 flags);

  virtual ~SDLTTFWrapper();

  //! Initialize SDL_ttf.
  //!
  //! You must successfully call this function before it is safe to call any
  //! other function in this library, with one exception: a human-readable error
  //! message can be retrieved from TTF_GetError() if this function fails.
  //!
  //! SDL must be initialized before calls to functions in this library, because
  //! this library uses utility functions from the SDL library.
  //!
  //! It is safe to call this more than once; the library keeps a counter of
  //! init calls, and decrements it on each call to TTF_Quit, so you must pair
  //! your init and quit calls.
  //!
  //! \returns 0 on success, -1 on error.
  virtual int Init();

  //! Deinitialize SDL_ttf.
  //!
  //! You must call this when done with the library, to free internal resources.
  //! It is safe to call this when the library isn't initialized, as it will
  //! just return immediately.
  //!
  //! Once you have as many quit calls as you have had successful calls to
  //! TTF_Init, the library will actually deinitialize.
  //!
  //! Please note that this does not automatically close any fonts that are
  //! still open at the time of deinitialization, and it is possibly not safe to
  //! close them afterwards, as parts of the library will no longer be
  //! initialized to deal with it. A well-written program should call
  //! TTF_CloseFont() on any open fonts before calling this function!
  virtual void Quit();

  //!
  //! Create a font from a file, using a specified point size.
  //!
  //! Some .fon fonts will have several sizes embedded in the file, so the point
  //! size becomes the index of choosing which size. If the value is too high,
  //! the last indexed size will be the default.
  //!
  //! When done with the returned TTF_Font, use TTF_CloseFont() to dispose of
  //! it.
  //!
  //! \param file path to font file.
  //! \param ptsize point size to use for the newly-opened font.
  //!
  //! \returns a valid TTF_Font, or NULL on error.
  //!
  virtual TTF_Font *OpenFont(const char *file, int ptsize);

  //! Dispose of a previously-created font.
  //!
  //! Call this when done with a font. This function will free any resources
  //! associated with it. It is safe to call this function on NULL, for example
  //! on the result of a failed call to TTF_OpenFont().
  //!
  //! The font is not valid after being passed to this function. String pointers
  //! from functions that return information on this font, such as
  //! TTF_FontFaceFamilyName() and TTF_FontFaceStyleName(), are no longer valid
  //! after this call, as well.
  //!
  //! \param font the font to dispose of.
  virtual void CloseFont(TTF_Font *font);

  //! Set a font's current style.
  //!
  //! Setting the style clears already-generated glyphs, if any, from the cache.
  //!
  //! The font styles are a set of bit flags, OR'd together:
  //!
  //! - `TTF_STYLE_NORMAL` (is zero)
  //! - `TTF_STYLE_BOLD`
  //! - `TTF_STYLE_ITALIC`
  //! - `TTF_STYLE_UNDERLINE`
  //! - `TTF_STYLE_STRIKETHROUGH`
  //!
  //! \param font the font to set a new style on.
  //! \param style the new style values to set, OR'd together.
  virtual void SetFontStyle(TTF_Font *font, int style);

  //! Query the total height of a font.
  //!
  //! This is usually equal to point size.
  //!
  //! \param font the font to query.
  //!
  //! \returns the font's height.
  virtual int FontHeight(const TTF_Font *font);

  //! Render Latin1 text at fast quality to a new 8-bit surface.
  //!
  //! This function will allocate a new 8-bit, palettized surface. The surface's
  //! 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
  //! will be set to the text color.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderText_Solid_Wrapped() instead if you need to wrap the output to
  //! multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! You almost certainly want TTF_RenderUTF8_Solid() unless you're sure you
  //! have a 1-byte Latin1 encoding. US ASCII characters will work with either
  //! function, but most other Unicode characters packed into a `const char //!`
  //! will need UTF-8.
  //!
  //! You can render at other quality levels with TTF_RenderText_Shaded,
  //! TTF_RenderText_Blended, and TTF_RenderText_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in Latin1 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 8-bit, palettized surface, or NULL if there was an error.
  virtual SDL_Surface *RenderText_Solid(TTF_Font *font, const char *text,
                                        SDL_Color fg);

  //! Render UTF-8 text at fast quality to a new 8-bit surface.
  //!
  //! This function will allocate a new 8-bit, palettized surface. The surface's
  //! 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
  //! will be set to the text color.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderUTF8_Solid_Wrapped() instead if you need to wrap the output to
  //! multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! You can render at other quality levels with TTF_RenderUTF8_Shaded,
  //! TTF_RenderUTF8_Blended, and TTF_RenderUTF8_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in UTF-8 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 8-bit, palettized surface, or NULL if there was an error.
  virtual SDL_Surface *RenderUTF8_Solid(TTF_Font *font, const char *text,
                                        SDL_Color fg);

  //! Render UCS-2 text at fast quality to a new 8-bit surface.
  //!
  //! This function will allocate a new 8-bit, palettized surface. The surface's
  //! 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
  //! will be set to the text color.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderUNICODE_Solid_Wrapped() instead if you need to wrap the output
  //! to multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! Please note that this function is named "Unicode" but currently expects
  //! UCS-2 encoding (16 bits per codepoint). This does not give you access to
  //! large Unicode values, such as emoji glyphs. These codepoints are
  //! accessible through the UTF-8 version of this function.
  //!
  //! You can render at other quality levels with TTF_RenderUNICODE_Shaded,
  //! TTF_RenderUNICODE_Blended, and TTF_RenderUNICODE_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in UCS-2 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 8-bit, palettized surface, or NULL if there was an error.
  virtual SDL_Surface *RenderUNICODE_Solid(TTF_Font *font, const Uint16 *text,
                                           SDL_Color fg);

  //! Render Latin1 text at high quality to a new ARGB surface.
  //!
  //! This function will allocate a new 32-bit, ARGB surface, using alpha
  //! blending to dither the font with the given color. This function returns
  //! the new surface, or NULL if there was an error.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderText_Blended_Wrapped() instead if you need to wrap the output to
  //! multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! You almost certainly want TTF_RenderUTF8_Blended() unless you're sure you
  //! have a 1-byte Latin1 encoding. US ASCII characters will work with either
  //! function, but most other Unicode characters packed into a `const char //!`
  //! will need UTF-8.
  //!
  //! You can render at other quality levels with TTF_RenderText_Solid,
  //! TTF_RenderText_Blended, and TTF_RenderText_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in Latin1 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 32-bit, ARGB surface, or NULL if there was an error.
  virtual SDL_Surface *RenderText_Blended(TTF_Font *font, const char *text,
                                          SDL_Color fg);

  //! Render UTF-8 text at high quality to a new ARGB surface.
  //!
  //! This function will allocate a new 32-bit, ARGB surface, using alpha
  //! blending to dither the font with the given color. This function returns
  //! the new surface, or NULL if there was an error.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderUTF8_Blended_Wrapped() instead if you need to wrap the output to
  //! multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! You can render at other quality levels with TTF_RenderUTF8_Solid,
  //! TTF_RenderUTF8_Shaded, and TTF_RenderUTF8_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in UTF-8 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 32-bit, ARGB surface, or NULL if there was an error.
  virtual SDL_Surface *RenderUTF8_Blended(TTF_Font *font, const char *text,
                                          SDL_Color fg);

  //! Render UCS-2 text at high quality to a new ARGB surface.
  //!
  //! This function will allocate a new 32-bit, ARGB surface, using alpha
  //! blending to dither the font with the given color. This function returns
  //! the new surface, or NULL if there was an error.
  //!
  //! This will not word-wrap the string; you'll get a surface with a single
  //! line of text, as long as the string requires. You can use
  //! TTF_RenderUNICODE_Blended_Wrapped() instead if you need to wrap the output
  //! to multiple lines.
  //!
  //! This will not wrap on newline characters.
  //!
  //! Please note that this function is named "Unicode" but currently expects
  //! UCS-2 encoding (16 bits per codepoint). This does not give you access to
  //! large Unicode values, such as emoji glyphs. These codepoints are
  //! accessible through the UTF-8 version of this function.
  //!
  //! You can render at other quality levels with TTF_RenderUNICODE_Solid,
  //! TTF_RenderUNICODE_Shaded, and TTF_RenderUNICODE_LCD.
  //!
  //! \param font the font to render with.
  //! \param text text to render, in UCS-2 encoding.
  //! \param fg the foreground color for the text.
  //! \returns a new 32-bit, ARGB surface, or NULL if there was an error.
  virtual SDL_Surface *RenderUNICODE_Blended(TTF_Font *font, const Uint16 *text,
                                             SDL_Color fg);
};

} // namespace sdl_ttf_opengl_cpp

#endif
