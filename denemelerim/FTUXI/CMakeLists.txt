cmake_minimum_required(VERSION 3.10)
project(FTXUI_Project)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG        main
)
FetchContent_MakeAvailable(ftxui)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ftxui::component ftxui::dom ftxui::screen)