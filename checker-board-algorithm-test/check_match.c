#include "header.h"

bool check_match(int ptr_sent[], int * ptr_received, int sent_size, int received_size)
{
	for (int i = 0, j = 0; i < sent_size || j < received_size; i++, j++)
	{
		if (ptr_sent[i] != ptr_received[j])
			return false;
	}
	return true;
}