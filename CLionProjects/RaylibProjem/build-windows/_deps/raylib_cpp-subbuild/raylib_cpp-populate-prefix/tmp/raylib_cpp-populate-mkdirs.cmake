# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-src")
  file(MAKE_DIRECTORY "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-src")
endif()
file(MAKE_DIRECTORY
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-build"
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix"
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/tmp"
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/src/raylib_cpp-populate-stamp"
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/src"
  "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/src/raylib_cpp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/src/raylib_cpp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/mac/C-Cpp-Laboratory/CLionProjects/RaylibProjem/build-windows/_deps/raylib_cpp-subbuild/raylib_cpp-populate-prefix/src/raylib_cpp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
