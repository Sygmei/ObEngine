cmake_minimum_required(VERSION 3.16)
set(OpenGL_GL_PREFERENCE GLVND)

include(cmake/check_vcpkg.cmake)

set(OBE_USE_VCPKG OFF)
obe_check_vcpkg(OBE_USE_VCPKG)

project(ObEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(extlibs/bezier)
if(OBE_USE_VCPKG)
    find_package(fmt CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(lua CONFIG REQUIRED)
    find_package(sol2 CONFIG REQUIRED)
    find_package(Catch2 CONFIG REQUIRED)
else()
    add_subdirectory(extlibs/catch)

    # fmtlib is required by spdlog
    add_subdirectory(extlibs/fmt)
    add_subdirectory(extlibs/spdlog)

    # lua is required by sol
    add_subdirectory(extlibs/lua)
    add_subdirectory(extlibs/sol3)
    
    if (APPLE OR USE_FILESYSTEM_FALLBACK)
        add_subdirectory(extlibs/tinydir)
    endif()
endif()
add_subdirectory(extlibs/dynamicLinker)
add_subdirectory(extlibs/fswrapper)
add_subdirectory(extlibs/vili)
add_subdirectory(extlibs/soloud)
add_subdirectory(extlibs/lunasvg)

if (APPLE OR USE_FILESYSTEM_FALLBACK)
    set(USE_FILESYSTEM_FALLBACK TRUE CACHE BOOL "Use filesystem fallback" FORCE)
endif()

# minizip is required by elzip
add_subdirectory(extlibs/minizip)
add_subdirectory(extlibs/elzip)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# SFML is required by sfe (SFml Extensions)
add_subdirectory(extlibs/sfe)

# Find git installation
find_package(Git QUIET)
if(NOT Git_FOUND)
    message(WARNING "Could not find git, trying to use git from PATH")
    set(GIT_EXECUTABLE "git")
endif()

add_subdirectory(src/Core)

if (BUILD_ANDROID)
    set(BUILD_PLAYER OFF)
    set(BUILD_DEV OFF)
    add_subdirectory(src/Android)
endif()

if (NOT DEFINED BUILD_PLAYER)
    set(BUILD_PLAYER ON CACHE BOOL "Build ObEngine Player ?")
endif()

if (WIN32)
    file(GLOB OBENGINE_ICON ${CMAKE_SOURCE_DIR}/resources/resource.rc)
else()
    set(OBENGINE_ICON "")
endif()

if (BUILD_PLAYER)
    add_subdirectory(src/Player)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ObEnginePlayer)
endif()

if (NOT DEFINED BUILD_DEV)
    set(BUILD_DEV ON CACHE BOOL "Build ObEngine Dev ?")
endif()

if (NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS OFF CACHE BOOL "Build ObEngine Tests ?")
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (NOT DEFINED RUN_CI_TOOLS)
    set(RUN_CI_TOOLS OFF CACHE BOOL "Run CI tools ?")
endif()

set(CMAKE_CXX_CLANG_TIDY "")
set(CMAKE_CXX_CPPCHECK "")
set(MAKE_CXX_INCLUDE_WHAT_YOU_USE "")
if (RUN_CI_TOOLS)
    find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
    find_program(CPPCHECK_COMMAND NAMES cppcheck)
    find_program(IWYU_COMMAND NAMES iwyu-tool)

    if (CLANG_TIDY_COMMAND)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-checks=*,-fuchsia-default-arguments")
    endif()
    if (CPPCHECK_COMMAND)
        set(CMAKE_CXX_CPPCHECK "${CPPCHECK_COMMAND};--std=c++11")
    endif()
    if (IWYU_COMMAND)
        set(MAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_COMMAND};--transitive_includes_only")
    endif()
    set(CMAKE_LINK_WHAT_YOU_USE TRUE)

    get_property(OBENGINE_CORE_SRC TARGET ObEngineCore PROPERTY SOURCES)
    get_property(OBENGINE_PLAYER_SRC TARGET ObEnginePlayer PROPERTY SOURCES)
    get_property(OBENGINE_DEV_SRC TARGET ObEngineDev PROPERTY SOURCES)

    include(clang_format)

    clangformat_setup(
        ${OBENGINE_CORE_SRC}
        ${OBENGINE_PLAYER_SRC}
        ${OBENGINE_DEV_SRC}
    )
endif()
