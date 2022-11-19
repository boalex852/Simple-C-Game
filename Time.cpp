#include "Time.h"

#undef UNICODE
#include <windows.h>

//Get the local system time.
void GetTime(int& Hour, int& Minute, int& Second)
{
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	Hour  = CurrentTime.wHour;
	Minute = CurrentTime.wMinute;
	Second = CurrentTime.wSecond;
}
