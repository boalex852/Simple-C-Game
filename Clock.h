#pragma once

class Clock
{
	//Screen height and width, needed for bumping.
	int ScreenWidth;
	int ScreenHeight;

	//Size of the clock.
	int Size;
	//Half size of clock, needed for correct drawings.
	int HalfSize;

	//Center coordinates.
	int CenterX;
	int CenterY;

	//Movement direction,
	int DirectionX;
	int DirectionY;
	
	///////////////////////////Modifiable properties/////////////
	//Hands size properties.
	int SecondsHandLength = HalfSize - 10;
	int MinutesHandLength = SecondsHandLength / 2;
	int HoursHandLength = MinutesHandLength - 5;
	//Max speed for clock movement.
	int MaxSpeed = 3;
	////////////////////////////////////////////////////////////

	//Current speed and direction rotation.
	int Speed;
	int DirectionRotation;

public:
	Clock(int PosX, int PosY, int InScreenWidth, int InScreenHeight, int InSize = 100, int Rotation = 0);

	/** Draw the clock.*/
	void DrawClockBorder();

	/** Set the clock to display the current time.*/
	void SetTime();

	/** Move the clock.*/
	void Translate();

	/** Check if clock collides with screen borders.*/
	void CheckBorderCollision();

	/** Check if two given clocks collide or not.*/
	static bool CheckClocksCollide(Clock& ClockOne, Clock& ClockTwo);

	/** Get the clocks direction.*/
	void GetDirection(int& X, int& Y) const;

	/** Get the clock's location and half size.*/
	void GetLocationAndSize(int& OutCenterX, int& OutCenterY, int& OutHalfSize) const;

	/** Get the clock's location.*/
	void GetLocation(int& OutCenterX, int& OutCenterY) const;

	/** Set the clocks movement direction to the given vector.*/
	void SetDirection(const int X, const int Y);

	/** Delete the clock from the screen. */
	void EraseClock();

	/** Return the clock size.*/
	int GetHalfSize() const;
};