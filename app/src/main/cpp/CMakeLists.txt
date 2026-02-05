cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Collect recompiled sources
# Since we are IN src/main/cpp, the path is just "recompiled/"
file(GLOB RECOMPILED_SOURCES CONFIGURE_DEPENDS "recompiled/*.cpp")

# 2. Define the main native library
# Use direct filenames since they are in the same directory as this script
add_library(dwa SHARED 
    native-lib.cpp 
    ${RECOMPILED_SOURCES}
)

# 3. Include Directories
# Explicitly set the current dir and recompiled dir for headers
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 5. Link all required libraries
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    aaudio
    android
    atomic
    m
)
