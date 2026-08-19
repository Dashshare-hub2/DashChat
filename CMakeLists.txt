cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DashChat VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(USE_TLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.4
)
FetchContent_MakeAvailable(ixwebsocket)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
endif()

find_package(Geode REQUIRED)


set(MOD_ID "daianhnguyen.dashchat")

setup_geode_mod(${MOD_ID}
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
)

target_link_libraries(${MOD_ID} PRIVATE ixwebsocket)