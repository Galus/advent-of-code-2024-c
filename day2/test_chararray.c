#include <stdio.h>
#include <string.h>

int main(void) {
    char *ptr = "hello";
    ptr++;
    printf("after ptr++: %s\n", ptr);

    char arr[strlen(ptr)];
    strcpy(arr, ptr);
    printf("after arr++: %s\n", arr);

    char ar2[999999];
    strcpy(ar2, ptr);
    printf("after ar2++: %s\n", ar2);

    char ar3[999999];
    memcpy(ar3, ptr, 99);
    printf("after ar3++: %s\n", ar3);
    return 0;
}
