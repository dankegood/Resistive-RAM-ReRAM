#include "header.h"

void tab_to_null(char str[], int * ptr, int ptr_size)
{
	for (int i = 0; i < ptr_size; i++)
		str[ptr[i]] = NULL;
}