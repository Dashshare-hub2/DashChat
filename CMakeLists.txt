cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

find_package(Geode REQUIRED)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(USE_LIBUNITTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    socketio
    GIT_REPOSITORY https://github.com/socketio/socket.io-client-cpp.git
    GIT_TAG v3.1.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(socketio)

add_geode_mod(dashchat
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
        src/ChatUI.cpp
)

target_link_libraries(dashchat PRIVATE sio_client)