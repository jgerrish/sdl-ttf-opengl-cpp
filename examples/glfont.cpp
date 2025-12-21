// This example is a modified version of the glfont.c file in the SDL_ttf
// project.
// It is based on an older version
//
// glfont:  An example of using the SDL_ttf library with OpenGL.
// Copyright (C) 2001-2024 Sam Lantinga <slouken@libsdl.org>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//
// A simple program to test the text rendering feature of the TTF library

// quiet windows compiler warnings
#define _CRT_SECURE_NO_WARNINGS

#include <functional>
#include <string>
#include <memory>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#else
#include "move_checker.h"
#include "spdlog/spdlog.h"
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#define HAVE_OPENGL

#ifdef HAVE_OPENGL

#include "SDL_opengl.h"

#include "sdl_ttf_font.h"
#include "sdl_opengl_runner.h"
#include "sdl_window.h"

#define DEFAULT_PTSIZE  18

#define DEFAULT_TEXT    "The quick brown fox jumped over the lazy dog"
#define WIDTH   640
#define HEIGHT  480

#define TTF_GLFONT_USAGE \
"Usage: %s [-utf8|-unicode] [-b] [-i] [-u] [-fgcol r,g,b] [-bgcol r,g,b] \
<font>.ttf [ptsize] [text]\n"

using namespace sdl_opengl_cpp;
using namespace sdl_ttf_opengl_cpp;

static void SDL_GL_Enter2DMode(std::shared_ptr<sdl_opengl_cpp::GLContext> &glcontext,
			       int width,
			       int height)
{
    /* Note, there may be other things you need to change,
       depending on how you have your OpenGL state set up.
    */
    glcontext->glPushAttrib(GL_ENABLE_BIT);
    glcontext->glDisable(GL_DEPTH_TEST);
    glcontext->glDisable(GL_CULL_FACE);
    glcontext->glEnable(GL_TEXTURE_2D);

    /* This allows alpha blending of 2D textures with the scene */
    glcontext->glEnable(GL_BLEND);
    glcontext->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glcontext->glViewport(0, 0, width, height);

    glcontext->glMatrixMode(GL_PROJECTION);
    glcontext->glPushMatrix();
    glcontext->glLoadIdentity();

    glcontext->glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, 0.0, 1.0);

    glcontext->glMatrixMode(GL_MODELVIEW);
    glcontext->glPushMatrix();
    glcontext->glLoadIdentity();

    glcontext->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static void SDL_GL_Leave2DMode(std::shared_ptr<sdl_opengl_cpp::GLContext> &glcontext)
{
    glcontext->glMatrixMode(GL_MODELVIEW);
    glcontext->glPopMatrix();

    glcontext->glMatrixMode(GL_PROJECTION);
    glcontext->glPopMatrix();

    glcontext->glPopAttrib();
}


static void cleanup(int exitcode)
{
  // Managed by SDL TTF class
  // ttf->Quit();

  // Managed by SDL class
  // SDL_Quit();

  exit(exitcode);
}



