cmake_minimum_required(VERSION 3.25)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Release|Debug)" FORCE)
endif()

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
endif()

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(raytracer ${SOURCES})

target_include_directories(raytracer PRIVATE
    include
    src
    src/CmdParser
    src/Factory
    src/Graphical
    src/Light
    src/Load
    src/Material
    src/Object
    src/RayTracer
    src/Render
    src/Scene
    src/Shape
    src/SpacePartitionning
    src/logger
)

target_compile_options(raytracer PRIVATE
    -Wall -Wextra -Wunused -Wpedantic
    -Wno-unused-parameter -Wunused-result
    -Wcast-align -Wcast-qual
    -Wformat=2 -Wstrict-aliasing=0 -Wunreachable-code
    -Wno-unknown-warning-option
    -Werror=format-nonliteral -Werror=return-type -Werror=write-strings
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-O0;-g>
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML    REQUIRED sfml-graphics sfml-window sfml-system)
pkg_check_modules(LIBCONF REQUIRED libconfig++)

target_include_directories(raytracer PRIVATE
    ${SFML_INCLUDE_DIRS}
    ${LIBCONF_INCLUDE_DIRS}
)

target_link_directories(raytracer PRIVATE
    ${SFML_LIBRARY_DIRS}
    ${LIBCONF_LIBRARY_DIRS}
)

target_link_libraries(raytracer PRIVATE
    ${SFML_LIBRARIES}
    ${LIBCONF_LIBRARIES}
    m
)

target_precompile_headers(raytracer PRIVATE
    <vector>
    <memory>
    <optional>
    <string>
    <cmath>
    <functional>
    "include/math3d.hpp"
)
