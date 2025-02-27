#include "util/point.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

bool isInside(int x, int y, int gridsize) {
  return x < gridsize && y < gridsize && x >= 0 && y >= 0;
}
int main(int argc, char *argv[]) {
  int part2 = 0;
  FILE *fp = fopen("input.txt", "r");
  int count_y = 0;
  char g[MAX][MAX];
  char line[MAX];

  while (fgets(line, sizeof(line), fp) != NULL && count_y < MAX) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    strcpy(g[count_y++], line);
  }
  fclose(fp);

  PointArray antennas[256];
  for (int i = 0; i < 256; i++) {
    antennas[i] = createPointArray(count_y * count_y);
  }

  for (int y = 0; y < count_y; y++) {
    for (int x = 0; x < count_y; x++) {
      char antennaType = g[y][x];
      addPoint(&antennas[antennaType], x, y);
    }
  }

  // Now for each antenna type
  for (int antennaType = 0; antennaType < 256; antennaType++) {

    // if more than 1 antenna
    if (antennas[antennaType].length > 1 && antennaType != '.') {

      // create antinodes
      for (int n = 0; n < antennas[antennaType].length; n++) {
        int nx = antennas[antennaType].data[n].x;
        int ny = antennas[antennaType].data[n].y;
        for (int m = 0; m < antennas[antennaType].length; m++) {
          if (n != m) {
            int mx = antennas[antennaType].data[m].x;
            int my = antennas[antennaType].data[m].y;
            int diff_x = mx - nx;
            int diff_y = my - ny;

            // draw antinodes
            g[my][mx] = '#';
            int an_x = mx + diff_x;
            int an_y = my + diff_y;

            // if this point is still in our grid.
            if (isInside(an_x, an_y, count_y)) {
              while (isInside(an_x, an_y, count_y)) {
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

  // count antinodes
  for (int y = 0; y < count_y; y++) {
    for (int x = 0; x < count_y; x++) {
      if (g[y][x] == '#') {
        part2++;
      }
    }
  }

  printf("part2: %d\n", part2);

  for (int i = 0; i < 256; i++) {
    free(antennas[i].data);
  }

  return EXIT_SUCCESS;
}
