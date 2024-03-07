"""
This is a binding to the solver.c sudoku solver
The function solve_sudoku defined in here just calls the same-named c function.
"""

import ctypes

INT_81 = ctypes.c_int * 81
INT8_81 = ctypes.c_int8 * 81

# load the compiled c file
_stefan_solver = ctypes.CDLL("./stefan_solver.so", winmode=0)
_silvan_solver = ctypes.CDLL("./silvan_solver.so")

# set up the c function
_silvan_solve = _silvan_solver.solveSudoku
_silvan_solve.argtypes = [INT_81]

_stefan_solve = _stefan_solver.solve_sudoku
_stefan_solve.argtypes = [INT8_81]
_stefan_solve.restype = ctypes.c_bool

def silvan_solve_sudoku(grid: list[int]) -> tuple[bool, list[int]]:
    """Takes in the code and returns the solution and whether it can be solved.
    If the sudoku cannot be solved, the returned solution should be the original sudoku."""
    array = INT_81(*grid)
    _silvan_solve(array)
    solved_grid = [int(x) for x in array]
    solvable = all(x != 0 for x in solved_grid)
    return solvable, solved_grid

def stefan_solve_sudoku(grid: list[int]) -> tuple[bool, list[int]]:
    array = INT8_81(*grid)
    result = _stefan_solve(array)
    return result, [int(x) for x in array]
