"""
This is a binding to the solver.c sudoku solver
The function solve_sudoku defined in here just calls the same-named c function.
"""

import ctypes

INT_81 = ctypes.c_int * 81

# load the compiled c file
_solverlib = ctypes.CDLL("./solverlib.so")

# set up the c function
_solve = _solverlib.solveSudoku
_solve.argtypes = [INT_81]
# _solve.restype = ctypes.c_bool

def solve_sudoku(grid: list[int]) -> tuple[bool, list[int]]:
    """Takes in the code and returns the solution and whether it can be solved.
    If the sudoku cannot be solved, the returned solution could be anything."""
    int_pointer = INT_81(*grid)
    _solve(int_pointer)
    sudoku = list(int_pointer.value)
    solved = all(x != 0 for x in sudoku)
    return solved, sudoku
