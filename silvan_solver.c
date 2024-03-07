// gcc -shared -o silvan_solver.so silvan_solver.c -fPIC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Used to find all the other places from a block/row/column in the char81
const int BLOCK_ITERATIONS[9] = { 0, 1, 2, 9, 10, 11, 18, 19, 20 };
const int ROW_ITERATIONS[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
const int COLUMN_ITERATIONS[9] = { 0, 9, 18, 27, 36, 45, 54, 63, 72 };

// All the starting positions of the blocks in the char81
const int BLOCK_STARTS[9] = { 0, 3, 6, 27, 30, 33, 54, 57, 60 };

// Finds all the other places a cel 'sees'. So block/row/column
const int findUpdatePlaces[81][20] = {
	{ 1, 9, 2, 18, 3, 27, 4, 36, 10, 5, 45, 11, 6, 54, 7, 63, 19, 8, 72, 20 },
	{ 0, 10, 2, 19, 3, 28, 9, 4, 37, 5, 46, 11, 6, 55, 18, 7, 64, 8, 73, 20 },
	{ 0, 1, 11, 20, 3, 29, 9, 4, 38, 10, 5, 47, 6, 56, 18, 7, 65, 19, 8, 74 },
	{ 0, 1, 12, 2, 21, 30, 4, 39, 13, 5, 48, 14, 6, 57, 7, 66, 22, 8, 75, 23 },
	{ 0, 1, 13, 2, 22, 3, 31, 12, 40, 5, 49, 14, 6, 58, 21, 7, 67, 8, 76, 23 },
	{ 0, 1, 14, 2, 23, 3, 32, 12, 4, 41, 13, 50, 6, 59, 21, 7, 68, 22, 8, 77 },
	{ 0, 1, 15, 2, 24, 3, 33, 4, 42, 16, 5, 51, 17, 60, 7, 69, 25, 8, 78, 26 },
	{ 0, 1, 16, 2, 25, 3, 34, 15, 4, 43, 5, 52, 17, 6, 61, 24, 70, 8, 79, 26 },
	{ 0, 1, 17, 2, 26, 3, 35, 15, 4, 44, 16, 5, 53, 6, 62, 24, 7, 71, 25, 80 },
	{ 0, 10, 1, 11, 18, 2, 12, 27, 13, 36, 14, 45, 15, 54, 16, 63, 19, 17, 72, 20 },
	{ 9, 1, 0, 11, 19, 2, 12, 28, 13, 37, 14, 46, 15, 55, 18, 16, 64, 17, 73, 20 },
	{ 9, 2, 0, 10, 1, 20, 12, 29, 13, 38, 14, 47, 15, 56, 18, 16, 65, 19, 17, 74 },
	{ 9, 3, 10, 4, 11, 21, 5, 30, 13, 39, 14, 48, 15, 57, 16, 66, 22, 17, 75, 23 },
	{ 9, 4, 3, 10, 11, 22, 5, 12, 31, 40, 14, 49, 15, 58, 21, 16, 67, 17, 76, 23 },
	{ 9, 5, 3, 10, 4, 11, 23, 12, 32, 13, 41, 50, 15, 59, 21, 16, 68, 22, 17, 77 },
	{ 9, 6, 10, 7, 11, 24, 8, 12, 33, 13, 42, 14, 51, 60, 16, 69, 25, 17, 78, 26 },
	{ 9, 7, 6, 10, 11, 25, 8, 12, 34, 13, 43, 14, 52, 15, 61, 24, 70, 17, 79, 26 },
	{ 9, 8, 6, 10, 7, 11, 26, 12, 35, 13, 44, 14, 53, 15, 62, 24, 16, 71, 25, 80 },
	{ 0, 19, 9, 1, 20, 2, 21, 27, 22, 36, 10, 23, 45, 11, 24, 54, 25, 63, 26, 72 },
	{ 18, 1, 0, 10, 20, 2, 21, 28, 9, 22, 37, 23, 46, 11, 24, 55, 25, 64, 26, 73 },
	{ 18, 2, 0, 19, 11, 1, 21, 29, 9, 22, 38, 10, 23, 47, 24, 56, 25, 65, 26, 74 },
	{ 18, 3, 19, 12, 4, 20, 5, 30, 22, 39, 13, 23, 48, 14, 24, 57, 25, 66, 26, 75 },
	{ 18, 4, 3, 19, 13, 20, 5, 21, 31, 12, 40, 23, 49, 14, 24, 58, 25, 67, 26, 76 },
	{ 18, 5, 3, 19, 14, 4, 20, 21, 32, 12, 22, 41, 13, 50, 24, 59, 25, 68, 26, 77 },
	{ 18, 6, 19, 15, 7, 20, 8, 21, 33, 22, 42, 16, 23, 51, 17, 60, 25, 69, 26, 78 },
	{ 18, 7, 6, 19, 16, 20, 8, 21, 34, 15, 22, 43, 23, 52, 17, 24, 61, 70, 26, 79 },
	{ 18, 8, 6, 19, 17, 7, 20, 21, 35, 15, 22, 44, 16, 23, 53, 24, 62, 25, 71, 80 },
	{ 0, 28, 9, 29, 18, 30, 31, 36, 37, 32, 45, 38, 33, 54, 34, 63, 46, 35, 72, 47 },
	{ 27, 1, 10, 29, 19, 30, 36, 31, 37, 32, 46, 38, 33, 55, 45, 34, 64, 35, 73, 47 },
	{ 27, 2, 28, 11, 20, 30, 36, 31, 38, 37, 32, 47, 33, 56, 45, 34, 65, 46, 35, 74 },
	{ 27, 3, 28, 12, 29, 21, 31, 39, 40, 32, 48, 41, 33, 57, 34, 66, 49, 35, 75, 50 },
	{ 27, 4, 28, 13, 29, 22, 30, 39, 40, 32, 49, 41, 33, 58, 48, 34, 67, 35, 76, 50 },
	{ 27, 5, 28, 14, 29, 23, 30, 39, 31, 41, 40, 50, 33, 59, 48, 34, 68, 49, 35, 77 },
	{ 27, 6, 28, 15, 29, 24, 30, 31, 42, 43, 32, 51, 44, 60, 34, 69, 52, 35, 78, 53 },
	{ 27, 7, 28, 16, 29, 25, 30, 42, 31, 43, 32, 52, 44, 33, 61, 51, 70, 35, 79, 53 },
	{ 27, 8, 28, 17, 29, 26, 30, 42, 31, 44, 43, 32, 53, 33, 62, 51, 34, 71, 52, 80 },
	{ 0, 37, 9, 28, 38, 18, 29, 39, 27, 40, 41, 45, 42, 54, 43, 63, 46, 44, 72, 47 },
	{ 36, 1, 27, 10, 38, 19, 29, 39, 28, 40, 41, 46, 42, 55, 45, 43, 64, 44, 73, 47 },
	{ 36, 2, 27, 37, 11, 28, 20, 39, 29, 40, 41, 47, 42, 56, 45, 43, 65, 46, 44, 74 },
	{ 36, 3, 37, 12, 31, 38, 21, 32, 30, 40, 41, 48, 42, 57, 43, 66, 49, 44, 75, 50 },
	{ 36, 4, 30, 37, 13, 38, 22, 32, 39, 31, 41, 49, 42, 58, 48, 43, 67, 44, 76, 50 },
	{ 36, 5, 30, 37, 14, 31, 38, 23, 39, 32, 40, 50, 42, 59, 48, 43, 68, 49, 44, 77 },
	{ 36, 6, 37, 15, 34, 38, 24, 35, 39, 33, 40, 41, 51, 60, 43, 69, 52, 44, 78, 53 },
	{ 36, 7, 33, 37, 16, 38, 25, 35, 39, 34, 40, 41, 52, 42, 61, 51, 70, 44, 79, 53 },
	{ 36, 8, 33, 37, 17, 34, 38, 26, 39, 35, 40, 41, 53, 42, 62, 51, 43, 71, 52, 80 },
	{ 0, 46, 9, 28, 47, 18, 29, 48, 27, 49, 36, 37, 50, 38, 51, 54, 52, 63, 53, 72 },
	{ 45, 1, 27, 10, 47, 19, 29, 48, 28, 36, 49, 37, 50, 38, 51, 55, 52, 64, 53, 73 },
	{ 45, 2, 27, 46, 11, 28, 20, 48, 29, 36, 49, 38, 37, 50, 51, 56, 52, 65, 53, 74 },
	{ 45, 3, 46, 12, 31, 47, 21, 32, 30, 49, 39, 40, 50, 41, 51, 57, 52, 66, 53, 75 },
	{ 45, 4, 30, 46, 13, 47, 22, 32, 48, 31, 39, 40, 50, 41, 51, 58, 52, 67, 53, 76 },
	{ 45, 5, 30, 46, 14, 31, 47, 23, 48, 32, 39, 49, 41, 40, 51, 59, 52, 68, 53, 77 },
	{ 45, 6, 46, 15, 34, 47, 24, 35, 48, 33, 49, 42, 43, 50, 44, 60, 52, 69, 53, 78 },
	{ 45, 7, 33, 46, 16, 47, 25, 35, 48, 34, 42, 49, 43, 50, 44, 51, 61, 70, 53, 79 },
	{ 45, 8, 33, 46, 17, 34, 47, 26, 48, 35, 42, 49, 44, 43, 50, 51, 62, 52, 71, 80 },
	{ 0, 55, 9, 56, 18, 57, 27, 58, 36, 64, 59, 45, 65, 60, 61, 63, 73, 62, 72, 74 },
	{ 54, 1, 10, 56, 19, 57, 28, 63, 58, 37, 59, 46, 65, 60, 72, 61, 64, 62, 73, 74 },
	{ 54, 2, 55, 11, 20, 57, 29, 63, 58, 38, 64, 59, 47, 60, 72, 61, 65, 73, 62, 74 },
	{ 54, 3, 55, 12, 56, 21, 30, 58, 39, 67, 59, 48, 68, 60, 61, 66, 76, 62, 75, 77 },
	{ 54, 4, 55, 13, 56, 22, 57, 31, 66, 40, 59, 49, 68, 60, 75, 61, 67, 62, 76, 77 },
	{ 54, 5, 55, 14, 56, 23, 57, 32, 66, 58, 41, 67, 50, 60, 75, 61, 68, 76, 62, 77 },
	{ 54, 6, 55, 15, 56, 24, 57, 33, 58, 42, 70, 59, 51, 71, 61, 69, 79, 62, 78, 80 },
	{ 54, 7, 55, 16, 56, 25, 57, 34, 69, 58, 43, 59, 52, 71, 60, 78, 70, 62, 79, 80 },
	{ 54, 8, 55, 17, 56, 26, 57, 35, 69, 58, 44, 70, 59, 53, 60, 78, 61, 71, 79, 80 },
	{ 0, 64, 9, 55, 65, 18, 56, 66, 27, 67, 36, 68, 45, 69, 54, 70, 73, 71, 72, 74 },
	{ 63, 1, 54, 10, 65, 19, 56, 66, 28, 67, 37, 68, 46, 69, 55, 72, 70, 71, 73, 74 },
	{ 63, 2, 54, 64, 11, 55, 20, 66, 29, 67, 38, 68, 47, 69, 56, 72, 70, 73, 71, 74 },
	{ 63, 3, 64, 12, 58, 65, 21, 59, 30, 67, 39, 68, 48, 69, 57, 70, 76, 71, 75, 77 },
	{ 63, 4, 57, 64, 13, 65, 22, 59, 66, 31, 40, 68, 49, 69, 58, 75, 70, 71, 76, 77 },
	{ 63, 5, 57, 64, 14, 58, 65, 23, 66, 32, 67, 41, 50, 69, 59, 75, 70, 76, 71, 77 },
	{ 63, 6, 64, 15, 61, 65, 24, 62, 66, 33, 67, 42, 68, 51, 60, 70, 79, 71, 78, 80 },
	{ 63, 7, 60, 64, 16, 65, 25, 62, 66, 34, 67, 43, 68, 52, 69, 61, 78, 71, 79, 80 },
	{ 63, 8, 60, 64, 17, 61, 65, 26, 66, 35, 67, 44, 68, 53, 69, 62, 78, 70, 79, 80 },
	{ 0, 73, 9, 55, 74, 18, 56, 75, 27, 76, 36, 64, 77, 45, 65, 78, 54, 79, 63, 80 },
	{ 72, 1, 54, 10, 74, 19, 56, 75, 28, 63, 76, 37, 77, 46, 65, 78, 55, 79, 64, 80 },
	{ 72, 2, 54, 73, 11, 55, 20, 75, 29, 63, 76, 38, 64, 77, 47, 78, 56, 79, 65, 80 },
	{ 72, 3, 73, 12, 58, 74, 21, 59, 30, 76, 39, 67, 77, 48, 68, 78, 57, 79, 66, 80 },
	{ 72, 4, 57, 73, 13, 74, 22, 59, 75, 31, 66, 40, 77, 49, 68, 78, 58, 79, 67, 80 },
	{ 72, 5, 57, 73, 14, 58, 74, 23, 75, 32, 66, 76, 41, 67, 50, 78, 59, 79, 68, 80 },
	{ 72, 6, 73, 15, 61, 74, 24, 62, 75, 33, 76, 42, 70, 77, 51, 71, 60, 79, 69, 80 },
	{ 72, 7, 60, 73, 16, 74, 25, 62, 75, 34, 69, 76, 43, 77, 52, 71, 78, 61, 70, 80 },
	{ 72, 8, 60, 73, 17, 61, 74, 26, 75, 35, 69, 76, 44, 70, 77, 53, 78, 62, 79, 71 }
};

// Bitarray maniplulation functions
const void bit_on(int* bit_number, unsigned index)
{
	*bit_number |= (1 << index);
}

const void bit_off(int* bit_number, unsigned index)
{
	*bit_number &= ~(1 << index);
}

const int bit_read(int bit_number, unsigned index)
{
	return (1 << index) & bit_number;
}

const int bit_read_tf(int bit_number, unsigned index)
{
	return ((1 << index) & bit_number) >> index;
}

const void bit_flip(int* bit_number, unsigned index)
{
	*bit_number ^= (1 << index);
}

// Returns amount of bits that are 1 in integer
const int bit_count(int bit_number)
{
	int count = 0;
	while (bit_number) {
		count += bit_number & 1;
		bit_number >>= 1;
	}
	return count;
}

// Return True or False of certain bit
int onlyOneBitTF(unsigned n)
{
	return n && (!(n & (n - 1)));
}


// Possibilities checker
const int possCounter(int loopList[9], int start, int number, int possibilitie[81])
{
	int count = 0;
	for (int i = 0; i < 9; i++) {
		count += bit_read_tf(possibilitie[start + loopList[i]], number);
	}
	return count;
}


// Update possibilies and bb
const void update(int numberLocation, int number, int possibilities[81])
{
	for (int i = 0; i < 20; i++) {
		bit_off(&possibilities[findUpdatePlaces[numberLocation][i]], number);
	}
}


// Function for algorithm 1
const void algoritme1(int loop, int RCB, int n, int possibilities[81], int bb[81]) {
	int location;
	for (int i = 0; i < 9; i++) {

		// RCB = 9 is Rows RCB = 1 is columns RCB = 0 is Blocks
		// Replacement of 3 way if or switch
		location = RCB ? RCB * loop + (9 / RCB) * i : BLOCK_STARTS[loop] + BLOCK_ITERATIONS[i];

		if (bit_read(possibilities[location], n)) {
			bb[location] = n + 1;
			possibilities[location] = 0;
			update(location, n, possibilities);
			break;
		}
	}
}


int solveSudoku(int bb[81]) {

	// For backend visualization only, not part of final product
	// printBitBoard2(bb);

	// Setup main paramaters to keep track of the board
	int possibilities[81] = { 0 };

	// Used to check if algorithm 3 has finished
	int algo3done[9] = { 0 };

	// Determening all the possible numbers per cell
	for (int loop = 0; loop < 81; loop++)
	{
		if (!bb[loop]) {
			possibilities[loop] = 511;
			for (int c = 0; c < 20; c++)
			{
				if (bb[findUpdatePlaces[loop][c]]) {
					bit_off(&possibilities[loop], bb[findUpdatePlaces[loop][c]] - 1);
				}
			}
		}
	}
	// Main solving loop, keeps looping untill no changes can be made
	int doneSmth = 1, count = 0;

	while (doneSmth) {
		doneSmth = 0;
		count++;

		// Algoritme 1
		// Finds any number that only has 1 place in a ROW or COLUMN or BLOCK
		for (int i = 0, loop = 0, n = 0; i < 81; i++, loop = i / 9, n = i % 9) {
			if (possCounter(ROW_ITERATIONS, loop * 9, n, possibilities) == 1) {
				algoritme1(loop, 9, n, possibilities, bb);
				doneSmth = 1;
			}
			if (possCounter(COLUMN_ITERATIONS, loop, n, possibilities) == 1) {
				algoritme1(loop, 1, n, possibilities, bb);
				doneSmth = 1;
			}
			if (possCounter(BLOCK_ITERATIONS, BLOCK_STARTS[loop], n, possibilities) == 1) {
				algoritme1(loop, 0, n, possibilities, bb);
				doneSmth = 1;
			}
		}

		// Algorithm 2
		// Finds any cel that only has 1 option available
		for (int loop = 0; loop < 81; loop++) {
			if (onlyOneBitTF(possibilities[loop])) {
				unsigned i = 1, pos = 0;
				// Iterate through bits of n till we find a set bit 
				// i&n will be non-zero only when 'i' and 'n' have a set bit at same position 
				while (!(i & possibilities[loop])) {
					// Unset current bit and set the next bit in 'i' 
					i = i << 1;
					// increment position 
					++pos;
				}
				bb[loop] = pos + 1;
				possibilities[loop] = 0;
				update(loop, pos, possibilities);
				doneSmth = 1;
			}
		}

		// Algoritme 3
		// It can read if all number possibilities in a block are in the same column or row
		// If so, remove the number from other possibilities in the row/column
		for (int loop = 0, block = 0, number = 0; loop < 81; loop++, block = loop / 9, number = loop % 9) {
			int checkVar = 0;
			int checkVar2 = 0;
			int alreadyDone = bit_read(algo3done[block], number);
			int countInBlock = possCounter(BLOCK_ITERATIONS, BLOCK_STARTS[block], number, possibilities);

			if (!alreadyDone && (countInBlock == 2 || countInBlock == 3)) {

				// Cycle places in block
				for (int i = 0; i < 9; i++) {

					if (checkVar2 && bit_read_tf(possibilities[BLOCK_STARTS[block] + BLOCK_ITERATIONS[i]], number)) {
						int tempPos = BLOCK_STARTS[block] + BLOCK_ITERATIONS[i];
						int sameColumn = checkVar % 9 == checkVar2 % 9 && checkVar2 % 9 == tempPos % 9;
						int sameRow = checkVar / 9 == checkVar2 / 9 && checkVar2 / 9 == tempPos / 9;

						if (sameColumn || sameRow) {
							bit_on(&algo3done[block], number);

							// Cycles through entire row/column to remove the possibilities in other blocks
							for (int r = 0; r < 9; r++) {
								if (sameRow) {
									if (r / 3 != block % 3 && bit_read_tf(possibilities[(tempPos / 9 * 9) + r], number)) {
										doneSmth = 1;
										bit_off(&possibilities[(tempPos / 9 * 9) + r], number);
									}
								}
								else if (r / 3 != block / 3 && bit_read_tf(possibilities[(r * 9) + tempPos % 9], number)) {
									doneSmth = 1;
									bit_off(&possibilities[(r * 9) + tempPos % 9], number);
								}
							}
							break;
						}
					}
					if (bit_read_tf(possibilities[BLOCK_STARTS[block] + BLOCK_ITERATIONS[i]], number) && !bb[BLOCK_STARTS[block] + BLOCK_ITERATIONS[i]]) {
						if (countInBlock == 3) {
							if (checkVar) {
								int tempPos = BLOCK_STARTS[block] + BLOCK_ITERATIONS[i];
								int sameColumn = (checkVar - 0) % 9 == tempPos % 9;
								int sameRow = (checkVar - 0) / 9 == tempPos / 9;
								if (!(sameColumn || sameRow)) {
									break;
								}
								checkVar2 = BLOCK_STARTS[block] + BLOCK_ITERATIONS[i];
							}
							continue;
						}
						else {
							checkVar2 = BLOCK_STARTS[block] + BLOCK_ITERATIONS[i];
						}
						checkVar = BLOCK_STARTS[block] + BLOCK_ITERATIONS[i];
					}
				}
			}
		}
	}
	return bb;
}
