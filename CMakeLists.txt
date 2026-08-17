cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

if(DEFINED ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
elseif(DEFINED GEODE_SDK)
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
endif()

if(NOT GEODE_SDK)
    if(EXISTS "$ENV{HOME}/.local/share/geode/sdk")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local/share/geode/sdk")
    elseif(EXISTS "$ENV{HOME}/Library/Application Support/geode/sdk")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/Library/Application Support/geode/sdk")
    endif()
endif()

find_package(Geode REQUIRED)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(USE_TLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/ixwebsocket/ixwebsocket.git
    GIT_TAG v11.4.4
)
FetchContent_MakeAvailable(ixwebsocket)

setup_geode_mod(dashchat
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
)

target_link_libraries(dashchat PRIVATE ixwebsocket)