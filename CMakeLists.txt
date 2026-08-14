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

find_program(GEODE_CLI geode PATHS "${CMAKE_SOURCE_DIR}/my-epic-geode-cli")

if(GEODE_CLI)
    message(STATUS "Setting Geode CLI path to: ${TARGET_GEODE_SDK}")
    execute_process(COMMAND "${GEODE_CLI}" sdk set-path "${TARGET_GEODE_SDK}")
    execute_process(COMMAND "${GEODE_CLI}" sdk install-binaries)
endif()

set(GEODE_LINK_NIGHTLY ON CACHE BOOL "" FORCE)

if(APPLE)
    set(GEODE_TARGET_PLATFORM "MacOS")
    set(GEODE_LOADER_NAME "Geode.dylib")
elseif(WIN32)
    set(GEODE_TARGET_PLATFORM "Win64")
    set(GEODE_LOADER_NAME "Geode.dll")
else()
    set(GEODE_TARGET_PLATFORM "Android64")
    set(GEODE_LOADER_NAME "libGeode.so")
endif()

file(MAKE_DIRECTORY "${TARGET_GEODE_SDK}/bin/${GEODE_TARGET_PLATFORM}")
file(TOUCH "${TARGET_GEODE_SDK}/bin/${GEODE_TARGET_PLATFORM}/${GEODE_LOADER_NAME}")
file(TOUCH "${TARGET_GEODE_SDK}/bin/${GEODE_LOADER_NAME}")

if(EXISTS "${TARGET_GEODE_SDK}/CMakeLists.txt")
    add_subdirectory("${TARGET_GEODE_SDK}" "${CMAKE_BINARY_DIR}/geode")
else()
    find_package(Geode REQUIRED)
endif()

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