// These store which numbers have already been used in each row/column/block//int* blockPointers[9] = { &block1[9], &block2[9], &block3[9], &block4[9], &block5[9], &block6[9], &block7[9], &block8[9], &block9[9] };
int rowPointers[9][9] = { 0 };
int columnPointers[9][9] = { 0 };
int blockPointers[9][9] = { 0 };

// These store the amount spaces where a certain number is available in row/column/block
int posRowPointers[9][9] = { 0 };
int posColumnPointers[9][9] = { 0 };
int posBlockPointers[9][9] = { 0 };

struct Sudoku {
	int rowPointers[9][9];
};

//struct Sudoku sudoku = { 0 };
//sudoku->rowPointers

//
int posLocations[81] = { 0 };

// Called via 'printHandler' to display the state of all the pointer variables
const void debugPointersInts(char* RCB) {
	// Displays all the values of the rows/column/block, visualizing what the computer sees


	// Prints out all the values of the stored values of the rows
	if (strcmp(RCB, "row") == 0 || strcmp(RCB, "all") == 0) {
		for (int i = 0; i < 81; i++) {
			if (i % 9 == 0) {
				printf("\nrow %d: ", i / 9 + 1);
			}
			printf("%d ", rowPointers[i / 9][i % 9]);
		}
	}
	printf("\n");
	// Prints out all the values of the stored values of the column
	if (strcmp(RCB, "column") == 0 || strcmp(RCB, "all") == 0) {
		for (int i = 0; i < 81; i++) {
			if (i % 9 == 0) {
				printf("\ncolumn %d: ", i / 9 + 1);
			}
			printf("%d ", columnPointers[i / 9][i % 9]);
		}
	}
	printf("\n");
	// Prints out all the values of the stored values of the block
	if (strcmp(RCB, "block") == 0 || strcmp(RCB, "all") == 0) {
		for (int i = 0; i < 81; i++) {
			if (i % 9 == 0) {
				printf("\nblock %d: ", i / 9 + 1);
			}
			printf("%d ", blockPointers[i / 9][i % 9]);
		}
	}
}


int solveSudoku(char* bb, int basisNumbersAmount) {
	int count = 0;

	// Repeats procces 10 times
	// To Implement: Repeat while the code made any changes, if not, its not gonna do anything the next iteration
	while (count < 10) {
		count++;
		// Checking blocks
		for (int i = 0; i < 9; i++) {
			// Cycling each number
			for (int n = 0; n < 9; n++) {
				// Check if the number isnt already in use in the block
				if (!blockPointers[i][n]) {
					// Cycling the places in the block
					for (int p = 0; p < 9; p++) {

						// Find Row Column and String locations
						int rowLocation = (p / 3) + (i / 3 * 3);
						int columnLocation = (p % 3) + (i % 3 * 3);
						int stringLocation = (9 * rowLocation) + columnLocation;

						// Checks if the numbers isnt already in the row or column
						// Or if the block is already used
						if (!rowPointers[rowLocation][n] && !columnPointers[columnLocation][n] && bb[stringLocation] == '0') {
							posLocations[stringLocation] = n + 1;
							posBlockPointers[i][n]++;
						}
					}
					if (posBlockPointers[i][n] == 1) {
						// This line can be used if the program is finding weird/incorrect numbers
						//printf("Found Block: %d Number: %d\n", i, n + 1);
						for (int p = 0; p < 9; p++) {
							int row = (i / 3 * 3) + (p / 3);
							int column = (p % 3) + (i % 3 * 3);
							int location = (9 * row) + column;

							if (posLocations[location] == n + 1) {
								bb[location] = n + '0' + 1;
								columnPointers[column][n] = n + 1;
								rowPointers[row][n] = n + 1;
								blockPointers[i][n] = n + 1;
								break;
							}
						}
					}
					// Reset amount of possible locations so it starts the next iteration fresh
					posBlockPointers[i][n] = 0;
				}
			}
		}
		// Checking the rows
		for (int i = 0; i < 9; i++) {
			// Cycling each number
			for (int n = 0; n < 9; n++) {

				// Check if the number isnt already in use in the row
				if (!rowPointers[i][n]) {

					// Cycling the columns in the row
					for (int p = 0; p < 9; p++) {

						int block = (i / 3 * 3) + (p / 3);
						int stringLocation = (9 * i) + p;

						if (!columnPointers[p][n] && !blockPointers[block][n] && bb[stringLocation] == '0') {
							posLocations[stringLocation] = n + 1;
							posRowPointers[i][n]++;
						}
					}
					if (posRowPointers[i][n] == 1) {
						//printf("Found Row: %d Number: %d\n", i, n + 1);
						for (int p = 0; p < 9; p++) {
							int location = (9 * i) + p;

							if (posLocations[location] == n + 1) {
								bb[location] = n + '0' + 1;
								columnPointers[p][n] = n + 1;
								rowPointers[i][n] = n + 1;
								int blockLocation = ((location / 9 / 3 * 3) + (location % 9 / 3));
								blockPointers[blockLocation][n] = n + 1;
								break;
							}
						}
					}
					posRowPointers[i][n] = 0;
				}
			}
		}
		// Checking columns
		for (int i = 0; i < 9; i++) {
			// Cycling each number
			for (int n = 0; n < 9; n++) {

				// Check if the number isnt already in use in the column
				if (!columnPointers[i][n]) {
					// Cycling the rows in the column
					for (int p = 0; p < 9; p++) {
						int block = (p / 3 * 3) + (i / 3);
						int stringLocation = (9 * p) + i;
						if (!rowPointers[p][n] && !blockPointers[block][n] && bb[stringLocation] == '0') {
							posLocations[stringLocation] = n + 1;
							posColumnPointers[i][n]++;
						}
					}
					if (posColumnPointers[i][n] == 1) {
						for (int p = 0; p < 9; p++) {
							int location = (9 * p) + i;

							if (posLocations[location] == n + 1) {
								bb[location] = n + '0' + 1;
								columnPointers[i][n] = n + 1;
								rowPointers[p][n] = n + 1;
								int blockLocation = ((location / 9 / 3 * 3) + (location % 9 / 3));
								blockPointers[blockLocation][n] = n + 1;
								break;
							}
						}
					}
					posColumnPointers[i][n] = 0;
				}
			}
		}
		int completed = 0;
		for (int i = 0; i < 81; i++) {
			if (*(bb + i) != '0') {
				completed++;
			}
		}
		if (completed == 81) {
			printf("Solved in %d cycles\n", count);
			return 0;
		}
	}
	printf("Unable to solve in %d cycles\n", count);
	printf("%d\n", rowPointers[0][5]);
	return 0;
}

