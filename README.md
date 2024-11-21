# `c-optim` - Developer Guide

## Overview

This project implements a mathematical expression optimizer in C. It parses mathematical expressions, builds an Abstract Syntax Tree (AST), and uses gradient descent to find local minima.

## Project Structure

```
c-optim/
├── include/
│   ├── lexer.h       - Token scanning and lexical analysis
│   ├── node.h        - AST node definitions and operations
│   ├── node_list.h   - Linked list implementation for nodes
│   ├── optimizer.h   - Gradient descent optimization
│   ├── parser.h      - Expression parsing
│   ├── tree_iter.h   - Tree traversal utilities
│   └── utils.h       - Utilities (panic)
├── src/
│   ├── lexer.c
│   ├── main.c
│   ├── node.c
│   ├── node_list.c
│   ├── optimizer.c
│   ├── parser.c
│   ├── tree_iter.c
│   └── utils.c
├── .gitignore
├── input.txt
├── CMakeLists.txt
├── USERS.md
└── README.md
```

## Core Components

### 1. Lexer

-   Handles tokenization of input expressions
-   Supports numbers, variables, and operators (+, -, \*, /, parentheses)
-   Implements 1-token lookahead for parsing

### 2. Parser

-   Implements recursive descent parsing
-   Grammar rules:
    ```
    F -> Var | Num | E | -F
    T → F * T | F / T | F
    E → T + E | T - E | T
    ```
-   Constructs AST from input expression

### 3. AST Nodes

-   Base node structure with polymorphic operations
-   Node types:
    -   Binary operations (add, subtract, multiply, divide)
    -   Unary operations (negation)
    -   Leaf nodes (variables, numbers)

### 4. Optimizer

-   Uses gradient descent to find local minima
-   Configuration constants:
    ```c
    const float LR = 10e-3;        // Learning rate
    const float MAX_GRAD = 10e-4;  // Gradient threshold
    const size_t MAX_STEPS = 10e6; // Maximum iterations
    ```

## Build Instructions

```bash
mkdir build
cd build

# Debug build
cmake \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_BUILD_TYPE=Release ..

make
./optimizer
```

## Usage

The program expects input files with the following format:

```
<expression>
<variable 1 definition>
<variable 2 definition>
...
<variable n definition>
```

`<expression>` can be any mathematical function consisting of the basic
operators `+`, `-`, `*`, `/` and the defined variables.

Example:

```
(x-2)*(x-3)*x*y + (y-10)*y + x+y
x = 2
y = 3
```

## Implementation Details

### Memory Management

-   All nodes must be freed using appropriate functions:
    -   `free_node()` for expression trees
    -   `free_numeric_var_node()` for variable nodes
    -   `free_node_list()` for node lists

### Gradient Computation

1. Forward pass (evaluation)
2. Backward pass (gradient computation)
3. Gradient descent update step

### Tree Traversal

-   Implements topological sorting for correct gradient flow
-   Uses visitor pattern for tree operations
