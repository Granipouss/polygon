#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>

#define HEIGHT 800
#define WIDTH 1200

void initSystem ();
void updateSystem ();
LRESULT CALLBACK WindowProc (HWND, UINT uMsg, WPARAM, LPARAM);

#endif // WINDOW_H_INCLUDED
