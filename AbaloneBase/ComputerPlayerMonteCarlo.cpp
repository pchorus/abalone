#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"

ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
{
}

ComputerPlayerMonteCarlo::~ComputerPlayerMonteCarlo()
{
}

BallMove ComputerPlayerMonteCarlo::CalculateNextMove() const
{
  BallMove ret;
  std::vector<BallMove*> ballMoves;

  GetGameManager()->AddPossibleMovesOneBall(ballMoves);
  GetGameManager()->AddPossibleMovesTwoBalls(ballMoves);
  GetGameManager()->AddPossibleMovesThreeBalls(ballMoves);
  return ret;
}
