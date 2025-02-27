#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/point.h"

#define MAX_Y 1000
#define MAX_X 1000

int main(int argc, char *argv[]) {
  int part2 = 0;

  char *arr = calloc(MAX_X, sizeof(char));
  FILE *fp = fopen("input.txt", "r");
  int count_y = 0;
  int count_x = 0;
  char lines[MAX_Y][MAX_X];

  char line[MAX_Y];
  while (fgets(line, sizeof(line), fp) != NULL) {
    count_x = strlen(line);
    if (count_x > 0 && line[count_x - 1] == '\n') {
      line[count_x - 1] = '\0';
    }
    strcpy(lines[count_y++], line);
  }
  fclose(fp);

  for (int i = 0; i < count_y; i++) {
    printf("Line %d: \t%s\n", i, lines[i]);
  }

  // Copy for mutable use
  char g[MAX_Y][MAX_X];
  for (int i = 0; i < count_y; i++) {
    for (int j = 0; j < count_y; j++) {
      g[i][j] = lines[i][j];
    }
  }

  // print chars
  for (int i = 0; i < count_y; i++) {
    for (int j = 0; j < count_y; j++) {
      g[i][j] = lines[i][j];
      printf("g[%d][%d]: %c\n", i, j, g[i][j]);
    }
  }

  // print lines
  for (int i = 0; i < count_y; i++) {
    printf("g[%d]: \t%s\n", i, g[i]);
  }

  PointArray antennas[256];
  for (int i = 0; i < count_y; i++) {
    for (int j = 0; j < count_y; j++) {
      char curr = g[i][j];
      if (antennas[curr].length == 0) {
        PointArray a = createPointArray(count_y * count_y);
        antennas[curr] = a;
        addPoint(&antennas[curr], j, i);
      } else {
        addPoint(&antennas[curr], j, i);
      }
      g[i][j] = lines[i][j];
    }
  }

  printf("Antenna '.' points:\n");
  printPointArray(antennas['.']);
  printf("Antenna '0' points:\n");
  printPointArray(antennas['0']);
  printf("Antenna 'A' points:\n");
  printPointArray(antennas['A']);

  // Now for each antenna type
  for (int i = 0; i < 256; i++) {
    // if more than 1 antenna
    if (antennas[i].length > 1) {
      printf("We have 2 or more '%c' antennas\n", i);
      if (i != '.') {
        // create antinodes
        for (int n = 0; n < antennas[i].length; n++) {
          int x = antennas[i].data[n].x;
          int y = antennas[i].data[n].y;
          printf("processing antenna %c point (%d,%d)\n", i, x, y);
          for (int m = 0; m < antennas[i].length; m++) {
            if (n != m) {
              int mx = antennas[i].data[m].x;
              int my = antennas[i].data[m].y;
              int nx = antennas[i].data[n].x;
              int ny = antennas[i].data[n].y;
              int diff_x = mx - nx;
              int diff_y = my - ny;
              printf("diff (%d,%d)-(%d,%d): (%d,%d)\n", mx, my, nx, ny, diff_x,
                     diff_y);
              // draw antinode
              int an_x = mx + (diff_x);
              int an_y = my + (diff_y);

              // set current antenna as its own antinode
              g[my][mx] = '#';
              // if this point is still in our grid.
              if (an_x < count_y && an_y < count_y && an_x >= 0 && an_y >= 0) {
                while (an_x < count_y && an_y < count_y && an_x >= 0 &&
                       an_y >= 0) {
                  g[an_y][an_x] = '#';
                  an_y += diff_y;
                  an_x += diff_x;
                }
              }
            }
          }
        }
      }
    }
  }

  // count antinodes
  for (int i = 0; i < count_y; i++) {
    for (int j = 0; j < count_y; j++) {
      if (g[i][j] == '#') {
        part2++;
      }
    }
  }

  for (int i = 0; i < count_y; i++) {
    printf("g[%d]: \t%s\n", i, g[i]);
  }

  printf("part2: %d\n", part2);

  return EXIT_SUCCESS;
}

