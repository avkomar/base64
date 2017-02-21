#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

void printHelp() {
	printf("base64.exe -e sourceFileName outputFileName - encoding.\n");
	printf("base64.exe -d sourceFileName outputFileName - decoding.\n");
	printf("base64.exe -d -i sourceFileName outputFileName - encoding with ignoring unknown symbols.\n");
}

void main(int argc, char* argv[]) {
	/*
	FILE* file = fopen("fewf.txt", "w");
	for (int i = 0; i < 256; i++) {
		fprintf(file, "%d, ", getSixByChar(i));
	}*/
	if (argc >= 4) {
		if (!strcmp(argv[1], "-e")) {
			if (argc == 4) {
				encode(argv[2], argv[3]);
				return;
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