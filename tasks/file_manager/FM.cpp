#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

#include <stdio.h>
// ����� 1, ����� ������������ ������������ �����
/*int main(void)
{
    setlocale(LC_ALL, "Russian"); // ��������� �������� �����
    printf("������!\n");
    wchar_t name[MAX_PATH];
    //GetModuleFileName(GetModuleHandle(0), name, MAX_PATH); // ��������� � name ��� ������������ ����� � ������ ���� �� ����
    GetCurrentDirectory(MAX_PATH, name); // ��������� � name ������ ���� �� ������� �����
    wprintf(name);
    system("pause");
    return 0;
}*/

// ����� 2, ������ ������ � �����
#include <iostream>
int main()
{
    WIN32_FIND_DATAW wfd;
    HANDLE const hFind = FindFirstFileW(L"C:\\All\\�����-������\\�����\\..\\..\\*", &wfd);
    setlocale(LC_ALL, "");
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            wprintf(wfd.cFileName);
            printf(" %lu\n", wfd.dwFileAttributes);
            //��� ���������� �� ������ ����
            //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataa
        } while (FindNextFileW(hFind, &wfd) != NULL);

        FindClose(hFind);
    }
    printf("\n\n\n");
    return 0;
}

// ������ ������ � ����� � ������� c++
/*using namespace std;
#include <iostream>
#include <string>
#include <filesystem>
int main() 
{
    setlocale(LC_ALL, "");
    std::string path = "C:\\Users\\��� ������������";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::cout << entry.path() << std::endl;
    }
}*/
