#include "util/dbg.h"
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

typedef struct {
  int left;
  int right;
} Rule;

typedef struct {
  int *pages;
  int pages_length;
  bool isCompliant;
} Update;

void printRule(Rule *rule) {
  printf("%d|%d", rule->left, rule->right);
}

void printUpdate(Update *update) {
  printf("*pages: %p\n", (void*)update->pages);
  printf("pages_length: %d\n", update->pages_length);
  printf("isCompliant: %s\n", update->isCompliant ? "true" : "false");
  printf("pages: \n");
  print_intarray(update->pages, update->pages_length, 0, update->pages_length);
}

void printUpdatePages(Update *update) {
  print_intarray(update->pages, update->pages_length, 0, update->pages_length);
}

int main() {
  char *file_name = "input.txt";
  size_t max_file_size = MAX_COLS * MAX_ROWS;
  char **lines = NULL;
  int lines_rows = 0;
  int rules_num = 0;
  int updates_num = 0;
  int update_pages_length = 0;

  lines =
      file_to_arr(file_name, max_file_size, MAX_ROWS, MAX_COLS, &lines_rows);

  int emptyline_idx = 0;
  while (strcmp("", lines[emptyline_idx]) != 0)
    emptyline_idx++;

  // allocate memory for the two new 2d-arrays
  Arena *mem = arena_init(CAPACITY);
  // 1. Allocate rules array
  rules_num = emptyline_idx;
  Rule **rules = (Rule **)arena_alloc(mem, sizeof(Rule *) * rules_num + 10);
  CHECK_NULL(rules);

  for (int i = 0; i < rules_num; i++) {
    rules[i] = (Rule *)arena_alloc(mem, sizeof(Rule));
    if (sscanf(lines[i], "%d|%d", &rules[i]->left, &rules[i]->right) != 2) {
      printf("Failed to scan in left and right number of rule %d", i);
    }
  }

  // 2. Allocate update array
  updates_num = lines_rows - emptyline_idx - 1;
  Update **updates =
      (Update **)arena_alloc(mem, sizeof(Update *) * updates_num);
  CHECK_NULL(updates);

  for (int i = 0; i < updates_num; i++) {
    updates[i] = (Update *)arena_alloc(mem, sizeof(Update));
    CHECK_NULL(updates[i]);
    char *update_str = lines[i+emptyline_idx+1];
    int update_pages_length = 0;
    updates[i]->pages = get_array_from_csv_int(update_str, &update_pages_length);
    updates[i]->pages_length = update_pages_length;
    printf("Update %d pages: ",i);
    print_intarray(updates[i]->pages, update_pages_length, 0, 999);
  }

  // free lines
  for (int i = 0; i < lines_rows; i++)
    free(lines[i]);
  free(lines);

  int total_sum = 0;
  // For each page update
  for (int i = 0; i < updates_num; i++) {
    // for each page in the page update
    updates[i]->isCompliant = true;
    int pages_length = updates[i]->pages_length;
    for (int j = 0; j < pages_length; j++) {
      int page = updates[i]->pages[j];

      // for each rule
      for (int k = 0; k < rules_num; k++) {
        int left = rules[k]->left;
        int right = rules[k]->right;
        if (page == right) {
          for (int z = j; z < pages_length && updates[i]->isCompliant == true; z++) {
            if (left == updates[i]->pages[z]) {
              updates[i]->isCompliant = false;
              printUpdate(updates[i]);
              break;
            }
          }
        }
        if (page == left) {
          // test the rule's right page on the pages to the left
          for (int z = j; z >= 0 && updates[i]->isCompliant == true; z--) {
            if (right == updates[i]->pages[z]) {
              updates[i]->isCompliant = false;
              printUpdate(updates[i]);
              break;
            }
          }
        }
      }
    }
    if (updates[i]->isCompliant == true) {
      // get middle page and add to total sum
      int middle_page = updates[i]->pages[(updates[i]->pages_length)/2];
      printUpdate(updates[i]);
      printf("\tMiddle page: %d\n", middle_page);
      total_sum += middle_page;

      // end
    }
  }

  printf("Total sum of middle pages of safe update: %d\n", total_sum);

  // Free allocated memory
  arena_free(mem);

  return EXIT_SUCCESS;
}