int editSudoku(char** inputList, char* bb, int* basisNumbers) {

	int numberChange = atoi(inputList[3]);
	for (int i = 1; i < 3; i++) {
		if (!(0 < atoi(inputList[i]) < 10)) {
			return 1;
		}
	}
	if (!(-1 < numberChange < 10)) {
		printf("Number must bea number between 1 and 9, or 0 for an empty space\n");
		return 1;
	}

	int location = atoi(inputList[1]) + ((atoi(inputList[2]) - 1) * 9) - 1;
	int blockLocation = ((location / 9 / 3 * 3) + (location % 9 / 3));

	if (basisNumbers[location]) {
		printf("You can not change a starting number\n");
		return 1;
	}
	else if (rowPointers[atoi(inputList[2]) - 1][numberChange - 1]) {
		printf("Number already in row\n");
		return 1;
	}
	else if (columnPointers[atoi(inputList[1]) - 1][numberChange - 1]) {
		printf("Number already in column\n");
		return 1;
	}
	else if (blockPointers[blockLocation][numberChange - 1]) {
		printf("Number already in block\n");
		return 1;
	}

	printf("%d\n", rowPointers[0][5]);

	columnPointers[atoi(inputList[1]) - 1][numberChange - 1] = numberChange;
	rowPointers[atoi(inputList[2]) - 1][numberChange - 1] = numberChange;
	blockLocation = ((location / 9 / 3 * 3) + (location % 9 / 3));
	blockPointers[blockLocation][numberChange - 1] = numberChange;
	return 0;
}

const translate(char* string, char* bb, int* basisNumbers) {
	int basisNumbersAmount = 0;
	for (int loop = 0; loop < 81; loop++)
	{
		if (string[loop] == '.' || string[loop] == '0') {
			bb[loop] = '0';
		}
		else {
			bb[loop] = *(string + loop);
			basisNumbers[loop] = 1;
			basisNumbersAmount++;

			columnPointers[loop % 9][string[loop] - '0' - 1] = string[loop] - '0';
			rowPointers[loop / 9][string[loop] - '0' - 1] = string[loop] - '0';

			int blockLocation = ((loop / 9 / 3 * 3) + (loop % 9 / 3));
			blockPointers[blockLocation][string[loop] - '0' - 1] = string[loop] - '0';
		}
	}
	return basisNumbersAmount;
}

