from flask import Flask, render_template, request

try:
    from sudoku_solver import silvan_solve_sudoku, stefan_solve_sudoku
except ImportError:
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

    solved, solution = silvan_solve_sudoku(int_grid)
    if not solved:
        print("Intuitive solver could not finish the sudoku, leaving it to backtracking.")
        # assuming any changes made by silvan_solver are correct
        solved, solution = stefan_solve_sudoku(solution)
    else:
        print("Intuitive solver completely solved the sudoku!")

    return {"grid": solution, "solvable": solved}


if __name__ == '__main__':
    app.run(debug=True)
