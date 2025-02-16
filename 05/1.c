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

  printf("after:\nlines_rows: %d\n", lines_rows);
  printf("line 0: %s\n", lines[0]);
  printf("line 1: %s\n", lines[1]);
  printf("-\\-\n");
  printf("line n-1: %s\n", lines[lines_rows - 2]);
  printf("line n: %s\n", lines[lines_rows - 1]);

  // details
  // lines now has both 'page ordering rules' and 'pages to produce in each
  // update' X|Y => page X must be printed at some point before page Y as part
  // of an update

  // split lines into 'page order rules' and 'pages to produce' in each update
  // figure out where the split is (the empty line)
  int emptyline_idx = 0;
  while(strcmp("", lines[emptyline_idx]) != 0) emptyline_idx++;
  printf("Found break at line %i\n", emptyline_idx);

  // allocate memory for the two new 2d-arrays
  printf("Allocating memory for page_rules\n");
  // (rules_num is human 1-based, emptyline_idx is array 0-based)
  int rules_num = emptyline_idx; 
  printf("Created rules_num %d\n", rules_num);
  char **page_rules = malloc(sizeof(char *) * rules_num + 10);
  printf("Allocated %lu memory for **page_rules\n", sizeof(char *) * rules_num);
  if (page_rules == NULL) {
    perror("page_rules malloc failed");
    return EXIT_FAILURE;
  }

  // PAGE RULES
  for (int i = 0; i < rules_num; i++) {
    /*printf("rules: %d/%d Allocating memory for string itself.\n", i,
     * rules_num-1);*/
    page_rules[i] = malloc(sizeof(char) * strlen(lines[i]) + 10);
    if (page_rules[i] == NULL) {
      perror("page_rules[i] malloc failed");
      for (int k = 0; k < i; k++) {
        free(page_rules[k]);
      }
      free(page_rules);
      return EXIT_FAILURE;
    }

    /*printf("Copying data into page_rules.\n");*/
    strcpy(page_rules[i], lines[i]);
    printf("DEBUG page_rules[%d]: %s\n", i, page_rules[i]);
  }

  /*page_rules[rules_num] = "\0";*/
  printf("Created page_rules[]\n");

  printf("Allocating memory for page_updates\n");
  int updates_num = lines_rows - emptyline_idx - 1;
  char **page_updates = malloc(sizeof(char *) * updates_num);
  if (page_updates == NULL) {
    perror("page_updates malloc failed");
    return EXIT_FAILURE;
  }

  printf("\n");

  // PAGE UPDATES
  int j = emptyline_idx + 1;
  printf("emptyline_idx: %d\n", emptyline_idx);
  for (int i = 0; i < updates_num; i++) {
    size_t allocsize = (sizeof(char) * strlen(lines[j]) + 1);
    printf("updates: %d/%d Allocating %lu for string itself.\n", i, j, allocsize);
    page_updates[i] = malloc(sizeof(char) * strlen(lines[j]) + 20);

//    if (page_updates[i] == NULL) {
//      perror("page_updates[i] malloc failed");
//      for (int k = 0; k < i; k++) {
//        free(page_updates[k]);
//      }
//      for (int l = 0; l < rules_num; l++) {
//        free(page_rules[l]);
//      }
//      free(page_rules);
//      free(page_updates);
//    }

    /*printf("Copying data into page_updates.\n");*/
    /*printf("i + emptylines = %d + %d = %d\n", i, emptyline_idx,
     * i+emptyline_idx);*/
    strcpy(page_updates[i], lines[j]);
    if(i == updates_num-1) {
      printf("ARSHISGAY: %s\n", page_updates[i]);
    }
    printf("DEBUG page_updates[%d]: %s\n", i, page_updates[i]);
    j++;
  }
  /*page_updates[updates_num] = "\0";*/
  printf("Populated %d page_updates\n", updates_num);

  printf("r0 %s\n", page_rules[0]);
  printf("rn %s\n", page_rules[rules_num-1]);
  printf("u0 %s\n", page_updates[0]);
  printf("un %s\n", page_updates[updates_num - 1]);

  for(int i = 0; i < updates_num; i++) {
    printf("page_updates[%d]: %s\n", i, page_updates[i]);
  }

  /*184/1361 Allocating 63 for string itself.*/
  /*[184]: 14,25,77,83,56,58,24,29,42,45,75,47,87,22,18,94,26,97,65,92,17*/
  /*185/1362 Allocating 45 for string itself.*/
  /*[185]: 65,26,38,62,24,29,94,45,84,17,51,42,52,87,28*/


  // For each page update
  for(int i = 0; i < updates_num; i++) {

    // convert from CSV string to Array of ints (decided to just strtok parse it instead)
    /*int page_update_length = strlen(page_updates[i]); // over-estimate b/c bytes>chars*/
    // note: This is magical for me because I am not allocating memory
    // and strtok magically maintains the current strings pointer for me!
    char *token=strtok(page_updates[i], ",");

    // for each page (skip first page)
    token = strtok(NULL, ",");

    // for each page in the page update
    while(token != NULL) {
      int page = atoi(token);
      printf("Page Update, Page (%d, %d)\n", i, page);

      // for each rule
      for (int j = 0; j < rules_num; j++) {
        printf("Rule %d: ", TO_ONE_BASED(j));
        int left;
        int right;
        if (sscanf(page_rules[j], "%d|%d", &left, &right) == 2) {
          printf(" %d|%d\n", left, right);
        }
        if (page == left) {
          // test the rule's right page on the pages to the left
          //  At this point I knew i should have converted page_updates from char ** to int **

        }
        if (page == right) {

        }
        //     for each page T of page update
        //       if Tptr < Nptr
        //         test rule | 'N|T' does not exist
        //       if Tptr > Nptr
        //         test rule | 'T|N' does not exist
        //       apply rule to page update
        //       if not safe go to next page

      }


      token = strtok(NULL, ",");
    }


    

  }
  // 

  // Free allocated memory
  for (int i = 0; i < lines_rows; i++) {
    /*printf("Freeing lines[%d]\n", i);*/
    free(lines[i]);
  }
  free(lines);
  return EXIT_SUCCESS;
}
