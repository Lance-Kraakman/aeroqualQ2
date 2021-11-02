/*
 * aeroqual.c
 *
 *  Created on: 26/10/2021
 *      Author: lance
 */
#include "aeroqual.h"

#define MAX_NUM_WORDS 1024 // maximum number of words in input file
#define REQUIRED_DELIMS "\n ,-!.?-:;"

#define REL_PATH "/home/lance/eclipse-workspace/aeroqualQ2"
#define INPUT_FILE REL_PATH "/infile.txt"
#define OUTPUT_FILE REL_PATH "/outfile.txt"

int main(int argc, char **argv) {

	if (strlen(REL_PATH)<1) {
		printf("PLEASE SET REL_PATH TO PATH OF YOUR DESIRED FILE LOCATION");
		return 0;
	}

	chdir(REL_PATH); 		// change to the correct relative path
	setbuf(stdout, NULL); 	// disable buffering in stdout

	// Open Input File
	FILE *infilePointer = NULL;
	infilePointer = fopen(INPUT_FILE, "r+");
	checkError(infilePointer, "infilePointer");

	// Open Output File
	FILE *outfilePointer;
	outfilePointer = fopen(OUTPUT_FILE, "w+");
	checkError(outfilePointer, "outfilePointer");

	// allocate memory for a string of the input file
	int fileLength = getFileLength(infilePointer);
	char *inputFileBuffer = malloc(sizeof(char)*(fileLength+1));

	// create an array of strings and an array of counted strings
	char **stringArray = malloc(sizeof(char)*MAX_NUM_WORDS);
	countedString **countedStringArray= malloc(sizeof(countedString)*MAX_NUM_WORDS);

	// Read the input file into our buffer and make all chars lowercase
	fread(inputFileBuffer, fileLength+1, 1, infilePointer);
	makeStringLowercase(inputFileBuffer);

	//create string array
	createStringArray(stringArray, inputFileBuffer, REQUIRED_DELIMS);

	// creates a null terminated array of counted strings from a string array
	createCountedStringArray(countedStringArray, stringArray);

	// write counted string array to file
	writeCountedString(countedStringArray, outfilePointer);

	// Close our FILEs
	fclose(outfilePointer);
	fclose(infilePointer);

	// After we are done using all of our data we need to free memory
	freeCountedStringData(countedStringArray);
	free(countedStringArray);
	freeStringArray(stringArray);
	free(stringArray);
	free(inputFileBuffer);

	return 0;
}

//writes the counted strings to a file
void writeCountedString(countedString **countedStringArray, FILE *filePtr) {
	int i=0;
	while((*(countedStringArray+i))) {
		fprintf(filePtr, "%d : %s\n", (*(countedStringArray+i))->count, (*(countedStringArray+i))->string);
		i++;
	}
}

/*
 * Frees the data in the counted string array but not the counted string array itself
 */
void freeCountedStringData(countedString **countedStringArray) {
	while((*(countedStringArray++))) {
		free((*(countedStringArray))->string);
	}
}

/**
 * Frees all of the strings in the string array
 */
void freeStringArray(char **stringArray) {
	while((*(stringArray++))) {
		free(stringArray);
	}
}

/**
 * Function loops through all of the strings in the stringArray.
 * Updates a countedStringArray with all of the unuique words and there occurance count
 */
void createCountedStringArray(countedString **countedStringArray, char **stringArray) {
	char escapeByte = 0x1B;
	int count;
	// loop through every string in the string array
	int i = 0; int countedStringCount = 0;
	while(*(stringArray+i)) {
		count=1;
		// If the first char of the string is not equal to an escape byte -> uncounted string is found.
		if ((char) **(stringArray+i) != escapeByte) {
			// Loop through all of the next strings in the string array
			int j=i+1;
			while (*(stringArray+j) != NULL) {
				// Count how many times the string occurs in the array, setting the fist byte of the new string
				// to indicate the string has already been counted
				if (!(strcmp((char *) *(stringArray+i), (char *) (*(stringArray+j))))) {
					count++;
					stringArray[j] = &escapeByte;
				}
				j++;
			}
			updateCountedString(&countedStringArray[countedStringCount], *(stringArray+i), count);
			countedStringCount++;
		}
		i++;
	}
	countedStringArray[countedStringCount] = NULL;
}

/*
 *	Creates string array from a string buffer.
 *	Splits the string up from the defined REQUIRED_DELIMS
 *	Returns # of strings appended
 */
int createStringArray(char **stringArray, char *inputFileBuffer, const char* delims) {
	char *tokenPtr;
	char *token = strtok_r(inputFileBuffer, REQUIRED_DELIMS, &tokenPtr);
	int i=0;
	while(token != NULL) {
		*(stringArray+i) = malloc(strlen(token));
		memccpy(*(stringArray+i), token,sizeof(char), strlen(token));
		token = strtok_r(NULL, REQUIRED_DELIMS, &tokenPtr);
		i++;
	}
	*(stringArray+i) = NULL;
	return i;
}


/*
 * Get the file length of the file
 */
int getFileLength(FILE *filePointer) {
	if (filePointer) {
		fseek(filePointer, 0, SEEK_END);
		long length = ftell(filePointer);
		fseek(filePointer, 0, SEEK_SET);
		return length;
	}
	return 0;
}

/**
 * Function prints the error if the file pointer is invalid
 */
void checkError(FILE *filePtr, const char *filePointerName) {
	if (!filePtr) {
		perror(filePointerName);
		printf("\nPLEASE CHECK REL PATH\n");
	}
}

/*
 * Converts every char in a buffer to lower case
 */
void makeStringLowercase(char *stringPointer) {
	while(*stringPointer) {
		*stringPointer = tolower(*stringPointer);
		stringPointer++; // increment pointer
	}
}

/**
 * Allocates memory and updates counted string
 * Memory should be free'd manually when done with the data.
 */
void updateCountedString(countedString **ctdStr, char *str, int count) {
	(*ctdStr) = malloc(sizeof(countedString));
	(*ctdStr)->count = count;
	(*ctdStr)->string = malloc(sizeof(char)*strlen(str));
	(*ctdStr)->string = str;
}



