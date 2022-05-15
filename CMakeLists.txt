cmake_minimum_required(VERSION 3.16)
project(MyAimbotESP)

set(CMAKE_CXX_STANDARD 14)
set(MODULE_NAME MyAimbotESP1)

include_directories(. $ENV{DXSDK_DIR}Include)
link_directories("$ENV{DXSDK_DIR}/Lib/x86")

add_library(${MODULE_NAME} SHARED
        dllmain.cpp
        drawing.cpp
        dx.cpp
        hook.cpp)

target_link_libraries(${MODULE_NAME} d3d9 d3dx9)
