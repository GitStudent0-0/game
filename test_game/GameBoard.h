#pragma once
#include<iostream>

struct HexPoint
{
	int q;
	int r;
};

class GameBoard
{
public:
	static const int MAX_POINTS = 37;
	int boardSize = 3;
private:
	int coordToIndex[7][7];
	HexPoint points[MAX_POINTS];

public:
	const HexPoint& getPoint(int index) const
	{
		return points[index];
	}
	HexPoint& getPoint(int index)
	{
		return points[index];
	}
	int getPointCoord(int q, int r)
	{
		int idx = coordToIndex[q + boardSize][r + boardSize];
		return idx;
	}
	GameBoard()
	{
		int n = boardSize;
		int countPoint = 0;
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				coordToIndex[i][j] = -1;
		for (int q = -n; q <= n; q++) 
			for (int r = -n; r <= n; r++) 
				if (abs(q + r) <= n) 
					if (countPoint < MAX_POINTS)
					{
						points[countPoint].q = q;
						points[countPoint].r = r;
						coordToIndex[q + n][r + n] = countPoint;
						countPoint++;
					}
	}
};