#include "check_utilities.h"
#include <algorithm>
void gauss_forward(double** A, double* b, const int n)
{
}
void gauss_backward(double** A, double* b, double *x, const int n)
{
}
int main()
{
    double** A, ** copy_A, * b, * copy_b, * x;
    const int n = 3000;
    A = new double* [n];
    copy_A = new double* [n];
    for (int i = 0; i < n; i++)
    {
        A[i] = new double[n];
        copy_A[i] = new double[n];
    }
    b = new double[n];
    copy_b = new double[n];
    x = new double[n];
    generate(A, b, n);
    copy(A, copy_A, n);
    copy(b, copy_b, n);
    //print(A, n);
    //print(b, n);
    //printf("\n");
    gauss_forward(A, b, n);
    gauss_backward(A, b, x, n);
    //print(A, n);
    //print(b, n);
    //printf("\n");
    //print(x, n);
    printf("%lf", check(copy_A, x, copy_b, n));
    return 0;
}