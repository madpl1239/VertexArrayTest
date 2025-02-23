# CMakeLists.txt for vaTest
# 23-07-2023 by LANA Systems

cmake_minimum_required(VERSION 3.31)
project(vaTest LANGUAGES CXX)

if(DEBUG)
	add_definitions(-g -DDEBUG -std=c++23 -Wno-subobject-linkage)
else()
	add_definitions(-Os -march=native -std=c++23 -Wno-subobject-linkage)
endif()

file(GLOB_RECURSE SOURCE ./source/*)
find_package(ASSIMP)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})
target_link_libraries(${CMAKE_PROJECT_NAME} 
	sfml-system sfml-window sfml-graphics assimp GLEW GL)
