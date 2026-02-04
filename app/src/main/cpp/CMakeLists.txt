cmake_minimum_required(VERSION 3.18.1)

project("dwa")

# Add the main native library
add_library(dwa SHARED native-lib.cpp)

# Set C++ standard to 17 for thread, chrono, and advanced math
set_target_properties(dwa PROPERTIES CXX_STANDARD 17)

# Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# Link all required libraries
# 'aaudio' is linked directly to fix the CI build failure
# 'android' is linked to allow the C++ code to use AAssetManager
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    aaudio
    android
)
