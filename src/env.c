#include <stdlib.h>
#include<stdio.h>

/**
 * Prints out all the current environment variables.
 * @param argc the number of command line arguments
 * @param argv a pointer that refers to an array containing all command line arguments
 * @param envp a pointer that refers to an array containing all the environment variables
 */ 
int main(int argc, char* argv[], char* envp[]){
	// 1) Iterate through the envp array until the value at index i returns null
	for(int i=0; envp[i] != NULL; i++){
		// 2) If there is a value at the index i, print out the environment variable contained at the index.
		printf("%s\n\n",envp[i]);
	}// for
	return 0;
}// main
