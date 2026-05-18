#include <doctest/doctest.h>
#include <memory>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#ifndef FONT_DIR
#define FONT_DIR "../../data/fonts/"
#endif

#ifndef VALID_ROBOTO_REGULAR_FONT
#define VALID_ROBOTO_REGULAR_FONT "Roboto/Roboto-Regular.ttf"
#endif

#ifndef INVALID_ROBOTO_REGULAR_FONT
#define INVALID_ROBOTO_REGULAR_FONT "Roboto/Robotor-Regular.ttf"
#endif

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "font_test.h"
#include "mock_sdl.h"
#include "mock_sdl_ttf.h"
#include "opengl.h"
#include "sdl_ttf_base.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;
using testing::StrEq;

using namespace sdl_ttf_opengl_cpp;

FontTester::FontTester(const std::shared_ptr<sdl_opengl_cpp::SDL> &sdl,
                       const std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> &ttf,
                       const std::string &font_fn) {

  font.emplace(Font(sdl, ttf, font_fn, 2));
}

// bool FontTester::initialized() const { return ttf->initialized; }

// void FontTester::set_uninitialized_ttf() { ttf->initialized = false; }

TEST_SUITE("sdl_ttf_opengl_cpp_font") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  // Tests without mocks
  // These tests test the actual SDL and SDL_ttf code calls

#ifndef NO_EXCEPTIONS
  TEST_CASE("testing that Font open works with exceptions enabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(sdl_opengl_cpp::SDL());
    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl));

    // seperating two strings with a space creates a single
    // concatenated string
    std::string font_fn(FONT_DIR VALID_ROBOTO_REGULAR_FONT);

    FontTester font_tester(sdl, ttf, font_fn);
  }
#else
  TEST_CASE("testing that Font open works with exceptions disabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(sdl_opengl_cpp::SDL());
    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl));

    std::string font_fn(FONT_DIR VALID_ROBOTO_REGULAR_FONT);

    FontTester font_tester(sdl, ttf, font_fn);
  }
#endif

#ifndef NO_EXCEPTIONS
  TEST_CASE("testing that Font open fails with invalid filename and exceptions "
            "enabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(sdl_opengl_cpp::SDL());
    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl));

    std::string font_fn(FONT_DIR INVALID_ROBOTO_REGULAR_FONT);

    CHECK_THROWS_WITH_AS([&] { FontTester font_tester(sdl, ttf, font_fn); }(),
                         "ERROR::FONT::FONT_OPEN_FAILED",
                         sdl_ttf::FontOpenError);
  }
#else
  TEST_CASE("testing that Font open fails with invalid filename and exceptions "
            "disabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(sdl_opengl_cpp::SDL());
    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl));

    std::string font_fn(FONT_DIR INVALID_ROBOTO_REGULAR_FONT);

    FontTester font_tester(sdl, ttf, font_fn);

    if (font_tester.font) {
      CHECK_EQ(font_tester.font->valid(), false);
      CHECK_EQ(font_tester.font->get_last_error(),
               sdl_opengl_cpp::error::SDLTTFFontOpenError);
    } else {
      CHECK(false);
    }
  }
#endif

  // Tests with mocks
  // The previous tests tested the functionality of the code without mocks
  // These tests show how to use the mocks

#ifndef NO_EXCEPTIONS
  TEST_CASE("testing that Font open works with MockSDLTTFWrapper and "
            "exceptions enabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::MockSDLWrapper> mock_sdl_wrapper =
        std::make_shared<sdl_opengl_cpp::MockSDLWrapper>();

    EXPECT_CALL(*mock_sdl_wrapper, Init(0))
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(mock_sdl_wrapper);

    std::shared_ptr<MockSDLTTFWrapper> mock_sdl_ttf_wrapper =
        std::make_shared<MockSDLTTFWrapper>();

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Init())
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl, mock_sdl_ttf_wrapper));

    std::string font_fn(FONT_DIR VALID_ROBOTO_REGULAR_FONT);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, OpenFont(StrEq(font_fn.c_str()), 2))
        .Times(1)
        .WillOnce(testing::Return(reinterpret_cast<TTF_Font *>(1)));

    EXPECT_CALL(*mock_sdl_ttf_wrapper,
                CloseFont(reinterpret_cast<TTF_Font *>(1)))
        .Times(1);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Quit()).Times(1);
    EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

    FontTester font_tester(sdl, ttf, font_fn);
  }
