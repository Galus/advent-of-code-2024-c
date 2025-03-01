#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char MUL[4] = {'m', 'u', 'l', '\0'};

int main(int argc, char *argv[]) {

  FILE *f = fopen("input.txt", "r");
  char line[10000];
  unsigned long sum = 0;
  int count = 0;
  while (fgets(line, 1000, f) != NULL) {
    printf("On line %d: %s\n", count++, line);
    char *token = (char *)line;
    while (*token != '\0') {
      /*printf("%c ", *token);*/
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
      token++;
    }
  }

  printf("%s\n", line);
  printf("part1: %lu\n", sum);
  return EXIT_SUCCESS;
}
