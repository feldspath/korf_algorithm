# Korf Algorithm

Author: Marius Debussche

This project uses Korf algorithm to find optimal solutions of the Rubik's Cube. It is an A* algorithm that uses pre-computed solves to evaluate the count of the remaining moves to solve the cube.


## Build the program

Execute `make` to build the entire project.

## Executables

There are 2 executables:

- `corner_databse` builds the database to solve the corners of the cube.
- `solver` is the program that solves the requested scrambled configuration.

To solve a given position, please modify the scramble string in the file `solver.cpp`. Please note that requesting to solve a fully scrambled cube can take several hours.
