#include "util/grid.h"
#include "util/point.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int traverse_internal(Grid *grid, Point start, char target) {
  int stack_max_items = 10 * grid->rows * grid->cols;
  size_t stack_size = stack_max_items * sizeof(Point);
  Point *stack = malloc(stack_size);
  if (stack == NULL)
    return 0;

  bool *visited = calloc(grid->rows * grid->cols, sizeof(bool));
  if (visited == NULL) {
    free(stack);
    return 0;
  }

  // populate first item on stack
  int stack_top = 0;
  stack[stack_top++] = start;

  // look for targets
  int found_target = 0;
  while (stack_top > 0) {
    Point current_point = stack[--stack_top];
    int curx = current_point.x;
    int cury = current_point.y;
    char cur = grid->data[cury][curx];
    printf("Processing (%d,%d): %c\n", curx, cury, cur);

    if (cur == '9') {
      found_target++;
      continue;
    }

    for (int d = 0; d < DIRECTION_COUNT; d++) {
      Point next = {curx + directions[d][0], cury + directions[d][1]};

      if (!isInBounds(grid, next.y, next.x)) {
        /*printf("Point is outside of the grid!\n");*/
        continue;
      }

      char next_char = grid->data[next.y][next.x];
      if (next_char != cur + 1) {
        continue;
      }
      stack[stack_top++] = next;
    }

    if (stack_top >= stack_max_items) {
      printf("stack overflow avoided.\n");
      break;
    }
  }
  free(stack);

  for (int y = 0; y < grid->rows; y++) {
    for (int x = 0; x < grid->cols; x++) {
      if (x == 0) {
        printf("\n");
      }
      if (visited[y * grid->cols + x]) {
        printf("X");
      } else {
        printf(".");
      }
    }
  }
  printf("\n");
  return found_target;
}

int traverse(Grid *grid, Point start, char target) {
  Grid *copiedGrid = copyGrid(grid);
  if (copiedGrid == NULL) {
    printf("Failed to copy grid.\n");
    return 0;
  }

  bool error = false;
  char *current_char_ptr =
      getGridCharValue(copiedGrid, start.y, start.x, &error);
  if (error || current_char_ptr == NULL) {
    printf("Start point is out of bounds.\n");
    return 0;
  }

  int paths = traverse_internal(copiedGrid, start, target);
  printf("paths: %d\n", paths);
  freeGrid(copiedGrid);
  return paths;
}

int main(int argc, char *argv[]) {
  int part2 = 0;

  Grid *grid = createGridFromFile("input.txt");
  printCharGrid(grid);
  printGridDimensions(grid);

  PointArray trailheads = createPointArray(1000);
  int trailheadcount = 0;
  int trailheadscore[1000];

  for (int y = 0; y < grid->rows; y++) {
    for (int x = 0; x < grid->cols; x++) {
      if (grid->data[y][x] == '0') {
        addPoint(&trailheads, x, y);
      }
    }
  }

  printf("trailheads: ");
  printPointArray(trailheads);

  // for each trailhead
  for (int i = 0; i < trailheads.length; i++) {
    Point trailhead = trailheads.data[i];
    printf("\n\nTraversing trailhead %d at (%d,%d)\n", i, trailhead.x,
           trailhead.y);
    part2 += traverse(grid, trailhead, '9');
    // traverse
  }
  // calculate trailhead score

  printf("part2: %d\n", part2);
  return EXIT_SUCCESS;
}
