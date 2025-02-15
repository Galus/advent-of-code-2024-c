#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define DIFF_MIN 0
#define DIFF_MAX 3

char *find_next_number_verbose(char *current_position, char *line) {
    printf("Finding the next number of [%s]", current_position);
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        return NULL;
    }

    // moves off curr number
    printf("moving past the current number.\n");
    while (*curr_pos && !isspace(*curr_pos)) curr_pos++;
    printf("curr_pos: [%s]\n", curr_pos);

    // move past whitespace
    printf("moving past the whitespace.\n");
    while (*curr_pos && isspace(*curr_pos)) curr_pos++;
    printf("curr_pos: [%s]\n", curr_pos);

    if (*curr_pos == '\0') {
        return NULL;
    } 

    return curr_pos;
}

char *find_previous_number_verbose(char *current_position, char *line) {
    printf("Finding the previous number of [%s]", current_position);
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        return NULL;
    }

    printf("curr_pos: [%s]\n", curr_pos);
    printf("Going back past non whitespace\n");
    while (*curr_pos && !isspace(*curr_pos)) curr_pos--; // moves off curr number
    if (curr_pos == line) {
        printf("We are at the start of the line.");
        return curr_pos;
    }
    printf("curr_pos: [%s]\n", curr_pos);

    printf("Going back past whitespace; to end of prev number.\n");
    while (*curr_pos && isspace(*curr_pos)) curr_pos--; // moves past whitespace
    if (curr_pos == line) {
        printf("We are at the start of the line.\n");
        printf("curr_pos: [%s]\n", curr_pos);
        return curr_pos;
    }

    printf("curr_pos: [%s]\n", curr_pos);

    printf("Going back past non whitespace; to front of prev number\n");
    while (*curr_pos && !isspace(*curr_pos)) curr_pos--; // moves prev number
    curr_pos++;
    if (curr_pos == line) {
        printf("We are at the start of the line.");
        printf("curr_pos: [%s]\n", curr_pos);
        return curr_pos;
    }
    
    /*printf("curr_pos: [%s]\n", curr_pos);*/

    /*printf("Going back onto the first char of the prev number\n");*/
    /*while (*curr_pos && isspace(*curr_pos)) curr_pos++; // moves past whitespace*/
    /*if (curr_pos == line) {*/
    /*    printf("We are at the start of the line.\n");*/
    /*    printf("curr_pos: [%s]\n", curr_pos);*/
    /*    return curr_pos;*/
    /*}*/

    printf("curr_pos: [%s]\n", curr_pos);

    if (curr_pos == line && isspace(*curr_pos)) {
        return NULL;
    }

    return curr_pos;
}

char *find_next_number(char *current_position, char *line) {
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        return NULL;
    }

    // moves off curr number
    while (*curr_pos && !isspace(*curr_pos)) curr_pos++;

    // move past whitespace
    while (*curr_pos && isspace(*curr_pos)) curr_pos++;

    if (*curr_pos == '\0') {
        return NULL;
    } 

    return curr_pos;
}

char *find_previous_number(char *current_position, char *line) {
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        return NULL;
    }

    while (*curr_pos && !isspace(*curr_pos)) curr_pos--; // moves off curr number
    while (*curr_pos && isspace(*curr_pos)) curr_pos--; // moves past whitespace

    while (*curr_pos && !isspace(*curr_pos)) curr_pos--; // moves prev number
    while (*curr_pos && isspace(*curr_pos)) curr_pos--; // moves past whitespace

    while (*curr_pos && !isspace(*curr_pos)) curr_pos++; // moves to start of number 
    
    if (curr_pos == line && isspace(*curr_pos)) {
        return NULL;
    }

    return curr_pos;
}

int is_last_number_verbose(char *current_position, char *line) {
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        printf("Received NULL input. Returning 0.\n");
        return 0;
    }

    char *next_number = find_next_number(curr_pos, line);

    if (next_number == NULL) {
        printf("\t LAST Number.\n");
        return 1;
    }

    if (*next_number == '\0' || *next_number == '\n') {
        printf("\t LAST Number. (type 2)\n");
        return 1;
    }

    for (int i = 0; *next_number != '\0'; next_number++) {
        printf("Hex value of '%c': 0x%X\n", *next_number, *next_number);
    }

    printf("returning 0 with next char * hex: 0x%p\n", (void*)next_number);
    return 0;
}

