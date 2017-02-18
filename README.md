# Base64
Here is a sample of [Base64](https://en.wikipedia.org/wiki/Base64) coder/decoder implementation in c-lang.

## Using
* `base64.exe -e sourceFileName outputFileName` — encodes (binary) source file to base64-formated text file named 'outputFileName'
* `base64.exe -d (-i) sourceFileName outputFileName` — decodes base64-formated text file to binary file named 'outputFileName'. Using -i flag, makes decoder ignoring unknown symbols.