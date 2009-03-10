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

  enum PlayerType { PLAYER_TYPE_NONE,
                    PLAYER_TYPE_HUMAN,
                    PLAYER_TYPE_COMPUTER_MONTE_CARLO,
                    PLAYER_TYPE_COMPUTER_ALPHA_BETA,
                    PLAYER_TYPE_COMPUTER_RANDOM_MOVES,
                    PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE
                  };
  enum PlayerNumber { PLAYER_NONE, PLAYER_ONE, PLAYER_TWO };

  // contructor / destructor
  Player(const CString& name, BoardField::Ball ball, PlayerType playerType);
  virtual ~Player();

  // getter and setter
  PlayerType GetType() const;
  CString GetName() const;

  void SetBall(BoardField::Ball ball);
  BoardField::Ball GetBall() const;

private:
  CString myName;
  BoardField::Ball myBall;
  PlayerType myType;
};

inline Player::PlayerType Player::GetType() const
{
  return myType;
}

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