void runit(const std::string &filename,
	   std::shared_ptr<GLContext> &gl_context,
	   std::shared_ptr<SDL> &sdl,
	   std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window) {
  std::unique_ptr<SDLSurface> text = nullptr;
  int done;
  SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
  SDL_Color black = { 0x00, 0x00, 0x00, 0 };
  SDL_Color *forecol;
  SDL_Color *backcol;
  render_style renderstyle;
    enum {
        RENDER_LATIN1,
        RENDER_UTF8,
        RENDER_UNICODE
    } rendertype;
  GLfloat texMinX, texMinY;
  GLfloat texMaxX, texMaxY;
  GLuint texture;
  int x, y, w, h;
  GLfloat texcoord[4];
  int dump;

  SDL_Event event;
  int ptsize;
  int i;
  GLenum gl_error;

  sdl_opengl_cpp::ClippingPlanes clipping_planes(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

  /* Look for special execution mode */
  dump = 0;

    /* Look for special rendering types */
  renderstyle = render_style::NORMAL;
  rendertype = RENDER_LATIN1;
  /* Default is black and white */
  forecol = &black;
  backcol = &white;

  const std::string msg(DEFAULT_TEXT);

  // Initialize the TTF library
  std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
    std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(sdl_ttf_opengl_cpp::SDLTTF(sdl));

  /* Open the font file with the requested point size */
  ptsize = 0;
  if (ptsize == 0) {
    i = 2;
    ptsize = DEFAULT_PTSIZE;
  } else {
    i = 3;
  }

  Font font = Font(sdl, ttf, filename, ptsize, renderstyle);

  if (dump) {
    font.dump();
    cleanup(0);
  }

  switch (rendertype) {
  case RENDER_LATIN1:
    text = font.RenderText_Blended(msg, *forecol);
    break;

  // case RENDER_UTF8:
  //   text = font.RenderUTF8_Blended(msg, *forecol);
  //   break;

  // case RENDER_UNICODE:
  //   {
  //     /* This doesn't actually work because you can't pass
  // 	 UNICODE text in via command line, AFAIK, but...
  //     */
  //     text = font.RenderUNICODE_Blended(msg, *forecol);
  //   }
  //   break;
  }


  if (text == NULL) {
    fprintf(stderr, "Couldn't render text: %s\n", sdl->GetError());
    // TTF_CloseFont(font);
    cleanup(2);
  }
  x = (WIDTH - text->w())/2;
  y = (HEIGHT - text->h())/2;
  w = text->w();
  h = text->h();
  printf("Font is generally %d big, and string is %d big\n",
	 font.FontHeight(), text->h());

  /* Convert the text into an OpenGL texture */
  gl_context->glGetError();
  texture = text->GL_LoadTexture(gl_context, texcoord);
  if ((gl_error = gl_context->glGetError()) != GL_NO_ERROR) {
    /* If this failed, the text may exceed texture size limits */
    printf("Warning: Couldn't create texture: 0x%x\n", gl_error);
  }

  /* Make texture coordinates easy to understand */
  texMinX = texcoord[0];
  texMinY = texcoord[1];
  texMaxX = texcoord[2];
  texMaxY = texcoord[3];

  /* We don't need the original text surface anymore */
  // But we can let it get deleted at the end of the scope
  // SDL_FreeSurface(text);
  // delete text;

  /* Initialize the GL state */
  gl_context->glViewport(0, 0, WIDTH, HEIGHT);
  gl_context->glMatrixMode(GL_PROJECTION);
  gl_context->glLoadIdentity();

  gl_context->glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

  gl_context->glMatrixMode(GL_MODELVIEW);
  gl_context->glLoadIdentity();

  gl_context->glEnable(GL_DEPTH_TEST);

  gl_context->glDepthFunc(GL_LESS);

  gl_context->glShadeModel(GL_SMOOTH);

  /* Wait for a keystroke, and blit text on mouse press */
  done = 0;

  while (!done) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_MOUSEMOTION:
	x = event.motion.x - w/2;
	y = event.motion.y - h/2;
	break;

      case SDL_KEYDOWN:
      case SDL_QUIT:
	done = 1;
	break;
      default:
	break;
      }
    }

    /* Clear the screen */
    gl_context->glClearColor(1.0, 1.0, 1.0, 1.0);
    gl_context->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /* Show the text on the screen */
    SDL_GL_Enter2DMode(gl_context, WIDTH, HEIGHT);
    gl_context->glBindTexture(GL_TEXTURE_2D, texture);

    gl_context->glBegin(GL_TRIANGLE_STRIP);
    gl_context->glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y);
    gl_context->glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y);
    gl_context->glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
    gl_context->glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
    gl_context->glEnd();

    SDL_GL_Leave2DMode(gl_context);

    /* Swap the buffers so everything is visible */
    window->GL_SwapWindow();
  }
}

int main(int argc, char *argv[])
{
    SDL_GLContext context;

    /* Check usage */
    if ((argc != 2) || !argv[0] || !argv[1]) {
        fprintf(stderr, TTF_GLFONT_USAGE, argv[0]);
        return(1);
    }

    std::string font_fn(argv[1]);

    
    // Initialize the SDL library
    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
      std::make_shared<sdl_opengl_cpp::SDL>(sdl_opengl_cpp::SDL());

    std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> window =
      std::make_unique<sdl_opengl_cpp::sdl_window::SDLWindow>
      (sdl_opengl_cpp::sdl_window::SDLWindow(sdl, "glfont",
					     SDL_WINDOWPOS_UNDEFINED,
					     SDL_WINDOWPOS_UNDEFINED,
					     WIDTH, HEIGHT, SDL_WINDOW_OPENGL));
    if (window == NULL) {
        fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
        cleanup(2);
    }

    // TODO: Replace this with a lambda expression
    // bind is no longer in fashion
    std::function<void(std::shared_ptr<GLContext> &context,
		       std::shared_ptr<SDL> &s,
		       std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window)> func =
      std::bind(runit,
		font_fn,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3);


    sdl_opengl_cpp::ClippingPlanes clipping_planes(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

    SDLOpenGL runner(sdl, window, func, clipping_planes);

    cleanup(0);

    /* Not reached, but fixes compiler warnings */
    return 0;
}


#else /* HAVE_OPENGL */

int main(int argc, char *argv[])
{
    printf("No OpenGL support on this system\n");
    return 1;
}

#endif /* HAVE_OPENGL */

/* vi: set ts=4 sw=4 expandtab: */
