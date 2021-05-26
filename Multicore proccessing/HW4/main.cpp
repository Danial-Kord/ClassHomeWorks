#include <iostream>
#include <omp.h>

using namespace std;

#define N 256


void multiply(int **mat1,
              int **mat2,
              int **res)
{
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}
void add(int **mat1,
              int **mat2,
              int **res)
{


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void parallel_add(int **mat1,
         int **mat2,
         int **res)
{

#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void parallel_multiply(int **mat1,
              int **mat2,
              int **res)
{
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int k = 0; k < N; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

void set_initial_values(int **mat){
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            mat[j][i] = rand()%100;
        }
    }
}

void print_matrix(int **mat){
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            printf("%d ",mat[j][i]);
        }
        printf("\n");
    }
}

int ** build_matrix(){
    int **arr = (int **)malloc(N * sizeof(int *));
    for (int i=0; i<N; i++)
        arr[i] = (int *)malloc(N * sizeof(int));
    return arr;
}

//all matrix arrays are square type
int** matrix_transposer(int **mat){
    int **matrix = build_matrix();
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            int a = mat[j][i];
            matrix[j][i] = mat[i][j];
            matrix[i][j] = a;
        }
    }
    return matrix;
}

//calculate serial 𝑅 = ((𝐴𝑇 × 𝐴) + (𝐵 × 𝐴)) × 𝐶𝑇 = ((𝐴𝑇 + B) * A) * CT
void serial_equation_calc(int** A,int** B,int** CT){
    int **AT = matrix_transposer(A);
    int **temp1 = build_matrix();
    int **temp2 = build_matrix();


    double start;
    double end;
    start = omp_get_wtime();
    //start
    add(AT,B,temp1);
    multiply(temp1,A,temp2);
    multiply(temp2,CT,temp1);
    int **result = temp1;
    //end
    end = omp_get_wtime();
    printf("Serial Work took %f seconds\n", end - start);
}

//calculate parallel 𝑅 = ((𝐴𝑇 × 𝐴) + (𝐵 × 𝐴)) × 𝐶𝑇 = ((𝐴𝑇 + B) * A) * CT
void parallel_equation_calc(int** A,int** B,int** CT){
    int **AT = matrix_transposer(A);
    int **temp1 = build_matrix();
    int **temp2 = build_matrix();


    double start;
    double end;
    start = omp_get_wtime();
    //start
    parallel_add(AT,B,temp1);
#pragma omp barrier
    parallel_multiply(temp1,A,temp2);
#pragma omp barrier
    parallel_multiply(temp2,CT,temp1);
    int **result = temp1;
    //end
    end = omp_get_wtime();
    printf("Parallel Work took %f seconds\n", end - start);
}

void copy_matrix(int ** from,int** to){
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            to[i][j] = from[i][j];
        }
    }
}

void deadlock_simulator(){
    for (int test=0;test<20;test++) {
        omp_lock_t locka,lockb;

        omp_init_lock(&locka);
        omp_init_lock(&lockb);
        int a =2,b=2;
#pragma omp parallel shared(locka, lockb, a, b)
#pragma omp sections nowait
        {
#pragma omp section
            {
                omp_set_lock(&locka);
                printf("thread :%d => lock a\n",omp_get_thread_num());

                for (int i = 0; i < 1000; ++i) {
                    a *= ((a * 10) / 26 + 7) / (1 + a) + 3;
                }
                omp_set_lock(&lockb);

                printf("thread :%d => lock b\n",omp_get_thread_num());
                for (int i = 0; i < 1000; ++i) {
                    b *= (((a + b) * 10) / 26 + 7) / (1 + b) + 3;
                }
                omp_unset_lock(&lockb);
                printf("thread :%d => released b\n",omp_get_thread_num());
                omp_unset_lock(&locka);
                printf("thread :%d => released a\n",omp_get_thread_num());
            }

#pragma omp section
            {
                int temp=0;
                int f = rand()%20000000 + 200000;
                for (int i = 0; i < f; ++i) {
                    temp++;
                    temp*=20;
                    temp/=4;
                    temp-=3;
                }
                omp_set_lock(&lockb);
                printf("thread :%d => lock b\n",omp_get_thread_num());

                for (int i = 0; i < 1000; ++i) {
                    b *= ((b * 10) / 26 + 7) / (1 + b) + 3;
                }
                omp_set_lock(&locka);
                printf("thread :%d => get a\n",omp_get_thread_num());


                for (int i = 0; i < 1000; ++i) {
                    b *= (((a + b) * 10) / 26 + 7) / (1 + b) + 3;
                }
                omp_unset_lock(&locka);
                printf("thread :%d => released a\n",omp_get_thread_num());
                omp_unset_lock(&lockb);
                printf("thread :%d => released b\n",omp_get_thread_num());

            }
        }  /* end of sections */
        /* end of parallel region */


#pragma omp barrier

        printf("program ended without deadlock on try:%d\n",test);
    }
}


int main() {
    int **matrix1 = build_matrix();
    int **matrix2 = build_matrix();
    int **matrix3 = build_matrix();

    set_initial_values(matrix1);
    set_initial_values(matrix2);
    set_initial_values(matrix3);

    //    make another matrix so we wont get the benefit from cache hit of the last process
    int **matrix11 = build_matrix();
    int **matrix22 = build_matrix();
    int **matrix33 = build_matrix();

    copy_matrix(matrix1,matrix11);
    copy_matrix(matrix2,matrix22);
    copy_matrix(matrix3,matrix33);
//
    parallel_equation_calc(matrix11,matrix22,matrix33);
//
    serial_equation_calc(matrix1,matrix2,matrix3);


    deadlock_simulator();
    return 0;
}
