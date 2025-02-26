#include "util/io.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 1000

typedef struct {
  unsigned long long total;
  int index;
  char expression[999];
} State;

char exprec[999];

char *calculateIterative(int goal, int *right, int rightCount) {
  /*printf("declare stack[%d]\n", MAX_STATES);*/
  State stack[MAX_STATES];
  int stackTop = 0;
  /*printf("stackTop = %d\n", stackTop);*/

  // Initial state
  if(stackTop >= MAX_STATES) {
    fprintf(stderr, "Overflow!\n");
    return NULL;
  }
  stack[stackTop++] = (State){1, 0, ""}; // total, index, expression
  /*printf("Before while\n");*/

  while (stackTop > 0) {
    if (stackTop == MAX_STATES) {
      printf("Max stack!\n");
    }
    /*printf("stackTop > 0 : %d\n", stackTop);*/
    State currentState = stack[--stackTop];
    /*printf("currentState.total: %lu\n", currentState.total);*/
    /*printf("currentState.index: %d\n", currentState.index);*/
    /*printf("currentState.expression: %s\n", currentState.expression);*/
    unsigned long long total = currentState.total;
    int index = currentState.index;
    char expression[999];
    strcpy(expression, currentState.expression);

    if (index == rightCount) {
      if (total == goal) {
        char *dup = strdup(expression); // Found a solution
        if (dup == NULL) {
          printf("dup is null!\n");
        }
        return dup;
      }
      continue; // Go to the next state
    }

    int currentNum = right[index];

    // Try addition
    char sumBuffer[999];
    if (index == 0) {
      sprintf(sumBuffer, "%d", currentNum);
    } else {
      sprintf(sumBuffer, "%s + %d", expression, currentNum);
    }
    stack[stackTop++] = (State){total + currentNum, index + 1, ""};
    strcpy(stack[stackTop - 1].expression, sumBuffer);

    // Try multiplication
    char mulBuffer[999];
    if (index == 0) {
      sprintf(mulBuffer, "%d", currentNum);
    } else {
      sprintf(mulBuffer, "%s * %d", expression, currentNum);
    }
    stack[stackTop++] = (State){total * currentNum, index + 1, ""};
    strcpy(stack[stackTop - 1].expression, mulBuffer);

    char catBuffer[999];
    if (index == 0) {
      sprintf(catBuffer, "%d", currentNum);
    } else {
      sprintf(catBuffer, "%s || %d", expression, currentNum);
    }
    char catNum[30];
    sprintf(catNum, "%llu%d", total, currentNum);
    char *endptr;
    errno = 0;
    unsigned long long cat = strtoull(catNum, &endptr, 10);
    stack[stackTop++] = (State){cat, index + 1, ""};
    strcpy(stack[stackTop - 1].expression, catBuffer);
  }

  return NULL; // No solution found
}

int main() {

  char *file_name = "input.txt";
  size_t max_file_size = 1700 * 100;
  size_t max_rows = 1000;
  size_t max_cols = 200;
  int lines_rows;
  char **lines;
  lines =
      file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);

  unsigned long long total = 0;
  for (int i = 0; i < lines_rows; i++) {
    unsigned long long left;
    int right[99];
    int rightCount = 0;
    char *token;
    char *rest = lines[i];

    token = strtok(rest, ":");
    char *endptr;
    errno = 0;
    left = strtoull(token, &endptr, 10);
    printf("%llu: ", left);

    token = strtok(NULL, " ");
    while (token != NULL && rightCount < max_cols) {
      right[rightCount++] = atoi(token);
      token = strtok(NULL, " ");
    }

    for (int j = 0; j < rightCount; j++) {
      printf("%d", right[j]);
      if (j < rightCount - 1) {
        printf(",");
      }
    }
    printf("\n");

    char *exp = calculateIterative(left, right, rightCount);
    if (exp != NULL) {
      printf("expression: %s\n", exp);
      total += left;
      free(exp);
    } else {
      printf("expression: No solution found\n");
    }

  }
  printf("part2 not working: %llu\n", total);

  return EXIT_SUCCESS;
}
