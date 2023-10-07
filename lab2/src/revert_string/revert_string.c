#include "revert_string.h"
#include <stdlib.h>
#include <stdio.h>

void RevertString(char *str)
{
	int i = 0;
	while(str[i] != '\0') {
		++i;
	}
	int len = i - 1;
	char* result = malloc(sizeof(char) * len);
	i = 0;
	while(str[i] != '\0') {
		result[len - i] = str[i];
		++i;
	}
	result[i] = '\0';
	i = 0;
	while(result[i] != '\0') {
		str[i] = result[i];
		++i;
	}
	free(result);
}

