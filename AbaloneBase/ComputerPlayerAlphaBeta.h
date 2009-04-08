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
  ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball, Player::PlayerType type = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA);
  virtual ~ComputerPlayerAlphaBeta();

  virtual BallMove CalculateNextMove();

  int GetTreeDepth() const;
  void SetTreeDepth(int treeDepth);
  int GetUsedEvaluation() const;
  void SetUsedEvaluation(int eval);

protected:
  int Max(int depth, int alpha, int beta);
  int Min(int depth, int alpha, int beta);

  GameManager* mySimGameManager;

  ComputerPlayer* myMaxPlayer;
  int myTreeDepth;

  BallMove*** myBallMoves;
  int* myBallMovesSize;

private:
  void DeleteBallMoves();
  ComputerPlayer* myMinPlayer;
  int myUsedEvaluation;
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
