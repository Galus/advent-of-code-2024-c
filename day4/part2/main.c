#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 9999
#define MAX_LINES 1000

void print_hex_memory(const void *ptr, size_t n) {
  if (ptr == NULL) {
    fprintf(stderr, "Error: Pointer is NULL\n");
    return;
  }

  if (n == 0) {
    printf("No bytes to print.\n");
    return;
  }

  const unsigned char *bytes =
      (const unsigned char *)ptr; // Treat as unsigned char array
  uintptr_t address =
      (uintptr_t)ptr; // Store the address as an unsigned integer

  printf("Memory dump of %zu bytes starting at address %p:\n", n, ptr);

  for (size_t i = 0; i < n; i++) {
    if (i % 16 == 0) { // New line every 16 bytes
      if (i > 0) {
        printf("  "); // Spacer after hex values
        for (size_t j = i - 16; j < i; j++) {
          if (bytes[j] >= 32 && bytes[j] <= 126) { // Printable ASCII
            printf("%c", bytes[j]);
          } else {
            printf("."); // Non-printable character
          }
        }
      }
      printf(
          "\n%p: ",
          (void *)(address + i)); // Print address at the beginning of the line
    }
    printf("%02X ", bytes[i]); // Print hex value with leading zero if needed
  }
  // Handle any remaining bytes on the last line
  if (n % 16 != 0) {
    int padding = 16 - (n % 16);
    for (int i = 0; i < padding; i++) {
      printf("   "); // Add padding for alignment
    }
    printf("  ");
    for (size_t j = n - (n % 16); j < n; j++) {
      if (bytes[j] >= 32 && bytes[j] <= 126) {
        printf("%c", bytes[j]);
      } else {
        printf(".");
      }
    }
  }
  printf("\n");
}

void subStr(char *src, char *dest, int start, int length) {
  printf("\tsrc: %s\n", src);
  printf("\tdest: %s (This value is from last time it was set by subStr)\n",
         dest);
  printf("\tstart: %d\n", start);
  printf("\tlength: %d\n", length);
  assert(src != NULL);
  assert(dest != NULL);
  assert(start >= 0);
  assert(length >= 0);
  assert(start + length <= strlen(src));

  // for learning purposes
  /*int i = 0;*/
  /*src += start;*/
  /*while (length--)*/
  /*  *dest++ = *src++;*/
  /**dest = '\0';*/

  strncpy(dest, src + start, length);
  dest[length] = '\0';
}

    // for learning purposes
    /*size_t len(char *str) {*/
    /*  char *s;*/
    /*  for (s = str; *s; ++s)*/
    /*    ;*/
    /*  return s - str;*/
    /*}*/

    int main(int argc, char *argv[]) {
  FILE *f;
  int line_num = 0;
  char **lines;

  lines = malloc(sizeof(char *) * MAX_LINES);
  if (lines == NULL) {
    perror("malloc failed");
    return 1;
  }

  // til: we can just do array declaration to skip mem allocation ceremony
  /*char *line;*/
  /*line = malloc(sizeof(char) * MAX_LINE_LENGTH);*/
  /*if (line == NULL) {*/
  /*  perror("malloc failed");*/
  /*  return 1;*/
  /*}*/
  char line[MAX_LINE_LENGTH];

  f = fopen("input.txt", "r");
  if (f == NULL) {
    perror("fopen failed");
    free(lines);
    return 1;
  }

  while (fgets(line, MAX_LINE_LENGTH, f) != NULL && line_num < MAX_LINES) {
    // printf("line %d: %s\n", line_num, lines[line_num]);
    lines[line_num] = malloc(sizeof(char) * (strlen(line) + 1));
    if (lines[line_num] == NULL) {
      perror("malloc failed for lines[line_num]");
      for (int i = 0; i < line_num; i++) {
        free(lines[i]);
      }
      free(lines);
      fclose(f);
      return 1;
    }
    /*sscanf(line, "%s", lines[line_num++]); // TODO: replace w fgets*/
    strcpy(lines[line_num], line);
    size_t len = strlen(lines[line_num]);
    if (len > 0 && lines[line_num][len - 1] == '\n') {
      lines[line_num][len - 1] = '\0';
    }
    line_num++;
  }
  fclose(f);

  /*printf("Printing line 8: %s\n", lines[8]);*/
  /*printf("Printing line 7: %s\n", lines[7]);*/
  /*printf("Printing line 6: %s\n", lines[6]);*/
  /*printf("Printing line 5: %s\n", lines[5]);*/

  int word_count = 0;

  for (int i = 0; i < line_num-1; i++) {
    printf("\nLine %d/%d: %s\n", i, line_num, lines[i]);
    size_t line_length = strlen(lines[i]);
    /*printf("line_length: %zu\n", line_length);*/

    // printf("j: ");
    for (int j = 0; j <= line_length - 1; j++) {
      printf("%d ", j);
      /*print_hex_memory(&j-5, 33);*/
      if (lines[i][j] == 'A') {

        // right
        if (j <= line_length - 1 && i <= line_num - 1 && j >= 1 && i >= 1) {

          /* M M
           *  A
           * S S */
          if (lines[i - 1][j - 1] == 'M' && lines[i + 1][j + 1] == 'S') {
            if (lines[i - 1][j + 1] == 'M' && lines[i + 1][j - 1] == 'S') {
              word_count++;
            }
          }
          /* M S
           *  A
           * M S */
          if (lines[i - 1][j - 1] == 'M' && lines[i + 1][j + 1] == 'S') {
            if (lines[i - 1][j + 1] == 'S' && lines[i + 1][j - 1] == 'M') {
              word_count++;
            }
          }

          /* S S
           *  A
           * M M */
          if (lines[i - 1][j - 1] == 'S' && lines[i + 1][j + 1] == 'M') {
            if (lines[i - 1][j + 1] == 'S' && lines[i + 1][j - 1] == 'M') {
              word_count++;
            }
          }

          /* S M
           *  A
           * S M */
          if (lines[i - 1][j - 1] == 'S' && lines[i + 1][j + 1] == 'M') {
            if (lines[i - 1][j + 1] == 'M' && lines[i + 1][j - 1] == 'S') {
              word_count++;
            }
          }
        }
      }
      printf("word_count: %d\n", word_count);
    }
  }

for (int i = 0; i < line_num; i++) {
  free(lines[i]);
}
free(lines);

printf("\ntotal lines processed: %d\n", line_num);
printf("word_count: %d\n", word_count);

return EXIT_SUCCESS;
}
