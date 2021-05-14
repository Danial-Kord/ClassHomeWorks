#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <cstdlib>
#include <iostream>

using namespace std;

#ifndef _OPENMP
fprintf( stderr, "openmp is not supported");
	exit( 0 );
#endif



void MyRank() {
    int my_rank = omp_get_thread_num();
    printf("%d\n",my_rank);

}

void questionOneSlowSpeedApproach(){
    int acc=0,i;

#  pragma omp parallel
    {

#pragma omp for
        for (i=0; i < 10000000; i++) {
#pragma omp atomic
            acc++;
        }

    }
    printf("acc: %d\n",acc);
}

void questionOne(){
    int temp=0;
    int acc=0,i;
    double start;
    double end;
    start = omp_get_wtime();
#  pragma omp parallel private(temp)
    {
        temp = 0;
        #pragma omp for
        for (i=0; i < 10000000; i++) {
            temp++;
        }
        #pragma omp atomic
        acc+=temp;
    }
    printf("acc: %d\n",acc);
    end = omp_get_wtime();
    printf("Work took %f seconds\n", end - start);
}

void questionTwoOldMethod(){

    double start;
    double end;
    start = omp_get_wtime();

    int E[300][300] = {0};
#  pragma omp parallel for
        for (int i=0; i < 650; i++) {
            for (int j=0; j < 650; j++) {
                E[j][i] = i;
            }
        }

    end = omp_get_wtime();
    printf("Work took %f seconds\n", end - start);
}

void questionTwoOptimized(){
    int r = 7000, c = 7000;
    int **arr = (int **)malloc(r * sizeof(int *));
    for (int i=0; i<r; i++)
        arr[i] = (int *)malloc(c * sizeof(int));

    for (int i=0; i < r; i++) {
        for (int j=0; j < c; j++) {
            arr[j][i] = 0;
        }
    }
    double start;
    double end;
    start = omp_get_wtime();

#  pragma omp parallel for
    for (int i=0; i < r; i++) {
        for (int j=0; j < c; j++) {
            arr[i][j] = j;
        }
    }
#pragma omp barrier
    end = omp_get_wtime();

    printf("Work took %f seconds\n", end - start);
}


void questionThree(){
    int E = 0;
    omp_set_num_threads(4);
    int arr_size = 929;
    int *arr = (int*)malloc(sizeof (int)*arr_size);

    double start;
    double end;
    start = omp_get_wtime();

#pragma omp parallel
    {
     int workload_size = arr_size/4;
     int start = omp_get_thread_num() * workload_size;
     int end;
     if(omp_get_thread_num() == omp_get_num_threads()){
         end = workload_size;
     } else{
         end = start + workload_size;
     }
        for (int i = start; i < end; i++) {
            arr[i] = 0;
        }
    }
    end = omp_get_wtime();

    printf("Work took %f seconds\n", end - start);
}


int main(int argc, char* argv[]) {

    questionThree();
    return 0;

}


