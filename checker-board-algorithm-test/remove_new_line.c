#include "header.h"

void remove_new_line(char ptr[])
{
	int length = strlen(ptr);
	ptr[length - 1] = '\0';
}