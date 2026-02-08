cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
# Added Controller.cpp to resolve thread-safety crashes during input
set(DWA_SOURCES
    native-lib.cpp
    Controller.cpp                  # Thread-safe Input Handling
    MapperMMC1.cpp                  # MMC1 Bank Switching logic
    PPU.cpp                         # NES Rendering & PPU registers
    recompiled/cpu_shared.cpp       # Core 6502 math and flag logic
    recompiled/Dispatcher.cpp       # Jump table for recompiled banks
    recompiled/Bank00.cpp
    recompiled/Bank01.cpp
    recompiled/Bank02.cpp
    recompiled/Bank03.cpp
)

# Build shared library
add_library(dwa SHARED ${DWA_SOURCES})

# Include directories
target_include_directories(dwa PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# Compiler optimizations for maximum performance on mobile
target_compile_options(dwa PRIVATE
    -O3                             # Aggressive optimization
    -flto                           # Link Time Optimization
    -fomit-frame-pointer
    -Wno-switch                     # Suppress switch warnings in large banks
    -fvisibility=hidden
    -fno-exceptions                 # Performance boost: no C++ exceptions
    -fno-rtti                       # Performance boost: no Run-Time Type Info
)

# Linker optimizations
# --gc-sections removes unused code from recompiled banks to reduce binary size
target_link_options(dwa PRIVATE
    -flto
    -Wl,--gc-sections
    -Wl,--strip-all
    -Wl,--no-rosegment
)

# Find required libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# Link libraries
# Note: latomic is placed twice or explicitly to ensure atomic ops on 32-bit ARM
target_link_libraries(dwa
    ${log-lib}
    ${graphics-lib}
    android
    atomic
    m 
)
