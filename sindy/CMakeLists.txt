cmake_minimum_required(VERSION 3.20)
project(sindy)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

file(GLOB SINDY_SRC
       ${PROJECT_SOURCE_DIR}/*.c
       ${PROJECT_SOURCE_DIR}/*.cc)

add_library(${PROJECT_NAME} STATIC ${SINDY_SRC})

# 预编译头
include_directories(.)
target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:./pch.h>")

message(">>> SINDY_SRC=${SINDY_SRC}")
