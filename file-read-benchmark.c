/*
   Author: Mario Galus
   Date: 02/28/2025
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#define SIMS 5

int main() {
  clock_t start, end;
  double cpu_time_used;
  double avg_cpu_time_used;
  double min_cpu_time_used = DBL_MAX;
  double max_cpu_time_used = DBL_MIN;

  clock_t start2, end2;
  double cpu_time_used2;
  double avg_cpu_time_used2;
  double min_cpu_time_used2 = DBL_MAX;
  double max_cpu_time_used2 = DBL_MIN;

  int simulations[SIMS] = {1, 10, 1000, 100000, 1000000};
  for (int i = 0; i < SIMS; i++) {
    int sims = simulations[i];
    while (sims > 0) {

      start = clock();
      FILE *fp = fopen("input.txt", "r");
      fseek(fp, 0, SEEK_END);
      long length = ftell(fp);
      /*printf("fseek length: %ld\n", length);*/
      end = clock();
      cpu_time_used = ((double)(end - start));
      if (avg_cpu_time_used == 0) {
        avg_cpu_time_used = cpu_time_used;

      } else {
        avg_cpu_time_used = (avg_cpu_time_used + cpu_time_used) / 2;
      }
      if (cpu_time_used < min_cpu_time_used)
        min_cpu_time_used = cpu_time_used;
      if (cpu_time_used > max_cpu_time_used)
        max_cpu_time_used = cpu_time_used;

      start2 = clock();
      FILE *fp2 = fopen("input.txt", "r");
      char line2[100000];
      fgets(line2, sizeof(line2), fp2);
      long length2 = strlen(line2);
      /*printf("fgets length: %ld\n", length2);*/
      end2 = clock();
      cpu_time_used2 = ((double)(end2 - start2));
      if (avg_cpu_time_used2 == 0) {
        avg_cpu_time_used2 = cpu_time_used2;

      } else {
        avg_cpu_time_used2 = (avg_cpu_time_used2 + cpu_time_used2) / 2;
      }
      if (cpu_time_used2 < min_cpu_time_used2)
        min_cpu_time_used2 = cpu_time_used2;
      if (cpu_time_used2 > max_cpu_time_used2)
        max_cpu_time_used2 = cpu_time_used2;

      fclose(fp);
      fclose(fp2);

      sims--;
    }

    printf("Simulations: %d \n", simulations[i]);
    printf("fseek: Avg cpu time: %f\n", avg_cpu_time_used);
    printf("fseek: Max cpu time: %f\n", max_cpu_time_used);
    printf("fseek: Min cpu time: %f\n", min_cpu_time_used);
    printf("fgets: Avg cpu time: %f\n", avg_cpu_time_used2);
    printf("fgets: Max cpu time: %f\n", max_cpu_time_used2);
    printf("fgets: Min cpu time: %f\n", min_cpu_time_used2);
  }

  return 0;
}
