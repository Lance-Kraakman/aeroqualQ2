/*
 * aeroqual.h
 *
 *  Created on: 26/10/2021
 *      Author: lance
 */

#ifndef AEROQUAL_H_
#define AEROQUAL_H_

#define RED "\033[31m"/* Red */

// Include Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "libs/countedString/countedString.h"

int getFileLength(FILE *filePointer);
void checkError(FILE *filePtr, const char *filePointerName);
void makeStringLowercase(char *stringPointer);
int createStringArray(char **stringArray, char *inputFileBuffer, const char* delims);
void freeStringArray(char **stringArray);

#endif /* AEROQUAL_H_ */
