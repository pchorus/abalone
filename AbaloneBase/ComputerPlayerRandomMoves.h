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

private:
  BallMove* myBallMoves[BALL_MOVES_ARRAY_SIZE];
  int myBallMovesSize;
};