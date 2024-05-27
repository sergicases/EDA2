#include "menu_utils.h"

static void print_line(const char* text)
{
	printf(text);
	size_t len = strlen(text);
	if (len < 1 || text[len - 1] != '\n')
		putchar('\n');
}

char* read_string_input(const char* message, char* string_out, size_t max_string_len)
{
	print_line(message);
	fgets(string_out, (int)max_string_len, stdin);
	
	size_t len = strlen(string_out);
	if(len > 0 && string_out[len - 1] == '\n')
		string_out[len - 1] = '\0';
	return string_out;
}

int read_int_input(const char* message)
{
	char buffer[64];
	int value = 0;
	while (true)
	{
		print_line(message);
		if (!fgets(buffer, 64, stdin))
			return -1;

		int sscanf_result = sscanf(buffer, "%d", &value);
		if (sscanf_result == 1)
			break;
	}
	return value;
}
