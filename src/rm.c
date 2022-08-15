#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

/**
 * Based on the given options array of the program for later use.
 * @param str the string continaing options from the command line
 * @param options the array containing "on/off" switches for "-r" and "-f" options in the current run of the program, both placed in array respectively.
 */ 
int toggleOptions(char* str, int options[2]){
	// 1) Iterate through each letter on the options string after '-'
	for(int i=1;i<strlen(str);i++){
		// 2) Note if there is an 'r' or an 'f' present
		if(str[i] == 'r'){
			options[0] = 1;
		}
		else if(str[i] == 'f'){options[1] = 1;}
		else{ // 3) Report any extraneous characters.
			perror(str);
			return 0;
		}// if
	}// for
	return 1;
}// toggleOptions

/**
 * Deletes all files and directories contained in the directory at the given pathname.
 * @param pathname the pathname of the directory that will be deleted
 * @param option an integer value that determines whether the function will prompt user to keep (0) or remove (1) the file hierarchy
 * @return an integer noting the status of the deletion: 0 if deletion was successful and 1 otherwise.
 */ 
int deleteDirectory(const char *pathname, int option) {
	// 1) Open the directory contained in the pathname
	DIR* directory = opendir(pathname);                                 // the pointer to the directory contained in pathname
	int status = -1;                                                    // the current status of the directory removal 

	// 2) Prompt user for deletion of directory
	if(option == 0){
		printf("Remove %s and all its contents?: ", pathname);
		char choice[100];
		scanf("%s", choice);
		if(choice[0] != 'y' && choice[0] != 'Y'){return 1;}
	}// if

	// 3) Return error if pathname is not a directory.
	if(directory == NULL){perror(pathname);}

	// 4) Proceed with deletion if directory exits.
	if(directory != NULL){
		struct dirent* ptr;                                             // pointer to the structure containing information about the directory.
		status = 0;

		// 4a. Read directory for as long as the directory can be read and files/directories can be deleted properly
		while( !status && (ptr = readdir(directory)) ){
			// 4b. Avoid the "." and ".." directories.
			if(!strcmp(ptr->d_name,".") || !strcmp(ptr->d_name,"..")){continue;}

			// 4c. Prepare filename for deletion calls
			int len = strlen(pathname) + strlen(ptr->d_name) + 2;       // length of anticipated name of pathname of current file
			char* filename = malloc(len);                               // buffer that will contain pathname of current file
			int removalStatus = -1;                                     // holds the status of the removal for the current file

			if(filename){
				struct stat fileinfo;                               // structure contianing information about the current file

				// 4d. Format relative pathname of current file for proper deletion.
				snprintf(filename, len, "%s/%s", pathname, ptr->d_name);
				// 4e. Check if the file is a directory or a file and record status of deletion.
				if (!stat(filename, &fileinfo)) {
					// Make a recursive call to if current file name refers to a directory.
					if (S_ISDIR(fileinfo.st_mode)){removalStatus = deleteDirectory(filename, option);}
					// Make an unlink(2) call if current filename contains a file.
					else{
						// Prompt user for confirmation if the file is write protected
						if(option == 0 && !(fileinfo.st_mode & S_IWUSR)){
							printf("remove %s, a write protected file? ", filename); 
							char choice[20];
							scanf("%s", choice);
							if(choice[0] == 'y' || choice[0] == 'Y'){removalStatus = unlink(filename);}
						}// if
						else{removalStatus = unlink(filename);}
					}// if
				}// if

				// 4f. Deallocate memory of filename buffer and update current status of the function.
				free(filename);
				status = removalStatus; 
			}// if
		}// while
		closedir(directory);
	}// if
	if(!status){status = rmdir(pathname);}
	return status;
}// deleteDirectory

/**
 * This program removes the given set of files or directories
 * @param argc the number of command-line arguments
 * @param argv an array containing the given command-line arguments
 */ 
int main(int argc, char* argv[]){
	// 1) Check if there are any command line arguments
	if(argc < 2){
		printf("usage: rm [-r | -f] file...\n       unlink file");
		return 1;
	}// if

	// 2) Determine if options are available
	if(argv[1][0] == '-'){ // 3) Options are presented
		// 3a. Check if the first one or two arguments contain arguments and record options for later
		int options[2] = {0,0};
		int index=1;
		while(index < argc && argv[index][0] == '-'){
			toggleOptions(argv[index], options);
			index++;
		}// while

		// 3b. Print instructions if options with no files are presented
		if(argv[index] == NULL && options[1] != 1){
			printf("usage: rm [-r | -f] file...\n       unlink file");
		}// if

		// 3c. Loop through remaining command line arguments, removing each file/directory along the way.
		for(int i=index; argv[i] != NULL; i++){
			struct stat fileinfo;
			if (!stat(argv[i], &fileinfo)) {
				// Make a recursive call to if current file name refers to a directory.
				if (S_ISDIR(fileinfo.st_mode)){deleteDirectory(argv[i], options[1]);}
				// Make an unlink(2) call if current file contains a file.
				else if(S_ISREG(fileinfo.st_mode)){unlink(argv[i]);}
				else if(options[1] != 1){perror(argv[i]);}
			}// if 
		}// for
	}// if
	else{// 4) No options are presented
		// 4a. Iterate through arguments and remove them one at a time.
		for(int i=1; argv[i] != NULL; i++){
			struct stat fileinfo;
			// 4b. Prompt user if the current file is write protected
			if(!stat(argv[i], &fileinfo)){
				if(!(fileinfo.st_mode & S_IWUSR)){
					printf("override ");
					printf( (fileinfo.st_mode & S_IRUSR) ? "r" : "-");
					printf( (fileinfo.st_mode & S_IWUSR) ? "w" : "-");
					printf( (fileinfo.st_mode & S_IXUSR) ? "x" : "-");
					printf( (fileinfo.st_mode & S_IRGRP) ? "r" : "-");
					printf( (fileinfo.st_mode & S_IWGRP) ? "w" : "-");
					printf( (fileinfo.st_mode & S_IXGRP) ? "x" : "-");
					printf( (fileinfo.st_mode & S_IROTH) ? "r" : "-");
					printf( (fileinfo.st_mode & S_IWOTH) ? "w" : "-");
					printf( (fileinfo.st_mode & S_IXOTH) ? "x" : "-");
					printf(" for %s? ", argv[i]);

					char choice[20];
					scanf("%s", choice);
					if(choice[0] == 'y' || choice[0] == 'Y'){
						if(unlink(argv[i]) != 0){perror(argv[i]);}
					}// if
				}
				else{ // 4c. Automatically delete all other files, except for directories.
					if( unlink(argv[i]) != 0){perror(argv[i]);}
				}// if
			}
			else{
				perror(argv[i]);
			}// if
		}// for
	}// if
	return EXIT_SUCCESS;
}// main
