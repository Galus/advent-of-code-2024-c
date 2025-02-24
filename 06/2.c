#include "util/array.h"
#include "util/dbg.h"
#include "util/io.h"
#include "util/str.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define guard '^'
#define obstacle '#'
#define MAX_OBSTACLES 6000

typedef enum { UP, RIGHT, DOWN, LEFT, ERROR } Direction;
const char *dirName[] = {"UP", "RIGHT", "DOWN", "LEFT", "ERROR"};

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
} Obstacle;

typedef struct {
  Obstacle *obstacles;
  size_t length;
  size_t capacity;
} ObstacleArray;

ObstacleArray createObstacleArray(size_t length) {
  Obstacle *obstacles = (Obstacle *)malloc(length * sizeof(Obstacle));
  ObstacleArray arr = {obstacles, 0, length};
  return arr;
}

void addObstacle(ObstacleArray *arr, int x, int y) {
  if (arr->length < arr->capacity) {
    Obstacle o = {x, y};
    arr->obstacles[arr->length] = o;
    arr->length++;
  } else {
    printf("At capacity\n");
  }
}

void freeObstacleArray(ObstacleArray *arr) {
  free(arr->obstacles);
  arr->obstacles = NULL;
  arr->length = 0;
  arr->capacity = 0;
}

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
    return ERROR;
    break;
  }
}

int canMove(char **lines, Direction direction, Guard g) {
  printf("Checking if guard at y,x : %d,%d can move dir %s\n", g.y, g.x,
         dirName[direction]);
  char future = 'F';
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
    return DUNNO;
    break;
  }
  printf("infront of guard: [%c]\n", future);
  if (future != 'F' && future == obstacle) {
    printf("Hit a wall at %d,%d\n", g.y, g.x);
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
  int total_lines = lines_num;
  int line_length = strlen(lines[0]);
  printf("Guard is at y,x %d,%d within total_lines: %d line_length: %d\n", g.y,
         g.x, total_lines, line_length);
  if (g.x < line_length - 1 && g.y < total_lines - 1) {
    if (g.x > 0 && g.y > 0) {
      return true;
    }
  }
  return false;
}

void simulate(char **lines, Guard *g) {

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
  ObstacleArray arr = createObstacleArray(MAX_OBSTACLES);

  // Find the blasted guard!
  for (int i = 0; i < lines_rows; i++) {
    for (int j = 0; lines[i][j]; j++) {
      if (isGuard(lines[i][j])) {
        g.y = i;
        g.x = j;
        setGuardDirection(lines, &g);
      }
      if (lines[i][j] == '#') {
        addObstacle(&arr, j, i);
      }
    }
  }

  for (int i = 0; i < arr.length; i++) {
    printf("Obstacle %d: y,x %d,%d\n", i, arr.obstacles[i].y, arr.obstacles[i].x);
  }

  printf("Found guard on line: %d col: %d\n", g.y, g.x);

  // Predict the guards movement
  int blocks = 0;
  int guards_last_x = -1;
  int guards_last_y = -1;
  while (checkBounds(lines, lines_rows, g)) {
    // Check if guard is looping
    if (guards_last_x == -1) {
      if (g.x == guards_last_x && g.y == guards_last_y) {
        printf("Error: Guard unable to move");
        return 1;
      }
    }
    guards_last_x = g.x;
    guards_last_y = g.y;

    printf("blocks: %d\n", blocks);
    print_linesarray(lines, lines_rows);
    printf("Guard is at (y,x): %d,%d\n", g.y, g.x);
    Direction dir = g.direction;
    int result = canMove(lines, dir, g);
    if (result == UNIQUE || result == BEEN_HERE) {
      // Check if possible blocker can be placed 4 infini-loop
      bool created = false;
      switch (dir) {
      case UP:
        for (int i = 0; i < arr.length && created == false; i++) {
          printf("Obstacle: y,x %d,%d\n", arr.obstacles[i].y,
                 arr.obstacles[i].x);
          printf("%d Checking if can add block %s\n", i, dirName[dir]);
          // if there is an obstacle on the y axis (updown)
          if (arr.obstacles[i].y == g.y && arr.obstacles[i].x > g.x) {
            printf("\t YES Can add obstruction at UP at y,x %d,%d\n", g.y - 1,
                   g.x);
            created = true;
            blocks++;
          }
        }
        break;
      case RIGHT:
        // check down => x axis
        for (int i = 0; i < arr.length && created == false; i++) {
          printf("Obstacle: y,x %d,%d\n", arr.obstacles[i].y,
                 arr.obstacles[i].x);
          printf("%d Checking if can add block %s\n", i, dirName[dir]);
          if (arr.obstacles[i].y > g.y && arr.obstacles[i].x == g.x) {
            printf("\t YES Can add obstruction at RIGHT at y,x %d,%d\n", g.y,
                   g.x + 1);
            created = true;
            blocks++;
          }
        }
        break;
      case DOWN:
        for (int i = 0; i < arr.length && created == false; i++) {
          printf("Obstacle: y,x %d,%d\n", arr.obstacles[i].y,
                 arr.obstacles[i].x);
          printf("%d Checking if can add block %s\n", i, dirName[dir]);
          if (arr.obstacles[i].y == g.y && arr.obstacles[i].x < g.x) {
            printf("\t YES Can add obstruction at DOWN at y,x %d,%d\n", g.y + 1,
                   g.x);
            created = true;
            blocks++;
          }
        }
        break;
      case LEFT:
        for (int i = 0; i < arr.length && created == false; i++) {
          printf("Obstacle: y,x %d,%d\n", arr.obstacles[i].y,
                 arr.obstacles[i].x);
          printf("%d Checking if can add block %s\n", i, dirName[dir]);
          if (arr.obstacles[i].y < g.y && arr.obstacles[i].x == g.x) {
            printf("\t YES Can add obstruction at LEFT at y,x %d,%d\n", g.y,
                   g.x - 1);
            created = true;
            blocks++;
          }
        }
        break;
      default:
        printf("Invalid direction\n");
        break;
      }
      printf("Moving guard %s.\n", dirName[dir]);
      move(lines, dir, &g);
    } else if (result == BLOCKED_BY_WALL) {

      // store the location of the obstacle
      turnRight(lines, &g);
      result = canMove(lines, g.direction, g);
      if (result == DUNNO) {
        printf("Error: Direction is DUNNO\n");
        return 1;
      }
      move(lines, g.direction, &g);
    } else {
      goto end;
    }
    // printf("Got direction %d\n", dir);
  }

end:
  printf("Guard exits at %d,%d\n", g.y, g.x);
  printf("Part2: %d\n", blocks);

  // Free allocated memory
  for (int i = 0; i < lines_rows; i++) {
    free(lines[i]);
  }
  free(lines);
  freeObstacleArray(&arr);

  return EXIT_SUCCESS;
}
