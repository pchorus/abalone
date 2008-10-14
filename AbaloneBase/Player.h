#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class Player
//    Base class for the human player and the computer player
//
//////////////////////////////////////////////////////////////////////////

class ABALONE_BASE_DLLINTERFACE Player {

public:

  enum PlayerType { PLAYER_TYPE_NONE, PLAYER_TYPE_HUMAN, PLAYER_TYPE_COMPUTER };

  // contructor / destructor
  Player();
  virtual ~Player();

  // getter and setter
  virtual PlayerType GetPlayerType() const = 0;
};