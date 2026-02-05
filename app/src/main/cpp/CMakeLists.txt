cmake_minimum_required(VERSION 3.18.1)

project("dwa")

# 1. Collect all recompiled C++ files generated from ASM
# These are located in the 'recompiled' directory created by your CI
file(GLOB RECOMPILED_SOURCES "recompiled/*.cpp")

# 2. Define the main native library
# We include both the core engine (native-lib.cpp) and all recompiled game logic
add_library(dwa SHARED 
    native-lib.cpp 
    ${RECOMPILED_SOURCES}
)

# Set C++ standard to 17 for thread, chrono, and advanced features
set_target_properties(dwa PROPERTIES CXX_STANDARD 17)

# 3. Locate standard NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 4. Link all required libraries
# 'android' is required for AAssetManager
# 'jnigraphics' is required for AndroidBitmap_lockPixels
# 'aaudio' handles sound output
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    aaudio
    android
)
