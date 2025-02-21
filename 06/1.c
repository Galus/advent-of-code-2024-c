#include "util/dbg.h"
#include "util/io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define guard '^'
#define obstacle '#'

typedef enum { UP, RIGHT, DOWN, LEFT } Direction;
const char *dirName[] = {"UP", "RIGHT", "DOWN", "LEFT"};

typedef struct {
  int x;
  int y;
  Direction direction;
} Guard;

bool isGuard(char c) { return (c == '^' || c == '>' || c == 'v' || c == '<'); }

// Checks the direction in the lines array and updates guard struct
Direction setGuardDirection(char **lines, Guard *g) {
  switch (lines[g->y][g->x]) {
  case '^':
    return UP;
    break;
  case '>':
    return RIGHT;
    break;
  case 'v':
    return DOWN;
    break;
  case '<':
    return LEFT;
    break;
  default:
    printf("Guard is in the upside-down.\n");
    return EXIT_FAILURE;
    break;
  }
}

int canMove(char **lines, Direction direction, Guard g) {
  printf("Checking if guard at y,x : %d,%d can move dir %s\n", g.y, g.x,
         dirName[direction]);
  char future;
  switch (direction) {
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
    return 2;
    break;
  }
  printf("infront of guard: [%c]\n", future);
  if (future && future == obstacle) {
    printf("Hit a wall at %d,%d\n", g.y, g.x);
    return 0;
  } else if (future != '\0') {
    // alter lines to move to next spot.
    if (future == '.') {
      return 1; // distinct position
    } else {
      return 2; // been here already
    }
  }
  {
    return 3;
  }
}

void move(char **lines, Direction direction, Guard *g) {
  switch (direction) {
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
    printf("Invalid direction\n");
    break;
    // TODO: Probably cleanup where the guard has already been.
  }
}

void turnRight(char **lines, Guard *g) {
  switch (g->direction) {
  case UP:
    lines[g->y][g->x] = '>';
    break;
  case RIGHT:
    lines[g->y][g->x] = 'v';
    break;
  case DOWN:
    lines[g->y][g->x] = '<';
    break;
  case LEFT:
    lines[g->y][g->x] = '^';
    break;
  default:
    printf("Invalid direction\n");
    break;
  }
  g->direction = ((g->direction + 1) % 4);
}

bool checkBounds(char **lines, int lines_num, Guard g) {
  int line_length = strlen(lines[0]);
  int total_lines = lines_num;
  printf("Checking bounds within line_length: %d total_lines: %d\n",
         line_length, total_lines);
  if (g.x < line_length - 1 && g.y < total_lines - 1) {
    if (g.x > 0 && g.y > 0) {
      return true;
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  char *file_name = "input.txt";
  size_t max_file_size = 200 * 200;
  size_t max_rows = 135;
  size_t max_cols = 135;
  int lines_rows = 0;
  char **lines =
      file_to_arr(file_name, max_file_size, max_rows, max_cols, &lines_rows);

  Guard g;

  // Find the blasted guard!
  for (int i = 0; i < lines_rows; i++) {
    for (int j = 0; lines[i][j]; j++) {
      if (isGuard(lines[i][j])) {
        g.y = i;
        g.x = j;
        setGuardDirection(lines, &g);
      }
    }
  }
  
  printf("Found guard on line: %d col: %d\n", g.y, g.x);

  // Predict the guards movement
  int steps = 1;
  while (checkBounds(lines, lines_rows, g)) {
    printf("steps: %d\n", steps);
    print_linesarray(lines, lines_rows);
    printf("Guard is at (y,x): %d,%d\n", g.y, g.x);
    Direction dir = g.direction;
    int result = canMove(lines, dir, g);
    if (result == 1 || result == 2) {
      if (result == 1) {
        steps++;
      }
      move(lines, dir, &g);
    } else if (result == 0) {

      turnRight(lines, &g);
      result = canMove(lines, g.direction, g);
      if (result == 1) {
        steps++;
      }
      move(lines, g.direction, &g);
    } else {
      goto end;
    }
    // printf("Got direction %d\n", dir);
  }

end:
  printf("Guard exits at %d,%d\n", g.y, g.x);
  printf("Part1: %d\n", steps);

  // Free allocated memory
  for (int i = 0; i < lines_rows; i++) {
    /*printf("Freeing lines[%d]\n", i);*/
    free(lines[i]);
  }
  free(lines);

  return EXIT_SUCCESS;
}
