cmake_minimum_required(VERSION 3.10)
project(TIME_TINKERERS)

find_package(SDL2 CONFIG REQUIRED)
set(SDL2_IMAGE_DIR "C:\\src\\SDL2_image\\i686-w64-mingw32\\include\\SDL2\\SDL_image.h")
find_path(SDL2_IMAGE_INCLUDE_DIRS SDL_image.h HINTS ${SDL2_IMAGE_DIR}/include)
find_library(SDL2_IMAGE_LIBRARIES SDL2_image HINTS ${SDL2_IMAGE_DIR}/lib)

add_executable(TIME_TINKERERS Time_Tinkerers.cpp Enemy.cpp Game.cpp GreenCircle.cpp Level.cpp Player.cpp)

target_include_directories(TIME_TINKERERS PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(TIME_TINKERERS PRIVATE SDL2::SDL2 SDL2::SDL2main ${SDL2_IMAGE_LIBRARIES})
