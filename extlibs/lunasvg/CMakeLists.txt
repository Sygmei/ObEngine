cmake_minimum_required(VERSION 3.3)

project(lunasvg VERSION 1.4.1 LANGUAGES CXX C)

add_library(lunasvg STATIC)

add_subdirectory(include)
add_subdirectory(source)
add_subdirectory(3rdparty/stb)
add_subdirectory(3rdparty/software)
add_subdirectory(3rdparty/plutovg)

