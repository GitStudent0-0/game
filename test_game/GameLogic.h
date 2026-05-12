#pragma once
#include <algorithm>
#include "GameBoard.h"
#include <vector>
using std::cout;
using std::vector;

enum class GameEndMode
{
	TokenLimit,
	NoMovesLeft
};
enum class GameResult
{
	None,
	Player1Win,
	Player2Win,
	Draw
};

enum class GameOption
{
	Classical,
	Modified
};

struct Move
{
	int a;
	int b;
};

class GameLogic
{
public:
	static const int MAX_TOKENS = 54;

private:
	friend class BoardRenderer;
	GameBoard& board;
	GameEndMode mode;
	GameResult result;
	GameOption version;
	int maxTokensToWin;
	struct TriangCenter
	{
		int a, b, c;   // индексы точек из массива points[]
		double q, r;  //координаты центра
		bool active;
	};

	vector<Move> possibleMoves;

	int totalTokens;
	int tokenCount1;
	int tokenCount2;

	TriangCenter tokens[MAX_TOKENS];

	bool connections[GameBoard::MAX_POINTS][GameBoard::MAX_POINTS];
	int usageCounter[GameBoard::MAX_POINTS];

	int distance(GameBoard::Point& p1, GameBoard::Point& p2)
	{
		int dx = abs(p1.q - p2.q);
		int dz = abs(p1.r - p2.r);
		int dy = abs((-p1.q - p1.r) - (-p2.q - p2.r));
		int dist = std::max({ dx, dy, dz });
		return dist;
	}

	bool isValidSteps(int index1, int index2)
	{
		//ближайшие соседи (q-1,r) (q-1,r+1) (q,r-1) (q,r+1) (q+1,r-1) (q+1,r)
		//отдаленные соседи по прямой 
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);
		bool isValid = true;
		int dq = abs(p1.q - p2.q);
		int dr = abs(p1.r - p2.r);
		int ds = abs((-p1.q - p1.r) - (-p2.q - p2.r));
		int dist = std::max({ dq, dr, ds });

		if (version == GameOption::Modified && (dist < 1 || dist > 3))
			isValid = false;
		else if (version == GameOption::Classical && dist != 3)
			isValid = false;
		if (dq != 0 && dr != 0 && ds != 0)
			isValid = false;
		return isValid;
	}

	void initPossibleMoves()
	{
		for (int i = 0; i < GameBoard::MAX_POINTS; i++)
			for (int j = i+1; j < GameBoard::MAX_POINTS; j++)
				if (isValidSteps(i, j))
					possibleMoves.push_back({ i, j });
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
		for (int i = 0; i < totalTokens; i++)
		{
			if (tokens[i].active)
				if (a == tokens[i].a && b == tokens[i].b && c == tokens[i].c)
					isEqual = true;

		}
		return isEqual;
	}

	bool createTriangCenter(int a, int b, int c, bool toggle)
	{
		int temp[3] = { a, b, c };
		std::sort(temp, temp + 3);

		if (hasToken(temp[0], temp[1], temp[2]) || totalTokens >= MAX_TOKENS)
			return false;

		tokens[totalTokens].a = temp[0];
		tokens[totalTokens].b = temp[1];
		tokens[totalTokens].c = temp[2];

		calculateTriangleCenter(board.getPoint(a), board.getPoint(b), board.getPoint(c),
			tokens[totalTokens].q, tokens[totalTokens].r);

		tokens[totalTokens].active = true;

		if (toggle)
			tokenCount1++;
		else
			tokenCount2++;

		totalTokens++;

		return true;
	}

	void intermediatePoints(int index1, int index2, bool toggle, bool printMessage = true)
	{
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);
		int dq = (p2.q > p1.q) ? 1 : (p2.q < p1.q ? -1 : 0);
		int dr = (p2.r > p1.r) ? 1 : (p2.r < p1.r ? -1 : 0);
		vector<int> path;
		int curQ = p1.q;
		int curR = p1.r;
		int prevIdx = index1;
		path.push_back(index1);
		usageCounter[index1]++;

		while (curQ != p2.q || curR != p2.r)
		{
			curQ += dq;
			curR += dr;
			int curIdx = board.getPointCoord(curQ, curR);
			if (curIdx != -1)
			{
				connections[prevIdx][curIdx] = 1;
				connections[curIdx][prevIdx] = 1;
				usageCounter[curIdx]++;
				path.push_back(curIdx);

				for (int k = 0; k < GameBoard::MAX_POINTS; k++)
				{	if (connections[prevIdx][k] && connections[curIdx][k])
						if (distance(board.getPoint(prevIdx), board.getPoint(curIdx)) == 1 &&
							distance(board.getPoint(prevIdx), board.getPoint(k)) == 1 &&
							distance(board.getPoint(curIdx), board.getPoint(k)) == 1)
							if (createTriangCenter(prevIdx, curIdx, k, toggle))
							{
								if (printMessage)
								{
									if (toggle)
										cout << "Фишки игрока 1: " << tokenCount1 << '\n';
									else
										cout << "Фишки игрока 2: " << tokenCount2 << '\n';
								}
							}
				}
				prevIdx = curIdx;
			}
		}
		for (int i = 0; i < path.size(); i++)
			for (int j = i + 1; j < path.size(); j++)
			{
				connections[path[i]][path[j]] = 1;
				connections[path[j]][path[i]] = 1;
			}
	}

	bool checkInterPoints(int index1, int index2, bool printMessage = true)
	{
		bool flag = true;
		GameBoard::Point& p1 = board.getPoint(index1);
		GameBoard::Point& p2 = board.getPoint(index2);

		int dq = (p2.q > p1.q) ? 1 : (p2.q < p1.q ? -1 : 0);
		int dr = (p2.r > p1.r) ? 1 : (p2.r < p1.r ? -1 : 0);

		int curQ = p1.q;
		int curR = p1.r;
		int prevIdx = index1;

		if (version == GameOption::Modified && usageCounter[index1] >= 4)
		{
			if (printMessage)
				cout << "Начальная точка переполнена\n";
			flag = false;
		}

		while (curQ != p2.q || curR != p2.r)
		{
			curQ += dq;
			curR += dr;
			int curIdx = board.getPointCoord(curQ, curR);

			if (curIdx == -1)
			{
				flag = false;
				break;
			}
			if (version == GameOption::Modified && usageCounter[curIdx] >= 4)
			{
				if (printMessage)
					cout << "Точка на пути переполнена\n";

				flag = false;
				break;
			}
			prevIdx = curIdx;
		}
		return flag;
	}


