#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"

BallMove::BallMove()
: myBalls(new std::vector<BoardField*>)
, myOpponentBalls(new std::vector<BoardField*>)
, myDirection(NO_VALID_DIRECTION)
, myIsAttacking(false)
, myEjectsBall(false)
{
}

BallMove::BallMove(const BallMove& other)
: myBalls(new std::vector<BoardField*>)
, myOpponentBalls(new std::vector<BoardField*>)
, myDirection(NO_VALID_DIRECTION)
, myIsAttacking(false)
, myEjectsBall(false)
{
  std::vector<BoardField*>::const_iterator i;

  for (i = other.GetBalls()->begin(); i != other.GetBalls()->end(); ++i) {
    AddBall(*i);
  }

  for (i = other.GetOpponentBalls()->begin(); i != other.GetOpponentBalls()->end(); ++i) {
    AddOpponentBall(*i);
  }

  myDirection = other.GetDirection();
  myIsAttacking = other.IsAttacking();
  myEjectsBall = other.GetEjectsBall();
}

BallMove::~BallMove()
{
  if (myBalls) {
    myBalls->clear();
    delete myBalls;
  }

  if (myOpponentBalls) {
    myOpponentBalls->clear();
    delete myOpponentBalls;
  }
}

BallMove& BallMove::operator= (const BallMove& other)
{
  std::vector<BoardField*>::const_iterator i;

  myBalls->clear();
  myOpponentBalls->clear();

  for (i = other.GetBalls()->begin(); i != other.GetBalls()->end(); ++i) {
    myBalls->push_back(*i);
  }

  for (i = other.GetOpponentBalls()->begin(); i != other.GetOpponentBalls()->end(); ++i) {
    myOpponentBalls->push_back(*i);
  }

  myDirection = other.GetDirection();
  myIsAttacking = other.IsAttacking();
  myEjectsBall = other.GetEjectsBall();

  return *this;
}
