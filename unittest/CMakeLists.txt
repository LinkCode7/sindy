cmake_minimum_required(VERSION 3.20)
project(unittest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

file(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/*.cc
				   ${PROJECT_SOURCE_DIR}/SindySqlite/*.cc
				   ${PROJECT_SOURCE_DIR}/SindySqlite/effective/*.cc
				   ${PROJECT_SOURCE_DIR}/SindySqlite/sample/*.cc
				   ${PROJECT_SOURCE_DIR}/SindySqlite/sql/*.cc)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} sindy)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

target_include_directories(${PROJECT_NAME} PRIVATE ../thirdparty/googletest/googletest/include)
