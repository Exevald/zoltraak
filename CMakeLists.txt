cmake_minimum_required(VERSION 3.27)
project(zoltraak)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(
        zoltraak
        src/CGameController.cpp
        src/main.cpp
)

target_link_libraries(zoltraak sfml-graphics sfml-window sfml-system)