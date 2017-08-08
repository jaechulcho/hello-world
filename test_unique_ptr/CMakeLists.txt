## cmake 최소 필요 버전
cmake_minimum_required (VERSION 2.8)

## 프로젝트명
project (HELLO)

##
set(CMAKE_BUILD_TYPE Release)

add_definitions(-std=c++11)

add_subdirectory(src)
