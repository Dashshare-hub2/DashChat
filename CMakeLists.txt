cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)
ư
set(GEODE_DONT_LINK_LOADER ON CACHE BOOL "Disable linking loader binary" FORCE)
set(GEODE_LINK_NIGHTLY ON CACHE BOOL "Link nightly binaries" FORCE)

if(DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}" CACHE PATH "Path to Geode SDK")
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
elseif(DEFINED GEODE_SDK)
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
endif()

find_package(Geode REQUIRED)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(USE_TLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/ixwebsocket.git
    GIT_TAG v11.4.4
)
FetchContent_MakeAvailable(ixwebsocket)

setup_geode_mod(dashchat
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
)

target_link_libraries(dashchat PRIVATE ixwebsocket)

if(MSVC)
    target_compile_options(dashchat PRIVATE /W4 /WX-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(dashchat PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()