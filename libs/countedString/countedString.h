/*
 * countedString.h
 *
 *  Created on: 3/11/2021
 *      Author: lance
 */

#ifndef LIBS_COUNTEDSTRING_COUNTEDSTRING_H_
#define LIBS_COUNTEDSTRING_COUNTEDSTRING_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

typedef struct {
	char *string;
	int count;
} countedString;

void createCountedStringArray(countedString **countedStringArray, char **stringArray);
void updateCountedString(countedString **ctdStr, char *str, int count);
void writeCountedString(countedString **countedStringArray, FILE *filePtr);
void freeCountedStringData(countedString **countedStringArray);

#endif /* LIBS_COUNTEDSTRING_COUNTEDSTRING_H_ */
