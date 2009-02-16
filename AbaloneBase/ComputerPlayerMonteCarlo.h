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

  // test if the move is allowed by the computer player
  virtual bool IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const;

private:
  size_t GetGamesToSimulate() const;
  double SimulateGamesWithMove(BallMove* ballMove) const;
  // evaluates the outcome of a simulated game
  double EvaluateSimGame() const;

  GameManager* mySimGameManager;
  size_t myNoPossibleMoves;
};

inline size_t ComputerPlayerMonteCarlo::GetGamesToSimulate() const
{
//  return 8 * (100 - myNoPossibleMoves) / 10;
  return 15 * (100 - myNoPossibleMoves) / 10;
}
