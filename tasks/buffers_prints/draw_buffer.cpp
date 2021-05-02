#include <windows.h>
#include <stdlib.h>
#define CCOLOR(r,g,b) ((FOREGROUND_RED*(r))|(FOREGROUND_GREEN*(g))|(FOREGROUND_BLUE*(b))|FOREGROUND_INTENSITY)
#define NUM_BUGS  64
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
static COORD      cscr = { 0 };
static CHAR_INFO* cbuf = NULL;

void fill_rect(short x, short y, short w, short h, WORD color);
void put_char(CHAR c, short x, short y, WORD color);
WORD async_key(HANDLE _in);

typedef struct {
    short x;
    short y;
    short dx;
    short dy;
    WORD  color;
    CHAR  ch;
} bug;

int main(void) {
    HANDLE cin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE cout = GetStdHandle(STD_OUTPUT_HANDLE);
    if ((cin == INVALID_HANDLE_VALUE) || (cout == INVALID_HANDLE_VALUE))
        return GetLastError();

    CONSOLE_SCREEN_BUFFER_INFO si;
    if (!GetConsoleScreenBufferInfo(cout, &si))
        return GetLastError();

    //si.dwSize.Y /= 7;
    if (!SetConsoleScreenBufferSize(cout, si.dwSize))
        return GetLastError();

    si.srWindow.Bottom = si.dwSize.Y - 1;
    SetConsoleWindowInfo(cout, TRUE, &si.srWindow);

    SetConsoleTitleW(L"Пример двойной буферизации");

    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(cout, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(cout, &ci);

    cscr = si.dwSize;
    const COORD pos = { 0 };

    cbuf = (CHAR_INFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, sizeof(CHAR_INFO) * (cscr.X * cscr.Y));
    if (cbuf == NULL)
        return ERROR_OUTOFMEMORY;

    //для примера
    bug bugs[NUM_BUGS];
    const bug* e = bugs + NUM_BUGS;
    for (bug* i = bugs; i != e; ++i) {
        i->ch = (CHAR)(rand() % 4) + 1;
        i->x = rand() % cscr.X;
        i->y = rand() % cscr.Y;
        i->dx = (rand() % 2) ? -1 : +1;
        i->dy = (rand() % 2) ? +1 : -1;
        i->color = CCOLOR(rand() % 2, rand() % 2, rand() % 2);
        if (!i->color)
            i->color = CCOLOR(1, 1, 1);
    }

    //выход клавиша Esc
    while (async_key(cin) != VK_ESCAPE) {
        fill_rect(0, 0, cscr.X, cscr.Y, 0);

        for (bug* i = bugs; i != e; ++i) {

            i->x += i->dx;
            if (i->x < 0 || i->x >= cscr.X) {
                i->x = max(min(i->x, cscr.X - 1), 0);
                i->dx = -i->dx;
            }

            i->y += i->dy;
            if (i->y < 0 || i->y >= cscr.Y) {
                i->y = max(min(i->y, cscr.Y - 1), 0);
                i->dy = -i->dy;
            }
            put_char(i->ch, i->x, i->y, i->color);
        }

        //выводим
        WriteConsoleOutputA(cout, cbuf, cscr, pos, &si.srWindow);
        Sleep(40);
    }

    HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, cbuf);
    return 0;
}

//прямоугольник
void fill_rect(short x, short y, short w, short h, WORD color) {
    int p;
    x = min(max(0, x), cscr.X);
    y = min(max(0, y), cscr.Y);
    w = min(cscr.X, x + w);
    h = min(cscr.Y, y + h);
    for (short i = y; i < h; ++i) {
        for (short j = x; j < w; ++j) {
            p = i * cscr.X + j;
            cbuf[p].Char.AsciiChar = '\0';
            cbuf[p].Attributes = color;
        }
    }
}

//символ
void put_char(CHAR c, short x, short y, WORD color) {
    if ((x >= 0 && x < cscr.X) && (y >= 0 && y < cscr.Y)) {
        int i = y * cscr.X + x;
        cbuf[i].Char.AsciiChar = c;
        cbuf[i].Attributes = color;
    }
}

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