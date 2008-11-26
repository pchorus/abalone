#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"

BallMove::BallMove()
: myBalls(new std::vector<BoardField*>)
, myDirection(NO_VALID_DIRECTION)
{
}

BallMove::BallMove(const BallMove& other)
: myBalls(new std::vector<BoardField*>)
, myDirection(NO_VALID_DIRECTION)
{
  std::vector<BoardField*>::const_iterator i;

  for (i = other.GetBalls()->begin(); i != other.GetBalls()->end(); ++i) {
    AddBall(*i);
  }
  myDirection = other.GetDirection();
}

BallMove::~BallMove()
{
  if (myBalls) {
    myBalls->clear();
    delete myBalls;
  }
}

BallMove& BallMove::operator= (const BallMove& other)
{
  std::vector<BoardField*>::const_iterator i;

  myBalls->clear();

  for (i = other.GetBalls()->begin(); i != other.GetBalls()->end(); ++i) {
    myBalls->push_back(*i);
  }
  myDirection = other.GetDirection();

  return *this;
}
