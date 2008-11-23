#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class HumanPlayer
//    Implementation of abstract base class Player.
//    Represents a human player for the game.
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
  virtual PlayerType GetType() const;
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
