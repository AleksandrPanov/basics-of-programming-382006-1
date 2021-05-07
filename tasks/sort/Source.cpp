// ���������� �� ���� :
// 1. Release
// 2. ����� ��������� n, ���� ���������� �������� ����� 30 ������
// 3. ������� - �������, ��� ����� ��������
// 4. � ����� �������� �������� ���������� :
// ��������������� ���������, ������, ������ ������. � �������.
// ��������� ���������� (��� ������ ������������� � ������ nlogn)

#include <stdio.h>
#include <cstdlib>
#include <locale.h>
#include <ctime>
#include <algorithm>

void copy_ar(double* ar, double* copy, int n)
{
    for (int i = 0; i < n; i++)
        copy[i] = ar[i];
}
void buble_sort(double* ar, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
        {
            if (ar[j] > ar[j + 1]) // swap ar[j], ar[j+1]
            {
                const int tmp = ar[j + 1];
                ar[j + 1] = ar[j];
                ar[j] = tmp;
            }
        }
}
void std_sort(double* ar, int n)
{
    std::sort(ar, ar + n);
}
bool check(double* ar, int n)
{
    for (int i = 0; i < n - 1; i++)
        if (ar[i + 1] < ar[i])
            return false;
    return true;
}
int get_str_len(const char* a)
{
    int count = 0;
    while (a[count] != '\0')
    {
        count++;
    }
    return count;
}
bool eq_str(const char *a, const char *b)
{
    int l1 = get_str_len(a);
    int l2 = get_str_len(b);
    if (l1 != l2) return false;
    for (int i = 0; i < l1; i++)
        if (a[i] != b[i]) return false;
    return true;
}
void (*my_sort) (double*, int);
int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian"); // ��������� �������� �����
    if (argc == 1)
    {
        my_sort = &buble_sort;
        printf("buble_sort\n");
    }
    else if (argc == 2)
    {
        if (eq_str(argv[1], "std_sort")) 
        {
            my_sort = &std_sort;
            printf("std_sort ");
        }
        else if (argv[1] == "merge") {/*my_sort = &merge;*/ }
        else
        {
            printf("bad sort");
            return -1;
        }
    }
    const int max_n = 100 * 50 * 10;
    double* ar = new double[max_n];
    double* copy = new double[max_n];
    for (int i = 0; i < max_n; i++)
        ar[i] = rand();
    int t1;

    //FILE* pfWrite = freopen("out1.txt", "w", stdout);

    for (int i = 100 * 1000; i <= max_n; i += 1000000)
    {
        printf("%d;", i);
    }
    printf("\n");

    //������������ ����������
    for (int i = 100 * 1000; i <= max_n; i += 1000000)
    {
        t1 = clock();
        // ������������
        my_sort(copy, i);
        t1 = clock() - t1;
        if (check(copy, i))
        {
            //printf("sort is true, time = %d milliseconds\n", t1);
            printf("%d;", t1);
        }
        else printf("������ ����������\n");
        copy_ar(ar, copy, i);
    }


    //fclose(pfWrite);
    //system("pause");
    return 0;
}