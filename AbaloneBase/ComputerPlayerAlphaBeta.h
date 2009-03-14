#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerAlphaBeta
//    Implementation of abstract base class ComputerPlayer.
//    Uses the Alpha-Beta algorithm to calculate the next move.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"


static const int DEFAULT_TREE_DEPTH = 5;


class ABALONE_BASE_DLLINTERFACE ComputerPlayerAlphaBeta : public ComputerPlayer {
public:
  // constructor / destructor
  ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerAlphaBeta();

  virtual BallMove CalculateNextMove();

  void SetTreeDepth(int treeDepth);
  void DeleteBallMoves();

private:
  double Max(int depth, double alpha, double beta);
  double Min(int depth, double alpha, double beta);

  double EvaluateBoard() const;

  GameManager* mySimGameManager;

  ComputerPlayer* myMaxPlayer;
  ComputerPlayer* myMinPlayer;
  int myTreeDepth;
  BallMove*** myBallMoves;
  int* myBallMovesSize;
};