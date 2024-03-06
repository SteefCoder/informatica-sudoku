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

function setValue(index, value) {
    if (value == "0" || value == "." || value == "") {
        document.getElementById(index).value = "";
        value = "0";
    } else {
        document.getElementById(index).value = value;
    }
    let v = document.getElementById("sudoku-code").value;
    document.getElementById("sudoku-code").value = v.substring(0, index) + value + v.substring(index + 1);
}

function setGrid(grid) {
    for (let i = 0; i < 81; ++i) {
        if (grid[i] == "0" || grid[i] == ".") {
            setValue(i, "");
        } else {
            setValue(i, grid[i]);
        }
    }
}

function emptyGrid() {
    for (let i = 0; i < 81; ++i) {
        setValue(i, "0");
    }
}

function handleInput(event, cell) {
    event.preventDefault();

    let key = event.keyCode;
    key = String.fromCharCode(key);
    if (event.keyCode == 8 || event.keyCode == 46) {
        setValue(cell, "0");
    } else if ('0' <= key && key <= '9') {
        setValue(cell, key);
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
    setGrid(solution.grid);
}

function handleArrows(event, cell) {
    switch (event.key) {
        case "ArrowLeft":
            if (cell % 9 != 0) {
                document.getElementById(cell - 1).focus();
            } break;
        case "ArrowUp":
            if (cell > 8) {
                document.getElementById(cell - 9).focus();
            } break;
        case "ArrowRight":
            if (cell % 9 != 8) {
                document.getElementById(cell + 1).focus();
            } break;
        case "ArrowDown":
            if (cell < 72) {
                document.getElementById(cell + 9).focus();
            } break;
    }
}
