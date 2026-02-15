#pragma once
#include <algorithm>
#include "GameBoard.h"

class GameLogic
{
	GameBoard board;
	struct TriangCenter 
	{
		int a, b, c;   // индексы точек из массива points[]
		double q, r;  //координаты центра
		bool active;
	};

	static const int MAX_TOKENS = 60;
	TriangCenter tokens[MAX_TOKENS];
	int tokenCount;

	bool connections[MAX_POINTS][MAX_POINTS];

	int distance(GameBoard::Point& p1, GameBoard::Point& p2)
	{
		int dx = abs(p1.q - p2.q);
		int dz = abs(p1.r - p2.r);
		int dy = abs((-p1.q - p1.r) - (-p2.q - p2.r));
		int dist = std::max({ dx, dy, dz });
		return dist;
	}

	bool isValidityStep(GameBoard::Point& p1, GameBoard::Point& p2)
	{
		//ближайшие соседи (q-1,r) (q-1,r+1) (q,r-1) (q,r+1) (q+1,r-1) (q+1,r)
		//отдаленные соседи по прямой 
		bool flag = true;
		int dx = abs(p1.q - p2.q);
		int dz = abs(p1.r - p2.r);
		int dy = abs((-p1.q - p1.r) - (-p2.q - p2.r));
		if ((distance(p1, p2) > 3) || (distance(p1, p2) >= 2) && !(dx == 0 || dy == 0 || dz == 0))
		{
			std::cout << "Неверный ход!\n";
			flag = false;
		}

		return flag;
	}
	
	void calculateTriangleCenter(GameBoard::Point& p1, GameBoard::Point& p2, GameBoard::Point& p3, double& q, double& r)
	{
		int x1 = p1.q, z1 = p1.r;
		int x2 = p2.q, z2 = p2.r;
		int x3 = p3.q, z3 = p3.r;
		q = (x1 + x2 + x3) / 3.0;
		r = (z1 + z2 + z3) / 3.0;
	}

	bool hasToken(int a, int b, int c)
	{
		bool isEqual = false;
		for (int i = 0; i < tokenCount; i++)
		{
			if (tokens[i].active)
				if (a == tokens[i].a && b == tokens[i].b && c == tokens[i].c)
					isEqual = true;
			
		}
		return isEqual;
	}

	void createTriangCenter(int a, int b, int c)
	{
		if (tokenCount >= MAX_TOKENS)
			return;

		int temp[3] = { a, b, c };
		std::sort(temp, temp + 3);

		if (hasToken(temp[0], temp[1], temp[2]))
			return;
		GameBoard::Point& p1 = board.getPoint(a);
		GameBoard::Point& p2 = board.getPoint(b);
		GameBoard::Point& p3 = board.getPoint(c);

		TriangCenter& token = tokens[tokenCount++];
		token.a = temp[0];
		token.b = temp[1];
		token.c = temp[2];
		token.active = 1;

		calculateTriangleCenter(p1, p2, p3, token.q, token.r);
	}

public:

	GameLogic() : tokenCount(0)
	{
		for (int i = 0; i < MAX_POINTS; i++)
			for (int j = 0; j < MAX_POINTS; j++)
				connections[i][j] = 0;
	}
	void game(int index1, int index2)
	{
		if ((index1 < 0 || index1 >= MAX_POINTS) || (index2 < 0 || index2 >= MAX_POINTS))
			return;
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);
		if (!connections[index1][index2] && p1.usageCounter != 4 && p2.usageCounter != 4)
			if (isValidityStep(p1, p2))
			{
				connections[index1][index2] = 1;
				connections[index2][index1] = 1;
				p1.usageCounter++;
				p2.usageCounter++;
			}

		for (int k = 0; k < MAX_POINTS; k++)
		{
			if (connections[index1][k] && connections[index2][k])
				if (distance(p1, p2) == 1 && distance(p1, board.getPoint(k)) == 1 && distance(p2, board.getPoint(k)) == 1)
				{
					int oldCount = tokenCount;
					createTriangCenter(index1, index2, k);

					if (tokenCount > oldCount)
					{
						std::cout << "Фишек на поле: " << tokenCount << '\n';
					}
				}
				
		}
	}
};