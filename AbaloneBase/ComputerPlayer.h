#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayer
//    Base class for the computer players using Monte Carlo search
//    or the Alpha-Beta algorithm
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "BallMove.h"

class GameManager;


static const double LOST_BALLS_EVALUATION_WEIGHT              = 0.3;
static const double CENTER_DISTANCE_EVALUATION_WEIGHT         = 0.3;
static const double GROUPING_EVALUATION_WEIGHT                = 0.3;
static const double ATTACKING_POWER_EVALUATION_WEIGHT         = 0.05;
static const double ATTACKED_BY_OPPONENT_EVALUATION_WEIGHT    = 0.05;


class ABALONE_BASE_DLLINTERFACE ComputerPlayer : public Player {

public:
  // constructor / destructor
  ComputerPlayer(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayer();

  // getter and setter
  virtual PlayerType GetType() const = 0;
  GameManager* GetGameManager() const;

  virtual BallMove CalculateNextMove() = 0;
  void TakeNextTurn();

protected:
  // methods to check whether a move should be considered possible
  int GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const;
  bool CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const;

private:
  GameManager* myGameManager;
};

inline GameManager* ComputerPlayer::GetGameManager() const
{
  return myGameManager;
}