#else
  TEST_CASE("testing that Font open works with MockSDLTTFWrapper and "
            "exceptions disabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::MockSDLWrapper> mock_sdl_wrapper =
        std::make_shared<sdl_opengl_cpp::MockSDLWrapper>();

    EXPECT_CALL(*mock_sdl_wrapper, Init(0))
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(mock_sdl_wrapper);

    std::shared_ptr<MockSDLTTFWrapper> mock_sdl_ttf_wrapper =
        std::make_shared<MockSDLTTFWrapper>();

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Init())
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl, mock_sdl_ttf_wrapper));

    std::string font_fn(FONT_DIR VALID_ROBOTO_REGULAR_FONT);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, OpenFont(StrEq(font_fn.c_str()), 2))
        .Times(1)
        .WillOnce(testing::Return(reinterpret_cast<TTF_Font *>(1)));

    EXPECT_CALL(*mock_sdl_ttf_wrapper,
                CloseFont(reinterpret_cast<TTF_Font *>(1)))
        .Times(1);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Quit()).Times(1);
    EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

    FontTester font_tester(sdl, ttf, font_fn);
  }
#endif

#ifndef NO_EXCEPTIONS
  TEST_CASE("testing that Font open fails with MockSDLTTFWrapper and invalid "
            "filename and exceptions "
            "enabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::MockSDLWrapper> mock_sdl_wrapper =
        std::make_shared<sdl_opengl_cpp::MockSDLWrapper>();

    EXPECT_CALL(*mock_sdl_wrapper, Init(0))
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(mock_sdl_wrapper);

    std::shared_ptr<MockSDLTTFWrapper> mock_sdl_ttf_wrapper =
        std::make_shared<MockSDLTTFWrapper>();

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Init())
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl, mock_sdl_ttf_wrapper));

    std::string font_fn(FONT_DIR INVALID_ROBOTO_REGULAR_FONT);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, OpenFont(StrEq(font_fn.c_str()), 2))
        .Times(1)
        .WillOnce(testing::Return(reinterpret_cast<TTF_Font *>(NULL)));

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Quit()).Times(1);
    EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

    CHECK_THROWS_WITH_AS([&] { FontTester font_tester(sdl, ttf, font_fn); }(),
                         "ERROR::FONT::FONT_OPEN_FAILED",
                         sdl_ttf::FontOpenError);
  }
#else
  TEST_CASE("testing that Font open fails with MockSDLTTFWrapper and invalid "
            "filename and exceptions "
            "disabled") {
    // std::shared_ptr<MockOpenGLContext> mock_opengl_context =
    //     std::make_shared<MockOpenGLContext>(glcontext);

    std::shared_ptr<sdl_opengl_cpp::MockSDLWrapper> mock_sdl_wrapper =
        std::make_shared<sdl_opengl_cpp::MockSDLWrapper>();

    EXPECT_CALL(*mock_sdl_wrapper, Init(0))
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_opengl_cpp::SDL> sdl =
        std::make_shared<sdl_opengl_cpp::SDL>(mock_sdl_wrapper);

    std::shared_ptr<MockSDLTTFWrapper> mock_sdl_ttf_wrapper =
        std::make_shared<MockSDLTTFWrapper>();

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Init())
        .Times(1)
        .WillOnce(testing::Return(0));

    std::shared_ptr<sdl_ttf_opengl_cpp::SDLTTF> ttf =
        std::make_shared<sdl_ttf_opengl_cpp::SDLTTF>(
            sdl_ttf_opengl_cpp::SDLTTF(sdl, mock_sdl_ttf_wrapper));

    std::string font_fn(FONT_DIR INVALID_ROBOTO_REGULAR_FONT);

    EXPECT_CALL(*mock_sdl_ttf_wrapper, OpenFont(StrEq(font_fn.c_str()), 2))
        .Times(1)
        .WillOnce(testing::Return(reinterpret_cast<TTF_Font *>(NULL)));

    EXPECT_CALL(*mock_sdl_ttf_wrapper, Quit()).Times(1);
    EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

    FontTester font_tester(sdl, ttf, font_fn);

    if (font_tester.font) {
      CHECK_EQ(font_tester.font->valid(), false);
      CHECK_EQ(font_tester.font->get_last_error(),
               sdl_opengl_cpp::error::SDLTTFFontOpenError);
    } else {
      CHECK(false);
    }
  }
#endif
}
