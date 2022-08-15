#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096

/**
 * Prints the contents of standard input into standard output.
 * @param option determines whether standard input will be read by number of lines or bytes
 * @param limit the number of lines/bytes that will be read and printed
 */ 
void writeToOut(char* option, int limit){
	int n;
	char buffer[BUFFSIZE];

	// 1) Based on the given option, decide whether to read in number of bytes "-c" or lines "-n"
	if(option[1] == 'n'){
		// 2a. Read through the file for as long as there lines left to read
		while( limit > 0 && (n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0 ){
			if( write(STDOUT_FILENO, buffer, n) !=n) {perror("write");}
			// 2b. Decrement when a line has been completely written
			else{limit--;}
		}// while
	}// if
	else{
		// 3a. Read for as long as there are bytes left to read
		while( limit > 0 && (n = read(STDIN_FILENO, buffer, 1)) > 0){
			if(write(STDOUT_FILENO, buffer, 1) != 1){perror("write");}
			// 3b. Decrement when a byte has been written
			else{limit--;}
		}// while
	}// if

	// 4) Write ending tab
	char tab[1] = {'\n'};
	write(STDOUT_FILENO, tab, sizeof tab);
}// writeToOut

/**
 * Prints a certain amount of bytes/lines from the beginning of the given input files to standard output.
 * @param fd the file descriptor that refers to the file being printed to standard output.
 * @param option determines whether the file will be printed by number of bytes ("-c") or lines ("-n").
 * @param limit the number of bytes or lines that will be printed from the input file
 */
void writeHead(int fd, char* option, int limit){
	char temp[1];       // a buffer that temporarily holds a single letter.

	// 1) Based on the option parameter, determine whether to read input file by lines or bytes.
	if(strcmp(option, "-n") == 0){ 
		// 2a. Read the file one character at a time until limit on lines or end of file has been reached.
		while(limit > 0 && (read(fd, temp, 1)>0)){
			
			// 2b. Decrement lines left (limit parameter) everytime a newline character is detected.
			if(temp[0] == '\n'){limit--;}

			// 2c. Write the current character in temp onto standard output.
			write(STDOUT_FILENO, temp, 1);
		}// while
	}
	else{ 
		// 3a. Read the file one character at a time until limit on bytes or end of file has been reached.
		while(limit > 0 && (read(fd,temp,1) > 0)){
			// 3b. Decrement number of bytes left to read, regardless of what it contains.
			limit--;
			// 3c. Write the current byte in temp to standard output.
			write(STDOUT_FILENO, temp, 1);
		}// while
	}// if
	
	// 4) Write ending tab
	char tab[1] = {'\n'};
	write(STDOUT_FILENO, tab, sizeof tab);
}// writeHead


/**
 * This program prints a given number of lines/bytes from the beginning of input file(s)
 * @param argc the number of command line arguments presented to the program
 * @param argv the array containing all command-line arguments
 */ 
int main(int argc, char* argv[]){
	// 1) Check if there are no arguments present and act accordingly
	if(argc < 2){
		writeToOut("-n", 10);
		return 0;
	}// if

	// 2) Look through the command-line arguments
	char* option = "-n";
	int limit = 10;

	// 2a. Check if the first argument contains an option
	if( ( ( strcmp(argv[1],"-n") == 0) || (strcmp(argv[1], "-c") == 0) ) ){
		// 2b. Check if the second argument after it contains a number
		if(argv[2] == NULL || (limit = atoi(argv[2])) == 0){
			perror("head");
			return EXIT_FAILURE;
		} 
		// 2c. Iterate through the rest of the arguments.
		else{
			option = argv[1];
			for(int i=3; argv[i] != NULL; i++){
				// 2d. If an argument is passed in as a dash, direct standard input to standard output
				if(strcmp(argv[i], "-")==0){
					char* titleBuf;
					titleBuf = "==> standard input <==\n";
					write(STDOUT_FILENO, titleBuf, strlen(titleBuf));

					writeToOut(argv[1], atoi(argv[2]));
				}
				else{
					char titleBuf[100];
					sprintf(titleBuf, "==> %s <==\n", argv[i]);
					write(STDOUT_FILENO, titleBuf, strlen(titleBuf));
					

					int fd = open(argv[i], O_RDONLY);
					if(fd == -1){perror(argv[i]);}
					writeHead(fd, option, limit);
					close(fd);
				}// if
			}// for
		}// if

		// 2d. If valid options and numbers are presented, then assume standard input
		if(argc == 3){
			writeToOut(argv[1], atoi(argv[2]));
		}// if
	}// if
	// 3) If there are no options, iterate through the files with default option & limit
	else{
		// 3a. Iterate through the files
		for(int i=1; argv[i] != NULL; i++){
			// 3b. If the file is denoted as a dash, assume standard input
			if(strcmp(argv[i], "-")==0){
				if(argc > 2){
					char* titleBuf;
					titleBuf = "==> standard input <==\n";
					write(STDOUT_FILENO, titleBuf, strlen(titleBuf));
				}// if
				writeToOut("-n", 10);
			}
			else{
				// 3c. Otherwise, the writeHead() function on the file, writing the title of the file if there are multiple files.
				if(argc > 2){
					char titleBuf[100];
					sprintf(titleBuf, "==> %s <==\n", argv[i]);
					write(STDOUT_FILENO, titleBuf, strlen(titleBuf));
				}// if

				int fd = open(argv[i], O_RDONLY);
				if(fd == -1){perror(argv[i]);}
				writeHead(fd, option, limit);
				close(fd);
			}// if
		}// for
	}// if
}// main
