cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DashChat VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(USE_TLS ON CACHE BOOL "" FORCE)

if(APPLE)
    set(USE_APPLE_SSL ON CACHE BOOL "" FORCE)
endif()

if(ANDROID)
    set(ENABLE_MBEDTLS_TESTS OFF CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.4
)
FetchContent_MakeAvailable(ixwebsocket)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/geode-sdk-clone/CMakeLists.txt")
    add_subdirectory(geode-sdk-clone ${CMAKE_CURRENT_BINARY_DIR}/geode)
elseif(DEFINED ENV{GEODE_SDK} AND EXISTS "$ENV{GEODE_SDK}/CMakeLists.txt")
    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
else()
    message(FATAL_ERROR "Unable to find Geode SDK source directory!")
endif()

set(MOD_ID "daianhnguyen.dashchat")

add_library(${MOD_ID} SHARED
    src/main.cpp
    src/Network/WebSocketManager.cpp
    src/UI/ChatOverlay.cpp
    src/UI/ChatCell.cpp
    src/hooks/PlayLayer.cpp
    src/hooks/PauseLayer.cpp
)

setup_geode_mod(${MOD_ID})
if(APPLE)
    target_link_libraries(${MOD_ID} "-Wl,-undefined,dynamic_lookup")
    set_target_properties(${MOD_ID} PROPERTIES
        ENABLE_EXPORTS TRUE
        XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING "NO"
    )
endif()
target_link_libraries(${MOD_ID} ixwebsocket)