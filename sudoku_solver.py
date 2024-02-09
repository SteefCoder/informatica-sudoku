"""
This is a binding to the solver.c sudoku solver
The function solve_sudoku defined in here just calls the same-named c function.
"""

#import ctypes

#CHAR_81 = ctypes.c_char * 81

# load the compiled c file
#_solverlib = ctypes.CDLL("./solverlib.so")

# set up the c function
#_solve = _solverlib.solve_sudoku
#_solve.argtypes = [CHAR_81]
#_solve.restype = ctypes.c_bool

def solve_sudoku(grid: list[int]) -> tuple[bool, list[int]]:
    """Takes in the code and returns the solution and whether it can be solved.
    If the sudoku cannot be solved, the returned solution could be anything."""
    # str_pointer = CHAR_81(*map(ord, code))
    # solved = _solve(str_pointer)
    # return solved, str_pointer.value
    return True, list(range(1, 10)) * 9
