cmake_minimum_required(VERSION 3.18.1)

# The project name doesn't strictly define the library name, but it's good practice
project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the source files
# Note: Ensure native-lib.cpp is in the same directory as this file
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

# Build the shared library. 
# IMPORTANT: I am naming this "native-lib" to match your System.loadLibrary call.
# If you prefer "dwa", change this name AND change it in MainActivity.java.
add_library(native-lib SHARED ${DWA_SOURCES})

# Include directories so the compiler can find PPU.h, cpu_shared.h, etc.
target_include_directories(native-lib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# Optimization flags essential for Dragon Warrior performance
# -O3: Maximum optimization
# -flto: Link Time Optimization (drastically reduces binary size and increases speed)
# -fvisibility=hidden: Improves load times by hiding internal symbols
target_compile_options(native-lib PRIVATE 
    -O3 
    -flto 
    -Wno-switch 
    -fvisibility=hidden
)

# Linker options
target_link_options(native-lib PRIVATE 
    -flto 
    -Wl,--gc-sections
)

# Locate Android system libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# Link dependencies
# 'android' for general NDK features
# 'jnigraphics' for AndroidBitmap_lockPixels (essential for PPU rendering)
# 'atomic' for thread-safe operations in your engine loop
target_link_libraries(native-lib 
    ${log-lib} 
    ${graphics-lib} 
    android 
    atomic 
    m
)
