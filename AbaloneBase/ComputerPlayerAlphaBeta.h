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

  int GetTreeDepth() const;
  void SetTreeDepth(int treeDepth);
  int GetUsedEvaluation() const;
  void SetUsedEvaluation(int eval);

private:
  void DeleteBallMoves();

  double Max(int depth, double alpha, double beta);
  double Min(int depth, double alpha, double beta);

  GameManager* mySimGameManager;

  ComputerPlayer* myMaxPlayer;
  ComputerPlayer* myMinPlayer;
  int myTreeDepth;
  int myUsedEvaluation;

  BallMove*** myBallMoves;
  int* myBallMovesSize;
};

inline int ComputerPlayerAlphaBeta::GetTreeDepth() const
{
  return myTreeDepth;
}

inline int ComputerPlayerAlphaBeta::GetUsedEvaluation() const
{
  return myUsedEvaluation;
}

inline void ComputerPlayerAlphaBeta::SetUsedEvaluation(int eval)
{
  myUsedEvaluation = eval;
}
