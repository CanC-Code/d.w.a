cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
# Added MapperMMC1.cpp to resolve "undefined symbol" linker errors
set(DWA_SOURCES
    native-lib.cpp
    MapperMMC1.cpp
    recompiled/Dispatcher.cpp
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

# Compiler optimizations for maximum performance
target_compile_options(dwa PRIVATE
    -O3
    -flto
    -fomit-frame-pointer
    -Wno-switch
    -fvisibility=hidden
    -fno-exceptions
    -fno-rtti
)

# Linker optimizations
# --gc-sections removes unused code, critical when linking 4 large recompiled banks
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
target_link_libraries(dwa
    ${log-lib}
    ${graphics-lib}
    android
    atomic
    m # Math library
)
