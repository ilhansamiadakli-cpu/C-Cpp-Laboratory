set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.20)

project(RaylibProjem VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# =========================================================================
# 1. HER İŞLETİM SİSTEMİ İÇİN RAYLIB'İ STATİK GÖM (Evrensel)
# =========================================================================
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib_cpp)

# =========================================================================
# 2. KAYNAK DOSYALAR
# =========================================================================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
        RAYMATH_NO_OPERATORS
)

# =========================================================================
# 3. PLATFORMA ÖZEL BAĞLANTILAR
# =========================================================================
if (WIN32)
    # Windows için C++ DLL'lerini göm ve siyah konsolu kapat
    target_link_options(${PROJECT_NAME} PRIVATE
            -mwindows
            -static
            -static-libgcc
            -static-libstdc++
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
            raylib
            raylib_cpp
            winmm
            gdi32
            opengl32
    )
elseif (APPLE)
    # macOS için sadece Raylib'i statik bağlamak yeterlidir (Apple frameworkleri otomatik bağlanır)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            raylib
            raylib_cpp
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${raylib_cpp_SOURCE_DIR}/include
)