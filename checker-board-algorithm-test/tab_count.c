#include "header.h"

int tab_count(char * ptr, int size)
{
	int count = 0;

	for (int i = 0; i < size; i++)
	{
		if (ptr[i] == '\t')
			count += 1;
	}

	return count;
}