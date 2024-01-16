// for use in python, compile with:
//  gcc -shared -o solverlib.so -fPIC solver.c
//
// for running directly, compile with:
//  gcc -fPIC solver.c
// you can then run .\a.exe to run the code (only the `main` function)
//
// don't forget to recompile after making changes

#include <stdbool.h>


bool solve_sudoku(char code[81]) {
    // solves the sudoku made by the code
    // modifies the code in-place to the solution
    // returns whether a solution was found

    // just set the code to a random value for testing
    for (int i = 0; i < 81; ++i) {
        code[i] = (i % 9) + '1';
    }
    return false;
}
