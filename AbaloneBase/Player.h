#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class Player
//    Base class for the human player and the computer player
//
//////////////////////////////////////////////////////////////////////////

#include "BoardField.h"

class ABALONE_BASE_DLLINTERFACE Player {

public:

  enum PlayerType { PLAYER_TYPE_NONE, PLAYER_TYPE_HUMAN, PLAYER_TYPE_COMPUTER };

  // contructor / destructor
  Player(const CString& name, BoardField::Ball ball);
  virtual ~Player();

  // getter and setter
  virtual PlayerType GetType() const = 0;
  CString GetName() const;

  void SetBall(BoardField::Ball ball);
  BoardField::Ball GetBall() const;

private:
  CString myName;
  BoardField::Ball myBall;
};

inline CString Player::GetName() const
{
  return myName;
}

inline BoardField::Ball Player::GetBall() const
{
  return myBall;
}

inline void Player::SetBall(BoardField::Ball ball)
{
  myBall = ball;
}
