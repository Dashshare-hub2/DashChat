cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

if (NOT DEFINED GEODE_SDK)
    if (DEFINED ENV{GEODE_SDK})
        set(GEODE_SDK "$ENV{GEODE_SDK}")
    else()
        message(FATAL_ERROR "Could not find Geode SDK! Please set GEODE_SDK environment variable.")
    endif()
endif()

list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
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

target_include_directories(dashchat PRIVATE src)
target_link_libraries(dashchat PRIVATE ixwebsocket)

if (MSVC)
    target_compile_options(dashchat PRIVATE /W4 /WX-)
else()
    target_compile_options(dashchat PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()