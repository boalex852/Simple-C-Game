#include "graphics.h"
#include <math.h>
#include "Utility.h"
#include "Projectile.h"

Projectile::Projectile(int InStartX, int InStartY, int Rotation)
	: StartX(InStartX)
	, StartY(InStartY)
{
	//Draw the line.
	EndY = StartY - Length;
	EndX = StartX;

	//Rotate the end point to face the correct direction and remember the direction.
	Utility::RotatePointAroundCenter(EndX, EndY, StartX, StartY, Rotation, EndX, EndY);

	//Calculate the direction. We normalize the vector so we remember it.
	double Length = sqrt((EndX - StartX) * (EndX - StartX) + (EndY - StartY) * (EndY - StartY));
	DirectionX = (EndX - StartX) / Length;
	DirectionY = (EndY - StartY) / Length;

	//Draw the line.
	DrawLine(StartX, StartY, EndX, EndY, GetColor(255, 255, 255));
}

void Projectile::Translate()
{
	//Delete the previous line.
	DrawLine(StartX, StartY, EndX, EndY, GetColor(0, 0, 0));
	 
	//Move the new start to end.
	StartX = EndX;
	StartY = EndY;

	//Calculate end from direction.
	EndX = static_cast<int>(StartX + DirectionX * Length);
	EndY = static_cast<int>(StartY + DirectionY * Length);

	//Draw the line.
	DrawLine(StartX, StartY, EndX, EndY, GetColor(255, 255, 255));
}

void Projectile::GetProjectileEnd(int& OutX, int& OutY) const
{
	OutX = EndX;
	OutY = EndY;
}

void Projectile::ClearProjectile()
{
	DrawLine(StartX, StartY, EndX, EndY, GetColor(0, 0, 0));
}