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

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator i;

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}
