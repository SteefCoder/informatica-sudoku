from flask import Flask, render_template, request

from .sudoku_solver import silvan_solve_sudoku, stefan_solve_sudoku

app = Flask(__name__)
app.secret_key = rb'43290&O*#@&$#@*Ofk#$#@#*^TIF$@#&B*'


@app.route('/')
def index():
    return render_template("homepage.html")

@app.route("/sudoku")
def sudoku():
    return render_template("sudoku.html")

@app.route('/about')
def about():
    return render_template("about.html")

@app.route('/contact')
def contact():
    return render_template("contact.html")

@app.route('/solve-sudoku', methods=['GET'])
def solve():
    grid = request.args["grid"]
    if not all(x.isdigit() for x in grid):
        return {"grid": "", "solvable": False} 
    
    int_grid = [int(x) for x in grid]
    solvable, solution = stefan_solve_sudoku(int_grid)
    return {"grid": solution, "solvable": solvable}


if __name__ == '__main__':
    app.run(debug=True)
