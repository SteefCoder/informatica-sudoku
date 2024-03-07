// gcc -shared -o stefan_solver.so -fPIC -Ofast stefan_solver.c

#include <stdint.h>
#include <stdbool.h>


typedef struct Sudoku {
    // The grid with all numbers in the sudoku.
    int8_t grid[81];

    // Bitmaps of the numbers present in each row, col and box.
    uint16_t rows[9];
    uint16_t cols[9];
    uint16_t boxes[9];
} sudoku_t;


typedef struct Square {
    int8_t cell;
    int8_t row;
    int8_t col;
    int8_t box;
} square_t;


// A precomputed array of all squares with their cell, row, col and box
const square_t SQUARES[81] = {
    {0,0,0,0},{1,0,1,0},{2,0,2,0},{3,0,3,1},{4,0,4,1},{5,0,5,1},{6,0,6,2},{7,0,7,2},{8,0,8,2},
    {9,1,0,0},{10,1,1,0},{11,1,2,0},{12,1,3,1},{13,1,4,1},{14,1,5,1},{15,1,6,2},{16,1,7,2},{17,1,8,2},
    {18,2,0,0},{19,2,1,0},{20,2,2,0},{21,2,3,1},{22,2,4,1},{23,2,5,1},{24,2,6,2},{25,2,7,2},{26,2,8,2},
    {27,3,0,3},{28,3,1,3},{29,3,2,3},{30,3,3,4},{31,3,4,4},{32,3,5,4},{33,3,6,5},{34,3,7,5},{35,3,8,5},
    {36,4,0,3},{37,4,1,3},{38,4,2,3},{39,4,3,4},{40,4,4,4},{41,4,5,4},{42,4,6,5},{43,4,7,5},{44,4,8,5},
    {45,5,0,3},{46,5,1,3},{47,5,2,3},{48,5,3,4},{49,5,4,4},{50,5,5,4},{51,5,6,5},{52,5,7,5},{53,5,8,5},
    {54,6,0,6},{55,6,1,6},{56,6,2,6},{57,6,3,7},{58,6,4,7},{59,6,5,7},{60,6,6,8},{61,6,7,8},{62,6,8,8},
    {63,7,0,6},{64,7,1,6},{65,7,2,6},{66,7,3,7},{67,7,4,7},{68,7,5,7},{69,7,6,8},{70,7,7,8},{71,7,8,8},
    {72,8,0,6},{73,8,1,6},{74,8,2,6},{75,8,3,7},{76,8,4,7},{77,8,5,7},{78,8,6,8},{79,8,7,8},{80,8,8,8}
};


void set_square(sudoku_t *sudoku, square_t square, int8_t value) {
    sudoku->grid[square.cell] = value;

    uint16_t mask = 1 << value;
    sudoku->rows[square.row] |= mask;
    sudoku->cols[square.col] |= mask;
    sudoku->boxes[square.box] |= mask;
}


int8_t reset_square(sudoku_t *sudoku, square_t square) {
    int8_t value = sudoku->grid[square.cell];
    sudoku->grid[square.cell] = 0;

    uint16_t mask = ~(1 << value);
    sudoku->rows[square.row] &= mask;
    sudoku->cols[square.col] &= mask;
    sudoku->boxes[square.box] &= mask;

    return value;
}


uint16_t find_neighbours(const sudoku_t *sudoku, square_t square) {
    return sudoku->rows[square.row] | sudoku->cols[square.col] | sudoku->boxes[square.box];
}


int8_t build_from_grid(sudoku_t *sudoku, const int8_t grid[81]) {
    int8_t zeros = 0;

    for (int i = 0; i < 81; ++i) {
        set_square(sudoku, SQUARES[i], grid[i]);

        if (grid[i] == 0) {
            zeros += 1;
        }
    }

    return zeros;
}


int8_t fill_basic(
    sudoku_t *sudoku,
    square_t basic_solved[81],
    int8_t current_basic_solved,
    int8_t *zero_index
) {
    int8_t filled = 0;
    int8_t max_neigbours = -1;

    for (int i = 0; i < 81; i++) {
		if (sudoku->grid[i] != 0) continue;

        square_t square = SQUARES[i];
		uint16_t neighbours = find_neighbours(sudoku, square);

		// count the number of different neighbours (aka the number of set bits)
		// ignore the zero bit, so shift by one before counting
		int neighbour_count = __builtin_popcount(neighbours >> 1);
        // printf("%u\n", neighbours);

		if (neighbour_count == 9) {
			// the square has neighbours of all variations
			// and therefore has no options
			// all previously set numbers must now be reset
			for (int i = 0; i < filled; i++) {
				reset_square(sudoku, basic_solved[current_basic_solved + i]);
			}
			return -1;

		} else if (neighbour_count == 8) {
			// 8 out of 9 numbers are not possible, so 
			// there is only one possibility
			for (int n = 1; n <= 9; n++) {
				if ((neighbours & (1 << n)) == 0) {
					set_square(sudoku, square, n);
					basic_solved[current_basic_solved + filled] = square;
					++filled;
					break;
				}
			}
		} else if (neighbour_count > max_neigbours) {
            max_neigbours = neighbour_count;
            *zero_index = i;
        }
	}
	return filled;
}


bool _solve_sudoku(
    sudoku_t *sudoku,
    square_t basic_solved[81],
    int8_t current_basic_solved,
    int8_t zero_count
) {

    if (zero_count == 0) {
        return true;
    }

    int8_t square_index = 0;

	// try to fill as many zeros as possible with a more basic algorithm
	// store these values, so they can be reset after
    int8_t filled = fill_basic(sudoku, basic_solved, current_basic_solved, &square_index);

    // the sudoku might be found to be unsolvable
    if (filled == -1) return false;

    // there are no zeros left, so the sudoku is solved
    if (square_index == -1) return true;

    square_t square = SQUARES[square_index];

    // the current zero might have been filled by the alg
    // in that case, go on to the next one
    if (sudoku->grid[square.cell] != 0) {
        return _solve_sudoku(
            sudoku,
            basic_solved,
            current_basic_solved + filled,
            zero_count - filled
        );
    }

    uint16_t neighbours = find_neighbours(sudoku, square);

    for (int n = 1; n <= 9; ++n) {
        if (neighbours & (1 << n)) continue;

        set_square(sudoku, square, n);
        if (_solve_sudoku(
            sudoku,
            basic_solved,
            current_basic_solved + filled,
            zero_count - filled - 1
        )) {
            return true;
        }
        reset_square(sudoku, square);
    }

    for (int i = 0; i < filled; ++i) {
        reset_square(sudoku, basic_solved[current_basic_solved + i]);
    }

    return false;
}


bool solve_sudoku(int8_t grid[81]) {
    sudoku_t sudoku = { 0 };
    int8_t zeros = build_from_grid(&sudoku, grid);
    square_t basic_solved[81] = { 0 };

    bool result = _solve_sudoku(&sudoku, basic_solved, 0, zeros);

    for (int i = 0; i < 81; ++i) {
        grid[i] = sudoku.grid[i];
    }

    return result;
}