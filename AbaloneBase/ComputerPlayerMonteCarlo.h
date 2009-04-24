#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerMonteCarlo
//    Implementation of abstract base class ComputerPlayer.
//    Uses the Monte Carlo search to calculate the next move.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"

class GameManager;

class ABALONE_BASE_DLLINTERFACE ComputerPlayerMonteCarlo : public ComputerPlayer {
public:
  // constructor / destructor
  ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerMonteCarlo();

  virtual BallMove CalculateNextMove();
  virtual void CheckTime();

  DWORD GetSecondsForGame() const;
  DWORD GetLeftSecondsForGame() const;
  void SetSecondsForGame(DWORD seconds);
  int GetTurnsPerSimGame() const;
  void SetTurnsPerSimGame(int turnsPerSimGame);
  int GetUsedEvaluation() const;
  void SetUsedEvaluation(int eval);

private:
  int myTurnsPerSimGame;
  int myUsedEvaluation;
  int SimulateGamesWithMove(BallMove* ballMove);

  GameManager* mySimGameManager;

  BallMove* myBallMoves[BALL_MOVES_ARRAY_SIZE];
  int myBallMovesSize;
  // current player on the sim game board
  ComputerPlayer* myCurrentPlayer;

  DWORD myMilliSecondsForGame;
  DWORD myLeftMilliSecondsForGame;
  DWORD myStart;
  bool myKeepSimulating;
};

inline DWORD ComputerPlayerMonteCarlo::GetSecondsForGame() const
{
  return myMilliSecondsForGame / 1000;
}

inline DWORD ComputerPlayerMonteCarlo::GetLeftSecondsForGame() const
{
  return myLeftMilliSecondsForGame / 1000;
}

inline void ComputerPlayerMonteCarlo::SetSecondsForGame(DWORD seconds)
{
  myMilliSecondsForGame = seconds*1000;
  myLeftMilliSecondsForGame = seconds*1000;
}

inline int ComputerPlayerMonteCarlo::GetTurnsPerSimGame() const
{
  return myTurnsPerSimGame;
}

inline void ComputerPlayerMonteCarlo::SetTurnsPerSimGame(int turnsPerSimGame)
{
  myTurnsPerSimGame = turnsPerSimGame;
}

inline int ComputerPlayerMonteCarlo::GetUsedEvaluation() const
{
  return myUsedEvaluation;
}

inline void ComputerPlayerMonteCarlo::SetUsedEvaluation(int eval)
{
  myUsedEvaluation = eval;
}
