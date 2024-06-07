#include "header.h"

bool tab_find(char * ptr, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (ptr[i] == '\t')
			return true;
	}
	return false;
}