cmake_minimum_required(VERSION 3.27)
project(zoltraak)

set(CMAKE_CXX_STANDARD 20)

set(FONT_FILE ${CMAKE_SOURCE_DIR}/src/res/fonts/8bitoperator_jve.ttf)
set(HERO1_AVATAR_FILE ${CMAKE_SOURCE_DIR}/src/res/sprites/hero1_avatar.png)
set(HERO2_AVATAR_FILE ${CMAKE_SOURCE_DIR}/src/res/sprites/hero2_avatar.png)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(
        src/common
        src/systems
)

add_executable(
        zoltraak

        src/common/events/CEventDispatcher.cpp

        src/systems/event/CEventSystem.cpp
        src/systems/movement/CMovementSystem.cpp
        src/systems/view/CViewSystem.cpp
        src/systems/collision/CCollisionSystem.cpp
        src/systems/camera/CCameraSystem.cpp

        src/CGameController.cpp
        src/main.cpp
)

target_link_libraries(zoltraak sfml-graphics sfml-window sfml-system)

add_custom_command(
        TARGET zoltraak POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${FONT_FILE} $<TARGET_FILE_DIR:zoltraak>
)

add_custom_command(
        TARGET zoltraak POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${HERO1_AVATAR_FILE} $<TARGET_FILE_DIR:zoltraak>
)

add_custom_command(
        TARGET zoltraak POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${HERO2_AVATAR_FILE} $<TARGET_FILE_DIR:zoltraak>
)