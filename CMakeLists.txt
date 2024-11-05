cmake_minimum_required(VERSION 3.27)
project(zoltraak)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(
        src/common
        src/systems
)

add_executable(
        zoltraak

        src/events/CEventDispatcher.cpp

        src/systems/event/CEventSystem.cpp
        src/systems/movement/CMovementSystem.cpp

        src/CGameController.cpp
        src/main.cpp
)

target_link_libraries(zoltraak sfml-graphics sfml-window sfml-system)