#include "AbaloneBase.h"

#include "ComputerPlayerRandomMoves.h"

#include "GameManager.h"

#include <stdlib.h>

ComputerPlayerRandomMoves::ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
{
}

ComputerPlayerRandomMoves::~ComputerPlayerRandomMoves()
{
}

BallMove ComputerPlayerRandomMoves::CalculateNextMove()
{
  BallMove move;

  std::vector<BallMove*> ballMoves;
  int bestRating = 0;
  int newRating = 0;

  AddPossibleMovesOneBall(ballMoves);
  AddPossibleMovesTwoBalls(ballMoves);
  AddPossibleMovesThreeBalls(ballMoves);

  int idx = (double)rand() / (double)RAND_MAX * (ballMoves.size()-1);

  move = *ballMoves.at(idx);

  std::vector<BallMove*>::iterator ballMoveIterator;
  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    delete *ballMoveIterator;
  }

  return move;
}

Player::PlayerType ComputerPlayerRandomMoves::GetType() const
{
  return PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
}
