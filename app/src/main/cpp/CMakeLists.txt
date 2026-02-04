cmake_minimum_required(VERSION 3.18.1)

# Define the project name
project("dwa")

# Add the main native library
add_library(dwa SHARED native-lib.cpp)

# Set C++ standard to 17 for thread, chrono, and advanced math support
set_target_properties(dwa PROPERTIES CXX_STANDARD 17)

# Locate the NDK log library for __android_log_print
find_library(log-lib log)

# Locate the jnigraphics library for direct Bitmap manipulation
find_library(graphics-lib jnigraphics)

# Locate the aaudio library for low-latency 8-bit sound synthesis
find_library(aaudio-lib aaudio)

# Link all required libraries to our "dwa" target
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    ${aaudio-lib}
)
