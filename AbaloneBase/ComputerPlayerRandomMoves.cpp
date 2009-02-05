#include "AbaloneBase.h"

#include "ComputerPlayerRandomMoves.h"

#include "GameManager.h"

#include <stdlib.h>

ComputerPlayerRandomMoves::ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
{
  myBallMoves.reserve(100);
}

ComputerPlayerRandomMoves::~ComputerPlayerRandomMoves()
{
}

BallMove ComputerPlayerRandomMoves::CalculateNextMove()
{
  BallMove move;

  myBallMoves.clear();

  AddPossibleMovesOneBall(myBallMoves);
  AddPossibleMovesTwoBalls(myBallMoves);
  AddPossibleMovesThreeBalls(myBallMoves);

  int idx = static_cast<int>((double)rand() / (double)RAND_MAX * (myBallMoves.size()-1));

  move = *myBallMoves.at(idx);

  std::vector<BallMove*>::iterator ballMoveIterator;
  for (ballMoveIterator = myBallMoves.begin(); ballMoveIterator != myBallMoves.end(); ++ballMoveIterator) {
    delete *ballMoveIterator;
  }

  return move;
}

Player::PlayerType ComputerPlayerRandomMoves::GetType() const
{
  return PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
}
