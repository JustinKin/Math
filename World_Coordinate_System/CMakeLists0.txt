cmake_minimum_required(VERSION 3.0.0)

project(SphereConsensus VERSION 0.1.0)

SET(CMAKE_CXX_COMPILER "D:/mingw64/bin/g++.exe")

SET(CMAKE_BUILD_TYPE "Release")  # Debug模式 选项: Release Debug MinSizeRel RelWithDebInfo

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录

INCLUDE_DIRECTORIES("D:/QinJunyou/C/eigen3")
INCLUDE_DIRECTORIES("D:/QinJunyou/C/Math/RANSAC/include")
#link_directories("D:/MKL2020/compilers_and_libraries_2020.1.216/windows/mkl/lib/intel64_win")

add_library(SphereConsensus_library SHARED
    src/function.cpp
)
add_library(SphereConsensus::library ALIAS SphereConsensus_library)

target_include_directories(SphereConsensus_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

#link_libraries(
#    mkl_intel_lp64_dll
#    mkl_intel_thread_dll
#    mkl_core_dll
#)
link_libraries(
    #libiomp5md
    pthread
    m
    #dl

)


add_executable(SphereConsensus
                src/main.cpp
)

target_link_libraries(SphereConsensus

    PRIVATE
    SphereConsensus::library
)

#要链接的库文件的名字 相当于gcc中的-l(小写的l)参数
#只写库名就可以，就是libxxx.so的中间xxx的部分
#target_link_libraries(SphereConsensus
#    mkl_intel_lp64_dll
#    mkl_intel_thread_dll
#    mkl_core_dll
    #libiomp5md
#    pthread
#    m
    #dl
#)
