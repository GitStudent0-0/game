#pragma once
#include <algorithm>
#include "GameBoard.h"

class GameLogic
{
	GameBoard& board;
	struct TriangCenter 
	{
		int a, b, c;   // индексы точек из массива points[]
		double q, r;  //координаты центра
		bool active;
	};

	static const int MAX_TOKENS = 60;
	TriangCenter tokens[MAX_TOKENS];
	int totalTokens;
	int tokenCount1;
	int tokenCount2;

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
		int dq = abs(p1.q - p2.q);
		int dr = abs(p1.r - p2.r);
		int ds = abs((-p1.q - p1.r) - (-p2.q - p2.r));
		int dist = std::max({ dq, dr, ds });

		if (dist < 1 || dist > 3)
		{
			std::cout << "Неверная длина линии!\n";
			flag = false;
		}
		if (dq != 0 && dr != 0 && ds != 0)
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
		int tokenCount = tokenCount1 + tokenCount2;
		for (int i = 0; i < tokenCount; i++)
		{
			if (tokens[i].active)
				if (a == tokens[i].a && b == tokens[i].b && c == tokens[i].c)
					isEqual = true;
			
		}
		return isEqual;
	}

	bool createTriangCenter(int a, int b, int c, bool toggle)
	{
		bool flag = true;
		if (totalTokens >= MAX_TOKENS) 
			flag = false;

		int temp[3] = { a, b, c };
		std::sort(temp, temp + 3);

		if (hasToken(temp[0], temp[1], temp[2])) 
			flag = false;

		tokens[totalTokens].a = temp[0];
		tokens[totalTokens].b = temp[1];
		tokens[totalTokens].c = temp[2];
		tokens[totalTokens].active = true;

		if (toggle)
			tokenCount1++;
		else
			tokenCount2++;

		totalTokens++; 
		return flag;
	}
	
	void intermediatePoints(int index1, int index2, bool toggle)
	{
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);
		int dq = (p2.q > p1.q) ? 1 : (p2.q < p1.q ? -1 : 0);
		int dr = (p2.r > p1.r) ? 1 : (p2.r < p1.r ? -1 : 0);

		int curQ = p1.q;
		int curR = p1.r;
		int prevIdx = index1;

		p1.usageCounter++;

		while (curQ != p2.q || curR != p2.r)
		{
			curQ += dq;
			curR += dr;
			int curIdx = board.getPointCoord(curQ, curR);
			if (curIdx != -1)
			{
				connections[prevIdx][curIdx] = 1;
				connections[curIdx][prevIdx] = 1;
				board.getPoint(curIdx).usageCounter++;

				for (int k = 0; k < MAX_POINTS; k++)
				
					if (connections[prevIdx][k] && connections[curIdx][k])
						if (distance(board.getPoint(prevIdx), board.getPoint(curIdx)) == 1 &&
							distance(board.getPoint(prevIdx), board.getPoint(k)) == 1 &&
							distance(board.getPoint(curIdx), board.getPoint(k)) == 1)
							if (createTriangCenter(prevIdx, curIdx, k, toggle))
							{
								if (toggle)
									std::cout << "Фишки игрока 1: " << tokenCount1 << '\n';
								else
									std::cout << "Фишки игрока 2: " << tokenCount2 << '\n';
							}
			}
			prevIdx = curIdx;
		}
	}

	bool checkInterPoints(int index1, int index2)
	{
		bool flag = true;
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);

		int dq = (p2.q > p1.q) ? 1 : (p2.q < p1.q ? -1 : 0);
		int dr = (p2.r > p1.r) ? 1 : (p2.r < p1.r ? -1 : 0);

		int curQ = p1.q;
		int curR = p1.r;
		int prevIdx = index1;

		if (p1.usageCounter >= 4)
			flag = false;

		while (curQ != p2.q || curR != p2.r)
		{
			curQ += dq;
			curR += dr;
			int curIdx = board.getPointCoord(curQ, curR);

			if (curIdx == -1)
				flag = false;

			if (board.getPoint(curIdx).usageCounter >= 4)
			{
				std::cout << "Точка на пути переполнена\n";
				flag = false;
				break;
			}
		}
		return flag;
	}


public:

	GameLogic(GameBoard& b) : board(b), totalTokens(0), tokenCount1(0), tokenCount2(0)
	{
		for (int i = 0; i < MAX_POINTS; i++)
			for (int j = 0; j < MAX_POINTS; j++)
				connections[i][j] = 0;
	}

	bool game(int index1, int index2, bool toggle)
	{
		bool moveIsMade = false;

		if ((index1 >= 0 && index1 < MAX_POINTS) && (index2 >= 0 && index2 < MAX_POINTS))
		{
			GameBoard::Point& p1 = board.getPoint(index1);
			GameBoard::Point& p2 = board.getPoint(index2);

			if (!connections[index1][index2] && isValidityStep(p1, p2) && checkInterPoints(index1, index2))
			{
				intermediatePoints(index1, index2, toggle);
				moveIsMade = true;
				
			}
		}
		else
			std::cout << "Такой точки нет \n";
		return moveIsMade;
	}
};