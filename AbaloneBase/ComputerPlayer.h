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


static const double LOST_BALLS_EVALUATION_WEIGHT              = 0.1;
static const double CENTER_DISTANCE_EVALUATION_WEIGHT         = 0.3;
static const double GROUPING_EVALUATION_WEIGHT                = 0.4;
static const double ATTACKING_POWER_EVALUATION_WEIGHT         = 0.1;
static const double ATTACKED_BY_OPPONENT_EVALUATION_WEIGHT    = 0.1;


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

  void AddPossibleMovesOneBall(std::vector<BallMove*>& ballMoves) const;
  void AddPossibleMovesTwoBalls(std::vector<BallMove*>& ballMoves) const;
  void AddPossibleMovesThreeBalls(std::vector<BallMove*>& ballMoves) const;

protected:
  // methods to check whether a move should be considered possible
  int GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const;
  bool CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const;

private:
  void CheckDirections(std::vector<BoardField*>* ballFields, std::vector<BallMove*>& ballMoves) const;
  BallMove* CreateBallMove(Direction direction, bool isAttacking, std::vector<BoardField*>* ballFields) const;
  // test if the move is allowed by the computer player
  virtual bool IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const;

  GameManager* myGameManager;
};

inline GameManager* ComputerPlayer::GetGameManager() const
{
  return myGameManager;
}
