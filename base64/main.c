#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

#define SUCCESS 1
#define FAILURE 0



void main(int argc, char* argv[]) {
	char* inputFileName = argc >= 2 ? argv[1] : "pic.png";
	char* outputFileName = argc >= 3 ? argv[2] : "output.txt";
	encode(inputFileName, outputFileName);
	decode(NULL, NULL);
	encode("pico.png", "pico.txt");
}