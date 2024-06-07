#include "header.h"

void print_bit(FILE * fpW, int num)
{
	for (int mask = 0x80; mask; mask >>= 1)
	{
		if (num & mask)
			fputc('1', fpW);
		else
			fputc('0', fpW);
	}
}