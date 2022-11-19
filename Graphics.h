#pragma once

#undef UNICODE
#include <windows.h>

//Get the color value.
inline unsigned int GetColor(unsigned int Red, unsigned int Green, unsigned int Blue)
{
	return Red + (Green<<8) + (Blue<<16);
}

//draw filled rectangle
void FillRect(int x, int y, int sx, int sy, unsigned int color);

//draw line
void DrawLine(int x1, int y1, int x2, int y2, unsigned int color);

struct ScreenInitialization
{
	HINSTANCE		Instance;
	WNDPROC			WindowProcess;
	const char*		CaptionText;
	int				ScreenWidth;
	int				ScreenHeight;
};

// Initialize the window.
HWND InitializeWindow(const ScreenInitialization&);

// Destroy the window.
void DestroyWindow();
