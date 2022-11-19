#include "Utility.h"
#include "Graphics.h"
#include <cstdio>
#include "Projectile.h"
#include "Clock.h"
#include "MainProcess.h"


MainProcess::MainProcess(int screenWidth, int screenHeight)
	: ScreenWidth(screenWidth)
	, ScreenHeight(screenHeight)
	, CannonX(screenWidth/2)
	, CannonY(screenHeight - ScreenCannonHeightOffset)
{
	InitialClocksSpawn();

	CannonCenterX = CannonX;
	CannonCenterY = CannonY + CannonHeight;

	CannonCornerOneX = CannonX;
	CannonCornerOneY = CannonY;
	CannonCornerTwoX = CannonX - CannonWidth;
	CannonCornerTwoY = CannonY + CannonHeight;
	CannonCornerThreeX = CannonX + CannonWidth;
	CannonCornerThreeY = CannonY + CannonHeight;
}


MainProcess::~MainProcess()
{

}


void MainProcess::Tick(Keys PressedKey)
{
	bool RotationOccured = false;

	if(PressedKey & KeyLeft)
	{
		ClearCannon();
		RotationAngle = Utility::Clamp(RotationAngle - RotationSpeed, -90, 90);
		RotationOccured = true;
	}

	if(PressedKey & KeyRight)
	{
		ClearCannon();
		RotationAngle = Utility::Clamp(RotationAngle + RotationSpeed, -90, 90);
		RotationOccured = true;
	}

	//if(PressedKey & s_KeyUp)
	//{
	//}

	//if(PressedKey & s_KeyDown)
	//{
	//}

	if (RotationOccured)
	{
		Utility::RotatePointAroundCenter(CannonX, CannonY, CannonCenterX, CannonCenterY, RotationAngle, CannonCornerOneX, CannonCornerOneY);
	}

	if(PressedKey & KeySpace)
	{
		CreateProjectile(CannonCornerOneX, CannonCornerOneY, RotationAngle);
	}

	// Tick the projectiles.
	if (ProjectilesStack.size() > 0)
	{
		int ProjectileEndX;
		int ProjectileEndY;

		for (size_t Index = 0; Index < ProjectilesStack.size(); Index++)
		{
			//Move the projectile. If cant, destroy it.
			ProjectilesStack[Index].Translate();	
			ProjectilesStack[Index].GetProjectileEnd(ProjectileEndX, ProjectileEndY);
			if (ProjectileEndX < 0 || ProjectileEndX > ScreenWidth || ProjectileEndY < 0 || ProjectileEndY > ScreenHeight)
			{
				ProjectilesStack[Index].ClearProjectile();
				Utility::RemoveAt(ProjectilesStack, Index);
				//We deleted the projectile in this index, need to recheck it.
				Index = Index - 1;
			}
		}
	}

	//Tick the clocks.
	if (ClocksStack.size() > 0)
	{
		//Move the clocks and set time.
		for (size_t Index = 0; Index < ClocksStack.size(); Index++)
		{
			ClocksStack[Index].Translate();
			
			//If clock is out of bounds, destroy it.	
			int LocationX;
			int LocationY;
			ClocksStack[Index].GetLocation(LocationX, LocationY);
			if (LocationX < 0 || LocationX > ScreenWidth || LocationY < 0 || LocationY > ScreenHeight)
			{
				ClocksStack[Index].EraseClock();
				Utility::RemoveAt(ClocksStack, Index);
				
				//We deleted the clock in this index, need to recheck it.
				Index = Index - 1;
				continue;
			}

			//Tick time.
			ClocksStack[Index].SetTime();
		}

		//Check for collisions between the clocks. We check for each with the rest if it collides or not.
		for (size_t Index = 0; Index < ClocksStack.size() - 1; Index++)
		{
			for (size_t JIndex = Index + 1; JIndex < ClocksStack.size(); JIndex++)
			{
				//If clocks collide, inverse their direction.
				if (Clock::CheckClocksCollide(ClocksStack[Index], ClocksStack[JIndex]))
				{
					int DirectionOneX;
					int DirectionOneY;

					int DirectionTwoX;
					int DirectionTwoY;

					ClocksStack[Index].GetDirection(DirectionOneX, DirectionOneY);
					Utility::RotatePointAroundCenter(DirectionOneX, DirectionOneY, 0, 0, 180, DirectionOneX, DirectionOneY);


					ClocksStack[JIndex].GetDirection(DirectionTwoX, DirectionTwoY);
					Utility::RotatePointAroundCenter(DirectionTwoX, DirectionTwoY, 0, 0, 180, DirectionTwoX, DirectionTwoY);

					ClocksStack[Index].SetDirection(DirectionOneX, DirectionOneY);
					ClocksStack[JIndex].SetDirection(DirectionTwoX, DirectionTwoY);
				}
			}
		}
	}

	//Collision of projectile with clock.
	if (ProjectilesStack.size() > 0 && ClocksStack.size() > 0)
	{
		for (size_t Index = 0; Index < ProjectilesStack.size(); Index++)
		{
			//Check if projectile collided with any of the clocks.
			bool Collision = false;
			int CollisionIndex = -1;
			for (size_t JIndex = 0; JIndex < ClocksStack.size(); JIndex++)
			{
				if (ProjectileClockCollision(ProjectilesStack[Index], ClocksStack[JIndex]))
				{
					//Remember the collision index.
					Collision = true;
					CollisionIndex = JIndex;
					break;
				}
			}

			//Did collision occur?
			if (Collision)
			{
				//Clear the projectile and delete it.
				ProjectilesStack[Index].ClearProjectile();
				Utility::RemoveAt(ProjectilesStack, Index);

				//Get the destroyed clock's location and size, and delete it.

				int ClockLocationX;
				int ClockLocationY;
				int ClockSize;

				ClocksStack[CollisionIndex].GetLocationAndSize(ClockLocationX, ClockLocationY, ClockSize);
				ClocksStack[CollisionIndex].EraseClock();
				Utility::RemoveAt(ClocksStack, CollisionIndex);

				//Attmpt to spawn two clocks. We try to place the first clock in all the possible positions. If we cant, then only one clock will spawn.
				if (!CreateClock(ClockLocationX + ClockSize + 2, ClockLocationY, 30, ClockSize))
				{
					if (!CreateClock(ClockLocationX - ClockSize - 2, ClockLocationY, 30, ClockSize))
					{
						if (!CreateClock(ClockLocationX, ClockLocationY + ClockSize + 2, 30, ClockSize))
						{
							CreateClock(ClockLocationX, ClockLocationY - ClockSize - 2, 30, ClockSize);
						}
					}
				}
				CreateClock(ClockLocationX, ClockLocationY, -30, ClockSize);

				//We deleted the projectile at this index, so we need to recheck it as new element is here.
				Index = Index - 1;
			}
		}
	}

	//If all clocks destroyed, respawn the initial ones.
	if (ClocksStack.size() == 0)
	{
		InitialClocksSpawn();
	}

	if (RotationOccured)
	{
		//Draw cannon. Calculate the corners of the triangle after rotation.
		//Corner one is calculated abocve since it is needed for projectiles as well.
		
		Utility::RotatePointAroundCenter(CannonX - CannonWidth, CannonY + CannonHeight, CannonCenterX, CannonCenterY, RotationAngle, CannonCornerTwoX, CannonCornerTwoY);
		Utility::RotatePointAroundCenter(CannonX + CannonWidth, CannonY + CannonHeight, CannonCenterX, CannonCenterY, RotationAngle, CannonCornerThreeX, CannonCornerThreeY);
	}

	DrawCannon();
}

