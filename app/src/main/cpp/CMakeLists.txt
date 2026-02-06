cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Automatic Source Discovery
# This finds all .cpp files in the root and the recompiled folder automatically
file(GLOB_RECURSE DWA_SOURCES 
    "*.cpp" 
    "recompiled/*.cpp"
)

# 2. Define the main native library
add_library(dwa SHARED ${DWA_SOURCES})

# 3. Include Directories
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Performance & Compatibility Flags
# Added -Wno-switch to ignore warnings from the massive switch-cases in Bank files
# Added -fno-plt for faster calls between shared logic
target_compile_options(dwa PRIVATE 
    -O3 
    -fexceptions 
    -frtti
    -ffast-math
    -flto
    -fomit-frame-pointer
    -fno-plt
    -Wno-switch
)

# 5. Linker Flags (Critical for LTO)
# Ensure the linker uses LTO to inline functions across Bank03 and Dispatcher
target_link_options(dwa PRIVATE 
    -flto
    -Wl,--gc-sections
)

# 6. System Libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 7. Final Linkage
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
