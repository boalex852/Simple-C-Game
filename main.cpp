#include "Time.h"
#include "Graphics.h"

#undef UNICODE
#include <windows.h>
#include "MainProcess.h"

MainProcess::Keys PressedKey = 0;

const int ScreenWidth  = 800;
const int ScreenHeight = 600;

static LRESULT CALLBACK WndProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch(Message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		FillRect(0, 0, ScreenWidth, ScreenHeight, 0);
		break;
	case WM_KEYDOWN:
		switch(WParam)
		{
		case VK_LEFT:  PressedKey |= MainProcess::KeyLeft;  break;
		case VK_UP:    PressedKey |= MainProcess::KeyUp;    break;
		case VK_RIGHT: PressedKey |= MainProcess::KeyRight; break;
		case VK_DOWN:  PressedKey |= MainProcess::KeyDown;  break;
		case VK_SPACE: PressedKey |= MainProcess::KeySpace; break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_KEYUP:
		switch(WParam)
		{
		case VK_LEFT:  PressedKey &= ~MainProcess::KeyLeft;  break;
		case VK_UP:    PressedKey &= ~MainProcess::KeyUp;    break;
		case VK_RIGHT: PressedKey &= ~MainProcess::KeyRight; break;
		case VK_DOWN:  PressedKey &= ~MainProcess::KeyDown;  break;
		case VK_SPACE: PressedKey &= ~MainProcess::KeySpace; break;
		}
		break;
	}
	
	return DefWindowProc(WindowHandle, Message, WParam, LParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ScreenInitialization Init;
	memset(&Init, 0, sizeof(Init));

	Init.CaptionText  = "Simple Game";
	Init.Instance     = hInstance;
	Init.WindowProcess = WndProc;
	Init.ScreenWidth  = ScreenWidth;
	Init.ScreenHeight = ScreenHeight;

	HWND MainWindowHandle = InitializeWindow(Init);
	
	// Clear the screen.
	FillRect(0, 0, ScreenWidth, ScreenHeight, 0);
	
	MainProcess Application(ScreenWidth, ScreenHeight);
	DWORD PreviousTime = timeGetTime();
		
	// Game loop.
	while(true)
	{
		// Pause the program for the given sleep time in milliseconds.
		const DWORD SleepTime = 20;			
		const DWORD CurrentTime = timeGetTime();
		const DWORD DeltaTime = CurrentTime - PreviousTime;

		if (DeltaTime < SleepTime)
		{
			Sleep(SleepTime - DeltaTime);
		}

		PreviousTime = CurrentTime;			

		// Windows messaging
		MSG Message;
		if(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if(Message.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		Application.Tick(PressedKey);
	}

	//If we reach this point, then destroy the window.
	DestroyWindow();

	return 0;
}
