#include "check_match.h"
#include "extract_number.h"
#include "header.h"
#include "introduction.h"
#include "print_bit.h"
#include "remove_new_line.h"
#include "reset.h"
#include "tab_count.h"
#include "tab_find.h"
#include "tab_location.h"
#include "tab_to_null.h"
#include "total_size.h"
#include "variable.h"

int main(void)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool total_result_sent, total_result_received;
	bool tab_result_sent, tab_result_received;
	int tab_count_sent, tab_count_received;
	int * tab_position_sent, * number_sent;
	int * tab_position_received, * number_received;
	bool sent_received_same;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	fp_sent_data = fopen("Sent Data.txt", "rt");
	fp_received_data = fopen("Received Data.txt", "rt");
	fpw = fopen("Checkerboard Algorithm Test.txt", "wt");
	
	if (fp_received_data == NULL || fp_sent_data == NULL || fpw == NULL)
	{
		puts("FILE OPEN FAIL!");
		return -1;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	introduction(fpw);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	total_result_sent = total_size(fp_sent_data, fpw, str_line_sent, "sent", sizeof(str_line_sent));
	total_result_received = total_size(fp_received_data, fpw, str_line_received, "received", sizeof(str_line_received));

	if (total_result_sent && total_result_received)
	{
		remove_new_line(str_line_sent);
		fputs(str_line_sent, fpw);
		fputs(" (Sent Data)", fpw);

		fputs("\t\t\t\t\t\t\t", fpw);

		remove_new_line(str_line_received);
		fputs(str_line_received, fpw);
		fputs(" (Received Data) \n", fpw);
	}

	else
		return -1;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	while (1)
	{
		reset(str_line_sent, sizeof(str_line_sent));

		if (fgets(str_line_sent, sizeof(str_line_sent), fp_sent_data) == NULL)
		{
			if (feof(fp_sent_data) != 0)
			{
				fputs("*** No sent data is found *** \n", fpw);
				break;
			}
				
			else
			{
				fputs("*** Error in the sent data *** \n", fpw);
				break;
			}
		}

		if (tab_find(str_line_sent, sizeof(str_line_sent)))
			break;
	}

	while (1)
	{
		reset(str_line_received, sizeof(str_line_received));

		if (fgets(str_line_received, sizeof(str_line_received), fp_received_data) == NULL)
		{
			if (feof(fp_received_data) != 0)
			{
				fputs("*** No received data is found *** \n", fpw);
				break;
			}
				
			else
			{
				fputs("*** Error in the received data *** \n", fpw);
				break;
			}
		}

		if (tab_find(str_line_received, sizeof(str_line_received)))
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	fputs("\n", fpw);
	fputs("<Sent Data Bits>", fpw);
	fputs("\t\t\t\t\t\t\t\t\t", fpw);
	fputs("<Received Data Bits> \n", fpw);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("In progress... \n");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	while (1)
	{
		tab_result_sent = tab_find(str_line_sent, sizeof(str_line_sent));
		tab_result_received = tab_find(str_line_received, sizeof(str_line_received));

		if (!tab_result_sent && !tab_result_received)
		{
			fputc('\n', fpw);
			fputs("*** Checkerboard Algorithm Memory Test is done *** \n", fpw);
			break;
		}
		
		else if (!tab_result_sent && tab_result_received)
		{
			fputc('\n', fpw);
			fputs("*** Number of sent data < Number of received data *** \n", fpw);
			fputs("*** Checkerboard Algorithm Memory Test is halted *** \n", fpw);
			break;
		}

		else if (tab_result_sent && !tab_result_received)
		{
			fputc('\n', fpw);
			fputs("*** Number of sent data > Number of received data *** \n", fpw);
			fputs("*** Checkerboard Algorithm Memory Test is halted *** \n", fpw);
			break;
		}

		remove_new_line(str_line_sent);
		remove_new_line(str_line_received);

		tab_count_sent = tab_count(str_line_sent, sizeof(str_line_sent));
		tab_count_received = tab_count(str_line_received, sizeof(str_line_received));

		////////////////////////////////////////////////////////////////////////////////////////////////////
		tab_position_sent = (int *)malloc(sizeof(int) * tab_count_sent);

		if(tab_count_sent == 7)
			number_sent = (int *)malloc(sizeof(int) * (tab_count_sent + 1));
		else
			number_sent = (int *)malloc(sizeof(int) * tab_count_sent);

		tab_location(str_line_sent, tab_position_sent, sizeof(str_line_sent), tab_count_sent);
		tab_to_null(str_line_sent, tab_position_sent, tab_count_sent);

		if(tab_count_sent == 7)
			extract_number(str_line_sent, tab_position_sent, number_sent, tab_count_sent + 1);
		else
			extract_number(str_line_sent, tab_position_sent, number_sent, tab_count_sent);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		tab_position_received = (int *)malloc(sizeof(int) * tab_count_received);

		if(tab_count_received == 7)
			number_received = (int *)malloc(sizeof(int) * (tab_count_received + 1));
		else
			number_received = (int *)malloc(sizeof(int) * tab_count_received);

		tab_location(str_line_received, tab_position_received, sizeof(str_line_received), tab_count_received);
		tab_to_null(str_line_received, tab_position_received, tab_count_received);

		if (tab_count_received == 7)
			extract_number(str_line_received, tab_position_received, number_received, tab_count_received + 1);
		else
			extract_number(str_line_received, tab_position_received, number_received, tab_count_received);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		if (tab_count_sent == 7)
		{
			if (tab_count_received == 7)
				sent_received_same = check_match(number_sent, number_received, tab_count_sent + 1, tab_count_received + 1);
			else
				sent_received_same = check_match(number_sent, number_received, tab_count_sent + 1, tab_count_received);
		}

		else
		{
			if (tab_count_received == 7)
				sent_received_same = check_match(number_sent, number_received, tab_count_sent, tab_count_received + 1);
			else
				sent_received_same = check_match(number_sent, number_received, tab_count_sent, tab_count_received);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		if (tab_count_sent == 7)
		{
			for (int i = 0; i < tab_count_sent + 1; i++)
				print_bit(fpw, number_sent[i]);

			if (sent_received_same)
				fputs(" 1", fpw);
			else
				fputs(" 0", fpw);
		}

		else
		{
			for (int i = 0; i < tab_count_sent; i++)
				print_bit(fpw, number_sent[i]);

			for (int j = 0; j < 8 - tab_count_sent; j++)
				fputs("        ", fpw);

			if (sent_received_same)
				fputs(" 1", fpw);
			else
				fputs(" 0", fpw);
		}
		fputs("\t\t\t", fpw);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		if (tab_count_received == 7)
		{
			for (int i = 0; i < tab_count_received + 1; i++)
				print_bit(fpw, number_received[i]);

			if (sent_received_same)
				fputs(" 1", fpw);
			else
				fputs(" 0", fpw);
		}

		else
		{
			for (int i = 0; i < tab_count_received; i++)
				print_bit(fpw, number_received[i]);

			for (int j = 0; j < 8 - tab_count_received; j++)
				fputs("        ", fpw);

			if (sent_received_same)
				fputs(" 1", fpw);
			else
				fputs(" 0", fpw);
		}
		fputc('\n', fpw);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		reset(str_line_sent, sizeof(str_line_sent));
		reset(str_line_received, sizeof(str_line_received));
		free(tab_position_sent), free(number_sent);
		free(tab_position_received), free(number_received);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////
		fgets(str_line_sent, sizeof(str_line_sent), fp_sent_data);
		fgets(str_line_received, sizeof(str_line_received), fp_received_data);
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	reset(str_line_sent, sizeof(str_line_sent));
	reset(str_line_received, sizeof(str_line_received));
	fclose(fp_sent_data), fclose(fp_received_data), fclose(fpw);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Finish Testing! \n");
	return 0;
}