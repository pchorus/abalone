#pragma once

#include "ComputerPlayer.h"

class ABALONE_BASE_DLLINTERFACE ComputerPlayerMonteCarlo : public ComputerPlayer {
public:
  // constructor / destructor
  ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerMonteCarlo();

  virtual BallMove CalculateNextMove() const;

private:
};