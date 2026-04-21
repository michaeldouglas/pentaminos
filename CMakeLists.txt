
cmake_minimum_required(VERSION 3.10)
project(Pentaminos)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Compilação com Interface Gráfica (Raylib)
find_package(raylib 4.0 QUIET)

if (raylib_FOUND)
    add_executable(pentaminos
        main.cpp
        src/Board.cpp
        src/Piece.cpp
        src/State.cpp
        src/GraphSolver.cpp
        src/AVLTree.cpp
    )
    target_link_libraries(pentaminos raylib)
else()
    message(FATAL_ERROR "Raylib nao encontrado. Instale: vcpkg install raylib:x64-mingw-dynamic")
endif()

