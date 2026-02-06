cmake_minimum_required(VERSION 3.18.1)

project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Core Source Files
# Listing these explicitly ensures CMake tracks dependencies accurately.
set(CORE_SOURCES
    native-lib.cpp
    MapperMMC1.cpp
    PPU.cpp
)

# 2. Recompiled Bank Discovery
# We use CONFIGURE_DEPENDS so CMake checks for new files during every build.
# This ensures that when your Python script adds a new Bank file, it gets compiled.
file(GLOB RECOMPILED_SOURCES CONFIGURE_DEPENDS
    "recompiled/Dispatcher.cpp"
    "recompiled/Bank00.cpp"
    "recompiled/Bank01.cpp"
    "recompiled/Bank02.cpp"
    "recompiled/Bank03.cpp"
    "recompiled/Header.cpp"
)

# 3. Define the Shared Library
add_library(dwa SHARED 
    ${CORE_SOURCES} 
    ${RECOMPILED_SOURCES}
)

# 4. Include Directories
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 5. Optimization & Performance Flags
# -O3: Maximum optimization.
# -flto: Link-Time Optimization (CRITICAL for performance across recompiled banks).
# -Wno-switch: The Bank files have thousands of cases; this keeps logs clean.
target_compile_options(dwa PRIVATE 
    -O3 
    -flto
    -fno-plt
    -fomit-frame-pointer
    -ffast-math
    -Wno-switch
    -Wno-unused-label
)



# 6. Linker Flags
# --gc-sections removes unused code from the final .so to reduce size.
target_link_options(dwa PRIVATE 
    -flto
    -Wl,--gc-sections
    -Wl,--as-needed
)

# 7. System Libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

# 8. Final Linkage
target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
