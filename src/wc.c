#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096


/**
 * Counts the number of bytes in a given file and places values into an array
 * @param fd the file descriptor which refers to the file that will be counted.
 * @param count an integer array containing the amount of newlines, words, and bytes in a file, each value placed in array respectively.
 */
void countBytes(int fd, int count[3]){
	char temp[1];
	lseek(fd, 0, SEEK_SET);

	while(read(fd, temp, 1) > 0){
		count[2]++;
	}// while
}// countBytes

/**
 * Counts the number of lines in a given file
 * @param fd the file descriptor which refers to the file that will be counted.
 * @param count an integer array containing the amount of newlines, words, and bytes in a file, each value placed in array respectively.
 */
void countLines(int fd, int count[3]){
	char temp[1];						// a temporary buffer containing the character being read
	lseek(fd, 0, SEEK_SET);

	while(read(fd, temp, 1) > 0){
		if(temp[0] == '\n'){count[0]++;}
	}// while
}// countLines

/**
 * Counts the number of words in a given file.
 * @param fd the file descriptor which refers to the file that will be counted.
 * @param count an integer array containing the amount of newlines, words, and bytes in a file, each value placed in array respectively.
 */
void countWords(int fd, int count[3]){
	char temp[1];						// a temporary buffer containing the character currently being read
	char prev;						// a buffer containing the previous character read

	// 1) Reset to beginning of file for reuse
	lseek(fd, 0, SEEK_SET);

	// 2) Read through the file one byte at a time.
	while(read(fd,temp,1) > 0){
		if(prev != '\n' && (temp[0] == ' ' || temp[0] == '\n')){count[1]++;}
		prev = temp[0];
	}// while
	
	// 3) Increment word count one more time for the word right before EOF.
	count[1]++;
}// countWords

/**
 * Counts number of bytes, lines, and words in a given file and prints them onto standard output in that order.
 * @param fd the file descriptor which refers to the file being read
 * @param count an integer array containing the amount of newlines, words, and bytes in a file, each value placed in array respectively.
 */
void countEverything(int fd, int count[3]){
	char temp[1];
	char prev;						// a buffer containing the previous character read

	// 1) Reset to beginning of file for reuse
	lseek(fd, 0, SEEK_SET);

	// 2) Read through the file one byte at a time and increment counts.
	while(read(fd,temp,1) > 0){
		count[2]++;
		if(temp[0] == '\n'){count[0]++;}
		if(prev != '\n' && (temp[0] == ' ' || temp[0] == '\n' || temp[0] == '\t')){count[1]++;}
		prev = temp[0];
	}// while

	// 3) Increment word count once more to account for the word right before EOF
	count[1]++;
}// countEverything 

/**
 * Counts the number of bytes, lines, and words written into standard input. 
 * @param count an integer array containing the amount of newlines, words, and bytes in a file, each value placed in array respectively.
 */ 
void countStandardIn(int options[3], int count[3]){
	char temp[1];

	while(read(STDIN_FILENO,temp,1) > 0){
		if(options[2] == 1){count[2]++;}
		if(temp[0] == '\n'){
			if(options[0] == 1){count[0]++;}
			if(options[1] == 1){count[1]++;}
		}
		else if(options[1] == 1 && temp[0] == ' '){count[1]++;}
	}// while
}// countStandardIn

/**
 * Sets true or false values for writing certain values based on the given options from the command line.
 * @param str the string containing options from the command line.
 * @param options the array that contains values that determine whether newlines, words, and/or bytes will be counted, each value is placed in array respectively.
 * @return 1 if operation was completed successfully and 0 otherwise.
 */
int toggleOptions(char* str, int options[3]){
	// 1) Assuming str begins with a dash "-", check to see if all options are either "c", "l", or "w".
	for(int i=1; i<strlen(str); i++){
		if(str[i] == 'c'){
			options[2] = 1;
		}
		else if(str[i] == 'l'){
			options[0] = 1;
		}
		else if(str[i] == 'w'){
			options[1] = 1;
		}
		else{
			perror(str);
			return 0;
		}// if
	}// for
	return 1;
}// toggleOptions

/**
 * Converts the given number into a string containing that value.
 * @param num the integer that will be converted to a string.
 * @param str the string that will contain the result of the conversion
 * @return a string representation of the input number
 */ 
char* intToString(int num, char* str){
	int i = 0;  // index on str

	// 1) Handle initial number being 0.
		if (num == 0){
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}// if

	// 2) Read integer from right to left, adding the rightmost integer to the string.
	while (num > 0){
		str[i] = ((num % 10) > 9)? ((num % 10)-10) + 'a' : (num % 10) + '0';
		num = num/10;
		i++;
	}// while

	// 3) Append the end of the string
	str[i] = '\0'; 

	// 4) Reverse the current string and return the resulting string
	for(int begin=0; begin<i; begin++){
		i--;
		char temp = str[begin];
		str[begin] = str[i];
		str[i] = temp;
	}// for
	return str;
}// intToString

