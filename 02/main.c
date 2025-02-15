#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // read file til newline
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
            return 1;
    }
    char line[999];
    int total_safe = 0;
    int line_num = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;
        int curr;
        int prev;
        char *token = line; // iterator
        int monotonic = -1;
        int increasing = -1;
        int decreasing = -1;
        while (sscanf(token, "%d", &curr) == 1) {
            int diff;
            if (prev) {
                // calc shit
                printf("prev: %d\t", prev);
                printf("curr: %d\t", curr);
                diff = prev - curr;
                printf("diff: %d\n", diff);

                if (diff == 0) {
                    printf("line %d is not safe because prev %d curr %d is the same (total_safe: %d)\n",
                           line_num,
                           prev,
                           curr,
                           total_safe);
                    prev = 0;
                    break;
                }

                if (diff > 0) {
                    if (monotonic == -1) {
                        monotonic = 1;
                        increasing = 1;
                        decreasing = 0;
                    }
                    if (monotonic) {
                        increasing = 1;
                        if (decreasing) {
                            printf("line %d: not safe, prev %d curr %d not monotonic, switch from inc to dec (total_safe: %d)\n",
                               line_num,
                               prev,
                               curr,
                               total_safe);
                            prev = 0;
                            break;
                        }
                    }
                    if (abs(diff) > 3) {
                        printf("line %d is not safe because prev %d curr %d differs too much (total_safe: %d)\n",
                           line_num,
                           prev,
                           curr,
                           total_safe);
                        prev = 0;
                        break;
                    }

                }
                if (diff < 0) {
                    if (monotonic == -1) {
                        monotonic = 1;
                        decreasing = 1;
                        increasing = 0;
                    }
                    if (monotonic) {
                        decreasing = 1;
                        if (increasing) {
                            printf("line %d: not safe, prev %d curr %d not monotonic, switch from dec to inc (total_safe: %d)\n",
                               line_num,
                               prev,
                               curr,
                               total_safe);
                            prev = 0;
                            break;
                        }
                    }
                    if (abs(diff) > 3) {
                        printf("line %d is not safe because prev %d curr %d differs too much (total_safe: %d)\n",
                           line_num,
                           prev,
                           curr,
                           total_safe);
                        prev = 0;
                        break;
                    }

                }
            }


            prev = curr;
            // move token over
            while (*token && !isspace(*token)){ // skips curr number
                token++;
            }
            while (*token && isspace(*token)){ // skips the whitespace
                token++;
            }
            if(*token == '\n') {
                total_safe++;
                prev = 0;
                break; // end of the line bub
            }
            if(*token == '\0') {
                total_safe++;
                prev = 0;
                break; // end of the string bub
            }
        }
    }

    fclose(fp);
    // init
    // curr and next
    // diff = curr - next;
    //
    // if diff positive
    //   check if its 1,2,3
    //      continue to next two numbers such that curr=next, and next=readnext()
    //   else seek forward to next line
    // if diff negative
    //   check if its -1,-2,-3
    //      continue to next two numbers such that curr=next, and next=readnext()
    //   else seek forward to next line
    printf("total safe: %d\n",total_safe);
    return 0;
}
