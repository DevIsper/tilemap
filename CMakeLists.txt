cmake_minimum_required(VERSION 3.28)
project(tilemap)

set(CMAKE_CXX_STANDARD 17)

add_executable(tilemap main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

INCLUDE_DIRECTORIES(  /usr/local/Cellar/allegro/5.2.10.1/include )
LINK_DIRECTORIES(  /usr/local/Cellar/allegro/5.2.10.1/lib )

file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.10.1/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(tilemap ${LIBRARIES})