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

find_package(Geode REQUIRED)

setup_geode_mod(${PROJECT_NAME}
    SOURCES
        src/main.cpp
        src/ChatManager.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ixwebsocket)