cmake_minimum_required(VERSION 3.18.1)
project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Anchor all paths to the current directory of the CMakeLists file
set(DWA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/native-lib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MapperMMC1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Dispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Bank00.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Bank01.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Bank02.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Bank03.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled/Header.cpp
)

add_library(dwa SHARED ${DWA_SOURCES})

target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

target_compile_options(dwa PRIVATE -O3 -flto -Wno-switch)
target_link_options(dwa PRIVATE -flto -Wl,--gc-sections)

find_library(log-lib log)
find_library(graphics-lib jnigraphics)

target_link_libraries(dwa ${log-lib} ${graphics-lib} android atomic m)
