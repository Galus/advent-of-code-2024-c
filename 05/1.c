#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/str.h"

int main(int argc, char *argv[])
{
	size_t len = len_m("Save The World!");
	printf("%zu\n", len);
	return EXIT_SUCCESS;
}
