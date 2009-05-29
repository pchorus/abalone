#include "AbaloneBase.h"

#include "ComputerPlayerRandomMoves.h"

#include "GameManager.h"

#include <stdlib.h>

ComputerPlayerRandomMoves::ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES)
, myBallMovesSize(0)
{
  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    myBallMoves[i] = new BallMove;
  }
}

ComputerPlayerRandomMoves::~ComputerPlayerRandomMoves()
{
  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    delete myBallMoves[i];
  }
}

BallMove ComputerPlayerRandomMoves::CalculateNextMove()
{
  myBallMovesSize = 0;
  GetGameManager()->AddPossibleMovesOneBall(this, myBallMoves, myBallMovesSize);
  GetGameManager()->AddPossibleMovesTwoBalls(this, myBallMoves, myBallMovesSize);
  GetGameManager()->AddPossibleMovesThreeBalls(this, myBallMoves, myBallMovesSize);

  int idx = (GetGameManager()->GetNextRandomNo() % myBallMovesSize);
  return *myBallMoves[idx];
}
