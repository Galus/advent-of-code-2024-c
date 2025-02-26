#include "util/array.h"
#include "util/io.h"
#include <stdio.h>
#include <stdlib.h>

#define DIFF_MIN 0
#define DIFF_MAX 3

int main(int argc, char *argv[]) {

  char *file_name = "input.txt";
  size_t max_file_size = 2000;
  size_t max_rows = 2000;
  size_t max_cols = 2000;
  int lines_rows;

  char **lines =
      file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);

  int part1 = 0;
  int total_safe = 0;
  int line_num = 0;

  for (int i = 0; i < lines_rows; i++) {
    char *line = lines[i];
    if (line == NULL) {
      printf("line is null\n");
    }

    Array ints = createArray(1050, sizeof(int));

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

    int dampener_used = 0;
    int direction = 0;
    int prev = 0;

    for (int j = 0; j < ints.length - 1; j++) {
      int curr = ((int *)ints.data)[j];
      int next = ((int *)ints.data)[(j + 1)];
      int diff = curr - next;
      if (diff != 0) {
        int dir = diff / abs(diff);
      }

      // check diff
      if (abs(diff) > DIFF_MAX || abs(diff) == 0) {
        printf("Line %d UNSAFE diff bad (%d,%d)\n", i, curr, next);
        dampener_used++;
        break;
      } else if (direction == 0) {
        // check dir
        direction = diff / abs(diff);
      } else if (direction != diff / abs(diff)) {
        printf("Line %d UNSAFE direciton change (%d,%d)\n", i, curr, next);
        dampener_used++;
        break;
      }
    }
    if (dampener_used == 0) {
      part1++;
    }
  }
  printf("Part1: %d", part1);
  return 0;
}
