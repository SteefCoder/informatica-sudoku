#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "printHandler.c"
#include "mathematicalSolver.c"

int main() {

	char bb[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	char bitBoard[] = ".1...8...3.472169...6....1....9.253..421.378..358.6....9....1...213874.9...5...2.";
	char bitBoard2[] = ".5..83.17...1..4..3.4..56.8....3...9.9.8245....6....7...9....5...729..861.36.72.4";
	char bitBoard3[] = "8..........36......7..9.2...5...7.......457.....1...3...1....68..85...1..9....4..";

	int basisNumbers[81] = { 0 };
	int basisNumbersAmount;
	basisNumbersAmount = translate(&bitBoard2, &bb, &basisNumbers);

	int inputing = 1;
	char* inputList[16];

	while (inputing)
	{
		char input[30];
		char* next_token = NULL;
		char* token;
		int count = 0;

		printf("\nInput here: ");
		gets_s(input, 29);

		inputList[count] = strtok_s(input, " ", &next_token);

		while (inputList[count] != NULL) {
			count++;
			inputList[count] = strtok_s(NULL, " ", &next_token);
		}
		char** ptr2 = inputList;
		if (strcmp(*inputList, "end") == 0) {
			return 0;
		}
		else if (strcmp(*inputList, "print") == 0 && count > 1) {
			printHandler(ptr2, &bb, &basisNumbers, count);
		}
		else if (strcmp(*inputList, "edit") == 0) {
			if (strcmp(inputList[1], "info") == 0 && count == 2) {
				printf("Edit notation is:\n\"edit row column newNumber\"\n");
			}
			else if (strcmp(inputList[1], "solve") == 0 && count == 2) {
				if (solveSudoku(bb, basisNumbersAmount) == 0) {
					clock_t start = clock();
					printBitBoard(bb, &basisNumbers);
					clock_t end = clock();
					printf("\n\n%d\n\n", end - start);
				}
			}
			else if (count < 4) {
				printf("Insufficiant input to edit Sudoku\n");
			}
			else {
				char* ptr4 = bb;
				if (editSudoku(ptr2, ptr4, &basisNumbers) == 0) {
					printBitBoard(bb, &basisNumbers);
				}
			}
		}
		else {
			printf("\nCommand not recognized\n");
		}
	}
}
