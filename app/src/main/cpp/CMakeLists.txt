cmake_minimum_required(VERSION 3.18.1)

# Define the project name
project("dwa")

# Add the main native library
add_library(dwa SHARED native-lib.cpp)

# Set C++ standard to 17 for thread, chrono, and math
set_target_properties(dwa PROPERTIES CXX_STANDARD 17)

# Locate standard NDK libraries
# We keep find_library for log and graphics as they are universally stable
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# Link all required libraries
# We link 'aaudio' directly to prevent the "NOTFOUND" error in CI
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    aaudio
    android # Added for AAssetManager support
)
