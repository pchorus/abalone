#include "AbaloneBase.h"

#include "ComputerPlayerRandomMoves.h"

#include "GameManager.h"

#include <stdlib.h>

ComputerPlayerRandomMoves::ComputerPlayerRandomMoves(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, myAllowAllMoves(false)
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

  if (myBallMoves.empty()) {
    myAllowAllMoves = true;
    AddPossibleMovesOneBall(myBallMoves);
    AddPossibleMovesTwoBalls(myBallMoves);
    AddPossibleMovesThreeBalls(myBallMoves);
    myAllowAllMoves = false;

  }
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

// TODO: for simPlayers in the Monte Carlo search, we have to implement this
// method, but for the AlphaBeta simPlayers, moves should always be allowed
// bool ComputerPlayerRandomMoves::IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const
// {
//   if (myAllowAllMoves)
//     return true;
// 
//   bool ret = true;
// 
//   if (GetCenterDistanceRatio(direction, balls) > 0) {
//     ret = false;
//   }
//   else if (balls->size() == 1 && CheckSingleBallMoveForLoneliness(direction, balls)) {
//     ret = false;
//   }
// 
//   return ret;
// }