void MainProcess::DrawCannon()
{
	DrawLine(CannonCornerOneX, CannonCornerOneY, CannonCornerTwoX, CannonCornerTwoY, GetColor(255, 0, 0));
	DrawLine(CannonCornerOneX, CannonCornerOneY, CannonCornerThreeX, CannonCornerThreeY, GetColor(0, 255, 0));
	DrawLine(CannonCornerTwoX, CannonCornerTwoY, CannonCornerThreeX, CannonCornerThreeY, GetColor(0, 0, 255));
}

void MainProcess::ClearCannon()
{
	DrawLine(CannonCornerOneX, CannonCornerOneY, CannonCornerTwoX, CannonCornerTwoY, GetColor(0, 0, 0));
	DrawLine(CannonCornerOneX, CannonCornerOneY, CannonCornerThreeX, CannonCornerThreeY, GetColor(0, 0, 0));
	DrawLine(CannonCornerTwoX, CannonCornerTwoY, CannonCornerThreeX, CannonCornerThreeY, GetColor(0, 0, 0));
}

void MainProcess::CreateProjectile(int SpawnX, int SpawnY, int Rotation)
{
	//We allow to fire projectiles only if passed more time than the projectile delay since we last fired.
	if (CurrentTime == -1 || clock() - CurrentTime > ProjectileDelay)
	{
		//Add the created projectile to the stack.
		ProjectilesStack.push_back(Projectile(SpawnX, SpawnY, Rotation));
		CurrentTime = clock();  
	}
}

