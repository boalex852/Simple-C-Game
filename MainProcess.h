#pragma once

#include <vector>
#include <ctime>

class Projectile;
class Clock;

class MainProcess
{
public:
	typedef unsigned int Keys;

	MainProcess(int screenWidth, int screenHeight);
	~MainProcess();

	// Tick is called on fix framerate (50fps).
	void Tick(Keys PressedKey);

	static const Keys KeyLeft  = 0x01;
	static const Keys KeyUp    = 0x02;
	static const Keys KeyRight = 0x04;
	static const Keys KeyDown  = 0x08;
	static const Keys KeySpace = 0x10;

private:
	const int ScreenWidth;
	const int ScreenHeight;
	
	///////////////Modifiable/////////////////////////
	/** How far from bottom the cannon should be.*/
	const int ScreenCannonHeightOffset = 100;
	/////////////////////////////////////////////////

	// Cannon location (needed for rotations and setting the corners correctly).
	int	CannonX;
	int	CannonY;
	
	//The cannon center point (around which we rotate).
	int	CannonCenterX;
	int	CannonCenterY;

	//Current cannon location.
	int CannonCornerOneX;
	int CannonCornerOneY;

	int CannonCornerTwoX;
	int CannonCornerTwoY;

	int CannonCornerThreeX;
	int CannonCornerThreeY;
	
	///////////////Modifiable/////////////////////////
	//Cannon properties.
	int CannonWidth = 10;
	int CannonHeight = CannonWidth * 3;
	/////////////////////////////////////////////////

	//The rotation angle.
	int RotationAngle = 0;
	
	///////////////Modifiable/////////////////////////
	//The speed we rotate with.
	int RotationSpeed = 2;
	/////////////////////////////////////////////////

	//How much to delay projecitles creation by.
	double ProjectileDelay = CLOCKS_PER_SEC * 0.01;
	//The projectiles stack.
	std::vector<Projectile> ProjectilesStack;
	clock_t CurrentTime = 0;

	// The clocks stack.
	std::vector<Clock> ClocksStack;
	
	///////////////Modifiable/////////////////////////
	// Smallest allowed clock size.
	int SmallestClockSize = 10;
	//Initial Spawn locations.
	int InitialCloakOneX = ScreenWidth / 4;
	int InitialClockOneY = ScreenHeight / 4;
	int InitialClockOneRotation = -30;

	int InitialCloakTwoX = 3 * ScreenWidth / 4;
	int InitialClockTwoY = ScreenHeight / 2;
	int InitialClockTwoRotation = 45;
	/////////////////////////////////////////////////

	/** Draw the cannon.*/
	void DrawCannon();

	/** Erase the cannon.*/
	void ClearCannon();

	/** Create a projectile.*/
	void CreateProjectile(int SpawnX, int SpawnY, int Rotation);

	/** Check if a projectile collided with clock.*/
	bool ProjectileClockCollision(Projectile& Projectile, Clock& Clock);

	/** Create a clock and add it to the clock array.*/
	bool CreateClock(int SpawnX, int SpawnY, int InitialRotation, int Size = 100);

	/** Spawn the initial clocks.*/
	void InitialClocksSpawn();

	/** Check if point intersects with any clock or not.*/
	bool IsClockSpawnPointValid(int X, int Y, int Size);
};
