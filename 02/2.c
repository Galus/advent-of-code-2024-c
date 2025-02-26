#include "util/array.h"
#include "util/dbg.h"
#include "util/io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIFF_MIN 0
#define DIFF_MAX 3

bool isSafe(int *arr, int l) {
  int dir = 0;
  for (int i = 0; i < l; i++) {
    int dif = arr[i - 1] - arr[i];
    if (dif == 0) {
      return false;
    }
    if (dir == 0) {
      dir = dif / abs(dif);
    } else if (dir != dif / abs(dif)) {
      return false;
    }
    if (abs(dif) > DIFF_MAX) {
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {

  char *file_name = "input.txt";
  size_t max_file_size = 9000;
  size_t max_rows = 3000;
  size_t max_cols = 3000;
  int lines_rows;

  char **lines =
      file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);

  int part2 = 0;
  int line_num = 0;

  for (int i = 0; i < lines_rows; i++) {
    char *line = lines[i];
    if (line == NULL) {
      printf("line is null\n");
    }

    // create and populate ints array
    Array ints = createArray(max_cols, sizeof(int));
    int curr;
    char *copy = strdup(line);
    char *token = strtok(copy, " ");
    int count = 0;
    while (token != NULL) {
      count++;
      curr = atoi(token);
      append(&ints, &curr);
      token = strtok(NULL, " ");
    }
    /*printIntArray(ints);*/
    free(copy);

    // check if the line is safe
    if (isSafe(ints.data, ints.length)) {
      part2++;
      printf("Line %d SAFE %d: ", i, part2);
      printIntArray(ints);
      /*print_intarray(ints.data, ints.length, 0, 99);*/
    } else {
      // rebuild all possible arrays removing 1 at a time.
      for (int j = 0; j < ints.length; j++) {
        int *temp = calloc(ints.length - 1, sizeof(int));
        // [0:j)
        for (int h = 0; h < j; h++) {
          temp[h] = ((int *)ints.data)[h];
        }
        // (j:n-1]
        for (int h = j + 1; h < ints.length; h++) {
          temp[h - 1] = ((int *)ints.data)[h];
        }
        // check generated array
        if (isSafe(temp, ints.length - 1) == true) {
          printf("Line %d Gen SAFE %d: ", i, part2);
          print_intarray(temp, ints.length - 1, 0, 99);
          part2++;
          break;
        }
      }
    }
  }
  printf("Part2: %d", part2);
  return 0;
}
