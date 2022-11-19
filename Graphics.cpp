#include "Graphics.h"

static HWND WindowHandle;

//Draw a filled rectangle.
void FillRect(int X, int Y, int XLength, int YLength, unsigned int Color)
{
	HDC DeviceContext = GetDC(WindowHandle);
	HBRUSH Brush = CreateSolidBrush(Color);
	RECT Rectangle;
	SetRect(&Rectangle, X, Y, X+XLength, Y+YLength);
	FillRect(DeviceContext, &Rectangle, Brush);
	DeleteObject(Brush);
	ReleaseDC(WindowHandle, DeviceContext);
}

//Draw a solid line.
void DrawLine(int StartX, int StartY, int EndX, int EndY, unsigned int Color)
{
	HDC DeviceContext = GetDC(WindowHandle);
	HPEN Pen = CreatePen(PS_SOLID, 1, Color);
	HGDIOBJ OldPen = SelectObject(DeviceContext, Pen);

	MoveToEx(DeviceContext, StartX, StartY, NULL);
	LineTo(DeviceContext, EndX, EndY);

	SelectObject(DeviceContext, OldPen);
	DeleteObject(Pen);
	ReleaseDC(WindowHandle, DeviceContext);
}

HWND InitializeWindow(const ScreenInitialization& InitialData)
{
	WNDCLASSEX WindowClass;
	memset(&WindowClass, 0, sizeof(WindowClass));
	WindowClass.cbSize        = sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc   = InitialData.WindowProcess;
	WindowClass.hInstance     = InitialData.Instance;
	WindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WindowClass.lpszClassName = "MainWindow";
	RegisterClassEx(&WindowClass);

	DWORD WindowStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
	RECT Rect;
	SetRect(&Rect, 0, 0, InitialData.ScreenWidth, InitialData.ScreenHeight);
	AdjustWindowRect(&Rect, WindowStyle, false);
	Rect.right  -= Rect.left;
	Rect.bottom -= Rect.top;

	WindowHandle = CreateWindowEx( 0,
		"MainWindow",           //ClassName
		InitialData.CaptionText,
		WindowStyle | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN)-Rect.right)/2,
		(GetSystemMetrics(SM_CYSCREEN)-Rect.bottom)/2,
		Rect.right, Rect.bottom,
		NULL,                   //hwndParent
		NULL,
		InitialData.Instance,
		NULL);                  //lpvParam

	return WindowHandle;
}

void DestroyWindow()
{
	DestroyWindow(WindowHandle);
}
