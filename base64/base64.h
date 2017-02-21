#ifndef BASE64
#define BASE64

#define SUCCESS 1
#define FAILURE 0

// Encodes a byte file named inputFileName to a base64-text file named outputFileName.
// Returns FAILURE on wrong file names or SUCCESS after encoding.
int encode(char* inputFileName, char* outputFileName);

// Decodes a base64-text file named inputFileName to a byte file named outputFileName.
// Returns FAILURE if inputFile contains mistakes (unknown symbols, less symbols) or
// SUCCESS after decoding. If (ignoreFlag != 0), unknown symbols won't triggering FAILURE.
int decode(char* inputFileName, char* outputFileName, int ignoreFlag);

#endif