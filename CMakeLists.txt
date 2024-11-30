cmake_minimum_required(VERSION 3.27)
project(zoltraak)

set(CMAKE_CXX_STANDARD 20)

set(FONTS
        ${CMAKE_SOURCE_DIR}/src/res/fonts/8bitoperator_jve.ttf
)

set(IMAGES
        ${CMAKE_SOURCE_DIR}/src/res/sprites/hero1_avatar.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/hero1_sprites.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/hero2_avatar.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/hero2_sprites.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/map_fieldOfHopesAndDreams.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/map_commonItems.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/menu_background.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/menu_soul.png
        ${CMAKE_SOURCE_DIR}/src/res/sprites/utils_windows.png
)

set(MAPS
        ${CMAKE_SOURCE_DIR}/src/res/maps/level1.txt
)

set(SAVED
        ${CMAKE_SOURCE_DIR}/src/res/saved/save1.txt
        ${CMAKE_SOURCE_DIR}/src/res/saved/save2.txt
        ${CMAKE_SOURCE_DIR}/src/res/saved/save3.txt
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(
        src/common
        src/systems
)

add_executable(
        zoltraak

        src/common/storage/CTextureStorage.cpp
        src/common/storage/CFontStorage.cpp
        src/common/level_generator/CLevelGenerator.cpp

        src/systems/event/CEventSystem.cpp
        src/systems/movement/CMovementSystem.cpp
        src/systems/view/CViewSystem.cpp
        src/systems/collision/CCollisionSystem.cpp
        src/systems/camera/CCameraSystem.cpp
        src/systems/animation/CAnimationSystem.cpp

        src/CGameController.cpp
        src/main.cpp
)

target_link_libraries(zoltraak sfml-graphics sfml-window sfml-system)

foreach (font ${FONTS})
    add_custom_command(
            TARGET zoltraak POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${font} ${CMAKE_BINARY_DIR}/
    )
endforeach ()

foreach (image ${IMAGES})
    add_custom_command(
            TARGET zoltraak POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${image} ${CMAKE_BINARY_DIR}/
    )
endforeach ()

foreach (map ${MAPS})
    add_custom_command(
            TARGET zoltraak POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${map} ${CMAKE_BINARY_DIR}/
    )
endforeach ()

foreach (save ${SAVED})
    add_custom_command(
            TARGET zoltraak POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${save} ${CMAKE_BINARY_DIR}/
    )
endforeach ()