public:

	int getTotalTokens() const
	{
		return totalTokens;
	}
	int& getTokenCount1()
	{
		return tokenCount1;
	}
	int& getTokenCount2()
	{
		return tokenCount2;
	}
	int getUsageCounter(int index) const
	{
		if (index >= 0 && index < GameBoard::MAX_POINTS)
			return usageCounter[index];
	}

	GameResult getResult() const
	{
		return result;
	}

	bool isGameOver() const
	{
		return result != GameResult::None;
	}

	bool isPointOverflow(int index)
	{
		return version == GameOption::Modified && getUsageCounter(index) >= 4;
	}

	GameLogic(GameBoard& b, GameEndMode selectedMode, GameOption version, int tokensToWin = 10) : board(b), totalTokens(0), tokenCount1(0), tokenCount2(0),
		mode(selectedMode), maxTokensToWin(tokensToWin), result(GameResult::None), version(version)
	{
		for (int i = 0; i < GameBoard::MAX_POINTS; i++)
		{
			usageCounter[i] = 0;
			for (int j = 0; j < GameBoard::MAX_POINTS; j++)
				connections[i][j] = 0;
		}
		initPossibleMoves();
	}

	bool canMakeMove(int index1, int index2, bool printMessage = true)
	{
		bool makeMove = false;
		if ((index1 >= 0 && index1 < GameBoard::MAX_POINTS) && (index2 >= 0 && index2 < GameBoard::MAX_POINTS))
		{
			if (!connections[index1][index2] && isValidSteps(index1, index2) && checkInterPoints(index1, index2, printMessage))
				makeMove = true;
			else if (printMessage)
				cout << "Неверный ход\n";
		}
		return makeMove;
	}

	vector<Move> getAvailableMoves()
	{
		vector<Move> moves;
		for (const auto& move : possibleMoves)
			if (canMakeMove(move.a, move.b, false))
				moves.push_back(move);
		return moves;
	}

	bool makeMove(Move move, bool toggle, bool printMessage = true)
	{
		return game(move.a, move.b, toggle, printMessage);
	}

	bool hasAvailableMoves()
	{
		for (const auto& move : possibleMoves)
			if (canMakeMove(move.a, move.b, false))
				return true;
		return false;
	}

	bool game(int index1, int index2, bool toggle, bool printMessage = true)
	{
		if (result == GameResult::None)
		{
			if (canMakeMove(index1, index2, printMessage))
			{
				intermediatePoints(index1, index2, toggle, printMessage);
				checkGameOver(printMessage);
				return true;
			}
		}
		else if (printMessage)
				cout << "Игра уже закончена\n";
		return false;
	}

	void checkGameOver(bool printMessage = true)
	{
		if (version == GameOption::Classical)
		{
			if (mode == GameEndMode::TokenLimit)
			{
				if (tokenCount1 >= maxTokensToWin)
				{
					result = GameResult::Player1Win;
					if (printMessage)
						cout << "Победил игрок 1\n";
				}
				else if (tokenCount2 >= maxTokensToWin)
				{
					result = GameResult::Player2Win;
					if (printMessage)
						cout << "Победил игрок 2\n";
				}
			}
			else if (mode == GameEndMode::NoMovesLeft)
			{
				if (!hasAvailableMoves() || totalTokens >= MAX_TOKENS)
				{
					if (printMessage)
						cout << "Ходы закончились\n";

					if (tokenCount1 > tokenCount2)
					{
						result = GameResult::Player1Win;
						if (printMessage)
							cout << "Победил игрок 1\n";
					}
					else if (tokenCount1 < tokenCount2)
					{
						result = GameResult::Player2Win;
						if (printMessage)
							cout << "Победил игрок 2\n";
					}
					else
					{
						result = GameResult::Draw;
						if (printMessage)
							cout << "Ничья\n";
					}
				}
			}
		}
		else
		{
			if (mode == GameEndMode::TokenLimit)
			{
				if (tokenCount1 >= maxTokensToWin)
				{
					result = GameResult::Player1Win;
					if (printMessage)
						cout << "Победил игрок 1\n";
				}
				else if (tokenCount2 >= maxTokensToWin)
				{
					result = GameResult::Player2Win;
					if (printMessage)
						cout << "Победил игрок 2\n";
				}
			}
			else if (mode == GameEndMode::NoMovesLeft)
			{
				if (!hasAvailableMoves())
				{
					if (printMessage)
						cout << "Ходы закончились\n";

					if (tokenCount1 > tokenCount2)
					{
						result = GameResult::Player1Win;
						if (printMessage)
							cout << "Победил игрок 1\n";
					}
					else if (tokenCount1 < tokenCount2)
					{
						result = GameResult::Player2Win;
						if (printMessage)
							cout << "Победил игрок 2\n";
					}
					else
					{
						result = GameResult::Draw;
						if (printMessage)
							cout << "Ничья\n";
					}
				}
			}
		}
	}
};