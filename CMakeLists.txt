cmake_minimum_required(VERSION 3.24)
project(WorldImageCreator VERSION 1.0.0)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

add_library(
    viewer_lib
    src/window.h
    src/window.cpp
    src/viewport.h
    src/viewport.cpp
)

add_executable(world-image-creator src/main.cpp)

target_link_libraries(world-image-creator PRIVATE viewer_lib)
target_link_libraries(world-image-creator PRIVATE "Dwmapi.lib" "gdiplus.lib")
