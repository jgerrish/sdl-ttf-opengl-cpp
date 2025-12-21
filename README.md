# sdl-ttf-opengl-wrapper: A C++ wrapper for the SDL2 TTF library

# Introduction #

This is a C++ wrapper for working with the SDL2 TTF library (SDL_ttf).

It provides a Font class that can be used to work with fonts.

# Installation #

## Prerequisites ##

The following build tools are required:
CMake
ninja

The following libraries are required:
SDL2
SDL2_ttf
spdlog
gMock (sometimes included with your distro's gTest package)
doctest
Doxygen
pkg-config

On Ubuntu, they can be installed with the following:

apt install cmake ninja-build doctest-dev doxygen google-mock googletest libsdl2-dev libsdl2-ttf-dev libspdlog-dev pkg-config

doctest can be downloaded from GitHub:
[doctest](https://github.com/doctest/doctest.git)


You also need the basic C++ toolchain.  It's the build-essential
package on Debian systems, Fedora and SUSE both have meta-packages or
groups that you can install to get C++ development.

The examples below use the Ninja build system:

apt install ninja-build

## Compiling ##

$ mkdir build

For debug builds:

$ cmake -DCMAKE\_BUILD\_TYPE=Debug -DBUILD\_TESTING=Yes ..

For release builds:

$ cmake -DCMAKE\_BUILD\_TYPE=Debug -DBUILD\_TESTING=Yes  ..

To build it:

$ cd build
$ make


You can test the library with the following command:

$ test/sdl-ttf-opengl-cpp-test

### Windows ###

To compile for Windows systems.

This project has been tested in Visual Studio 2022 with the Desktop
development with C++ Workload installed.

You'll also need a copy of CMake installed and vcpkg.


Install prerequisites with vcpkg:

For 64-bit machines:

vcpkg.exe install doctest gtest sdl2 sdl2_ttf spdlog --triplet x64-windows


Generate CMake configuration

Open cmake-gui up.

Select the sdl-ttf-opengl directory as the source directory in the
"Where's the source code" edit box.

Select the sdl-ttf-opengl directory as the build directory in the
"Where to build the binaries" edit box.  Then append build to the
path.  So the path should be something like:

BLAHBLAHBLAH/sdl-ttf-opengl-cpp/build

Click Configure.

Choose the option Specify toolchain file for cross-compiling and click
Next.

Select vcpkg.cmake in the scripts/buildsystems directory of your vcpkg
install.

Click Finish.

Click Generate after it is done configuring.

Click Open Project after it is done generating.


In Visual Studio

Right click on the sdl-ttf-opengl-cpp-test project and click build.
Execute it from a command prompt to test it.


# Testing #

To run the tests, just execute the following after compiling:

$ test/sdl-ttf-opengl-cpp-test


The project must be configured with the CMake BUILD\_TESTING flag set
to true.
