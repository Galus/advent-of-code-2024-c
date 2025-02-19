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

typedef struct {
  Rule *rules;
  int size;
} RulesArray;

void printRule(Rule *rule) { printf("Rule: %d|%d\n", rule->left, rule->right); }

void printUpdate(Update *update) {
  /*printf("*pages: %p\n", (void *)update->pages);*/
  /*printf("pages_length: %d\n", update->pages_length);*/
  /*printf("isCompliant: %s\n", update->isCompliant ? "true" : "false");*/
  printf("pages: \n");
  print_intarray(update->pages, update->pages_length, 0, update->pages_length);
}

void printUpdatePages(Update *update) {
  print_intarray(update->pages, update->pages_length, 0, update->pages_length);
}

bool isCompliant(Rule **rules, int rules_length, Update *update) {
  for (int rule_idx = 0; rule_idx < rules_length; rule_idx++) {
    int l = rules[rule_idx]->left;
    int r = rules[rule_idx]->right;

    int l_idx = -1;
    int r_idx = -1;

    for (int i = 0; i < update->pages_length; i++) {
      if (l == update->pages[i]) {
        l_idx = i;
      }
      if (r == update->pages[i]) {
        r_idx = i;
      }
    }

    if (l_idx != -1 && r_idx != -1 && r_idx < l_idx) {
      return false;
    }
  }
  return true;
}

/*bool isCompliant(Rule **rules, int rules_length, Update *update) {*/
/*  bool compliant = true;*/
/*  for (int rule_idx = 0; rule_idx < rules_length; rule_idx++) {*/
/*    Update *fixed = update;*/
/*    int l = rules[rule_idx]->left;*/
/*    int r = rules[rule_idx]->right;*/
/**/
/*    int l_idx = -1;*/
/*    int r_idx = -1;*/
/*    int found_left = false;*/
/*    int found_right = false;*/
/**/
/*    for (int i = 0; i < update->pages_length; i++) {*/
/*      if (l == update->pages[i]) {*/
/*        l_idx = i;*/
/*        found_left = true;*/
/*      }*/
/**/
/*      if (r == update->pages[i]) {*/
/*        r_idx = i;*/
/*        found_right = true;*/
/*      }*/
/**/
/*      if (found_left == true && found_right == true && r_idx < l_idx) {*/
/*        compliant = false;*/
/*      }*/
/*    }*/
/*  }*/
/*  return compliant;*/
/*}*/

// Checks each rule against an update to see if its compliant
// If compliant, does nothing.
// If not compliant, will bring the update's pages into compliance
// by swapping the positions of the pages till all rules pass.
bool fixUpdate(Rule **rules, int rules_length, Update *update) {
  bool changed;
  do {
    changed = false;
    for (int rule_idx = 0; rule_idx < rules_length; rule_idx++) {
      int l = rules[rule_idx]->left;
      int r = rules[rule_idx]->right;

      int l_idx = -1;
      int r_idx = -1;

      // Find the positions of the left and right pages
      for (int i = 0; i < update->pages_length; i++) {
        if (l == update->pages[i]) {
          l_idx = i;
        }
        if (r == update->pages[i]) {
          r_idx = i;
        }
      }

      // If the rule is violated, swap the pages
      if (l_idx != -1 && r_idx != -1 && r_idx < l_idx) {
        printf("Fixing with a swap...");
        printRule(rules[rule_idx]);
        update->pages[r_idx] = l;
        update->pages[l_idx] = r;
        changed = true;
      }
    }
  } while (changed); // Repeat until no more changes are needed

  // Check if the update is now compliant
  update->isCompliant = isCompliant(rules, rules_length, update);
  return update->isCompliant;
}

/*void fixUpdate(Rule **rules, int rules_length, Update *update) {*/
/*  bool allRulesPass = true;*/
/*  for (int rule_idx = 0; rule_idx < rules_length; rule_idx++) {*/
/*    Update *fixed = update;*/
/*    int l = rules[rule_idx]->left;*/
/*    int r = rules[rule_idx]->right;*/
/**/
/*    int l_idx = -1;*/
/*    int r_idx = -1;*/
/*    int foundleft = false;*/
/*    int foundright = false;*/
/**/
/*    // for each page*/
/*    for (int i = 0; i < update->pages_length; i++) {*/
/*      // extract the position of left and right pages*/
/*      if (l == update->pages[i]) {*/
/*        l_idx = i;*/
/*        foundleft = true;*/
/*      }*/
/*      if (r == update->pages[i]) {*/
/*        r_idx = i;*/
/*        foundright = true;*/
/*      }*/
/**/
/*      // swap if the rule is violated.*/
/*      if (r_idx < l_idx && foundleft == true && foundright == true) {*/
/*        allRulesPass = false;*/
/*        printf("Fixing with a swap...");*/
/*        printRule(rules[rule_idx]);*/
/*        update->pages[r_idx] = l;*/
/*        update->pages[l_idx] = r;*/
/*      }*/
/*    }*/
/*  }*/
/*  if (allRulesPass == true) {*/
/*    printf("All rules pass.\n");*/
/*    (update->isCompliant) = true;*/
/*  }*/
/*  printf("\tAfter fix: \n");*/
/*  printUpdate(update);*/
/*}*/

