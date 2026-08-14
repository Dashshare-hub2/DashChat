cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

set(GEODE_DONT_LINK_LOADER ON CACHE BOOL "Disable linking loader binary" FORCE)
set(GEODE_LINK_NIGHTLY ON CACHE BOOL "Link nightly binaries" FORCE)

if(DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}" CACHE PATH "Path to Geode SDK" FORCE)
    add_subdirectory("${GEODE_SDK}" "${CMAKE_BINARY_DIR}/geode")
else()
    find_package(Geode REQUIRED)
endif()

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

set(ASIOTPL_USE_BOOST OFF CACHE BOOL "" FORCE)
set(SIO_USE_OPENSSL OFF CACHE BOOL "" FORCE)
add_definitions(-DASIO_STANDALONE)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG ASIO_1_28_0
)
FetchContent_MakeAvailable(asio)

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
)

target_link_libraries(dashchat PRIVATE sio_client asio)