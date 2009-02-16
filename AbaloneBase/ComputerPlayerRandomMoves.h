#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerRandomMoves
//    Implementation of abstract base class ComputerPlayer.
//    Takes absolute random moves to simulate games.
//    This class is used in the Monte Carlo approach to simulate
//    many games.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"

class ComputerPlayerRandomMoves : public ComputerPlayer {
public:
  // contructor / destructor
  ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerRandomMoves();

  virtual BallMove CalculateNextMove();

  virtual PlayerType GetType() const;

  // test if the move is allowed by the computer player
//  virtual bool IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const;

private:
  std::vector<BallMove*> myBallMoves;
  bool myAllowAllMoves;
};