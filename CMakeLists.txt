
cmake_minimum_required(VERSION 3.10)
project(Pentaminos)
set(CMAKE_CXX_STANDARD 17)

include_directories(
    src/avl
    src/board
    src/graph
    src/piece
    src/state
)

# Compilação com Interface Gráfica (Raylib)
find_package(raylib 4.0 QUIET)

if (raylib_FOUND)
    add_executable(pentaminos
        main.cpp
        src/board/Board.cpp
        src/piece/Piece.cpp
        src/state/State.cpp
        src/graph/GraphSolver.cpp
        src/avl/AVLTree.cpp
    )
    target_link_libraries(pentaminos raylib)
else()
    message(FATAL_ERROR "Raylib nao encontrado. Instale: vcpkg install raylib:x64-mingw-dynamic")
endif()

