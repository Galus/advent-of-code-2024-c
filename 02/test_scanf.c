#include <stdio.h>

int main() {
    char str[] = "  123 abc 456";
    int num;
    int chars_read;
    char *ptr = str;

    int result = sscanf(ptr, "%d%n", &num, &chars_read);

    if (result == 1) {
        printf("Number: %d\n", num);
        printf("Characters read: %d\n", chars_read);
        printf("Pointer after sscanf: %s\n", ptr); // Pointer is advanced
        ptr += chars_read; // Manually advance the pointer
        printf("Pointer after manual advance: %s\n", ptr);
    }

    return 0;
}
