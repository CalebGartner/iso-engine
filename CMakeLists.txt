cmake_minimum_required(VERSION 3.12)
project(iso_engine)

set(CMAKE_CXX_STANDARD 11)

add_executable(iso_engine main.cpp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
# set(SDL2_TTF_PATH "/path/to/sdl2_ttf" CACHE BOOL "" FORCE)

# Find SDL2, SDL2_image and SDL2_gfx libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::GFX)

