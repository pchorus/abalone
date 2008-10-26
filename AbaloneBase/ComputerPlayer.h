#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class HumanPlayer
//    Implementation of abstract base class Player.
//    Represents a human player for the game.
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"

class ABALONE_BASE_DLLINTERFACE ComputerPlayer : public Player {

public:
  // constructor / destructor
  ComputerPlayer(const CString& name);
  virtual ~ComputerPlayer();

  // getter and setter
  virtual PlayerType GetType() const;
};