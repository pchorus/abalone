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

  int GetGamesToSimulate() const;
  void SetGamesToSimulate(int gamesToSimulate);
  int GetTurnsPerSimGame() const;
  void SetTurnsPerSimGame(int turnsPerSimGame);

private:
  int myGamesToSimulate;
  int myTurnsPerSimGame;
  double SimulateGamesWithMove(BallMove* ballMove) const;

  GameManager* mySimGameManager;

  BallMove* myBallMoves[BALL_MOVES_ARRAY_SIZE];
  int myBallMovesSize;
  // current player on the sim game board
  ComputerPlayer* myCurrentPlayer;
};

inline int ComputerPlayerMonteCarlo::GetGamesToSimulate() const
{
  return myGamesToSimulate;
}

inline void ComputerPlayerMonteCarlo::SetGamesToSimulate(int gamesToSimulate)
{
  myGamesToSimulate = gamesToSimulate;
}

inline int ComputerPlayerMonteCarlo::GetTurnsPerSimGame() const
{
  return myTurnsPerSimGame;
}

inline void ComputerPlayerMonteCarlo::SetTurnsPerSimGame(int turnsPerSimGame)
{
  myTurnsPerSimGame = turnsPerSimGame;
}
