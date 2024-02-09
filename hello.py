from flask import Flask, render_template, request

from sudoku_solver import solve_sudoku

app = Flask(__name__)
app.secret_key = rb'43290&O*#@&$#@*Ofk#$#@#*^TIF$@#&B*'


@app.route('/')
def index():
    return render_template("index.html")


@app.route('/solve-sudoku', methods=['GET'])
def solve():
    grid = request.args["grid"]
    if not all(x.isdigit() for x in grid):
        return {"grid": "", "solvable": False}
    
    int_grid = [int(x) for x in grid]
    solvable, solution = solve_sudoku(int_grid)
    return {"grid": solution, "solvable": solvable}
