#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"

BallMove::BallMove()
: myBalls(new std::vector<BoardField*>)
, myDirection(NO_VALID_DIRECTION)
{
}

BallMove::~BallMove()
{
}

void BallMove::AddBall(BoardField* field)
{
  myBalls->push_back(field);
  // TODO
  //  SortSelectedBalls();
}
