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

# 1. ZEKİ FİLTRELEME: Tüm .cpp dosyalarını ve Ana dosyaları bul
file(GLOB ALL_SRCS "*.cpp")
file(GLOB MAIN_SRCS "*_main.cpp")

# 2. Yardımcıları Ayır (HELPER_SRCS = ALL_SRCS - MAIN_SRCS)
set(HELPER_SRCS ${ALL_SRCS})
if(MAIN_SRCS)
    list(REMOVE_ITEM HELPER_SRCS ${MAIN_SRCS})
endif()

# 3. OTOMATİK ÜRETİM: Her bir _main.cpp için ayrı program oluştur
foreach(MAIN_FILE ${MAIN_SRCS})
    # Dosya adını al (örn: pencere_main.cpp -> pencere_main)
    get_filename_component(BASE_NAME ${MAIN_FILE} NAME_WE)
    # '_main' kısmını kırp (pencere_main -> pencere)
    string(REPLACE "_main" "" TARGET_NAME ${BASE_NAME})

    # Programı oluştur (Ana dosya + Yardımcılar)
    add_executable(${TARGET_NAME} ${MAIN_FILE} ${HELPER_SRCS})
    
    # KRİTİK DÜZELTME: Raylib yerine FTXUI kütüphanelerini bağlıyoruz!
    target_link_libraries(${TARGET_NAME} PRIVATE ftxui::component ftxui::dom ftxui::screen) 
endforeach()