/**
 * Write the given file and its to standard output, similar to the wc utility command.
 * @param file the name of the file that will be presented to stanard output
 * @param count the array contianing the counted number of newlines, words, and bytes respectively. 
 */
void writeResults(char* file, int count[3]){
	// 1) Create buffers representing tab, space, and nextline characters
	char space[1] = {' '};
	char nextline[1] = {'\n'};

	// 2) Begin one line by writing a space 
	write(STDOUT_FILENO, space, sizeof space);

	// 3) Write the number of newlines, words, and bytes of the current file
	for(int i=0; i<3; i++){
		if(count[i] != 0){
			char temp[sizeof count[i]];
			intToString(count[i], temp);
			write(STDOUT_FILENO, temp, strlen(temp));
			write(STDOUT_FILENO, space, sizeof space);
			write(STDOUT_FILENO, space, sizeof space);
		}// if
	}// for

	// 4) Write  the name of the file
	write(STDOUT_FILENO, file, strlen(file));
	write(STDOUT_FILENO, nextline, sizeof nextline);
}// writeResults

/**
 * This program counts the number of newlines, words, and bytes in a file and prints them to standard output, respectively
 * @param argc the number of command-line arguments presented to the program.
 * @param argv an array containing all the command-line arguments presented
 */ 
int main(int argc, char* argv[]){   
	// 1) Check if there are any command line arguments and act accordingly
	if(argc < 2){
		// 1a. Instantiate count variable to store number of newlines, words, and bytes.
		int count[3] = {0,0,0};    
		// 1b. Count from standard input
		int options[3] = {1,1,1};
		countStandardIn(options, count);
		// 1c. Print results to standard output
		writeResults("", count);
		return EXIT_SUCCESS;
	}// if

	// 2) Check if the first argument contains an option
	if(argv[1][0] == '-'){ /* 3) An option is presented as the first command line argument. */
		// 3a. Determine the options avaliable
		int options[3] = {0,0,0};						// array determine wheter to count (1) or not count(0) [newlines, words, bytes]
		int totalCount[3] = {0,0,0};						// the total number of newlines, words, and bytes ina  given file
		int startIndex = 1;							// the index on the command-line argument array where the program will read files

		while(startIndex < argc && argv[startIndex][0] == '-'){
			if(toggleOptions(argv[startIndex], options) == 0){
				perror(argv[startIndex]);
				return EXIT_FAILURE;
			}// if
			startIndex++;
		}// while

		// 3b. If valid options are presented and no files are read, assume standard input.
		if(argv[startIndex] == NULL){
			int count[3] = {0,0,0};
		
			countStandardIn(options,count);
			writeResults("", count);
		}// if

		// 3c. Run through the rest of the command line arguments
		for(int i=startIndex; argv[i] != NULL; i++){
			int count[3] = {0,0,0};

			// 3d. Open the current file
			int fd = open(argv[i], O_RDONLY);
			
			// 3e. If the file name is denoted as a dash, count from standard input. Otherwise, return an error if the file does not exist.
			if(strcmp(argv[i], "-") == 0){
				countStandardIn(options, count);
				writeResults(argv[i], count);
				continue;
			}// if
			else if(fd == -1){
				perror(argv[i]);
				return EXIT_FAILURE;
			}// if

			// 3f. For each file, store the number of newlines, words, and bytes from a file.
			for(int i=0; i<3;i++){
				if(options[i] == 1){
					if(i == 0){
						countLines(fd, count);
						countLines(fd, totalCount);
					}
					else if(i==1){
						countWords(fd, count);
						countWords(fd, totalCount);
					}
					else{
						countBytes(fd, count);
						countBytes(fd, totalCount);
					}// if
				}// if
			}// for
			close(fd);

			// 3g. Print the results to standard output before moving onto the next file
			writeResults(argv[i], count);
		}// for
		
		// 3h. Print total counts, if there is more than one input file
		if(argc-startIndex > 1){
			writeResults("total", totalCount);
		}// if
	}// if
	else{ /* 4) No options are presented and all command line arguments should be files. */
		int totalCount[3] = {0,0,0};
		int options[3] = {1,1,1};

		// 4a. Loop through the command line arguments
		for(int i=1; argv[i]!=NULL; i++){
		int count[3] = {0,0,0};    

		// 4b. If the file name is denoted as a dash, count from standard input.
		if(strcmp(argv[i], "-") == 0){
			countStandardIn(options, count);
			writeResults("", count);
		}
		else{
			// 4c. Open the current file.
			int fd = open(argv[i], O_RDONLY);
			if(fd == -1){
				perror(argv[i]);
				return 1;
			}// if
			// 4d. Count the number of newlines, bytes, and words in the opened file. 
			countEverything(fd, count);
			countEverything(fd, totalCount);
			close(fd);
		}// if

		// 4e. Print results to standard output.
		writeResults(argv[i], count);

		}// for
		if(argc > 2){
			writeResults("total", totalCount);
		}// if
	}// if
	return EXIT_SUCCESS;
}// main 
