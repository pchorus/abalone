#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerEvaluateNextStep
//    Implementation of abstract base class ComputerPlayer.
//    Uses the evaluation function only for the next step
//    to calculate the next move.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"

class ABALONE_BASE_DLLINTERFACE ComputerPlayerEvaluateNextMove : public ComputerPlayer {
public:
  // constructor / destructor
  ComputerPlayerEvaluateNextMove(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerEvaluateNextMove();

  virtual BallMove CalculateNextMove();

  virtual PlayerType GetType() const;

};
