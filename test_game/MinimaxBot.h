#pragma once
#include "GameLogic.h"
#include <random>

class MinimaxBot
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
    return tokenVal;
  }

  int heuristic(const GameLogic& state, const Move& move) 
  {
    int cnt = 0;
    int a = move.a;
    int b = move.b;
    for (int k = 0; k < GameBoard::MAX_POINTS; k++) 
      if (state.getConnections(a, k) && state.getConnections(b, k)) 
        cnt++;
    return cnt;
  }

  void sortMovesByHeuristic(vector<Move>& moves, const GameLogic& state) 
  {
    for (size_t i = 0; i < moves.size(); i++)
    {
      size_t bestIdx = i;
      int maxVal = heuristic(state, moves[i]);

      for (size_t j = i + 1; j < moves.size(); j++)
      {
        int curVal = heuristic(state, moves[j]);
        if (curVal > maxVal)
        {
          maxVal = curVal;
          bestIdx = j;
        }
      }
      if (bestIdx != i)
        std::swap(moves[i], moves[bestIdx]);
    }
  }

  int minimax(GameLogic& play, int depth, bool curPlayer, int alpha, int beta)
  {
    if (depth == 0 || play.isGameOver())
      return evaluation(play);
    vector<Move> moves;
    play.getAvailableMoves(moves);
    if (moves.empty())
      return evaluation(play);
    sortMovesByHeuristic(moves, play);
    if (curPlayer == botPlayer)
    {
      int bestValue = -inf;
      auto i = moves.begin();
      while (i != moves.end())
      {
        int oldTokens = play.getTotalTokens();
        if (play.makeMove(*i, curPlayer, false))
        {
          int value = minimax(play, depth - 1, !curPlayer, alpha, beta);
          play.unmakeMove(*i, curPlayer, oldTokens);
          bestValue = std::max(bestValue, value);
          alpha = std::max(alpha, bestValue);
          if (alpha >= beta)
            break;
        }
        i++;
      }
      return bestValue;
    }
    else
    {
      int bestValue = inf;
      auto i = moves.begin();
      while (i != moves.end())
      {
        int oldTokens = play.getTotalTokens();
        if (play.makeMove(*i, curPlayer, false))
        {
          int value = minimax(play, depth - 1, !curPlayer, alpha, beta);
          play.unmakeMove(*i, curPlayer, oldTokens);
          bestValue = std::min(bestValue, value);
          beta = std::min(beta, bestValue);
          if (alpha >= beta)
            break;
        }
        i++;
      }
      return bestValue;
    }
  }

public:

  MinimaxBot(GameLogic& logic, int depth = 3, int range = 0) : logic(logic), maxDepth(depth), rng(std::random_device{}()), noise(-range, range) {}

  Move choiceMove()
  {
    Move bestMove = { -1, -1 };
    vector<Move> moves;
    logic.getAvailableMoves(moves);
    if (!moves.empty())
    {
      bestMove = moves[0];
      int bestValue = -inf;
      int currentAlpha = -inf; 

      std::shuffle(moves.begin(), moves.end(), rng);
      sortMovesByHeuristic(moves, logic);
      for (const auto& move : moves)
      {
        int oldTokens = logic.getTotalTokens();
        if (logic.makeMove(move, botPlayer, false))
        {
          int value = minimax(logic, maxDepth - 1, !botPlayer, currentAlpha, inf);
          logic.unmakeMove(move, botPlayer, oldTokens);

          currentAlpha = std::max(currentAlpha, value);

          int noisyValue = value + noise(rng);
          if (noisyValue > bestValue)
          {
            bestValue = noisyValue;
            bestMove = move;
          }
        }
      }
    }
    return bestMove;
  }
};