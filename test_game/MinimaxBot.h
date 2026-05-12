#pragma once
#include "GameLogic.h"
#include <random>

class Bot
{
  GameLogic& logic;
  bool botPlayer = false;
  int maxDepth;
  std::mt19937 rng;
  std::uniform_int_distribution<int> noise;

  const int inf = 1000;
  int evaluation(GameLogic& play)
  {
    if (play.getResult() == GameResult::Player1Win)
      return -inf;
    else if (play.getResult() == GameResult::Player2Win)
      return inf;
    else if (play.getResult() == GameResult::Draw)
      return 0;

    int tokenVal = play.getTokenCount2() - play.getTokenCount1();
    int noiseVal = noise(rng);
    return tokenVal + noiseVal;
  }

  int minimax(GameLogic& play, int depth, bool curPlayer)
  {
    if (depth == 0 || play.isGameOver())
      return evaluation(play);
    vector<Move> moves = play.getAvailableMoves();

    if (moves.empty())
      return evaluation(play);

    std::shuffle(moves.begin(), moves.end(), rng); 

    if (curPlayer == botPlayer)
    {
      int bestValue = -inf;
      for (const auto& move : moves)
      {
        GameLogic copy = play;
        copy.makeMove(move, curPlayer, false);
        int value = minimax(copy, depth - 1, !curPlayer);
        if (value > bestValue)
          bestValue = value;
      }
      return bestValue;
    }
    else
    {
      int bestValue = inf;
      for (const auto& move : moves)
      {
        GameLogic copy = play;
        copy.makeMove(move, curPlayer, false);
        int value = minimax(copy, depth - 1, !curPlayer);
        if (value < bestValue)
          bestValue = value;
      }
      return bestValue;
    }
  }

public:

  Bot(GameLogic& logic, int depth = 3, int range = 0) : logic(logic), maxDepth(depth), rng(std::random_device{}()), noise(-range, range) {}

  Move choiceMove()
  {
    vector<Move> moves = logic.getAvailableMoves();
    if (moves.empty())
      return { -1, -1 };
    Move bestMove = moves[0];
    int bestValue = -inf;

    std::shuffle(moves.begin(), moves.end(), rng);

    for (const auto& move : moves)
    {
      GameLogic copy = logic;
      copy.makeMove(move, botPlayer, false);
      int value = minimax(copy, maxDepth - 1, !botPlayer);
      if (value > bestValue)
      {
        bestValue = value;
        bestMove = move;
      }
    }
    return bestMove;
  }
};