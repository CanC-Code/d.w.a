cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Collect recompiled sources
# GLOB is usually discouraged, but for generated files, it's the most practical way.
file(GLOB RECOMPILED_SOURCES CONFIGURE_DEPENDS "recompiled/*.cpp")

# 2. Define the main native library
add_library(dwa SHARED 
    native-lib.cpp 
    ${RECOMPILED_SOURCES}
)

# 3. Include Directories
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Optimization & Debugging Flags
# We use -O3 for performance, as 6502 emulation logic can be heavy on mobile.
target_compile_options(dwa PRIVATE -O3 -fexceptions -frtti)

# 5. Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 6. Link all required libraries
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
