#include "Utility.h"
#include <cmath>

static const float k_PI = 3.1415926536f;

int Utility::Clamp(int Number, int Min, int Max)
{
	//If number smaller than min or greater than max, return the correct number.
	if (Number < Min)
	{
		return Min;
	}

	if (Number > Max)
	{
		return Max;
	}

	return Number;
}

void Utility::RotatePointAroundCenter(int PointX, int PointY, int CenterX, int CenterY, int RotationAngle, int& NewPointX, int& NewPointY)
{
	//Calcualte the rotation in radians.
	double RotationCos = cos(RotationAngle * k_PI / 180);
	double RotationSin = sin(RotationAngle * k_PI / 180);

	//Use the formula for rotating a point around a given center.
	int XDifference = PointX - CenterX;
	int YDifference = PointY - CenterY;

	NewPointX = static_cast<int>(round(CenterX + RotationCos * XDifference - RotationSin * YDifference));
	NewPointY = static_cast<int>(round(CenterY + RotationSin * XDifference + RotationCos * YDifference));
}

void Utility::RotateNormalizedVector(int X, int Y, int RotationAngle, int& NewX, int& NewY)
{
	//Calcualte the rotation in radians.
	double RotationCos = cos(RotationAngle * k_PI / 180.0f);
	double RotationSin = sin(RotationAngle * k_PI / 180.0f);

	//Use the formula to return the new direction. If we don't round, we loose precision.
	NewX = static_cast<int>(round(X * RotationCos - Y * RotationSin));
	NewY = static_cast<int>(round(X * RotationSin + Y * RotationCos));
}

void Utility::CalculateReflection(int& DirectionX, int& DirectionY, int NormalX, int NormalY)
{
	//We use the formula R = Direction - 2*(DotProduct(Direction,Normal))*Normal.

	DirectionX = DirectionX - 2 * (DirectionX * NormalX + DirectionY * NormalY) * NormalX;
	DirectionY = DirectionY - 2 * (DirectionX * NormalX + DirectionY * NormalY) * NormalY;
}