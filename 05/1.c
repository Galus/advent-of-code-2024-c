#include "util/dbg.h"
#include "util/io.h"
#include "util/str.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 2000
#define MAX_ROWS 2000
#define TO_ZERO_BASED(index) ((index) - 1)
#define TO_ONE_BASED(index) ((index) + 1)

int main() {
  size_t len = len_m("Save The World!");
  assert(len == 15);

  char *file_name = "input.txt";
  size_t max_file_size = MAX_COLS * MAX_ROWS;
  char **lines = NULL; // malloc(sizeof(char*) * MAX_ROWS);
  int lines_rows = 0;

  lines =
      file_to_arr(file_name, max_file_size, MAX_ROWS, MAX_COLS, &lines_rows);

  // details
  // lines now has both 'page ordering rules' and 'pages to produce in each
  // update' X|Y => page X must be printed at some point before page Y as part
  // of an update

  // split lines into 'page order rules' and 'pages to produce' in each update
  // figure out where the split is (the empty line)
  int emptyline_idx = 0;
  while (strcmp("", lines[emptyline_idx]) != 0)
    emptyline_idx++;

  // allocate memory for the two new 2d-arrays
  // (rules_num is human 1-based, emptyline_idx is array 0-based)
  int rules_num = emptyline_idx;
  char **page_rules = malloc(sizeof(char *) * rules_num + 10);
  if (page_rules == NULL) {
    perror("page_rules malloc failed");
    return EXIT_FAILURE;
  }

  // PAGE RULES
  for (int i = 0; i < rules_num; i++) {
    page_rules[i] = malloc(sizeof(char) * strlen(lines[i]) + 10);
    if (page_rules[i] == NULL) {
      perror("page_rules[i] malloc failed");
      for (int k = 0; k < i; k++) {
        free(page_rules[k]);
      }
      free(page_rules);
      return EXIT_FAILURE;
    }

    strcpy(page_rules[i], lines[i]);
  }

  int updates_num = lines_rows - emptyline_idx - 1;
  char **page_updates = malloc(sizeof(char *) * updates_num);
  if (page_updates == NULL) {
    perror("page_updates malloc failed");
    return EXIT_FAILURE;
  }


  // PAGE UPDATES
  int j = emptyline_idx + 1;
  for (int i = 0; i < updates_num; i++) {
    size_t allocsize = (sizeof(char) * strlen(lines[j]) + 1);
    page_updates[i] = malloc(sizeof(char) * strlen(lines[j]) + 20);

    strcpy(page_updates[i], lines[j]);
    j++;
  }

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
              printf("update %d UNSAFE:\n\tPage: %d\n\tUpdate: %s\n\tRule: %s\n", i, page, update, page_rules[k]);
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
              printf("update %d UNSAFE:\n\tPage: %d\n\tUpdate: %s\n\tRule: %s\n", i, page, update, page_rules[k]);
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
  for (int i = 0; i < lines_rows; i++) {
    /*printf("Freeing lines[%d]\n", i);*/
    free(lines[i]);
  }
  free(lines);

  printf("Total sum of middle pages of safe update: %d\n", total_sum);
  return EXIT_SUCCESS;
}
