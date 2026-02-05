cmake_minimum_required(VERSION 3.18.1)

project("dwa")

# 1. Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Collect recompiled sources
# Note: file(GLOB) is convenient but doesn't track new files well. 
# CONFIGURE_DEPENDS ensures CMake re-scans when you add/remove files in CI.
file(GLOB RECOMPILED_SOURCES CONFIGURE_DEPENDS "src/main/cpp/recompiled/*.cpp")

# 3. Define the main native library
# We include the core engine, the Mapper, and all recompiled banks
add_library(dwa SHARED 
    src/main/cpp/native-lib.cpp 
    ${RECOMPILED_SOURCES}
)

# 4. Include Directories
# This allows native-lib.cpp and MapperMMC1.h to find each other
target_include_directories(dwa PRIVATE 
    src/main/cpp 
    src/main/cpp/recompiled
)

# 5. Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 6. Link all required libraries
# added -latomic for 64-bit integer operations on some ARM builds
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    aaudio
    android
    atomic
    m
)
