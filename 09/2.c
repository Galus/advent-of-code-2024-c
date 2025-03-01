#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100000

void printDisk(int *arr, int length) {
  for ( int i = 0; i < length; i++) {
    if (arr[i] == -1) {
      printf(".");
    } else {
      printf("%d", arr[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  int part2 = 0;
  char *arr = calloc(MAX, sizeof(char));
  FILE *fp = fopen("input.txt", "r");
  int length;

  char line[MAX];
  if (fgets(line, sizeof(line), fp) != NULL) {
    length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
      line[length - 1] = '\0';
    }
  } else {
    printf("Cant read line.\n");
  }
  fclose(fp);

  int disk[MAX];

  // calc checksum
  printf("part2: %llu\n", part2);
  return EXIT_SUCCESS;
}
