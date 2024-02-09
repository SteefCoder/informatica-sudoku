function parseGrid() {
    let grid = "";
    for (let i = 0; i < 81; ++i) {
        let el = document.getElementById(i);
        if (el.value == "") {
            grid += "0";
        } else {
            grid += el.value;
        }
    }
    return grid;
}

function setGrid(grid) {
    for (let i = 0; i < 81; ++i) {
        if (grid[i] == "0" || grid[i] == ".") {
            document.getElementById(i).value = "";
        } else if ("1" <= grid[i] || grid[i] <= 9) {
            document.getElementById(i).value = grid[i];
        }
    }
}

function emptyGrid() {
    for (let i = 0; i < 81; ++i) {
        document.getElementById(i).value = "";
    }
}

function handleInput(event, cell) {
    event.preventDefault();

    let key = event.keyCode;
    key = String.fromCharCode(key);

    if (key == '0') {
        document.getElementById(cell).value = "";
    } else if ('1' <= key && key <= '9') {
        document.getElementById(cell).value = key;
    }
}

function setGridFromCode() {
    let code = document.getElementById("sudoku-code").value;
    if (code.length === 81) {
        setGrid(code);
    }
}


async function solveGrid() {
    const grid = parseGrid();
    const response = await fetch("/solve-sudoku?grid=" + grid);
    const solution = await response.json();
    if (solution.solvable) {
        setGrid(solution.grid);
    }
}