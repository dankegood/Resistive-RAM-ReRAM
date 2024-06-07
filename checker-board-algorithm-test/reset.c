#include "header.h"

void reset(char ptr[], int size)
{
	for (int i = 0; i < size; i++)
		ptr[i] = 0;
}