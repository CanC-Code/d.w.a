cmake_minimum_required(VERSION 3.18.1)
project("dwa")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Manually list every file. 
# This ensures Ninja creates a build edge for each recompiled bank.
set(DWA_SOURCES
    native-lib.cpp
    MapperMMC1.cpp
    PPU.cpp
    recompiled/Dispatcher.cpp
    recompiled/Bank00.cpp
    recompiled/Bank01.cpp
    recompiled/Bank02.cpp
    recompiled/Bank03.cpp
)

# 2. Define the library
add_library(dwa SHARED ${DWA_SOURCES})

# 3. Standard Includes
target_include_directories(dwa PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/recompiled
)

# 4. Critical Flags
# -flto is essential for performance but can sometimes hide linker errors 
# during debugging; keep it enabled for the final build.
target_compile_options(dwa PRIVATE 
    -O3 
    -flto 
    -Wno-switch 
    -Wno-unused-label
)

target_link_options(dwa PRIVATE 
    -flto 
    -Wl,--gc-sections
)

# 5. Libraries
find_library(log-lib log)
find_library(graphics-lib jnigraphics)

target_link_libraries(dwa 
    ${log-lib} 
    ${graphics-lib}
    android
    atomic
    m
)
