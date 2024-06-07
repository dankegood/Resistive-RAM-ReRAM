#include "header.h"

void extract_number(char str[], int * tab_ptr, int * num_ptr, int num_ptr_size)
{
	num_ptr[0] = atoi(&str[0]);

	for (int i = 1; i < num_ptr_size; i++)
		num_ptr[i] = atoi(&str[tab_ptr[i - 1] + 1]);
}