#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

void printHelp() {
	printf("base64.exe -e sourceFileName outputFileName - encoding.\n");
	printf("base64.exe -d sourceFileName outputFileName - decoding.\n");
	printf("base64.exe -d -i sourceFileName outputFileName - encoding with ignoring unknown symbols.\n");
}

int checkFormatFlag(char* flag) {
	if (flag[0] == '-') {
		if (flag[1] == 'f') {
			for (int i = 2; i < strlen(flag); i++) {
				if ((flag[i] < '0') || (flag[i] > '9')) {
					return 0;
				}
			}
			return 1;
		}
	}
	return 0;
}

unsigned int parseFormatFlag(char* flag) {
	int result = 0;
	int i = strlen(flag) - 1;
	int multiplier = 1;

	while (1) {
		if ((flag[i] < '0') || (flag[i] > '9')) {
			break;
		}
		result += (flag[i] - '0') * multiplier;
		multiplier *= 10;
		i--;
	}

	return result;
}

void main(int argc, char* argv[]) {
	if (argc >= 4) {
		if (!strcmp(argv[1], "-e")) {
			if (argc == 4) {
				encode(argv[2], argv[3], 0);
				return;
			}
			if (argc == 5) {
				if (checkFormatFlag(argv[4])) {
					encode(argv[2], argv[3], parseFormatFlag(argv[4]));
				}
			}
		}
		if (!strcmp(argv[1], "-d")) {
			if (argc == 4) {
				decode(argv[2], argv[3], NORMAL_DECODE_MODE);
				return;
			}
			if (argc == 5) {
				if (!strcmp(argv[2], "-i")) {
					decode(argv[3], argv[4], IGNORE_DECODE_MODE);
					return;
				}
			}
		}
	}

	printHelp();
}