cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

project(SmithEngine VERSION 1.0)

find_program(CCACHE_PATH ccache HINTS ENV PATH)
if(CCACHE_PATH)
    message(STATUS "Using ccache: ${CCACHE_PATH}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PATH}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PATH}")
endif ()

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# Zrobione imgui dla Rayliba ALE nie ma go w releasie tak jak szajerman mówił
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    FetchContent_Declare(
        imgui
        URL https://github.com/ocornut/imgui/archive/refs/heads/master.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(imgui)

    FetchContent_Declare(
        rlimgui
        URL https://github.com/raylib-extras/rlImGui/archive/refs/heads/main.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(rlimgui)

    add_library(imgui_lib STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${rlimgui_SOURCE_DIR}/rlImGui.cpp
    )
    target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${rlimgui_SOURCE_DIR}
    )
    target_link_libraries(imgui_lib PUBLIC raylib)
endif()

set(SOURCES
        src/main.cpp
        src/core/Engine.cpp
        src/core/Node.cpp
        src/core/SceneManager.cpp
        src/nodes/Node3D.cpp
        src/input/InputManager.cpp
)

set(HEADERS
        include/core/Engine.h
        include/core/Node.h
        include/core/SceneManager.h
        include/core/Transform.h
        include/nodes/Node3D.h
        include/input/InputManager.h
        include/core/Scene3D.h
        include/core/ResourceManager.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE src include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SMITH_DEBUG)
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui_lib)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}
        ${rlimgui_SOURCE_DIR}
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)