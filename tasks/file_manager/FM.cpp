#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

#include <stdio.h>
// часть 1, поиск расположения исполняемого файла
/*int main(void)
{
    setlocale(LC_ALL, "Russian"); // установка русского языка
    printf("Привет!\n");
    wchar_t name[MAX_PATH];
    //GetModuleFileName(GetModuleHandle(0), name, MAX_PATH); // сохраняет в name имя исполняемого файла и полный путь до него
    GetCurrentDirectory(MAX_PATH, name); // сохраняет в name полный путь до текущей папки
    wprintf(name);
    system("pause");
    return 0;
}*/

// часть 2, список файлов в папке
#include <iostream>
int main()
{
    WIN32_FIND_DATAW wfd;
    HANDLE const hFind = FindFirstFileW(L"C:\\All\\учеба-лекции\\архив\\..\\..\\*", &wfd);
    setlocale(LC_ALL, "");
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            wprintf(wfd.cFileName);
            printf(" %lu\n", wfd.dwFileAttributes);
            //вся информация по ссылке ниже
            //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataa
        } while (FindNextFileW(hFind, &wfd) != NULL);

        FindClose(hFind);
    }
    printf("\n\n\n");
    return 0;
}

// список файлов в папке с помощью c++
/*using namespace std;
#include <iostream>
#include <string>
#include <filesystem>
int main() 
{
    setlocale(LC_ALL, "");
    std::string path = "C:\\Users\\Все пользователи";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::cout << entry.path() << std::endl;
    }
}*/
