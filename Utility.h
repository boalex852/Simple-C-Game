#pragma once
#include <vector>

class Utility 
{

public:
	/**Remove an element from the vector at the given index.*/
	template <typename T>
	static void RemoveAt(std::vector<T>& Vector, size_t Index)
	{
		//Don't do anything if index is out-of-bound.
		if (Index >= Vector.size())
		{
			return;
		}

		//Delete the specified element.
		Vector.erase(Vector.begin() + Index);
	}

	/** Insert element in array, notice if it already exists, nothing happens.*/
	template <typename T>
	static void InsertUnique(std::vector<T>& Vector, T Element)
	{
		for (const auto& Iterator : Vector)
		{
			if (Iterator == Element)
			{
				return;
			}
		}

		Vector.push_back(Element);
	}

	template <typename T>
	static inline bool IsValidIndex(std::vector<T>& Vector, int Index)
	{
		if (Index < Vector.size())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/** Clamp the given number between Min and Max. */
	static int Clamp(int Number, int Min, int Max);

	/** Rotate a point around a given center. */
	static void RotatePointAroundCenter(int PointX, int PointY, int CenterX, int CenterY, int RotationAngle, int& NewPointX, int& NewPointY);

	/** Rotate normalized vector.*/
	static void RotateNormalizedVector(int X, int Y, int RotationAngle, int& NewX, int& NewY);

	/** Return the dot product of two vectors.*/
	static void CalculateReflection(int& DirectionX, int& DirectionY, int NormalX, int NormalY);
};
