#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/str.h"
#include "util/io.h"

#define MAX_COLS 2000
#define MAX_ROWS 2000

int main()
{
	size_t len = len_m("Save The World!");
	assert(len == 15);

	char *file_name = "input.txt";
	size_t max_file_size = MAX_COLS * MAX_ROWS;
	char **lines = malloc(sizeof(char*) * MAX_ROWS);
	int lines_rows=0;
	
	file_to_arr(file_name, max_file_size, lines, MAX_ROWS, MAX_COLS, &lines_rows);

	printf("after:\nlines_rows: %d\n", lines_rows);
	printf("%s\n", lines[0]);
	printf("%s\n", lines[1]);
	printf("-\\-\n");
	printf("%s\n", lines[lines_rows-2]);
	printf("%s\n", lines[lines_rows-1]);

	return EXIT_SUCCESS;
}
