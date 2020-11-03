#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void print(double* A, int n)
{
    if (n > 50) return;
    for (int j = 0; j < n; j++)
        printf("%10lf ", A[j]);
    printf("\n");
}
void print(double** A, int n)
{
    if (n > 50) return;
    for (int i = 0; i < n; i++)
        print(A[i], n);
    printf("\n");
}
void generate(double** A, double* b, int n)
{
    srand(0);
    int mmin = -100, mmax = 1000;
    const int delta = mmax - mmin;
    double order = 1.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            A[i][j] = (mmin + (rand() % delta)) / order;
        b[i] = (mmin + (rand() % delta)) / order;
    }
}
void copy(double* a, double* copy_a, int n)
{
    for (int i = 0; i < n; i++)
        copy_a[i] = a[i];
}
void copy(double** A, double** copy_A, int n)
{
    for (int i = 0; i < n; i++)
        copy(A[i], copy_A[i], n);
}
void mult(double** A, double* x, double* res, int n)
{
    for (int i = 0; i < n; i++)
    {
        res[i] = 0.0;
        for (int j = 0; j < n; j++)
            res[i] += A[i][j] * x[j];
    }
}
double check(double** A, double* x, double* b, int n)
{
    double* res = new double[n];
    mult(A, x, res, n);
    //print(res, n);
    double mmax = 0.0;
    for (int i = 0; i < n; i++)
        mmax = __max(mmax, fabs(res[i] - b[i]));
    delete[] res;
    return mmax;
}