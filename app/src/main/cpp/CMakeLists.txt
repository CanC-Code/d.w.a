cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Explicit Source List
# Explicitly listing files is safer for the linker to track dependencies.
set(DWA_SOURCES
    native-lib.cpp
    recompiled/Dispatcher.cpp
    recompiled/Bank00.cpp
    recompiled/Bank01.cpp
    recompiled/Bank02.cpp
    recompiled/Bank03.cpp
)

# 2. Define the main native library
add_library(dwa SHARED ${DWA_SOURCES})

# 3. Include Directories
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Compiler Flags
# Added -flto (Link Time Optimization). 
# This is VITAL for you because native-lib calls Dispatcher, 
# which calls Banks. LTO allows the compiler to inline these calls
# across different files, significantly increasing speed.
target_compile_options(dwa PRIVATE 
    -O3 
    -fexceptions 
    -frtti
    -ffast-math
    -flto
)

# 5. Linker Flags
# Ensure the linker also uses LTO
target_link_options(dwa PRIVATE -flto)

# 6. Locate NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 7. Link all required libraries
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
