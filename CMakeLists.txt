cmake_minimum_required(VERSION 3.10)
project(Optimizer)

# Set source files
set(SOURCES src/main.c src/lexer.c src/operations.c src/optimizer.c src/parser.c)

# Add executable target
add_executable(optimizer ${SOURCES})

# Include headers
target_include_directories(optimizer PRIVATE include)

# Compiler flags
target_compile_options(optimizer PRIVATE -Wall)
