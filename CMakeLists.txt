cmake_minimum_required(VERSION 4.3)
project(Echoes_of_Shadows)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Echoes_of_Shadows
        Battle.cpp
        Battle.h
        Dialogue.cpp
        Dialogue.h
        Item.h
        main.cpp
        Player.cpp
        Player.h
        Save.cpp
        Save.h
        World.cpp
        World.h)
