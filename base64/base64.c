#include <stdio.h>
#include "base64.h"

#define FAKE -5
#define FAKE_SYMBOL '='
#define FAKE_SYMBOL_CODE -2
#define UNKNOWN_SYMBOL -1

char getCharBySix(int six) {

	if ((six >= 0) && (six <= 51)) {
		return (six <= 25) ? ('A' + six) : ('a' + (six - 26));
	}

	if ((six >= 52) & (six < 62)) {
		return '0' + (six - 52);
	}

	if (six == 62) {
		return '+';
	}
	if (six == 63) {
		return '/';
	}

	return FAKE_SYMBOL;
}

int getSixByChar(char chr) {

	if ((chr >= 'A') && (chr <= 'Z')) {
		return chr - 'A';
	}

	if ((chr >= 'a') && (chr <= 'z')) {
		return chr - 'a' + 26;
	}

	if ((chr >= '0') && (chr <= '9')) {
		return chr - '0' + 52;
	}

	if (chr == '+') {
		return 62;
	}

	if (chr == '/') {
		return 63;
	}

	if (chr == FAKE_SYMBOL) {
		return FAKE_SYMBOL_CODE;
	}

	return UNKNOWN_SYMBOL;
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