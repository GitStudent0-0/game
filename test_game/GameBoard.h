#pragma once
#include<iostream>


class GameBoard
{
public:
	static const int MAX_POINTS = 37;
	int boardSize = 3;
private:
	int coordToIndex[MAX_POINTS][MAX_POINTS];
	struct HexPoint
	{
		int q, r;
		int usageCounter;
	};
	HexPoint points[MAX_POINTS];

public:
	using Point = HexPoint;
	const Point& getPoint(int index) const
	{
		if (index >= 0 && index < MAX_POINTS)
			return points[index];
	}
	Point& getPoint(int index)
	{
		return points[index];
	}
	int getPointCoord(int q, int r)
	{
		int idx = coordToIndex[q + boardSize][r + boardSize];
		if (idx == -1)
			std::cout << "Точка не найдена\n";
		return idx;
	}
	GameBoard()
	{
		int n = boardSize;
		int countPoint = 0;
		for (int i = 0; i < MAX_POINTS; i++)
			for (int j = 0; j < MAX_POINTS; j++)
				coordToIndex[i][j] = -1;
		for (int q = -n; q <= n; q++) 
			for (int r = -n; r <= n; r++) 
				if (abs(q + r) <= n) 
					if (countPoint < MAX_POINTS)
					{
						points[countPoint].q = q;
						points[countPoint].r = r;
						points[countPoint].usageCounter = 0;
						coordToIndex[q + n][r + n] = countPoint;
						countPoint++;
					}
	}
};