bool MainProcess::ProjectileClockCollision(Projectile& Projectile, Clock& Clock)
{
	//Get the projectile end locaiton.
	int ProjectileEndX;
	int ProjectileEndY;
	Projectile.GetProjectileEnd(ProjectileEndX, ProjectileEndY);

	//Get the clock bounding box.
	int ClockCenterX;
	int ClockCenterY;
	int ClockSize;
	Clock.GetLocationAndSize(ClockCenterX, ClockCenterY, ClockSize);

	//Check if the projectile is within the clock.
	if (ProjectileEndX >= ClockCenterX - ClockSize && ProjectileEndX <= ClockCenterX + ClockSize)
	{
		if (ProjectileEndY >= ClockCenterY - ClockSize && ProjectileEndY <= ClockCenterY + ClockSize)
		{
			return true;
		}
	}

	return false;
}

bool MainProcess::CreateClock(int SpawnX, int SpawnY, int InitialRotation, int Size)
{
	//We need size to be divided by 2, for half size, else results in badly drawn graphics.
	if (Size % 2 == 1)
	{
		Size -= 1;
	}

	//Only spawn clock if bigger than smallest clock size.
	if (Size >= SmallestClockSize)
	{
		//Make sure we can spawn the clock at this point.
		if (IsClockSpawnPointValid(SpawnX, SpawnY, Size))
		{
			ClocksStack.push_back(Clock(SpawnX, SpawnY, ScreenWidth, ScreenHeight, Size, InitialRotation));
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void MainProcess::InitialClocksSpawn()
{
	//Spawn two clocks.
	CreateClock(InitialCloakOneX, InitialClockOneY, InitialClockOneRotation);
	CreateClock(InitialCloakTwoX, InitialClockTwoY, InitialClockTwoRotation);
}

bool MainProcess::IsClockSpawnPointValid(int X, int Y, int Size)
{
	//We use same algorithm as checking if two clocks collide, only on the given point and size.
	//This way, we know if the clock we about to create, collides with any others or not.

	//If point is out of bounds, is not valid spawn point.
	if (X <= 0 || X >= ScreenWidth || Y <= 0 || Y >= ScreenHeight)
	{
		return false;
	}

	//We only deal with even size.
	if (Size % 2 == 1)
	{
		Size += 1;
	}

	//We need to operate with half size, and we recieve the whole clock size.
	Size = Size / 2;

	//Check the clock is within the screen borders.
	if (X - Size <= 0 || X + Size >= ScreenWidth || Y - Size <= 0 || Y + Size >= ScreenHeight)
	{
		return false;
	}

	int ClockCenterX;
	int ClockCenterY;
	int ClockSize;

	for (size_t Index = 0; Index < ClocksStack.size(); Index++)
	{
		ClocksStack[Index].GetLocationAndSize(ClockCenterX, ClockCenterY, ClockSize);

		//If point inside any clock, then it is not valid.

		if (X - Size < ClockCenterX + ClockSize &&
			X + Size > ClockCenterX - ClockSize &&
			Y - Size < ClockCenterY + ClockSize &&
			Y + Size > ClockCenterY - ClockSize)
		{
			return false;
		}
	}

	return true;
}
