#include "util/grid.h"
#include "util/point.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Point point;
  char last_char;
} StackItem;

int traverse_internal(Grid *grid, Point point, char target,
                      char last_point_char) {
  if (!isInBounds(grid, point.y, point.x)) {
    /*printf("Point is outside of the grid!\n");*/
    return 0;
  }
  char *current_char_ptr = getGridCharValue(grid, point.y, point.x, NULL);
  if (current_char_ptr == NULL) {
    /*printf("current char ptr is NULL\n");*/
    return 0;
  }
  char current_char = *current_char_ptr;
  if (last_point_char != '0') {
    if (current_char - last_point_char != 1) {
      /*printf("Too large of a step.\n");*/
      return 0;
    }
    if (current_char == target) {
      /*printf("Found target at (%d,%d)\n", point.x, point.y);*/
      grid->data[point.y][point.x] = 'X'; // prevent seeing this spot again
      return 1;
    }
  }


  printf("at (%d,%d)\n", point.x, point.y);
  printf("last: %c\n", last_point_char);
  printf("curr: %c\n", current_char);
  if (current_char - last_point_char > 1) {
    printf("Too large of a step.\n");
    return 0;
  }
  int found_target = 0;

  // go right
  Point right = point;
  right.x++;
  found_target += traverse_internal(grid, right, target, current_char);

  // go down
  Point down = point;
  down.y++;
  found_target += traverse_internal(grid, down, target, current_char);

  // go left
  Point left = point;
  left.x--;
  found_target += traverse_internal(grid, left, target, current_char);

  // go up
  Point up = point;
  up.y--;
  found_target += traverse_internal(grid, up, target, current_char);
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
  /*printf("current_char %c\n", *current_char_ptr);*/
  int paths = traverse_internal(copiedGrid, start, target, '0');
  printf("paths: %d\n", paths);
  freeGrid(copiedGrid);
  return paths;
}

int main(int argc, char *argv[]) {
  int part1 = 0;

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
    printf("Traversing trailhead %d at (%d,%d)\n", i, trailhead.x, trailhead.y);
    part1 += traverse(grid, trailhead, '9');
    // traverse
  }
  // calculate trailhead score

  printf("part1: %d\n", part1);
  return EXIT_SUCCESS;
}
