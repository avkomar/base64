#include <stdio.h>
#include "base64.h"

#define FAKE -5
#define FAKE_SYMBOL '='
#define FAKE_SYMBOL_CODE -2
#define UNKNOWN_SYMBOL -1

static char alphabet[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', ' y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

static int codes[256] = { UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, 62, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, FAKE_SYMBOL_CODE, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL, UNKNOWN_SYMBOL };

char getCharBySix(int six) {
	if ((six >= 0) && (six <= 64)) {
		return alphabet[six];
	}

	return FAKE_SYMBOL;
}

int getSixByChar(char chr) {
	return codes[chr];
}

int encode(char* inputFileName, char* outputFileName) {

	if ((!inputFileName) || (!outputFileName)) {
		return FAILURE;
	}

	FILE* inputFile = fopen(inputFileName, "rb");
	FILE* outputFile = fopen(outputFileName, "w");
	if ((!inputFile) || (!outputFile)) {
		return FAILURE;
	}

	// Groups of bits as dec format.
	int eight[3];	// 101010 11 - 0011 1001 - 11 000111
	int six[4];		// 101010-11   0011-1001   11-000111
	// Reached the end of file, need to put fakes.
	int eofFlag = 0;

	do {

		for (int i = 0; i < 3; i++) {
			eight[i] = fgetc(inputFile);
			if ((eight[i]) == EOF) {
				eofFlag = 1;
				// Cleaning trash bytes.
				for (int j = 2; j > i; j++) {
					eight[i] = EOF;
				}
				break;
			}
		}

		six[0] = eight[0] >> 2;
		six[1] = ((eight[0] & 0b11) << 4) | (eight[1] >> 4);
		six[2] = ((eight[1] & 0b1111) << 2) | (eight[2] >> 6);
		six[3] = (eight[2] & 0b111111);

		if (eofFlag) {
			// Inserting fakes.
			if (eight[2] == EOF) {
				six[3] = FAKE;
				six[2] = ((eight[1] & 0b1111) << 2) | 0;
				if (eight[1] == EOF) {
					six[2] = FAKE;
					six[1] = ((eight[0] & 0b11) << 4) | 0;
					if (eight[0] == EOF) {
						continue;
					}
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			char newChar = getCharBySix(six[i]);
			fprintf(outputFile, "%c", newChar);
		}

	} while (!eofFlag);

	fclose(inputFile);
	fclose(outputFile);

	return SUCCESS;
}

int decode(char* inputFileName, char* outputFileName, int ignoreFlag) {

	if ((!inputFileName) || (!outputFileName)) {
		return FAILURE;
	}

	FILE* inputFile = fopen(inputFileName, "r");
	FILE* outputFile = fopen(outputFileName, "wb");
	if ((!inputFile) || (!outputFile)) {
		return FAILURE;
	}

	char six[4];  // 101010-11   0011-1001   11-000111
	int eight[3]; // 101010 11 - 0011 1001 - 11 000111
	int eofFlag = 0;

	while (1) {
		for (int i = 0; i < 4; i++) {
			char chr = fgetc(inputFile);
			if (chr == EOF) {
				if (i != 0) {
					// Bad encoded text.
					return FAILURE;
				}
				six[0] = EOF;
				break;
			}
			six[i] = getSixByChar(chr);
			if (six[i] == UNKNOWN_SYMBOL) {
				if (ignoreFlag) {
					// Dropping wrong symbol.
					--i;
				}
				else {
					return FAILURE;
				}
			}
		}

		if (six[0] == EOF) {
			break;
		}

		eight[0] = (six[0] << 2) | (six[1] >> 4);
		eight[1] = ((six[1] & 0b1111) << 4) | (six[2] >> 2);
		eight[2] = ((six[2] & 0b11) << 6) | six[3];

		if (six[3] == FAKE_SYMBOL_CODE) {
			eight[2] = FAKE;
			if (six[2] == FAKE_SYMBOL_CODE) {
				eight[1] == FAKE;
				if (six[1] == FAKE_SYMBOL_CODE) {
					return FAILURE;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			if (eight[i] != FAKE) {
				fputc(eight[i], outputFile);
			}
		}
	}

	fclose(inputFile);
	fclose(outputFile);

	return SUCCESS;
}