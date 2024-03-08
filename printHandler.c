const void printBitBoard(char* bitboard, int* basisNumbers) {
	printf("\n");
	for (int loop = 0; loop < 81; loop++)
	{
		if (bitboard[loop] == '0') {
			printf("   ");
		}
		else  if (basisNumbers[loop]) {
			printf(" \033[4m%c\033[0m ", bitboard[loop]);
		}
		else {
			printf(" %c ", bitboard[loop]);
		}

		if (loop % 9 == 8)
		{
			printf("\n");
		}
		if (loop == 26 || loop == 53)
		{
			printf("____________________________\n\n");
		}
		if (loop % 9 == 2 || loop % 9 == 5)
		{
			printf("|");
		}
	}
}

const void debug(char** inputList, int count) {
	if (count == 3 && (strcmp(inputList[2], "all") == 0 || strcmp(inputList[2], "row") == 0 || strcmp(inputList[2], "column") == 0 || strcmp(inputList[2], "block") == 0)) {
		debugPointersInts(inputList[2]);
	}
	else if (count != 3) {
		printf("Incorrect amount of arguments for debug\n");
	}
	else {
		printf("'%s' not recognised as debug argument\n", inputList[2]);
	}
}

const void printHandler(char** inputList, char* bb, int* basisNumbers, int count) {
	if (strcmp(inputList[1], "sudoku") == 0) {
		if (!inputList[2]) {
			printBitBoard(bb, basisNumbers);
		}
		else if (strcmp(inputList[2], "string") == 0) {
			printf("%s", bb);
		}
		else {
			printf("'%s' not recognized as print sudoku command\n", inputList[2]);
		}
	}
	else if ((strcmp(inputList[1], "info") == 0)) {
		printf("\nPrint commands:\n - sudoku : display the selected sudoku\n - debug : display numbers detected by system per row, column or block\n");
	}
	else if ((strcmp(inputList[1], "debug") == 0)) {
		debug(inputList, count);
	}
	else {
		printf("'%s' not recognized as print command\n", inputList[1]);
	}
}