cmake_minimum_required(VERSION 3.5) 

project(nsga2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)

file(GLOB_RECURSE sources src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${sources})

add_executable(test_nsga2 test/test_nsga2.cpp)
target_link_libraries(test_nsga2 ${PROJECT_NAME})

