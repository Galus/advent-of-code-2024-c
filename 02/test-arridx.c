#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char arr[] = "ab21cd";

    arr[-1] = 'z';
    arr[3] = 'q';

    printf("[%s]", arr);

    return EXIT_SUCCESS;
}
