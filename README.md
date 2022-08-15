
# Unix Utilities
## About
This file contains my implementations of eight commonly-used UNIX command-line functions.

## CONFIGURATION
1. To compile and link all source code related to this project, enter the following command:
```
$ make
```
Eight executable files should now appear in the home directory of this project: wc, head, tail, true, false, rm, cat, and env.

If you wish to look through any of the source files for this project, they will be located inside the src subdirectory.

2. Enter the following command to clean up all executables:
```
$ make clean
```

## Usage

Below is a list of all the UNIX command-line functions included in this file.

1) **head** - copies given input files to the standard output, starting from the beginning of the file.
```
$ ./head [-c # | -n #] [files...]
```
This program comes with two options: "-c" and "-n"
- c: copy a given number (substitute # with a non-negative integer) of characters
- n: copy a given number (substitute # with a non-negative integer) of lines.

Please enter only one of these options, followed by a valid number, each time this program is run.

The program will assume to read from standard input if this program is run with no input files or if one of the input files is denoted as a dash "-"

2) **tail** - copies given input files to the standard output, starting from the end of the file
```
$ ./tail [-c # | -n #] [files...]
```
This program comes with two options: "-c" and "-n"
- c: copy a given number (substitute # with a non-negative integer) of characters
- n: copy a given number (substitute # with a non-negative integer) of lines.

Please enter only one of these options, followed by a valid number, each time this program is run.

The program will assume to read from standard input if this program is run with no input files or if one of the input files is denoted as a dash "-"

3) **wc** - reads one or more input files and, by default, writes the number of newlines, words, and bytes (in that order) contained in the files
```
$ ./wc [-c | -l | -w] [files...]
```
This program comes with three options: "-c", "-l", and "-w"
 - c: print the number of characters contained in the input file(s) 
 - l: print the number of newlines contained in the input file(s)
 - w: print the number of words contained in the input file(s)

Feel free to use any number and combination of options as long as they are passed in first.

The program will assume to read from standard input (your keyboard) if you run this program with no input files or if one of the input files is a dash "-"

4) **true** - returns with an exit status of 0 or EXIT_SUCCESS when run.
To determine whether this program has achieved its intended goal, enter the following command after running the program.
```
$ echo status $?
```
This command should return 0 if the program has run correctly.


5) **false** - returns an exit status of 1 or EXIT_FAILURE when run.
To determine whether this program has achieved its intended goal, enter the following command after running the program.
```
$ echo status $?
```
This command should return 1 if the program has run correctly.

6) **rm** - attempts to remove the non-directory type files specified on the command line. 
```
$ ./rm [-rf] file...
```
- r: enables the function to remove the file hierarchy rooted in each file argument. The user will be prompted to confirm their decision to remove the given files. If the user does not respond affirmatively, the file hierarchy rooted in that directory is skipped.
- f:  has the **rm** function assume affirmative responses from the user, foregoing any prompting.
- file... : the file(s) to be removed by the program

7) **cat** - reads files sequentially, writing them to standard output.
```
$ ./cat [file_name...]
```

8) **env** - prints all of the currently set environmental variables to standard output.
```
env
```