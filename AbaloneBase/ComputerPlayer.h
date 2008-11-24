#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayer
//    Base class for the computer players using Monte Carlo search
//    or the Alpha-Beta algorithm
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "BallMove.h"

class GameManager;

class ABALONE_BASE_DLLINTERFACE ComputerPlayer : public Player {

public:
  // constructor / destructor
  ComputerPlayer(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayer();

  // getter and setter
  virtual PlayerType GetType() const = 0;
  GameManager* GetGameManager() const;

  virtual BallMove CalculateNextMove() const = 0;
  void TakeNextTurn();

private:
  GameManager* myGameManager;
};

inline GameManager* ComputerPlayer::GetGameManager() const
{
  return myGameManager;
}
