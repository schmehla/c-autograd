# debug:
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug ..
# release:
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release ..

cmake_minimum_required(VERSION 3.10)
project(Optimizer)

# Set source files
file(GLOB SOURCES src/*.c)

# Add executable target
add_executable(optimizer ${SOURCES})

# Include headers
target_include_directories(optimizer PRIVATE include)

# Compiler flags
target_compile_options(optimizer PRIVATE -Wall)

# Add ASan for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(optimizer PRIVATE -fsanitize=address)
    target_link_libraries(optimizer PRIVATE -fsanitize=address)
endif()

# Optimize compile stage for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(optimizer PRIVATE -O3)
endif()
