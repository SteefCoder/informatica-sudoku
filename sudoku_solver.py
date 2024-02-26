"""
This is a binding to the solver.c sudoku solver
The function solve_sudoku defined in here just calls the same-named c function.
"""

import ctypes

INT_81 = ctypes.c_int * 81

# load the compiled c file
_solverlib = ctypes.CDLL("./solverlib.so", winmode=0)

# set up the c function
_solve = _solverlib.solveSudoku2
_solve.argtypes = [INT_81]

def solve_sudoku(grid: list[int]) -> tuple[bool, list[int]]:
    """Takes in the code and returns the solution and whether it can be solved.
    If the sudoku cannot be solved, the returned solution should be the original sudoku."""
    array = INT_81(*grid)
    _solve(array)
    solved_grid = [int(x) for x in array.value]
    solvable = all(x != 0 for x in solved_grid)
    return solvable, solved_grid
