cmake_minimum_required(VERSION 3.20)

project(Toy_Engien LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(game ${PROJECT_SOURCES})

target_include_directories(game PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party"

    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/x86_64-w64-mingw32/include/SDL2"
)

target_link_directories(game PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86_64-w64-mingw32/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/x86_64-w64-mingw32/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/x86_64-w64-mingw32/lib"
)

target_link_libraries(game PRIVATE
    mingw32
    SDL2main
    SDL2
    SDL2_ttf
    SDL2_image
)