#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 200 			// maximum size of buffer, in characters

/**
* Copies the contents of the given input file to STDOUT
* @param inputFD the file descriptor of the file whose contents will be inserted into STDOUT
*/
void copy(int inputFD){
	// 1) Check if the given file descriptor is valid.
	if(inputFD == -1){perror("open");}

	int n; 						// the number of bytes being read & written by functions.
	char buffer[BUFFSIZE];		// the buffer that temporarily stores characters that will be printed.

	// 2) For as long as the file can be read, write the contents onto STDOUT.
	while( (n = read(inputFD, buffer, BUFFSIZE)) > 0){

		// 3) While writing to STDOUT, check for any mistakes during writing.
		if(write(STDOUT_FILENO, buffer, n) != n){perror("write");};
	}// while
}

/**
 * The main program that the user will interact with.
 * @param argc the amount of command line arguments presented to the program
 * @param argv the array of command line arguments presented to the program
 * @return the contents of given text files, or prints STDOUT if no text files are provided
 */
int main(int argc, char* argv[]){
	// 1) If no arguments are specified, print user input into STDOUT
	if(argc == 1){copy(STDIN_FILENO);}

	// 2) Loop through the command line arguments.
	for(int i=1; argv[i] != NULL; i++){
		// 3) If a dash is presented as a command line argument, print STDIN to STDOUT.
		if(*argv[i] == '-'){copy(STDIN_FILENO);}

		// 4) Otherwise, print contents of given file to STDOUT.
		else{
			int fd = open(argv[i], O_RDONLY);
			copy(fd);
			close(fd);
		}
	}// for
}// main
