/*
*				In His Exalted Name
*	Title:	Prefix Sum Sequential Code
*	Author: Ahmad Siavashi, Email: siavashi@aut.ac.ir
*	Date:	29/04/2018
*/





#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>

void omp_check();
void fill_array(int *a, size_t n);
void prefix_sum(int *a, size_t n);
void prefix_sum_method1_parallel(int *a, size_t n);
void print_array(int *a, size_t n);


void scanHillisSteele(int *x,int *y, int n) {

    if (n == 1) {
        return;
    }
    int  j, k;
    int  *temp;


    for (k = 1, j = 0; k < n; k <<= 1, j++) {
#pragma omp parallel for
        for (int i = 0; i < k; i++) {
            y[i] = x[i];
        }
#pragma omp parallel for
        for (int i = k; i < n; i++) {
            y[i] = x[i - k] + x[i];
        }

        temp = x;
        x = y;
        y = temp;
    }

    if (j % 2 == 0) {
        free(y);
    }
}






int main(int argc, char *argv[]) {

	// Check for correct compilation settings
	omp_check();
	// Input N
	size_t n = 0;
	printf("[-] Please enter N: ");
	scanf("%uld\n", &n);
	// Allocate memory for array
	int * a = (int *)malloc(n * sizeof a);
    int *y = (int*) malloc(sizeof (int*) * n);
	// Fill array with numbers 1..n
	fill_array(a, n);



	// Print array
//	print_array(a, n);
	// Compute prefix sum

    double start;
    double end;
    start = omp_get_wtime();
//    prefix_sum(a, n);
//    prefix_sum_method1_parallel(a,n);
    scanHillisSteele(a,y, n);
    end = omp_get_wtime();
    printf("Parallel Work took %f seconds\n", end - start);


	// Print array
//	print_array(a, n);
	// Free allocated memory
	free(a);
	return EXIT_SUCCESS;
}


void prefix_sum_method1_parallel(int *a, size_t n){
    bool check = false;
    int step;
    int mod;
#pragma omp parallel
    {
#pragma single
        {
            step = (n - 1) / omp_get_num_threads();
            mod = (n - 1) % omp_get_num_threads();
        }

#pragma omp for firstprivate(check) schedule(static)
        for (int i = 1; i < n; i++) {
            if (!check && i != 1) {
                check = true;
                // printf("%d\n",i);
            } else {
                check = true;
                a[i] = a[i] + a[i - 1];
            }
        }
    }
#pragma omp barrier

        int index = step+1;
        if(mod != 0){
            index += 1;
            mod -= 1;
        }
//    printf("%d %d",n,index);

    while (index < n){
//            printf("ss");
            int target;
            if(mod != 0){
                target = index + step + 1;
                mod--;
            } else{
                target = index + step;
            }
#pragma omp parallel for
            for (int i = index; i < index + step; i++) {
                a[i] += a[index -1];
            }
            index = target;
        }
}

void prefix_sum(int *a, size_t n) {

        int i;
        for (i = 1; i < n; ++i) {
            a[i] = a[i] + a[i - 1];
        }

}

void print_array(int *a, size_t n) {
	int i;
	printf("[-] array: ");
	for (i = 0; i < n; ++i) {
		printf("%d, ", a[i]);
	}
	printf("\b\b  \n");
}

void fill_array(int *a, size_t n) {
	int i;
	for (i = 0; i < n; ++i) {
		a[i] = i + 1;
	}
}

void omp_check() {
	printf("------------ Info -------------\n");
#ifdef _DEBUG
	printf("[!] Configuration: Debug.\n");
#pragma message ("Change configuration to Release for a fast execution.")
#else
	printf("[-] Configuration: Release.\n");
#endif // _DEBUG
#ifdef _M_X64
	printf("[-] Platform: x64\n");
#elif _M_IX86 
	printf("[-] Platform: x86\n");
#pragma message ("Change platform to x64 for more memory.")
#endif // _M_IX86 
#ifdef _OPENMP
	printf("[-] OpenMP is on.\n");
	printf("[-] OpenMP version: %d\n", _OPENMP);
#else
	printf("[!] OpenMP is off.\n");
	printf("[#] Enable OpenMP.\n");
#endif // _OPENMP
	printf("[-] Maximum threads: %d\n", omp_get_max_threads());
	printf("[-] Nested Parallelism: %s\n", omp_get_nested() ? "On" : "Off");
#pragma message("Enable nested parallelism if you wish to have parallel region within parallel region.")
	printf("===============================\n");
}