int is_last_number(char *current_position, char *line) {
    char *curr_pos = current_position;

    if (curr_pos == NULL || line == NULL) {
        return 0;
    }

    char *next_number = find_next_number(curr_pos, line);

    if (next_number == NULL) {
        return 1;
    }

    if (*next_number == '\0' || *next_number == '\n') {
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[999];
    int total_safe = 0;
    int line_num = 0;
    int dampener_used = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        dampener_used = 0; // refresh the dampener per level (line)
        line_num++;
        int prev = 0;

        char *token = line;
        int curr;
        int direction = 0;

        // go number by number (manually manipulating char pointer 'token' position)
        while (sscanf(token, "%d", &curr) == 1) {

            // Check if prev has been set (not the first number)
            if (prev != 0) {
                int diff = prev - curr;

                if (diff == 0) {

                    // check if we are at the end of the line
                    if (is_last_number(token, line)) {
                        /*printf("prev: %d curr: %d line %s", prev, curr, line);*/
                        /*printf("We are at the end of the line.\n");*/
                        dampener_used = 1; // useless (gets reset anyway), but doing it anyway.
                        goto safe_next_line;
                    }
                    printf("Line %d: Not safe (same numbers: %d, %d)\n", line_num, prev, curr);
                    if (dampener_used == 1) {
                        goto next_line;
                    } else {
                        dampener_used = 1;
                        // check if this is the last number
                        goto next_number;
                    }

                }

                if (abs(diff) > DIFF_MAX) {
                    if (is_last_number(token, line)) {
                        /*printf("prev: %d curr: %d line %s", prev, curr, line);*/
                        /*printf("We are at the end of the line.\n");*/
                        dampener_used = 1; // useless (gets reset anyway), but doing it anyway.
                        goto safe_next_line;
                    }
                    printf("Line %d: Not safe (difference too large: %d, %d)\n", line_num, prev, curr);
                    if (dampener_used == 1) {
                        goto next_line; 
                    } else {
                        dampener_used = 1;
                        goto next_number;
                    }
                }

                if (direction == 0) {
                    direction = diff/abs(diff);
                } else {
                    if (direction != diff/abs(diff)) {
                        if (is_last_number(token, line)) {
                            /*printf("prev: %d curr: %d line %s", prev, curr, line);*/
                            /*printf("We are at the end of the line.\n");*/
                            dampener_used = 1; // useless (gets reset anyway), but doing it anyway.
                            goto safe_next_line;
                        } else {
                            /*printf("Line %d: %s", line_num, line);*/
                            /*printf("curr: %d is not last number.\n", curr);*/
                        }

                        if (dampener_used == 1) {
                            printf("Line %d: Not safe (monotonicity broken: %d, %d)\n", line_num, prev, curr);
                            printf("Monacity fails at %d. ", curr);
                            printf("Dampener already used.\n");
                            goto next_line;
                        } else {
                            dampener_used = 1;
                            // check 2 vals back
                            printf("Line %d: [%s]\n", line_num, line);
                            printf("curr: %d\n", curr);

                            int nprev;
                            char *prev_num = find_previous_number_verbose(token,line);
                            sscanf(prev_num, "%d", &nprev);
                            printf("prev_num: %d.\n", nprev);

                            int nprevprev;
                            char *prev_prev_num = find_previous_number_verbose(prev_num,line);
                            sscanf(prev_prev_num, "%d", &nprevprev);
                            printf("prev_prev_num: %d.\n", nprevprev);

                            /*if (prev_prev_num > line) {*/
                            /*    sscanf(prev_prev_num, "%d", &prev);*/
                            /*} else {*/
                            /*    printf("We are at the start of the line.\n");*/
                            /*}*/
                            /**/
                            //char *prev_prev_prev_num = find_previous_number(prev_prev_num,line);
                            /*printf("token: [%s]", token);*/
                            /*printf("prev_num: [%s]", prev_num);*/
                            /*printf("prev_prev_num: [%s]", prev_prev_num);*/
                            /*printf("line: [%s]", line);*/

                            prev = nprevprev;
                            if (prev_prev_num == line) {
                                diff = prev - curr;
                                printf("reset the monotonic direction b/c comparing with start of line %d\n",nprevprev);
                                direction = diff/abs(diff);
                            }
                            printf("Recalculated diff: %d \tprev: %d\tcurr %d\n", diff, prev, curr);
                            /*// restore token*/
                            /*token = curr_pos;*/
                            if (direction != diff/abs(diff)) {
                                // check the prev and the next instead
                                prev = nprev;
                                char *next_num = find_next_number_verbose(token, line);
                                int nnext;
                                sscanf(next_num, "%d", &nnext);
                                printf("next number: %d\n", nnext);
                                diff = prev - nnext;
                                if (direction != diff/abs(diff)) {
                                    printf("Line %d: (Dampened) Not safe (monotonicity broken: %d, %d)\n", line_num, prev, curr);
                                    goto next_line;
                                } else {
                                    curr = nnext;
                                }
                            }
                        }
                    }

                }
            }
            prev = curr;

            next_number:;

            // move forward to start of next number
            while (*token && !isspace(*token)) token++;
            while (*token && isspace(*token)) token++;
        }

        safe_next_line:;
        total_safe++;

        next_line:;
    }

    fclose(fp);
    printf("Total safe lines: %d\n", total_safe);
    return 0;
}
