cmake_minimum_required(VERSION 3.16)
project(crack-world)

set(CMAKE_CXX_STANDARD 14)
set(MODULE_NAME cw)

include_directories(. $ENV{DXSDK_DIR}Include)
link_directories("$ENV{DXSDK_DIR}/Lib/x86")

aux_source_directory(. SRC_DIR)

add_library(${MODULE_NAME} SHARED ${SRC_DIR})

target_link_libraries(${MODULE_NAME} d3d9 d3dx9)
