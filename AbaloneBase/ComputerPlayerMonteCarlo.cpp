#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"

#include <stdlib.h>

static const int GAMES_TO_SIMULATE = 300;

ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, mySimGameBoard(new GameBoard)
{
}

ComputerPlayerMonteCarlo::~ComputerPlayerMonteCarlo()
{
  if (mySimGameBoard) {
    delete mySimGameBoard;
    mySimGameBoard = 0;
  }
}

BallMove ComputerPlayerMonteCarlo::CalculateNextMove() const
{
  BallMove ret;
  std::vector<BallMove*> ballMoves;
  int bestRating = 0;
  int newRating = 0;

  GetGameManager()->AddPossibleMovesOneBall(ballMoves);
  GetGameManager()->AddPossibleMovesTwoBalls(ballMoves);
  GetGameManager()->AddPossibleMovesThreeBalls(ballMoves);

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator ballMoveIterator;


  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    newRating = SimulateGamesWithMove(*ballMoveIterator);
    if (newRating > bestRating) {
      bestRating = newRating;
      ret = **ballMoveIterator;
    }
  }

  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    delete *ballMoveIterator;
  }

  return ret;
}

Player::PlayerType ComputerPlayerMonteCarlo::GetType() const
{
  return PLAYER_TYPE_COMPUTER_MONTE_CARLO;
}

int ComputerPlayerMonteCarlo::SimulateGamesWithMove(BallMove* ballMove) const
{
  unsigned int ret = 0;

  mySimGameBoard = GetGameManager()->GetGameBoard();

  return ret;
}
