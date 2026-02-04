cmake_minimum_required(VERSION 3.18.1)

# Define the project name
project("dwa")

# Add the main native library
# Ensure native-lib.cpp is in the same directory as this file
add_library(dwa SHARED native-lib.cpp)

# Set C++ standard to 17 for thread and chrono support
set_target_properties(dwa PROPERTIES CXX_STANDARD 17)

# Locate the NDK log library for __android_log_print
find_library(log-lib log)

# Locate the jnigraphics library for AndroidBitmap_lockPixels
# This is required for the nativeUpdateSurface function
find_library(graphics-lib jnigraphics)

# Link all required libraries to our "dwa" target
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
)
