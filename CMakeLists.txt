cmake_minimum_required(VERSION 3.20)

project(Echoes_of_Shadows
    VERSION 0.1.0
    DESCRIPTION "A story-driven C++ terminal RPG prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Echoes_of_Shadows
    Battle.cpp
    Dialogue.cpp
    Player.cpp
    Save.cpp
    World.cpp
    main.cpp
)

target_include_directories(Echoes_of_Shadows PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(Echoes_of_Shadows PRIVATE /W4 /permissive-)
else()
    target_compile_options(Echoes_of_Shadows PRIVATE -Wall -Wextra -Wpedantic)
endif()
