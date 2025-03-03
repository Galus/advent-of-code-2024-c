#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200000
#define ASCII_NUMBER_OFFSET 48

int main(int argc, char *argv[]) {
  int part1 = 0;
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
  int diskIndex = 0;
  int id = 0;
  // create the disk representation:
  // input: 2333512 -> disk: '00...111...22222.33'
  // i.e. 2 b, 3 s, 3 b, 3 s, 5 b, 1 s, 2 s
  //   where b - block, s - space
  for (int i = 0; i < length; i++) {
    /*printf("Disk: \t\t");*/
    /*for (int i = 0; i < diskIndex; i++) {*/
    /*  printf("[%d]", disk[i]);*/
    /*}*/
    /*printf("\n");*/
    char string[2];
    string[0] = line[i];
    string[1] = '\0';
    int size = atoi(string);
    /*printf("size: %d\n", size);*/
    if (i % 2 == 0) {
      for (int j = 0; j < size; j++) {
        disk[diskIndex + j] = id;
      }
      id++;
    } else {
      for (int j = 0; j < size; j++) {
        disk[diskIndex + j] = -1;
      }
    }
    diskIndex += size;
    /*printf("diskIndex: %d,", diskIndex);*/
  }
  /*disk[diskIndex] = '\0';*/

  /*printf("Line (size %d): %s\n", length, line);*/
  /*printf("Disk: \t\t");*/
  /*for (int i = 0; i < diskIndex; i++) {*/
  /*  printf("[%d]", disk[i]);*/
  /*}*/
  /*printf("\n");*/

  // de-fragment
  int l = 0;
  int r = diskIndex-1;

  while (l < r) {
    /*printf("l: %d r: %d\n", l, r);*/
    if (disk[l] == -1) {
      while (l < r && disk[r] == -1) {
        r--;
      }
      if (l < r) {
        disk[l] = disk[r];
        disk[r] = -1;
        r--;
      }
    }
    l++;
  }

  printf("\nDefrag Disk: \t\t");
  for (int i = 0; i < diskIndex; i++) {
    printf("[%d]", disk[i]);
  }
  printf("\n");

  bool split = false;
  unsigned long long checksum = 0;
  for (int i = 0; i < diskIndex; i++) {
    if (disk[i] != -1) {
      checksum += i * disk[i];
    } else {
      if (!split) {
        printf("\n\n");
        split = true;
      }
    }
    printf("c:%llu=%d*%d\n", checksum, i, disk[i]);
  }

  // calc checksum
  printf("checksum: %llu\n", checksum);
  return EXIT_SUCCESS;
}
