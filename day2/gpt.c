#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[999];
    int total_safe = 0;
    int line_num = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;
        int prev = 0;

        char *token = line;
        int curr;
        int direction = 0;

        while (sscanf(token, "%d", &curr) == 1) {
            if (prev != 0) { // Check if prev has been set (not the first number)
                int diff = prev - curr;

                if (diff == 0) {
                    printf("Line %d: Not safe (same numbers: %d, %d)\n", line_num, prev, curr);
                    goto next_line; // Use goto to break out of inner loop and go to the next line
                }

                if (abs(diff) > 3) {
                    printf("Line %d: Not safe (difference too large: %d, %d)\n", line_num, prev, curr);
                    goto next_line;
                }

                if (direction == 0) {
                    direction = diff/abs(diff);
                } else {
                    if (direction != diff/abs(diff)) {
                        printf("Line %d: Not safe (monotonicity broken: %d, %d)\n", line_num, prev, curr);
                        goto next_line;
                    }

                }
            }
            prev = curr;

            while (*token && !isspace(*token)) token++;
            while (*token && isspace(*token)) token++;
        }

        total_safe++;

        next_line:;
    }

    fclose(fp);
    printf("Total safe lines: %d\n", total_safe);
    return 0;
}
