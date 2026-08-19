cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DashChat VERSION 1.0.0 LANGUAGES CXX)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

if(EXISTS "$ENV{GEODE_SDK}/CMakeLists.txt")
    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
else()
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")
    find_package(Geode REQUIRED)
endif()

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(USE_TLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.4
)
FetchContent_MakeAvailable(ixwebsocket)

set(GEODE_MOD_ID "daianhnguyen.dashchat")

setup_geode_mod(${GEODE_MOD_ID}
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
)

if(COMMAND geode_target_link_libraries)
    geode_target_link_libraries(${GEODE_MOD_ID} PRIVATE ixwebsocket)
else()
    target_link_libraries(${GEODE_MOD_TARGET} PRIVATE ixwebsocket)
endif()