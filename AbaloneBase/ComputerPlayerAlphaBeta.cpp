#include "AbaloneBase.h"

#include "ComputerPlayerAlphaBeta.h"

#include "GameManager.h"

ComputerPlayerAlphaBeta::ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
{
}

ComputerPlayerAlphaBeta::~ComputerPlayerAlphaBeta()
{
}

BallMove ComputerPlayerAlphaBeta::CalculateNextMove() const
{
  ASSERT(false);
  // TODO: implement
  BallMove ret;
  std::vector<BallMove*> ballMoves;

  AddPossibleMovesOneBall(ballMoves);
  AddPossibleMovesTwoBalls(ballMoves);
  AddPossibleMovesThreeBalls(ballMoves);

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator i;

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}

Player::PlayerType ComputerPlayerAlphaBeta::GetType() const
{
  return PLAYER_TYPE_COMPUTER_ALPHA_BETA;
}
