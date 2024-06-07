#include "header.h"
#include "tab_find.h"
#include "reset.h"

bool total_size(FILE * fpR, FILE * fpW, char * ptr, char * string, int size)
{
	bool result;

	while (1)
	{
		if (fgets(ptr, size, fpR) == NULL)
		{
			if (feof(fpR) != 0)
			{
				fputs("No total number of ", fpW);
				fputs(string, fpW);
				fputs(" byte is found! \n", fpW);
				break;
			}

			else
			{
				fputs("Error in accessing the ", fpW);
				fputs(string, fpW);
				fputs(" data file! \n", fpW);
				break;
			}
		}

		result = strncmp(ptr, "Total number of byte", 20);

		if (!result)
			return true;

		if (tab_find(ptr, strlen(ptr)))
		{
			fputs("No total number of ", fpW);
			fputs(string, fpW);
			fputs(" byte is found! \n", fpW);
			break;
		}

		reset(ptr, size);
	}
	return false;
}