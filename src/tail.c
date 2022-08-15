#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096

/**
 * Prints the ending number of lines/bytes from standard input to standard output
 * @param option a string that determines whether to write by number of lines "-n" or characters "-c"
 * @param limit the number of lines/bytes that will be written to standard output
 * @return 1 if operation was sucessful and 0 otherwise
 */ 
int writeToOut(char* option, int limit){
	char buffer[BUFFSIZE];

	// 1) Determine which option to handle
	if(strcmp(option, "-n")==0){
		int tempIndex = 0;
		char* tempArray = malloc( 1 * sizeof(char));
		if(tempArray == NULL){
			perror(tempArray);
			return 0;
		}// if
		
		// 2a. Read from standard input and place into a temporary array.
		while( read(STDIN_FILENO, buffer, 1) > 0){
			tempArray = realloc(tempArray, tempIndex+1 * sizeof(char));
			if(tempArray == NULL){
				perror(tempArray);
				return 0;
			}// if

			tempArray[tempIndex] = buffer[0];
			tempIndex++;
		}// while
		int tempLength = tempIndex;

		// 2b. Read backwards on temporary array until either beginning of array or limit on the lines has been reached
		while(tempIndex >= 0 && limit > 0){
			if(tempArray[tempIndex] == '\n'){limit--;}
			tempIndex--;
		}// while

		// 2c. Read forward on the array and write each character to standard output.
		while(tempIndex <= tempLength){
			char tempChar[1];
			tempChar[0] = tempArray[tempIndex];
			write(STDOUT_FILENO, tempChar, 1);
			tempIndex++;
		}// while
		free(tempArray);
	}
	else{
		int tempIndex = 0;
		char* tempArray = malloc( 1 * sizeof(char));
		if(tempArray == NULL){
			perror(tempArray);
			return 0;
		}// if
		
		// 3a. Read from standard input and place into a temporary array.
		while( read(STDIN_FILENO, buffer, 1) > 0){
			tempArray = realloc(tempArray, tempIndex+1 * sizeof(char));
			if(tempArray == NULL){
				perror(tempArray);
				return 0;
			}// if

			tempArray[tempIndex] = buffer[0];
			tempIndex++;
		}// while
		int tempLength = tempIndex;

		// 3b. Read backwards on temporary array until either beginning of array or limit on the lines has been reached
		while(tempIndex >= 0 && limit > 0){
			limit--;
			tempIndex--;
		}// while

		// 3c. Read forward on the array and write each character to standard output.
		while(tempIndex <= tempLength){
			char tempChar[1];
			tempChar[0] = tempArray[tempIndex];
			write(STDOUT_FILENO, tempChar, 1);
			tempIndex++;
		}// while
		free(tempArray);
	}// if
	return 1;
}// writeToOut

/**
 * Prints a certain amount of bytes/lines from the end of the given input file to standard output.
 * @param fd the file descriptor that refers to the file being printed to standard output.
 * @param option determines whether the file will be printed by number of bytes ("-c") or lines ("-n").
 * @param limit the number of bytes or lines that will be printed from the input file
 */
void writeTail(int fd, char* option, int limit){
	char temp[1];       // a buffer that temporarily holds a single letter.

	// 1) Depending on the given option, choose between reading by number of lines or bytes. 
	if(strcmp(option, "-n")==0){ /* 2) Limit writing by number of lines */
		// 2a. Offset to near the end of the file
		lseek(fd, -1, SEEK_END);

		// 2b. Backtrack from the end of the file until the number of lines needed or the beginning of the file has been reached.
		int lines = 0;
		while(lines != limit+1 && (read(fd, temp, 1) > 0)){
			// 2c. If a newline character has been detected, increment the number of lines found.
			if(temp[0] == '\n'){lines++;}
			lseek(fd, -2, SEEK_CUR);
		}// while

		// 2d. Extra movement needed to account for the last '\n' being the EOF.
		lseek(fd, 2, SEEK_CUR);

		// 2e. From the current offset, read forward to the end of the file.
		while(read(fd, temp, 1) > 0){
			write(STDOUT_FILENO, temp, 1);
		}// while
	}
	else{ /* 3) Limit writing by the number of characters */
		// 3a. Offset to the end of the file.
		lseek(fd, -1, SEEK_END);
		limit--;

		// 3b. Backtrack to the end of the file until the number of bytes needed or the beginning of the file has been reached.
		while(limit > 0 && (read(fd, temp, 1) > 0)){
			lseek(fd, -2, SEEK_CUR);
			// 3c. Decrement the amount of bytes needed for each character read, regardless of what's in it.
			limit--;
		}// while

		// 3d. From the current offset, read forward to the end of the file.
		while(read(fd, temp,1)>0){
			write(STDOUT_FILENO, temp, 1);
		}// while
	}// if
}// writeTail 

/**
 * This program prints the ending number of bytes/lines in a given set of input files
 * @param argc the number of command line arguments
 * @param argv the array containing all command-line arguments presented to the program
 */ 
int main(int argc, char* argv[]){
	// 1) Check if there are no arguments present and act accordingly
	if(argc < 2){
		if(writeToOut("-n", 10) == 0){return EXIT_FAILURE;}
		return EXIT_SUCCESS;
	}// if

	// 2) Look through the command-line arguments
	char* option = "-n";
	int limit = 10;
	
	// 2a. Check if the first argument contains an option
	if( ( ( strcmp(argv[1],"-n") == 0) || (strcmp(argv[1], "-c") == 0) ) ){
		// 2b. Check if the second argument after it contains a number
		if(argv[2] == NULL || (limit = atoi(argv[2]) ) == 0){
			perror("head");
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
					if(writeToOut(argv[1], limit) == 0){return EXIT_FAILURE;};
				}
				else{
					char titleBuf[100];
					sprintf(titleBuf, "==> %s <==\n", argv[i]);
					write(STDOUT_FILENO, titleBuf , strlen(titleBuf));

					int fd = open(argv[i], O_RDONLY);
					if(fd == -1){perror(argv[i]);}
					writeTail(fd, option, limit);
					close(fd);
				}// if
			}// for
		}// if
		
		// 2e. If valid options and numbers are presented, assume standard input
		if(argc == 3){
			if(writeToOut(argv[1], atoi(argv[2])) == 0){return EXIT_FAILURE;}
		}// if
	}// if
	// 3) If there are no options, iterate through the files with default option & limit
	else{
		// 3a. Iterate through the files
		for(int i=1; argv[i] != NULL; i++){
			// 3b. If the file is denoted as a dash, assume standard input
			if(strcmp(argv[i], "-")==0){
				char* titleBuf;
				titleBuf = "==> standard input <==\n";
				write(STDOUT_FILENO, titleBuf, strlen(titleBuf));

				if(writeToOut("-n", 10) == 0){return EXIT_FAILURE;}
			}
			// 3c. Otherwise, perform writeToTail() on the file, writing the title of the file if there are multiple files.
			else{
				if(argc > 2){
					char titleBuf[100];
					sprintf(titleBuf, "==> %s <==\n", argv[i]);
					write(STDOUT_FILENO, titleBuf , strlen(titleBuf));
				}// if

				int fd = open(argv[i], O_RDONLY);
				if(fd == -1){perror(argv[i]);}
				writeTail(fd, option, limit);
				close(fd);

			}// if
		}// for
	}// if
}// main
