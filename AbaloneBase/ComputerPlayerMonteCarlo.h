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

  virtual BallMove CalculateNextMove();

  virtual PlayerType GetType() const;

private:
  size_t GetGamesToSimulate() const;
  // test if the move is allowed by the computer player
  virtual bool IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const;
  double SimulateGamesWithMove(BallMove* ballMove) const;
  // methods to check whether a move should be considered possible
  int GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const;
  bool CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const;
  // evaluates the outcome of a simulated game
  double EvaluateSimGame() const;
  // calculates the ratio of lost balls
  double CalcLostBallsRatioSimGame() const;
  // returns the average distance of the player's marbles to the game board's center
  // at the end of a simulated game
  double CalcAvgCenterDistanceSimGame() const;
  // calculates the average grouping of the marbles
  // TODO: this can be calculated in the same loop as in GetAvgCenterDistanceSimGame
  // put them into one to improve performance
  double CalcAvgGroupingSimGame() const;
  // calculates the attacking power on the opponent
  double CalcAttackingPowerOnOpponentSimGame() const;

  // returns the Manhattan distance of the passed coordinates to the center
  int CalcCenterDistance(CPoint coord) const;
  // returns the amount of neighbor marbles of the same color the
  // marble at coord
  int CalcGroupingSimGameField(CPoint coord) const;

  GameManager* mySimGameManager;
  size_t myNoPossibleMoves;
};

inline size_t ComputerPlayerMonteCarlo::GetGamesToSimulate() const
{
//  return 8 * (100 - myNoPossibleMoves) / 10;
  return 15 * (100 - myNoPossibleMoves) / 10;
}
