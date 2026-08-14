cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

if(DEFINED ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
    include("$ENV{GEODE_SDK}/share/Geode/cmake/GeodeFile.cmake" OPTIONAL)
endif()

if(NOT TARGET Geode)
    include(FetchContent)
    FetchContent_Declare(
        GeodeSDK
        GIT_REPOSITORY https://github.com/geode-sdk/geode.git
        GIT_TAG v3.8.0 
    )
    FetchContent_MakeAvailable(GeodeSDK)
endif()

find_package(Geode REQUIRED)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    socketio
    GIT_REPOSITORY https://github.com/socketio/socket.io-client-cpp.git
    GIT_TAG 3.1.0
)
FetchContent_MakeAvailable(socketio)

setup_geode_mod(dashchat
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
        src/ChatUI.cpp
)

target_link_libraries(dashchat PRIVATE sio_client)