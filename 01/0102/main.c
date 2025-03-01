#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define MAX_SIZE 9999

int compare_integers(const void *a, const void *b) {
    int *ia = (int *)a; // Cast to integer pointers
    int *ib = (int *)b;
    return *ia - *ib;    // Difference returns <0, 0, or >0
}

int main(int argc, char *argv[]) {
	int arr1[MAX_SIZE];
	int arr2[MAX_SIZE];
	int i = 0;
	FILE *fp;
	char f[99];
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		return 1;
	}

	while(fscanf(fp, "%d %d", &arr1[i], &arr2[i]) == 2 && i < MAX_SIZE) {
		i++;
	}
	fclose(fp);


	/*for (int j = 0; j < i; j++) {*/
	/*	printf("%d ", arr1[j]);*/
	/*}*/
	printf("\n");

	int num_elements = i;
	printf("we have %d elements.\n", num_elements);

	printf("\n");
	qsort(arr1, num_elements, sizeof(int), compare_integers);
	qsort(arr2, num_elements, sizeof(int), compare_integers);

	int sum = 0;
	for (int j = 0; j < i; j++) {
		sum = sum + abs(arr1[j]-arr2[j]);
	}

	printf("The magic number is: [%d]\n", sum);

	printf("\npart2\n");

	int max = arr1[num_elements-1];
	printf("max num in arr1: %d\n", max);

	int *seen = (int *)malloc((max + 1) * sizeof(int)); // Dynamically allocate 'seen'
	if (seen == NULL) {
		perror("Memory allocation failed\n");
		return 1; // Or handle the error appropriately
	}

	// Initialize seen array.  This is CRUCIAL!
	for (int j = 0; j <= max; j++) {
		seen[j] = 0;
	}

	struct timeval stop, start;
	int score = 0;
	gettimeofday(&start, NULL);
	for (int j = 0; j < i; j++) {
		int count = 0;
		for (int k = 0; k < i; k++) {
			if (arr1[j] == arr2[k]) {
				count++;
			}
		}
		if (seen[arr1[j]] == 0) {
			seen[arr1[j]] = count * arr1[j]; 
			score += seen[arr1[j]];
		} 
	}
	gettimeofday(&stop,NULL);

	printf("part 2 score: %d\n", score);
	printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);


	// optimizing
	
	for (int j =0; j <= max; j++) {
		seen[j] = -1;
	}

	score = 0;
	gettimeofday(&start,NULL);
	for(int j=0; j < i; j++) {
		if(seen[arr1[j]]==-1) {
			int count = 0;
			for (int k=0; k<i; k++) {
				if (arr1[j] == arr2[k]) {
					count++;
				}
			}
			seen[arr1[j]] = count * arr1[j];
		}
		score += seen[arr1[j]];
	}
	gettimeofday(&stop,NULL);
	printf("optimized part2 score: [%d]\n", score);
	printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);


	free(seen);
	return 0;
}

