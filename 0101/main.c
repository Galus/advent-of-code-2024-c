#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 9999

int compare_integers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b); // Ascending order
    // return (*(int *)b - *(int *)a); // Descending order
}

int main(int argc, char *argv[]) {
	printf("hello %s", "Mario");

	FILE *fp;
	char f[99];
	int arr1[MAX_SIZE];
	int arr2[MAX_SIZE];
	int diff[MAX_SIZE];
	int i = 0;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		return 1;
	}

	while(fscanf(fp, "%d %d", &arr1[i], &arr2[i]) == 2 && i < MAX_SIZE) {
		i++;
	}

	printf("Array 1:\n");
	for (int j = 0; j < i; j++) {
		printf("%d ", arr1[j]);
	}
	printf("\n");

	//int num_elements = sizeof(arr1) / sizeof(arr1[0]);
	int num_elements = i;
	printf("we have %d elements.\n", num_elements);

	/*printf("Before:\n");*/
	/*for (int i = 0; i < 5; i++) {*/
	/*	printf(" %d ", arr1[i]);*/
	/*}*/

	printf("\n");
	qsort(arr1, num_elements, sizeof(int), compare_integers);
	qsort(arr2, num_elements, sizeof(int), compare_integers);

	int sum = 0;
	for (int j = 0; j < i; j++) {
		sum = sum + abs(arr1[j]-arr2[j]);
	}

	printf("The magic number is: [%d]", sum);

	/*printf("After:\n");*/
	/*for (int i = 0; i < 5; i++) {*/
	/*	printf(" %d ", arr1[i]);*/
	/*}*/

	/*printf("\n");*/
	/*printf("Minimum value of int: %d\n", INT_MIN);*/
	/*printf("Maximum value of int: %d\n", INT_MAX);*/

	return 0;

}

