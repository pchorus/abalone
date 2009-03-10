#include "AbaloneBase.h"

#include "ComputerPlayerRandomMoves.h"

#include "GameManager.h"

#include <stdlib.h>

ComputerPlayerRandomMoves::ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, myBallMovesSize(0)
{
}

ComputerPlayerRandomMoves::~ComputerPlayerRandomMoves()
{
}

BallMove ComputerPlayerRandomMoves::CalculateNextMove()
{
  GetGameManager()->AddPossibleMovesOneBall(this, myBallMoves, myBallMovesSize);
  GetGameManager()->AddPossibleMovesTwoBalls(this, myBallMoves, myBallMovesSize);
  GetGameManager()->AddPossibleMovesThreeBalls(this, myBallMoves, myBallMovesSize);

  int idx = static_cast<int>((double)rand() / (double)RAND_MAX * (myBallMovesSize-1));

  BallMove move = *myBallMoves[idx];

  for (int i = 0; i < myBallMovesSize; ++i) {
    delete myBallMoves[i];
  }
  myBallMovesSize = 0;

  return move;
}

Player::PlayerType ComputerPlayerRandomMoves::GetType() const
{
  return PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
}
