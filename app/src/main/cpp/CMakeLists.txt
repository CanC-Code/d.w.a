cmake_minimum_required(VERSION 3.18.1)
project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly defining every file ensures the build system 
# creates the necessary object files (.o) for the linker.
set(DWA_SOURCES
    native-lib.cpp
    MapperMMC1.cpp
    PPU.cpp
    recompiled/Dispatcher.cpp
    recompiled/Bank00.cpp
    recompiled/Bank01.cpp
    recompiled/Bank02.cpp
    recompiled/Bank03.cpp
    recompiled/Header.cpp
)

add_library(dwa SHARED ${DWA_SOURCES})

target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# Performance optimizations
target_compile_options(dwa PRIVATE -O3 -flto -Wno-switch)
target_link_options(dwa PRIVATE -flto -Wl,--gc-sections)

find_library(log-lib log)
find_library(graphics-lib jnigraphics)

target_link_libraries(dwa ${log-lib} ${graphics-lib} android atomic m)
