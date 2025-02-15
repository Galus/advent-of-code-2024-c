#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[]) {
    FILE *f;
    int line_num = 0;
    char **lines;

    // 1. Allocate space for the array of pointers:
    lines = malloc(sizeof(char *) * MAX_LINES); // Corrected size
    if (lines == NULL) {
        perror("malloc failed");
        return 1;
    }

    f = fopen("input.txt", "r");
    if (f == NULL) {
        perror("fopen failed");
        free(lines); // Free allocated memory before exiting
        return 1;
    }

    while (fgets(lines[line_num], MAX_LINE_LENGTH, f) != NULL && line_num < MAX_LINES) {
        // 2. Allocate memory for each individual line:
        size_t len = strlen(lines[line_num]);
        lines[line_num] = realloc(lines[line_num], sizeof(char) * (len + 1)); // +1 for '\0'
        if (lines[line_num] == NULL) {
            perror("realloc failed");
            // Handle error (free previously allocated memory, close file, etc.)
            for (int i = 0; i < line_num; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(f);
            return 1;
        }

        // Remove newline if present:
        if (len > 0 && lines[line_num][len - 1] == '\n') {
            lines[line_num][len - 1] = '\0';
        }

        line_num++;
    }

    fclose(f);

    // Print lines (with bounds checking):
    for (int i = 5; i <= 8; i++) {  // Loop for lines 5 to 8 (inclusive)
        if (i < line_num) { // Check if the line exists
            printf("Printing line %d: %s\n", i, lines[i]);
        } else {
            printf("Line %d not found in file.\n", i); // Handle missing lines.
        }
    }

    // 3. Free allocated memory (crucial!)
    for (int i = 0; i < line_num; i++) {
        free(lines[i]);
    }
    free(lines);

    return EXIT_SUCCESS;
}
