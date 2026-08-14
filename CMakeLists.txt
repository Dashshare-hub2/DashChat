cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(dashchat LANGUAGES CXX)

if(DEFINED ENV{GEODE_SDK})
    set(TARGET_GEODE_SDK "$ENV{GEODE_SDK}")
else()
    set(TARGET_GEODE_SDK "${CMAKE_SOURCE_DIR}/geode-sdk-clone")
endif()

set(GEODE_LINK_NIGHTLY ON CACHE BOOL "" FORCE)
set(GEODE_DONT_LINK_LOADER ON CACHE BOOL "" FORCE)

find_program(GEODE_CLI geode PATHS "${CMAKE_SOURCE_DIR}/my-epic-geode-cli")
if(GEODE_CLI)
    message(STATUS "Running Geode CLI install-binaries...")
    execute_process(COMMAND "${GEODE_CLI}" sdk set-path "${TARGET_GEODE_SDK}")
    execute_process(COMMAND "${GEODE_CLI}" sdk install-binaries)
endif()


if(APPLE)
    set(GEODE_BIN_NAME "Geode.dylib")
    set(GEODE_PLATFORM_DIR "MacOS")
elseif(WIN32)
    set(GEODE_BIN_NAME "Geode.dll")
    set(GEODE_PLATFORM_DIR "Win64")
else()
    set(GEODE_BIN_NAME "libGeode.so")
    set(GEODE_PLATFORM_DIR "Android64")
endif()

file(MAKE_DIRECTORY "${TARGET_GEODE_SDK}/bin/${GEODE_PLATFORM_DIR}")
file(MAKE_DIRECTORY "${TARGET_GEODE_SDK}/bin/v2.2081")
if(NOT EXISTS "${TARGET_GEODE_SDK}/bin/${GEODE_PLATFORM_DIR}/${GEODE_BIN_NAME}")
    file(WRITE "${TARGET_GEODE_SDK}/bin/${GEODE_PLATFORM_DIR}/${GEODE_BIN_NAME}" "")
endif()

if(EXISTS "${TARGET_GEODE_SDK}/CMakeLists.txt")
    add_subdirectory("${TARGET_GEODE_SDK}" "${CMAKE_BINARY_DIR}/geode")
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
        src/ChatUI.cpp
)

target_link_libraries(dashchat PRIVATE sio_client asio)