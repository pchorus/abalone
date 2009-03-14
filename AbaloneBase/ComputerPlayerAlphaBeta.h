#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerAlphaBeta
//    Implementation of abstract base class ComputerPlayer.
//    Uses the Alpha-Beta algorithm to calculate the next move.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"

static const int TREE_DEPTH = 5;

class ABALONE_BASE_DLLINTERFACE ComputerPlayerAlphaBeta : public ComputerPlayer {
public:
  // constructor / destructor
  ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerAlphaBeta();

  virtual BallMove CalculateNextMove();

private:
  double Max(int depth, double alpha, double beta);
  double Min(int depth, double alpha, double beta);

  double EvaluateBoard() const;

  GameManager* mySimGameManager;

  ComputerPlayer* myMaxPlayer;
  ComputerPlayer* myMinPlayer;
  BallMove* myBallMoves[TREE_DEPTH][BALL_MOVES_ARRAY_SIZE];
  int myBallMovesSize[TREE_DEPTH];
};