# doctest configuration from exe_with_static_libs example

find_package(doctest REQUIRED)
if (doctest_FOUND)
	message("Using doctest from find_package")
else()
	message("doctest not found")
endif()


set(TEST_MAIN "${PROJECT_NAME}-test")

include_directories(include)

add_executable(${TEST_MAIN}
  src/sdl_ttf_test.cpp
  src/font_test.cpp
)

# object libraries cannot "link" to any target so this is how we get
# the INTERFACE include directories of the doctest target
get_property(doctest_include_dir TARGET doctest::doctest PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(${TEST_MAIN} PRIVATE ${doctest_include_dir})

# group them together in a single folder inside IDEs
# I think something like this fixes the ALL_BUILD issue in Visual Studio
set_target_properties(${TEST_MAIN} PROPERTIES FOLDER ${TEST_MAIN})

target_link_libraries(${TEST_MAIN} PUBLIC ${PROJECT_NAME})

target_link_libraries(${TEST_MAIN} PUBLIC doctest::doctest)

# target_link_libraries(${TEST_MAIN} PRIVATE "/home/josh/src/graphics/sdl/sdl-opengl-cpp/build/libsdl-opengl-cpp.a")

if (${sdl-opengl-cpp_FOUND})
  message("Found sdl-opengl-cpp using find_package")
  target_link_libraries(${TEST_MAIN} PUBLIC "sdl-opengl-cpp::sdl-opengl-cpp")
else()
  add_dependencies(${TEST_MAIN} "sdl-opengl-cpp-install")
  add_dependencies(glfont "sdl-opengl-cpp-install")

  ExternalProject_Get_Property("sdl-opengl-cpp" INSTALL_DIR)

  message("INSTALL_DIR: ${INSTALL_DIR}")
  include_directories(${INSTALL_DIR}/include/sdl-opengl-cpp)
  target_link_libraries(${TEST_MAIN} PUBLIC ${INSTALL_DIR}/lib/libsdl-opengl-cpp.a)
endif()
find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 CONFIG REQUIRED SDL2main)

if (SDL2_FOUND)
	message("Using SDL2 from find_package")
	target_include_directories(${TEST_MAIN} PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(${TEST_MAIN} PUBLIC ${SDL2_LIBRARIES})
	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
	unset(CHECK_SDL_VERSION CACHE)

else()
	# Fall back on pkg-config
	include(FindPkgConfig)
	pkg_search_modules(SDL2 REQUIRED sdl2)
	message("Using SDL2 from pkg-config")
	target_include_directories(${TEST_MAIN} PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(${TEST_MAIN} PUBLIC ${SDL2_LIBRARIES})
	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
	unset(CHECK_SDL_VERSION CACHE)
endif()

# if (SDL2_FOUND)
#   target_link_libraries(${TEST_MAIN} PUBLIC ${SDL2_LIBRARIES})
# endif()

find_package(SDL2_ttf REQUIRED)
target_link_libraries(${TEST_MAIN} PUBLIC SDL2_ttf::SDL2_ttf)


# include CMake common file for doctest_add_test
# This also enables a bunch of additional checks and turns warnings into errors
# It reveals a bug somewhere in my CMakeLists.txt where I'm including
# "${PROJECT_NAME}/src" and it's using it as a subdirectory inside the project.
# include(${PROJECT_SOURCE_DIR}/scripts/cmake/common.cmake)
# doctest_add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}> --no-version)

# doctest_add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} --no-version)


# We use Google Mock aka gMock for mocking the OpenSDL functions

enable_testing()

find_package(GTest CONFIG)

if (GTest_FOUND)
  message(STATUS "Using GoogleTest from find_package")
  target_compile_options(${TEST_MAIN} PRIVATE ${GTest_CFLAGS})
  target_link_libraries(${TEST_MAIN} PRIVATE GTest::gmock GTest::gmock_main)
else()
  # Ubuntu doesn't compile the GoogleTest package, it's just a source
  # package.  This is actually recommended by Google, otherwise there
  # may be incompatible differences in compile flags.  So we have some
  # extra logic here to build the source.

  # TODO: Other distributions may put the source elsewhere
  set(GTEST_DIR "/usr/src/googletest")

  if(EXISTS ${GTEST_DIR} AND IS_DIRECTORY ${GTEST_DIR})
    message(STATUS "GoogleTest directory exists in ${GTEST_DIR}")
    set(GTEST_SRC_FOUND True)
  endif()

  if(GTEST_SRC_FOUND)
    # The following two commented out subdirectories are built by
    # googlemock, so they aren't needed.
    #
    # set(GTEST_ROOT "${GTEST_DIR}/googletest")
    # add_subdirectory(${GTEST_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/googletest" EXCLUDE_FROM_ALL)
    #
    # We just use gMock, and gMock depends on gTest.  It automatically
    # creates a googletest build directory and builds in it.
    set(GMOCK_ROOT "${GTEST_DIR}/googlemock")
    add_subdirectory(${GMOCK_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/googlemock" EXCLUDE_FROM_ALL)
    target_link_libraries(${TEST_MAIN} PRIVATE GTest::gmock GTest::gmock_main)
  endif()

  # TODO: One final attempt could be made by pulling in with CMake's
  # FetchContent
  # The advantage to that is we can pin to a specific version of
  # GoogleTest.  We can also build on a ton of more distributions.
  # I'm trying to balance redundant trusted sources with stable builds
  # and it's exhausting.
  # On the plus side it's reusable work, I hope.
endif()

add_test(NAME ${TEST_MAIN} COMMAND $<TARGET_FILE:${TEST_MAIN}> --no-version)

if(FONT_DIR)
  message("FONT_DIR=${FONT_DIR} passed in to CMake, overriding font directory")
  target_compile_definitions(${TEST_MAIN} PRIVATE "FONT_DIR=\"${FONT_DIR}\"")
endif()

include(${PROJECT_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${TEST_MAIN})
