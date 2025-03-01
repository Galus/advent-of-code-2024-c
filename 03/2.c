#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char MUL[4] = {'m', 'u', 'l', '\0'};
char DO[5] = {'d', 'o', '(', ')', '\0'};
char DONT[8] = {'d', 'o', 'n', '\'', 't', '(', ')', '\0'};

int main(int argc, char *argv[]) {

  FILE *f = fopen("input.txt", "r");
  char line[10000];
  unsigned long sum = 0;
  int count = 0;
  int enable = true;
  while (fgets(line, 10000, f) != NULL) {
    printf("On line %d: %s\n", count++, line);
    char *token = (char *)line;
    while (*token != '\0') {
      /*printf("%c ", *token);*/

      if (*token == 'd') {
        char next7[8];
        strncpy(next7, token, 7);
        next7[7] = '\0';
        if (strcmp(next7, DONT) == 0) {
          enable = false;
        }
        char next4[5];
        strncpy(next4, token, 4);
        next4[4] = '\0';
        if (strcmp(next4, DO) == 0) {
          enable = true;
        }
      }

      if (*token == 'm') {
        char next3[4];
        strncpy(next3, token, 3);
        next3[3] = '\0';

        char next20[21];
        strncpy(next20, token, 20);
        next20[20] = '\0';
        /*printf("\nnext3: [%s]\n", next3);*/
        /*printf("MUL: [%s]\n", MUL);*/
        /*printf("strcmp: %d\n", strcmp(next3, MUL));*/
        if (strcmp(next3, MUL) == 0) {
          int a, b;
          char close_brace;
          if (sscanf(token, "mul(%d,%d%c", &a, &b, &close_brace) == 3) {
            if (close_brace == ')') {
              if (enable) {
                printf("found int a: %d , b: %d at %p: %s\n", a, b, token,
                       next20);
                long product = a * b;
                printf("product: %ld\n", product);
                sum += product;
                printf("sum: %lu\n", sum);
              }
            }
          }
        }
      }
      token++;
    }
  }

  printf("%s\n", line);
  printf("part1: %lu\n", sum);
  return EXIT_SUCCESS;
}
