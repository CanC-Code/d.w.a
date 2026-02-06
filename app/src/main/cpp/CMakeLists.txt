cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Collect recompiled sources (Static Recompilation / Helper logic)
# We check if the directory exists first to prevent CMake from complaining 
# if you haven't generated the recompiled C++ files yet.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/recompiled")
    file(GLOB RECOMPILED_SOURCES CONFIGURE_DEPENDS "recompiled/*.cpp")
else()
    set(RECOMPILED_SOURCES "")
endif()

# 2. Define the main native library
add_library(dwa SHARED 
    native-lib.cpp 
    ${RECOMPILED_SOURCES}
)

# 3. Include Directories
# Includes the current dir (for MapperMMC1.h) and recompiled headers
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Optimization & Debugging Flags
# -O3 is critical for NES emulation to maintain 60FPS on mid-range devices.
# -flto (Link Time Optimization) is added to optimize across native-lib and recompiled files.
target_compile_options(dwa PRIVATE 
    -O3 
    -fexceptions 
    -frtti
    -ffast-math
)

# 5. Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 6. Link all required libraries
# 'atomic' is required for the std::mutex used in the screen_buffer lock.
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
