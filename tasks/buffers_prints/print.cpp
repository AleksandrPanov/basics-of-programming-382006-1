#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

int main()
{
    setlocale(0, "");
    int button;
    while (true)
    {
        if (_kbhit())
        {
            button = _getch();
            int extendent = 0;
            if (button)
            {
                cout << "������ ������ " << button << " " <<(char)button << endl;
            }
        }
        else
        {
            //cout << "������ �� ������" << endl;
        }
        Sleep(50);
    }
    return 0;
}