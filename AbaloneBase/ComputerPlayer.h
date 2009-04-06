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


class ABALONE_BASE_DLLINTERFACE ComputerPlayer : public Player {

public:
  // constructor / destructor
  ComputerPlayer(GameManager* gameManager, const CString& name, BoardField::Ball ball, PlayerType playerType);
  virtual ~ComputerPlayer();

  // getter and setter
  GameManager* GetGameManager() const;

  virtual BallMove CalculateNextMove() = 0;
  void TakeNextTurn();

protected:
  // methods to check whether a move should be considered possible
//   int GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const;
//   bool CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const;

private:
  GameManager* myGameManager;
};

inline GameManager* ComputerPlayer::GetGameManager() const
{
  return myGameManager;
}
