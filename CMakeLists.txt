    cmake_minimum_required(VERSION 3.28)

    project (
        SimpleGL
        VERSION 0.1
        LANGUAGES CXX
    )

    set(CMAKE_CXX_STANDARD 20)

    set (
        SOURCES
        src/main.cpp
        src/mesh.cpp
        src/objloader.cpp
        src/window.cpp
    )

    set (
        EXECUTABLE
        ${PROJECT_NAME}
    )

    find_package(SDL2 REQUIRED)

    add_executable(${EXECUTABLE} ${SOURCES})

    target_include_directories(${EXECUTABLE} PRIVATE include)

    target_link_libraries(${EXECUTABLE} PRIVATE SDL2::SDL2)

    include_directories(${EXECUTABLE} ${SDL2_INCLUDE_DIRS})

    target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -Wpedantic)