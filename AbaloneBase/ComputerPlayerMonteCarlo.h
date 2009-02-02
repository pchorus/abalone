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

  virtual BallMove CalculateNextMove() const;

  virtual PlayerType GetType() const;

private:
  // test if the move is allowed by the computer player
  virtual bool IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const;
  int SimulateGamesWithMove(BallMove* ballMove) const;

  GameManager* mySimGameManager;
};