cmake_minimum_required(VERSION 3.1)

project(Subverse)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED audio network graphics window system)

#Include directory
include_directories(src/Dependencies/)

#Add files
set(SOURCES 
    src/main.cpp
    src/Game.cpp
    src/GameObject.cpp
    src/Game/WorldTextures.cpp
    src/Game/CYGameLevel.cpp
    src/Game/PlayerMovement.cpp
    src/Maths/Matrix.cpp
    src/Util/MouseRay.cpp
    src/Util/FileUtil.cpp
    src/Util/FPSCounter.cpp
    src/Util/Maths.cpp
    src/Util/ToggleKey.cpp
    src/Util/Collision.cpp
    src/GUI/nuklear.cpp
    src/GUI/DebugLogGUI.cpp
    src/GUI/EditorGUI.cpp
    src/Input/KeyboardController.cpp
    src/ResourceManager/ResourceHolder.cpp
    src/States/StatePlaying.cpp
        src/Editor/BoundingBox.cpp
    src/Editor/GeoOctree.cpp
    src/Editor/Grid.cpp
    src/Editor/OldFormat/OldFormatUtil.cpp
    src/Editor/OldFormat/OldFormatConvertor.cpp
    src/Editor/OldFormat/OldFormatExtractor.cpp
    src/Editor/CYObjects/MeshBuilder.cpp
    src/Editor/CYObjects/CYPlat.cpp
    src/Editor/CYObjects/CYWall.cpp
    src/Renderer/Model.cpp
    src/Renderer/Primitives.cpp
    src/Renderer/Camera.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Mesh.cpp
    src/Renderer/OpenGLErrorCheck.cpp
    src/Renderer/Shader/Shader.cpp
    src/Renderer/Shader/ShaderLoader.cpp
        src/Editor/FreeroamEditorView.cpp src/Editor/FreeroamEditorView.h src/Editor/GridEditorView.cpp src/Editor/GridEditorView.h src/Editor/PlayerEditorView.cpp src/Editor/PlayerEditorView.h src/Editor/EditorCamera.cpp src/Editor/EditorCamera.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#Define the executable
add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
)
# glad
set(DEP_DIR "src/Dependencies")
add_library("glad" "${DEP_DIR}/glad.c")
target_include_directories("glad" PRIVATE "${DEP_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${DEP_DIR}")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# NativeFileDialog (for linux compilation)
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

# NativeFileDialog utilizes GTK+ libraries to compile
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Link NFD with the executable
target_link_libraries(${PROJECT_NAME} ${LIB_DIR}/libnfd.a ${GTK3_LIBRARIES})

# Link SFML libraries & its dependancies with the executable
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})