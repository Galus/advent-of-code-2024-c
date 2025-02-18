#include "util/io.h"
#include "util/mem.h"
#include "util/str.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 1024 * 40
#define MAX_COLS 2000
#define MAX_ROWS 2000
#define TO_ZERO_BASED(index) ((index) - 1)
#define TO_ONE_BASED(index) ((index) + 1)

struct Rule {
  int left;
  int right;
};

struct Update {
  int *pages;
  bool isCompliant;
};

int main() {
  char *file_name = "input.txt";
  size_t max_file_size = MAX_COLS * MAX_ROWS;
  char **lines = NULL;
  int lines_rows = 0;

  lines =
      file_to_arr(file_name, max_file_size, MAX_ROWS, MAX_COLS, &lines_rows);

  int emptyline_idx = 0;
  while (strcmp("", lines[emptyline_idx]) != 0)
    emptyline_idx++;

  // allocate memory for the two new 2d-arrays
  Arena *mem = arena_init(CAPACITY);
  int rules_num = emptyline_idx;
  struct Rule rules[rules_num];
  char **page_rules =
      (char **)arena_alloc(mem, sizeof(char *) * rules_num + 10);
  CHECK_NULL(page_rules);

  // PAGE RULES
  // (rules_num is human 1-based, emptyline_idx is array 0-based)
  for (int i = 0; i < rules_num; i++) {
    page_rules[i] = arena_alloc(mem, sizeof(char) * strlen(lines[i]) + 10);
    CHECK_NULL(page_rules[i]);
    strcpy(page_rules[i], lines[i]);
  }

  int updates_num = lines_rows - emptyline_idx - 1;
  char **page_updates = arena_alloc(mem, sizeof(char *) * updates_num);
  CHECK_NULL(page_updates);

  // PAGE UPDATES
  int j = emptyline_idx + 1;
  for (int i = 0; i < updates_num; i++) {
    page_updates[i] = arena_alloc(mem, sizeof(char) * strlen(lines[j]) + 10);
    CHECK_NULL(page_updates[i]);
    strcpy(page_updates[i], lines[j]);
    j++;
  }

  // free lines
  for (int i = 0; i < lines_rows; i++)
    free(lines[i]);
  free(lines);

  int total_sum = 0;
  // For each page update
  for (int i = 0; i < updates_num; i++) {
    bool valid = true;
    char *update = page_updates[i];
    int update_pages_length = 0;
    int *update_arr = get_array_from_csv_int(update, &update_pages_length);

    // for each page in the page update
    for (int j = 0; j < update_pages_length; j++) {
      int page = update_arr[j];

      // for each rule
      for (int k = 0; k < rules_num; k++) { // reminder: i:update, j:page,
                                            // k:rule
        int left;
        int right;
        if (sscanf(page_rules[k], "%d|%d", &left, &right) == 2) {
        }
        if (page == right) {
          for (int z = j; z < update_pages_length && valid == true; z++) {
            if (left == update_arr[z]) {
              printf(
                  "update %d UNSAFE:\n\tPage: %d\n\tUpdate: %s\n\tRule: %s\n",
                  i, page, update, page_rules[k]);
              valid = false;
              break;
            }
          }
        }
        if (page == left) {
          // test the rule's right page on the pages to the left
          for (int z = j; z >= 0 && valid == true; z--) {
            if (right == update_arr[z]) {
              // break
              printf(
                  "update %d UNSAFE:\n\tPage: %d\n\tUpdate: %s\n\tRule: %s\n",
                  i, page, update, page_rules[k]);
              valid = false;
              break;
            }
          }
        }
      }
    }
    if (valid) {
      // get middle page and add to total sum
      int middle_page = update_arr[update_pages_length / 2];
      printf("update %d SAFE:\n\tUpdate: %s\n", i, update);
      printf("\tMiddle page: %d\n", middle_page);
      total_sum += middle_page;

      // end
    }
  }

  // Free allocated memory
  arena_free(mem);

  printf("Total sum of middle pages of safe update: %d\n", total_sum);
  return EXIT_SUCCESS;
}
