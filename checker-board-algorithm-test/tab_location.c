#include "header.h"

void tab_location(char * str, int ptr[], int str_size, int ptr_size)
{
	int count = 0;

	for (int i = 0; i < str_size && count < ptr_size; i++)
	{
		if (str[i] == '\t')
		{
			ptr[count] = i;
			count++;
		}	
	}
}