function setValue(index, value) {
    if (value == "0" || value == "." || value == "") {
        document.getElementById(index).value = "";
        value = "0";
    } else {
        document.getElementById(index).value = value;
    }
    let v = document.getElementById("sudoku-code").value;
    document.getElementById("sudoku-code").value = v.substring(0, index) + value + v.substring(index + 1);

    checkForDuplicates();
}

function getValue(index) {
    return document.getElementById("sudoku-code").value.charAt(index);
}

function checkForDuplicates() {
    const boxes = [0, 3, 6, 27, 30, 33, 54, 57, 60];
    const boxOffsets = [0, 1, 2, 9, 10, 11, 18, 19, 20];
    for (let cell = 0; cell < 81; ++cell) {
        if (getValue(cell) == 0) {
            document.getElementById(cell).classList.remove("highlighted");
            continue;
        }
        let row = Math.floor(cell / 9);
        let col = cell % 9;
        let box = 3 * Math.floor(row / 3) + Math.floor(col / 3);
        box = boxes[box];
        
        let do_highlight = false;
        for (let i = 0; i < 9; ++i) {
            if (
                (i != col && getValue(cell) == getValue(9 * row + i)) ||
                (i != row && getValue(cell) == getValue(9 * i + col)) ||
                (box + boxOffsets[i] != cell && getValue(cell) == getValue(box + boxOffsets[i]))
             ) {
                do_highlight = true;
                break;
            }
        }

        if (do_highlight) {
            document.getElementById(cell).classList.add("highlighted");
        } else {
            document.getElementById(cell).classList.remove("highlighted");
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
    if (96 <= key && key <= 105) {
        // numpad key values
        key -= 48;
    }
    key = String.fromCharCode(key);
    if (event.keyCode == 8 || event.keyCode == 46) {
        setValue(cell, "0");
    } else if ('0' <= key && key <= '9') {
        setValue(cell, key);
    }
}

function onGridChange() {
    let grid = document.getElementById("sudoku-code").value;
    if (grid.length === 81) {
        for (let i = 0; i < 81; ++i) {
            setValue(i, grid[i]);
        }
    }
}

async function solveGrid() {
    const grid = document.getElementById("sudoku-code").value;
    const response = await fetch("/solve-sudoku?grid=" + grid);
    const solution = await response.json();
    for (let i = 0; i < 81; ++i) {
        setValue(i, solution.grid[i]);
    }
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
