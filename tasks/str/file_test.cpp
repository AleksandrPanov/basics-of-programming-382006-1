#include <stdio.h>
#include <cstdlib>

enum Errors {
    INDEX_OUT_OF_BOUNDS = 1,
    FILE_NOT_FOUND,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    OUT_OF_MEMORY
};
//рассказать про fprintf
/*int main()
{
    //fix fopen by: https://stackoverflow.com/questions/22450423/how-to-use-crt-secure-no-warnings
    FILE* fp = fopen("test.txt", "r"); //где fp - это переменная типа FILE*

    if (fp == NULL) 
    {
        puts("Cannot open file.");
        return Errors::FILE_NOT_FOUND;
    }
    const int num = 1024;
    char line[num];
    while (fgets(line, num, fp)) //Maximum number of characters to be copied into str (including the terminating null-character).
    {
        printf("%s", line);
    }
    //int fputs(const char *str, FILE *stream) для записи строк

    //size_t fread(void *buf, size_t size, size_t count, FILE *stream)
    //size_t fwrite(const void *buf, size_t size, size_t count, FILE *stream)

    fclose(fp);
    system("pause");
    return 0;
}*/

/*int main()
{
    FILE* fp = freopen("test.txt", "r", stdin);
    if (fp == NULL)
    {
        puts("Cannot open file.");
        return Errors::FILE_NOT_FOUND;
    }
    const int num = 1024;
    char line[num];
    while (scanf("%s", line) != -1)
    {
        printf("%s\n", line);
    }
    fclose(fp);
    system("pause");
    return 0;
}*/

//http://www.michurin.net/computer-science/rsa.html