int getMiddlePage(Update *update) {
  return (update->pages[update->pages_length / 2]);
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

  // 1. Allocate and populate rules array
  rules_num = emptyline_idx;
  Rule **rules = (Rule **)arena_alloc(mem, sizeof(Rule *) * rules_num + 10);
  for (int i = 0; i < rules_num; i++) {
    rules[i] = (Rule *)arena_alloc(mem, sizeof(Rule));
    if (sscanf(lines[i], "%d|%d", &rules[i]->left, &rules[i]->right) != 2) {
      printf("Failed to scan in left and right number of rule %d", i);
    }
  }

  // 2. Allocate and populate update array
  updates_num = lines_rows - emptyline_idx - 1;
  Update **updates =
      (Update **)arena_alloc(mem, sizeof(Update *) * updates_num);
  for (int i = 0; i < updates_num; i++) {
    updates[i] = (Update *)arena_alloc(mem, sizeof(Update));
    char *update_str = lines[i + emptyline_idx + 1];
    int update_pages_length = 0;
    updates[i]->pages =
        get_array_from_csv_int(update_str, &update_pages_length);
    updates[i]->pages_length = update_pages_length;
    /*printf("Update %d pages: ", i);*/
    /*print_intarray(updates[i]->pages, update_pages_length, 0, 999);*/
  }

  // free lines
  for (int i = 0; i < lines_rows; i++)
    free(lines[i]);
  free(lines);

  int total_sum = 0;
  // For each page update
  for (int i = 0; i < updates_num; i++) {
    printf("Processing update %d:\n", i);
    printUpdate(updates[i]);
    while (!updates[i]->isCompliant) {
      fixUpdate(rules, rules_num, updates[i]);
    }
    printf("After fixing:\n");
    printUpdatePages(updates[i]);
    int middle_page = getMiddlePage(updates[i]);
    printf("Middle page: %d\n", middle_page);
    total_sum += middle_page;
    printf("Total sum so far:: %d\n", total_sum);

    // for each page in the page update
    /*updates[i]->isCompliant = true;*/
    /*int pages_length = updates[i]->pages_length;*/
    /*for (int j = 0; j < pages_length; j++) {*/
    /*  int page = updates[i]->pages[j];*/
    /**/
    /*  // for each rule*/
    /*  for (int k = 0; k < rules_num; k++) {*/
    /*    int left = rules[k]->left;*/
    /*    int right = rules[k]->right;*/
    /*    if (page == right) {*/
    /*      for (int z = j; z < pages_length && updates[i]->isCompliant ==
     * true;*/
    /*           z++) {*/
    /*        if (left == updates[i]->pages[z]) {*/
    /*          updates[i]->isCompliant = false;*/
    /*          printUpdate(updates[i]);*/
    /*          break;*/
    /*        }*/
    /*      }*/
    /*    }*/
    /*    if (page == left) {*/
    /*      // test the rule's right page on the pages to the left*/
    /*      for (int z = j; z >= 0 && updates[i]->isCompliant == true; z--) {*/
    /*        if (right == updates[i]->pages[z]) {*/
    /*          updates[i]->isCompliant = false;*/
    /*          printUpdate(updates[i]);*/
    /*          break;*/
    /*        }*/
    /*      }*/
    /*    }*/
    /*  }*/
    /*}*/

    /*if (updates[i]->isCompliant == true) {*/
    /*  // get middle page and add to total sum*/
    /*  int middle_page = getMiddlePage(updates[i]);*/
    /*  printUpdate(updates[i]);*/
    /*  printf("\tMiddle page: %d\n", middle_page);*/
    /*  total_sum += middle_page;*/
    /**/
    /*  // end*/
    /*} else {*/
    /*  printf("Remedying update %d: ", TO_ONE_BASED(i));*/
    /*  printUpdatePages(updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  fixUpdate(rules, rules_num, updates[i]);*/
    /*  int middle_page = getMiddlePage(updates[i]);*/
    /*  printUpdate(updates[i]);*/
    /*  printf("\tMiddle page: %d\n", middle_page);*/
    /*  total_sum += middle_page;*/
    /*}*/
  }

  /*printf("START Fixed pages: \n");*/
  /*for (int i = 0; i < updates_num; i++) {*/
  /*  printUpdatePages(updates[i]);*/
  /*}*/
  /*printf("END Fixed pages: ");*/

  printf("Total sum of middle pages of all updates update: %d\n", total_sum);
  printf("To get part 2, subtrace part 1 from this number.\n");

  // Free allocated memory
  arena_free(mem);

  return EXIT_SUCCESS;
}
