#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool startsWith(const char *token, const char *prefix) {
    return strncmp(token, prefix, strlen(prefix)) == 0;
}

int main(int argc, char *argv[]) {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening input.txt");
        return EXIT_FAILURE;
    }

    char line[10000];
    unsigned long sum = 0;
    int lineCount = 0;
    bool enabled = true;

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("On line %d: %s", lineCount++, line);

        char *token = line;
        while (*token != '\0') {
            if (*token == 'd') {
                if (startsWith(token, "don't()")) {
                    enabled = false;
                } else if (startsWith(token, "do()")) {
                    enabled = true;
                }
            } else if (*token == 'm') {
                if (startsWith(token, "mul(")) {
                    int a, b;
                    char closeBrace;
                    if (sscanf(token, "mul(%d,%d%c", &a, &b, &closeBrace) == 3 && closeBrace == ')') {
                        if (enabled) {
                            long product = (long)a * (long)b;
                            sum += product;
                            printf("found int a: %d, b: %d, sum: %lu\n", a, b, sum);
                        }
                    }
                }
            }
            token++;
        }
    }

    fclose(file);
    printf("part1: %lu\n", sum);
    return EXIT_SUCCESS;
}
