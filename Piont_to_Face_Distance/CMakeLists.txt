cmake_minimum_required(VERSION 3.0.0)

project(point2face VERSION 0.1.0)

SET(CMAKE_CXX_COMPILER "D:/mingw64/bin/g++.exe") # 默认c++编译器

SET(CMAKE_BUILD_TYPE "Release")  # 选项: Release Debug MinSizeRel RelWithDebInfo

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

add_library(point2face_library STATIC
    src/fuction.cpp
   # src/person.cpp
)
#add_library(point2face::library ALIAS point2face_library)

target_include_directories(point2face_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(point2face
    src/point2face.cpp
)

# link the new hello_library target with the hello_binary target
target_link_libraries( point2face
    PRIVATE
   point2face_library
)
