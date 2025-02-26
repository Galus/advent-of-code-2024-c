#include "util/io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define guard '^'
#define obstacle '#'
#define MAX_OBSTACLES 6000

typedef enum { UP, RIGHT, DOWN, LEFT } Direction;
const char *dirName[] = {"UP", "RIGHT", "DOWN", "LEFT"};

typedef enum { BLOCKED_BY_WALL, UNIQUE, BEEN_HERE, DUNNO } Ahead;
const char *aheadName[] = {"BLOCKED_BY_WALL", "UNIQUE", "BEEN_HERE", "DUNNO"};

typedef struct {
  int x;
  int y;
  Direction direction;
} Guard;

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  Point *data;
  size_t length;
  size_t capacity;
} PointArray;

PointArray createPointArray(size_t length) {
  Point *data = (Point *)malloc(length * sizeof(Point));
  PointArray arr = {data, 0, length};
  return arr;
}

void addPoint(PointArray *arr, int x, int y) {
  /*printf("Adding %zu/%zu\n", arr->length, arr->capacity);*/
  if (arr->length < arr->capacity) {
    /*printf("adding point x %d y %d\n", x, y);*/
    Point o = {x, y};
    arr->data[arr->length] = o;
    arr->length++;
  } else {
    printf("At capacity\n");
  }
}

void freePointArray(PointArray *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->length = 0;
  arr->capacity = 0;
}

bool isGuard(char c) { return (c == '^' || c == '>' || c == 'v' || c == '<'); }

int canMove(char **lines, Guard g) {
  /*printf("Checking if guard at y,x : %d,%d can move dir %s\n", g.y, g.x,
   * dirName[g.direction]);*/
  char future = 'F';
  switch (g.direction) {
  case UP:
    future = lines[(g.y) - 1][(g.x)];
    break;
  case RIGHT:
    future = lines[(g.y)][(g.x) + 1];
    break;
  case DOWN:
    future = lines[(g.y) + 1][(g.x)];
    break;
  case LEFT:
    future = lines[(g.y)][(g.x) - 1];
    break;
  default:
    printf("Invalid direction\n");
    return DUNNO;
    break;
  }
  /*printf("infront of guard: [%c]\n", future);*/
  if (future != 'F' && future == obstacle) {
    /*printf("Hit a wall at %d,%d\n", g.y, g.x);*/
    return BLOCKED_BY_WALL; // BLOCKED_BY_WALL
  } else if (future != '\0') {
    // alter lines to move to next spot.
    if (future == '.') {
      return UNIQUE; // distinct position // UNIQUE
    } else {
      return BEEN_HERE; // been here already // BEEN_HERE
    }
  }
  {
    return DUNNO; // WHAT?
  }
}

void moveDrawX(char **lines, Guard *g) {
  switch (g->direction) {
  case UP:
    lines[(g->y) - 1][(g->x)] = 'X';
    g->y = (g->y) - 1;
    break;
  case RIGHT:
    lines[(g->y)][(g->x) + 1] = 'X';
    g->x = (g->x) + 1;
    break;
  case DOWN:
    lines[(g->y) + 1][g->x] = 'X';
    g->y = (g->y) + 1;
    break;
  case LEFT:
    lines[(g->y)][(g->x) - 1] = 'X';
    g->x = (g->x) - 1;
    break;
  default:
    printf("Invalid g->direction\n");
    break;
    // TODO: Probably cleanup where the guard has already been.
  }
}

void move(char **lines, Guard *g) {
  switch (g->direction) {
  case UP:
    lines[(g->y) - 1][(g->x)] = '^';
    g->y = (g->y) - 1;
    break;
  case RIGHT:
    lines[(g->y)][(g->x) + 1] = '>';
    g->x = (g->x) + 1;
    break;
  case DOWN:
    lines[(g->y) + 1][g->x] = 'v';
    g->y = (g->y) + 1;
    break;
  case LEFT:
    lines[(g->y)][(g->x) - 1] = '<';
    g->x = (g->x) - 1;
    break;
  default:
    printf("Invalid g->direction\n");
    break;
    // TODO: Probably cleanup where the guard has already been.
  }
}

void turnRight(Guard *g) {
  g->direction = (g->direction + 1) % 4;
}

