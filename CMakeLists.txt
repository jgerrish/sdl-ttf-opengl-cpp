cmake_minimum_required(VERSION 3.24)

# This is a C++ library of simple RAII classes to manage SDL TTF fonts
# on OpenGL
project(sdl-ttf-opengl-cpp VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Follow the current Google C++ Style Guides and target C++ 20
# They are aggressive in their adoption of C++ versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# This is configurable so we can test both code paths in CI and let
# users easily build what version they want.
# To disable exceptions, specify -DNO_EXCEPTIONS=true on the CMake
# command line.
if(NO_EXCEPTIONS)
  message("NO_EXCEPTIONS=${NO_EXCEPTIONS} passed in to CMake, disabling exceptions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_EXCEPTIONS=1 -fno-exceptions")
else()
  message("NO_EXCEPTIONS=${NO_EXCEPTIONS} passed in to CMake, enabling exceptions")
endif()

include_directories(include)

add_library(
  ${PROJECT_NAME}
  src/sdl_ttf_wrapper.cpp
  src/sdl_ttf_base.cpp
  src/font.cpp
)

add_subdirectory(src)


# This works
# TODO: Maybe use this instead of the other loading method in the other projects?
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 CONFIG REQUIRED SDL2main)

if (SDL2_FOUND)
	message("Using SDL2 from find_package")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
	unset(CHECK_SDL_VERSION CACHE)

else()
	# Fall back on pkg-config
	include(FindPkgConfig)
	pkg_search_modules(SDL2 REQUIRED sdl2)
	message("Using SDL2 from pkg-config")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
	unset(CHECK_SDL_VERSION CACHE)
endif()

find_package(SDL2_ttf CONFIG REQUIRED COMPONENTS SDL2_ttf)
if(SDL2_ttf_FOUND)
  message("Found SDL2_ttf")
  target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_ttf::SDL2_ttf)
  # target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
else()
  message("Didn't find SDL2_ttf")
endif()

# Setup logging with spdlog

find_package(spdlog CONFIG REQUIRED)

if(spdlog_FOUND)
  message("Found spdlog")
  target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()


# find_package(doctest REQUIRED)
# if (doctest_FOUND)
# 	message("Using doctest from find_package")
# else()
# 	message("doctest not found")
# endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)



# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)

# find_program(MAKE_EXECUTABLE NAMES nmake gmake make)

set(installDir ${CMAKE_CURRENT_BINARY_DIR}/install)

set(SDL_OPENGL_CPP_INCLUDE_DIR ${installDir}/include/sdl-opengl-cpp)
set(SDL_OPENGL_CPP_LIB ${installDir}/lib/libsdl-opengl-cpp.a)


message("CMAKE_FIND_PACKAGE_REDIRECTS_DIR: ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}")

add_executable(
  glfont
  examples/glfont.cpp
)

target_link_libraries(glfont PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libsdl-ttf-opengl-cpp.a)
target_link_libraries(glfont PRIVATE ${PROJECT_NAME})
target_link_libraries(glfont PUBLIC SDL2::SDL2)
target_link_libraries(glfont PUBLIC SDL2_ttf::SDL2_ttf)
target_link_libraries(glfont PUBLIC fmt::fmt)


find_package("sdl-opengl-cpp" CONFIG 0.3.2)
if (${sdl-opengl-cpp_FOUND})
  message("Found sdl-opengl-cpp using find_package")
  target_link_libraries(${PROJECT_NAME} PUBLIC "sdl-opengl-cpp::sdl-opengl-cpp")
else()
  message("Did not find sdl-opengl-cpp using find_package")
  include(ExternalProject)
  ExternalProject_Add(
    "sdl-opengl-cpp"
    DEPENDS OpenGL::GL SDL2::SDL2 SDL2_ttf::SDL2_ttf spdlog::spdlog

    # This points to a commit in the add-sdl-ttf-support-features
    # branch of sdl-opengl-cpp.  It should have all the features for
    # the first version of this library.
    GIT_REPOSITORY https://github.com/jgerrish/sdl-opengl-cpp.git
    GIT_TAG 17e07d1d837963355b963d76ad2ae5f34630448f

    # For local installs:
    # SOURCE_DIR ../sdl-opengl-cpp

    # We don't want to build the sdl-opengl-cpp tests.
    # INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/sdl-opengl-cpp"
    INSTALL_DIR ${installDir}

    # Disable installs
    # INSTALL_COMMAND ""
    # INSTALL_COMMAND ${MAKE_EXECUTABLE} DESTDIR=<INSTALL_DIR> install

    CMAKE_ARGS -DBUILD_TESTING:BOOL=NO
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
    -DNO_EXCEPTIONS:BOOL=${NO_EXCEPTIONS}

    BUILD_BYPRODUCTS ${SDL_OPENGL_CPP_INCLUDE_DIR} ${SDL_OPENGL_CPP_LIB}

    STEP_TARGETS build install
  )
  add_dependencies(${PROJECT_NAME} "sdl-opengl-cpp-install")
  add_dependencies(glfont "sdl-opengl-cpp-install")

  ExternalProject_Get_Property("sdl-opengl-cpp" INSTALL_DIR)

  message("INSTALL_DIR: ${INSTALL_DIR}")
  include_directories(${INSTALL_DIR}/include/sdl-opengl-cpp)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${INSTALL_DIR}/lib/libsdl-opengl-cpp.a)
  target_link_libraries(glfont PUBLIC ${INSTALL_DIR}/lib/libsdl-opengl-cpp.a)
endif()


target_link_libraries(glfont PUBLIC spdlog::spdlog)


if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()


set(CMAKE_INSTALL_INCLUDEDIR "include")

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(PUBLIC_HEADERS
  "include/SDL_glfuncs.h"
  "include/sdl_ttf_wrapper.h"
  "include/sdl_ttf_base.h"
  "include/sdl_ttf_font.h"
  "include/sdl_ttf_error.h"
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT "sdl-ttf-opengl-cpp-targets"
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Try another install option from
# guide/importing-exporting/index.html
export(TARGETS ${PROJECT_NAME}
       NAMESPACE sdl-ttf-opengl-cpp::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake"
)
install(EXPORT "${PROJECT_NAME}-targets"
        FILE "${PROJECT_NAME}-targets.cmake"
        NAMESPACE sdl-ttf-opengl-cpp::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Make them usable from the build tree too
# TODO: Add integration tests for this
export(EXPORT "${PROJECT_NAME}-targets"
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-targets.cmake"
       NAMESPACE sdl-ttf-opengl-cpp::
)

# This shouldn't be required, adding a library alias that is
# namespaced.  But it's done in this example:
# guide/importing-exporting/index.html
# And here: manual/cmake-buildsystem.7.html#id41
# And it appears to fix the import issue in the integration tests
add_library(
  "${PROJECT_NAME}::${PROJECT_NAME}"
  ALIAS ${PROJECT_NAME}
)

message("CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/sdl-ttf-opengl-cpp
    CACHE PATH "Location of header files" )

include(CMakePackageConfigHelpers)
configure_package_config_file(scripts/sdl-ttf-opengl-cpp-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/sdl-ttf-opengl-cpp-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/cmake/sdl-ttf-opengl-cpp
  PATH_VARS INCLUDE_INSTALL_DIR) # SYSCONFIG_INSTALL_DIR)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/sdl-ttf-opengl-cpp-config-version.cmake
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/sdl-ttf-opengl-cpp-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/cmake/sdl-ttf-opengl-cpp-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sdl-ttf-opengl-cpp )

