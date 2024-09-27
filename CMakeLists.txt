cmake_minimum_required(VERSION 3.27)
project(zoltraak)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        zoltraak
        src/CGameController.cpp
        src/main.cpp
)