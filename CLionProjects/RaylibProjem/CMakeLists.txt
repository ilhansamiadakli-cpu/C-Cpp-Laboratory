set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.20)

# Proje adını buraya yazabilirsin (CLion hedefi bu isimle tanır)
project(RaylibProjem VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Sistemdeki (Homebrew ile kurulan) hazır Raylib'i bul
find_package(raylib REQUIRED)

# 2. raylib-cpp başlık dosyalarını anında çek (Sürüm 5.5.0 - Brew ile uyumlu)
include(FetchContent)
FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG master # v5.5.0 yerine doğrudan güncel kodu alıyoruz
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib_cpp)

# 3. src klasöründeki tüm dosyaları otomatik tara
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# 4. Yürütülebilir dosyayı oluştur
add_executable(${PROJECT_NAME} ${SOURCES})

# raymath C operatör çakışmalarını engelle
target_compile_definitions(${PROJECT_NAME} PRIVATE
        RAYMATH_NO_OPERATORS
)

# 5. Kütüphaneleri projeye bağla (macOS frameworkleri Brew tarafından otomatik halledilir)
target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        raylib_cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${raylib_cpp_SOURCE_DIR}/include
)