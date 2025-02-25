#include "util/io.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100000

typedef struct {
  unsigned long long total;
  int index;
  char expression[999];
} State;

char exprec[999];
char *calculate(unsigned long long goal, unsigned long long total, int *right,
                int rightCount, int rightIdx, char *exprec) {
  char *copy = strdup(exprec);
  if (rightIdx == rightCount) {
    if (goal == total) {
      return strdup(copy);
    } else {
      return NULL;
    }
  }

  char *result = NULL;
  char *sumExp = NULL;
  char *mulExp = NULL;

  char sumBuff[999];
  if (rightIdx == 0) {
    sprintf(sumBuff, "%d", right[rightIdx]);
  } else {
    sprintf(sumBuff, "%s + %d", exprec, right[rightIdx]);
  }
  sumExp = calculate(goal, total + right[rightIdx], right, rightCount,
                     rightIdx + 1, sumBuff);
  if (sumExp != NULL) {
    result = sumExp;
  }

  char mulBuff[999];
  if (rightIdx == 0) {
    sprintf(mulBuff, "%d", right[rightIdx]);
  } else {
    sprintf(mulBuff, "%s * %d", exprec, right[rightIdx]);
  }
  mulExp = calculate(goal, total * right[rightIdx], right, rightCount,
                     rightIdx + 1, mulBuff);

  if (mulExp != NULL) {
    if (result != NULL) {
      free(result);
    }
    result = mulExp;
  }
  return result;
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

    char *exp = calculate(left, 1, right, rightCount, 0, " ");
    if (exp != NULL) {
      printf("expression: %s\n", exp);
      total += left;
      free(exp);
    } else {
      printf("expression: No solution found\n");
    }

    /*char *expIt = calculateIterative(left, right, rightCount);*/
    /*if (expIt != NULL) {*/
    /*  printf("expression it: %s\n", expIt);*/
    /*  free(expIt);*/
    /*} else {*/
    /*  printf("expression it: No Solution found\n");*/
    /*}*/
  }
  printf("part1: %llu\n", total);

  return EXIT_SUCCESS;
}

char *calculateIterative(int goal, int *right, int rightCount) {
  State stack[MAX_STATES];
  int stackTop = 0;

  // Initial state
  stack[stackTop++] = (State){1, 0, ""}; // Start with total 1

  while (stackTop > 0) {
    State currentState = stack[--stackTop];
    int total = currentState.total;
    int index = currentState.index;
    char expression[999];
    strcpy(expression, currentState.expression);

    if (index == rightCount) {
      if (total == goal) {
        return strdup(expression); // Found a solution
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
  }

  return NULL; // No solution found
}
