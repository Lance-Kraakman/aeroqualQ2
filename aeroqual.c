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

int countTokens(char *inputFileBuffer, const char* delims);

/**
 * Setup Function reads the infile.txt into a buffer and makes any chars lowercase.
 * Memory is allocated into this buffer.
 * Opens the output file.
 *
 */
void setup(FILE **infilePp, FILE **outfilePp, char **fileBufferPtr) {

	chdir(REL_PATH); 		// change to the correct relative path
	setbuf(stdout, NULL); 	// disable buffering in stdout

	// Open Input File
	*infilePp = fopen(INPUT_FILE, "r+");
	checkError(*infilePp, "infilePp");

	// Open Output File
	*outfilePp = fopen(OUTPUT_FILE, "w+");
	checkError(*outfilePp, "outfilePp");

	// allocate memory for a string of the input file
	int fileLength = getFileLength(*infilePp);
	*fileBufferPtr = malloc(sizeof(char)*(fileLength+1));

	// Read the input file into our buffer and make all chars lowercase
	fread(*fileBufferPtr, fileLength+1, 1, *infilePp);
	makeStringLowercase(*fileBufferPtr);

}

int main(int argc, char **argv) {

	if (strlen(REL_PATH)<1) {
		printf("PLEASE SET REL_PATH TO PATH OF YOUR DESIRED FILE LOCATION for infile.txt and outfile.txt");
		return 0;
	}

	char *infileBuffer; FILE *infilePointer=NULL; FILE *outfilePointer=NULL;

	// setup our FILE ptrs and get our lowercase input string buffer
	setup(&infilePointer, &outfilePointer, &infileBuffer); // setups stuff

	// calculate the size our buffers need to be
	int wordArraySize = countTokens(infileBuffer, REQUIRED_DELIMS) + 1;

	// Allocate memory for an array of strings and an array of counted strings
	char *stringArray[wordArraySize];
	countedString *countedStringArray[wordArraySize];

	//create string array
	createStringArray(stringArray, infileBuffer, REQUIRED_DELIMS);

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
	free(infileBuffer);

	return 0;
}

/**
 * Frees all of the strings in the string array
 */
void freeStringArray(char **stringArray) {
	while((*(stringArray++))) {
		free(stringArray);
	}
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
 * Conuts the number of tokens that will be generated from the delims
 */
int countTokens(char *inputFileBuffer, const char* delims) {
	// we need to copy the *inputFileBuffer because we cannot use strtok_r twice on the same buffer
	int len = strlen(inputFileBuffer);
	char *bufferCpy = malloc(len*sizeof(char));
	strcpy(bufferCpy, inputFileBuffer);
	char *tokenPtr;
	char *token = strtok_r(bufferCpy, REQUIRED_DELIMS, &tokenPtr);
	int i=0;
	while(token != NULL) {
		token = strtok_r(NULL, REQUIRED_DELIMS, &tokenPtr);
		i++;
	}
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





