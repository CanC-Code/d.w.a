cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Explicit Source List
# Added MapperMMC1.cpp to handle the hardware logic
set(DWA_SOURCES
    native-lib.cpp
    MapperMMC1.cpp
    recompiled/dispatcher.cpp
    recompiled/Bank00.cpp
    recompiled/Bank01.cpp
    recompiled/Bank02.cpp
    recompiled/Bank03.cpp
)

# 2. Define the main native library
add_library(dwa SHARED ${DWA_SOURCES})

# 3. Include Directories
# Added the parent directory so recompiled files can find MapperMMC1.h and cpu_shared.h
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Compiler Flags
# -O3 and -flto are excellent choices for an emulator.
# Added -fomit-frame-pointer to squeeze out more performance on ARM.
target_compile_options(dwa PRIVATE 
    -O3 
    -fexceptions 
    -frtti
    -ffast-math
    -flto
    -fomit-frame-pointer
)

# 5. Linker Flags
target_link_options(dwa PRIVATE -flto)

# 6. Locate NDK libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 7. Link all required libraries
# 'atomic' is required for some C++17 features on older Android NDKs
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
