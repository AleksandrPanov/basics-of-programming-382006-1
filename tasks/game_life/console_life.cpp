#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <ctime>
int n, m, x = 0, y = 0, tick = 300;
char** field, ** field_next;
COORD position, newPosition;					  // объявление необходимой структуры
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// получение дескриптора устройства стандартного вывода
HANDLE cin = GetStdHandle(STD_INPUT_HANDLE);
WORD async_key(HANDLE _in) {
    INPUT_RECORD rs[128];
    DWORD n;
    if (!PeekConsoleInput(_in, rs, 128, &n))
        return WORD(~0);

    for (DWORD i = 0; i < n; ++i) {
        if ((rs[i].EventType == KEY_EVENT) && rs[i].Event.KeyEvent.bKeyDown) {
            FlushConsoleInputBuffer(_in);
            return rs[i].Event.KeyEvent.wVirtualKeyCode;
        }
    }
    return WORD(~0);
}
void updateAllConsole()
{
    system("cls");
    for (int i = 0; i < n; i++)
        fwrite(field[i], sizeof(char), m + 1, stdout);
    SetConsoleCursorPosition(hConsole, position);
}
int getCount(int x, int y)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (field[(x + i + n) % n][(y + j + m) % m] == '#')
                count++;
    return count;
}
void lifeNext()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            const char el = field[i][j];
            field_next[i][j] = el;
            int count = getCount(i, j);
            if (el == '.' && count == 3) field_next[i][j] = '#';
            else if (el == '#' && (count < 3 || count > 4)) field_next[i][j] = '.';
        }
}
void updateNext()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            field[i][j] = field_next[i][j];
            field_next[i][j] = '.';
        }
        newPosition.Y = i;
        newPosition.X = 0;
        SetConsoleCursorPosition(hConsole, newPosition);
        fwrite(field[i], sizeof(char), m + 1, stdout);
    }
    SetConsoleCursorPosition(hConsole, position);
}
int main()
{
    srand(0);
    printf("print n and m ");
    scanf_s("%d%d", &n, &m);
    field = new char* [n];
    field_next = new char* [n];
    for (int i = 0; i < n; i++)
    {
        field[i] = new char[m + 1];
        field_next[i] = new char[m + 1];
        for (int j = 0; j < m; j++)
        {
            if (rand() % 4 == 0) field[i][j] = '#';
            else field[i][j] = '.';
            field_next[i][j] = '.';
        }
        field[i][m] = '\n';
        field_next[i][m] = '\n';
    }
    updateAllConsole();
    while (1)
    {
draw:   char ch = _getch();
        if (ch == 'd') x = (x + 1 + m) % m;
        else if (ch == 'a') x = (x - 1 + m) % m;
        else if (ch == 'w') y = (y - 1 + n) % n;
        else if (ch == 's') y = (y + 1 + n) % n;
        else if (ch == ' ')
        {
            if (field[y][x] == '.')
            {
                field[y][x] = '#';
                putchar('#');
            }
            else
            {
                field[y][x] = '.';
                putchar('.');
            }
        }
        else if (ch == 13)
            break;
        position.X = x;									    // ”становка координаты X
        position.Y = y;									    // ”становка координаты Y
        SetConsoleCursorPosition(hConsole, position);		// ѕеремещение каретки по заданным координатам
    }
    while (async_key(cin) != VK_ESCAPE)
    {
        lifeNext();
        updateNext();
        Sleep(tick);
        if (async_key(cin) == VK_SPACE) goto draw;
    }
    return 0;
}