#include "Console.h"
#include <stdlib.h>
#include <windows.h>

// ---------- Debugging -------------
#define __DEBUG_OBJECT__ "Console"
#include "dbg/dbg.h"

void
console_set_size (int w, int h)
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, w, h, TRUE);
}

void
console_set_col (int col)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
}

void
console_set_cursor_visibility (int visible)
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
