cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

set(GEODE_DONT_LINK_LOADER ON CACHE BOOL "Disable linking loader binary" FORCE)
set(GEODE_LINK_NIGHTLY ON CACHE BOOL "Link nightly binaries" FORCE)

if(DEFINED ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
elseif(DEFINED GEODE_SDK)
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
endif()

find_package(Geode REQUIRED)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

set(ASIOTPL_USE_BOOST OFF CACHE BOOL "" FORCE)
set(SIO_USE_OPENSSL OFF CACHE BOOL "" FORCE)
add_definitions(-DASIO_STANDALONE -D_WIN32_WINNT=0x0601 -D_WEBSOCKETPP_CPP11_STL_)

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

target_link_libraries(dashchat PRIVATE SYSTEM sio_client asio)