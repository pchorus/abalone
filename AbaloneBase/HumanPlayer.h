#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class HumanPlayer
//    Implementation of abstract base class Player.
//    Represents a human player for the game.
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"

class ABALONE_BASE_DLLINTERFACE HumanPlayer : public Player {

public:
  // constructor / destructor
  HumanPlayer();
  virtual ~HumanPlayer();

  // getter and setter
  virtual PlayerType GetPlayerType() const;
};