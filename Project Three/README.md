# Project Name

This project implements a binary search tree data structure for calcualting the integral of mathematical expression, including both definite and indefinite integral. Supported antiderivative include sin(x), cos(x), and x^a.

## Description

The project consists of the following files:

### `main.cpp`

Main program file that parses mathematical expressions from an input file, computes antiderivatives, and evaluates definite integrals. It handles input validation, expression parsing, binary search tree operations, integration, and output formatting efficiently.

### `BinarySearchTree.h` and `Node.h`

These files define the Binary Search Tree used to store the mathematical expression.

### `Term.cpp` and `Term.h`

These files define the `Term` class, which represents a mathematical expression, including components such as fraction, power, functions, etc.

## File Format

The input file contains mathematical expressions in the format `lowerbound | upperbound expression dx`, where:

- `lowerbound | upperbound` (optional) represents the limits of integration.
- `expression` represents the mathematical expression to be integrated.
- `dx` denotes the differential variable.

## Output

The output consists of the antiderivative of the given expressions, with "+ C" appended if the integral is indefinite and "= [result]" added if the integral is definite.