bool checkBounds(char **lines, int lines_num, Guard g) {
  int total_lines = lines_num;
  int line_length = strlen(lines[0]);
  /*printf("Guard is at y,x %d,%d within total_lines: %d line_length: %d\n",
   * g.y, g.x, total_lines, line_length);*/
  if (g.x < line_length - 1 && g.y < total_lines - 1) {
    if (g.x > 0 && g.y > 0) {
      return true;
    }
  }
  return false;
}

int simulate(char **lines, bool drawX) {

  Guard g;
  PointArray obstacles = createPointArray(MAX_OBSTACLES);

  // Find the blasted guard and obstacles;
  for (int i = 0; i < strlen(lines[0]); i++) {
    for (int j = 0; lines[i][j]; j++) {
      if (isGuard(lines[i][j])) {
        g.y = i;
        g.x = j;
        g.direction = UP;
      }
      if (lines[i][j] == '#') {
        /*printf("adding point (%d,%d)\n", i, j);*/
        addPoint(&obstacles, (size_t)j, (size_t)i);
      }
    }
  }

  PointArray visited = createPointArray(MAX_OBSTACLES * 99);
  int visited_dir[MAX_OBSTACLES * 99];
  int visited_idx = 0;
  while (checkBounds(lines, strlen(lines[0]), g)) {

    // record that we stepped here and our direction.
    addPoint(&visited, g.x, g.y);
    visited_dir[visited_idx++] = g.direction;

    /*print_linesarray(lines, strlen(lines[0]));*/
    /*printf("Guard is going %s at (y,x): %d,%d\n", dirName[g.direction], g.y,
     * g.x);*/

    int result = canMove(lines, g);
    if (result == UNIQUE || result == BEEN_HERE) {
      /*printf("Moving guard %s.\n", dirName[g.direction]);*/
      if (drawX) {
        moveDrawX(lines, &g);
      } else {
        move(lines, &g);
      }
    } else if (result == BLOCKED_BY_WALL) {

      // check if guard has already been here
      turnRight(&g);
      for (int i = 0; i < visited_idx; i++) {
        if (visited.data[i].x == g.x && visited.data[i].y == g.y &&
            visited_dir[i] == g.direction) {
          freePointArray(&obstacles);
          freePointArray(&visited);
          return 1;
        }
      }
      /*result = canMove(lines, *g);*/
      if (result == DUNNO) {
        printf("Error: Direction is DUNNO\n");
      }
      /*move(lines, g);*/
    } else {
      printf("Wuh woh!\n");
    }
  }
  freePointArray(&obstacles);
  freePointArray(&visited);
  return 0;
}

int main(int argc, char *argv[]) {
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  char *file_name = "input.txt";
  size_t max_file_size = 500 * 500;
  size_t max_rows = 1300;
  size_t max_cols = 1300;
  int lines_rows = 0;
  char **lines =
      file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);

  simulate(lines, true);

  PointArray traversedPath = createPointArray(MAX_OBSTACLES);
  /*print_linesarray(lines, strlen(lines[0]));*/
  for (int i = 0; i < lines_rows; i++) {
    for (int j = 0; lines[i][j]; j++) {
      if (lines[i][j] == 'X') {
        addPoint(&traversedPath, j, i);
      }
    }
  }

  /*for (size_t i = 0; i < traversedPath.length; i++) {*/
  /*  printf("TraversedPath %zu: y,x %d,%d\n", i, traversedPath.data[i].y,*/
  /*         traversedPath.data[i].x);*/
  /*}*/

  int part2 = 0;
  for (int i = 0; i < traversedPath.length; i++) {
    // instead of making a copy of lines, just reread the input.
    char **linez =
        file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);
    // inject a new obstacle
    linez[(traversedPath.data[i]).y][(traversedPath.data[i]).x] = '#';
    /*printf("Lets go %d\n",i);*/
    if (simulate(linez, false) == 1) {
      /*printf("Adding a wall at (%d,%d) will cause infinite loop\n", traversedPath.data[i].x, traversedPath.data[i].y);*/
      part2++;
    }
  }

  end = clock();
  cpu_time_used = ((double)(end - start));
  printf("Time taken: %f seconds\n", cpu_time_used);

  /*printf("Guard exits at %d,%d\n", g.y, g.x);*/
  printf("Part1: %zu\n", traversedPath.length);
  printf("Part2: %d\n", part2);

  // Free allocated memory
  for (int i = 0; i < lines_rows; i++) {
    free(lines[i]);
  }
  free(lines);

  return EXIT_SUCCESS;
}
