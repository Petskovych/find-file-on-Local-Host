# find-file-on-Local-Host

This program searches for a given file name in a specified directory and its subdirectories using multiple threads. It provides a command-line interface for users to enter the filename to search for.

## Compilation Instructions

To compile the program, you need a C compiler (GCC) and a C++ compiler (G++). Use the provided Makefile to compile the program:
Follow the prompts to enter the name of the file you want to find. The program will search for the file in the directory that you specify!!! `C:\\` and its subdirectories.
## File Structure

- `main.cpp`: Contains the main function and user interface.
- `file_search_lib.c` and `file_search_lib.h`: Implements the file search functionality using multiple threads.
- `Makefile`: Instructions for compiling the program.
- `README.md`: This file.

## Compilation Notes

- Compilation of the static library `file_search_lib.a` should be done separately from compiling the main program. This ensures that changes in the library code trigger recompilation only when necessary.
- 
## Author

Petskovych Andrii
