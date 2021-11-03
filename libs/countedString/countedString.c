/*
 * countedString.c
 *
 *  Created on: 3/11/2021
 *      Author: lance
 */


#include "countedString.h"

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
	countedStringArray[countedStringCount] = NULL; // Null Terminate the counted string array
}
