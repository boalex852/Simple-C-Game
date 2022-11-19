#include "Utility.h"
#include "Graphics.h"
#include "Time.h"
#include "Clock.h"


Clock::Clock(int PosX, int PosY, int InScreenWidth, int InScreenHeight, int InSize, int Rotation)
	: CenterX(PosX)
	, CenterY(PosY)
	, ScreenWidth(InScreenWidth)
	, ScreenHeight(InScreenHeight)
	, Size(InSize)
	, HalfSize(InSize / 2)
	, DirectionRotation(Rotation)
{
	//Set speed to random between 1 and max speed.
	Speed = rand() % MaxSpeed + 1;

	DrawClockBorder();

	SetTime();

	//Set the movement direction for the clock.
	Utility::RotatePointAroundCenter(0, -1, 0, 0, DirectionRotation, DirectionX, DirectionY);
}

void Clock::DrawClockBorder()
{
	//Draw the clock rectangle.
	DrawLine(CenterX - HalfSize, CenterY - HalfSize, CenterX - HalfSize, CenterY + HalfSize, GetColor(255, 0, 0));
	DrawLine(CenterX - HalfSize, CenterY + HalfSize, CenterX + HalfSize, CenterY + HalfSize, GetColor(255, 0, 0));
	DrawLine(CenterX + HalfSize, CenterY + HalfSize, CenterX + HalfSize, CenterY - HalfSize, GetColor(255, 0, 0));
	DrawLine(CenterX + HalfSize, CenterY - HalfSize, CenterX - HalfSize, CenterY - HalfSize, GetColor(255, 0, 0));
}

void Clock::SetTime()
{
	int Seconds;
	int Minutes;
	int Hours;

	//Delete previous clock hands.
	FillRect(CenterX - HalfSize + 1, CenterY - HalfSize + 1, Size - 1, Size - 1, GetColor(0, 0, 0));

	GetTime(Hours, Minutes, Seconds);

	//Notice the clock's hands X end points are same as center point, because when there is no rotation, the hands are looking up along Y axis.
	//So to give the hands a length, we do center - hand legnth so the hands go up, and then we rotate them according to the current seconds/minutes/hours.

	//Each second and minute is 6 degrees since 360/60=6. Hours is 360/12 = 30 degrees.
	int NewSecondsX;
	int NewSecondsY;

	int NewMinutesX;
	int NewMinutesY;

	int NewHoursX;
	int NewHoursY;

	Utility::RotatePointAroundCenter(CenterX, CenterY - SecondsHandLength, CenterX, CenterY, 6 * Seconds, NewSecondsX, NewSecondsY);
	Utility::RotatePointAroundCenter(CenterX, CenterY - MinutesHandLength, CenterX, CenterY, 6 * Minutes, NewMinutesX, NewMinutesY);
	Utility::RotatePointAroundCenter(CenterX, CenterY - HoursHandLength, CenterX, CenterY, 30 * Hours, NewHoursX, NewHoursY);

	//Draw seconds hand.
	DrawLine(CenterX, CenterY, NewSecondsX, NewSecondsY, GetColor(255, 0, 0));

	//Draw minutes hand.
	DrawLine(CenterX, CenterY, NewMinutesX, NewMinutesY, GetColor(0, 255, 0));

	//Draw hours hand.
	DrawLine(CenterX, CenterY, NewHoursX, NewHoursY, GetColor(0, 0, 255));
}

void Clock::Translate()
{
	//First delete previous clock, check collision, move, and then draw new border.

	EraseClock();

	CheckBorderCollision();

	CenterX += DirectionX * Speed;
	CenterY += DirectionY * Speed;

	DrawClockBorder();
}

void Clock::CheckBorderCollision()
{
	//Collision with right edge.
	if (CenterX + HalfSize + DirectionX * Speed > ScreenWidth)
	{
		//Right edge normal is (-1,0).
		Utility::CalculateReflection(DirectionX, DirectionY, -1, 0);
	}
	//Collision with left edge.
	else if (CenterX - HalfSize + DirectionX * Speed < 0)
	{
		//Left edge normal is (1,0)
		Utility::CalculateReflection(DirectionX, DirectionY, 1, 0);
	}
	//Collision with top edge.
	else if (CenterY - HalfSize + DirectionY * Speed < 0)
	{
		//Top edge normal is (0,1).
		Utility::CalculateReflection(DirectionX, DirectionY, 0, 1);
	}
	//Collision with bottom edge.
	else if (CenterY + HalfSize + DirectionY * Speed > ScreenHeight)
	{
		//Bottom edge normal is (0,-1).
		Utility::CalculateReflection(DirectionX, DirectionY, 0, -1);

	}
}

bool Clock::CheckClocksCollide(Clock& ClockOne, Clock& ClockTwo)
{
	//Get the location of clock one and its size.
	int ClockOneCenterX;
	int ClockOneCenterY;
	int ClockOneSize;
	ClockOne.GetLocationAndSize(ClockOneCenterX, ClockOneCenterY, ClockOneSize);

	//Get the location of clock two and its size.
	int ClockTwoCenterX;
	int ClockTwoCenterY;
	int ClockTwoSize;
	ClockTwo.GetLocationAndSize(ClockTwoCenterX, ClockTwoCenterY, ClockTwoSize);

	//Check if the bounding boxes collide.
	if (ClockOneCenterX - ClockOneSize < ClockTwoCenterX + ClockTwoSize &&
		ClockOneCenterX + ClockOneSize > ClockTwoCenterX - ClockTwoSize &&
		ClockOneCenterY - ClockOneSize < ClockTwoCenterY + ClockTwoSize &&
		ClockOneCenterY + ClockOneSize > ClockTwoCenterY - ClockTwoSize)
	{
		return true;
	}

	return false;
}

void Clock::GetDirection(int& X, int& Y) const
{
	X = DirectionX;
	Y = DirectionY;
}

void Clock::GetLocationAndSize(int& OutCenterX, int& OutCenterY, int& OutHalfSize) const
{
	OutCenterX = CenterX;
	OutCenterY = CenterY;
	OutHalfSize = HalfSize;
}

void Clock::GetLocation(int& OutCenterX, int& OutCenterY) const
{
	OutCenterX = CenterX;
	OutCenterY = CenterY;
}

void Clock::SetDirection(const int X, const int Y)
{
	DirectionX = X;
	DirectionY = Y;
}

void Clock::EraseClock()
{
	//Remove the previous clock.
	FillRect(CenterX - HalfSize, CenterY - HalfSize, Size + 1, Size + 1, GetColor(0, 0, 0));
}

int Clock::GetHalfSize() const
{
	return HalfSize;
}
