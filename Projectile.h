#pragma once

class Projectile
{
	//The X coordinate we draw the projectile from.
	int StartX;
	//The Y coordinate we draw the projectile from.
	int StartY;
	//The X coordinate we end drawing projectile on.
	int EndX;
	//The Y coordinate we end drawing projectile on.
	int EndY;

	//The projectile direction.
	double DirectionX;
	double DirectionY;

	///////////////////////////Modifiable properties/////////////
	//Projectile length.
	int Length = 5;
	////////////////////////////////////////////////////////////

public:
	Projectile(int InStartX, int InStartY, int Rotation);

	/** Move the projectile along its direction. Returns true if projectile can be moved, false if needs to be destroyed. */
	void Translate();

	/** Return the projectile end.*/
	void GetProjectileEnd(int& OutEndX, int& OutEndY) const;

	/** Remove the projectile from screen.*/
	void ClearProjectile();
};