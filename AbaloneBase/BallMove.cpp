#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"

BallMove::BallMove()
: myBall1(0)
, myBall2(0)
, myBall3(0)
, myOpponentBall1(0)
, myOpponentBall2(0)
, myOpponentBall3(0)
, myDirection(NO_VALID_DIRECTION)
, myIsAttacking(false)
, myEjectsBall(false)
{
}

BallMove::BallMove(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, BoardField* opponentBall1, BoardField* opponentBall2, BoardField* opponentBall3)
: myBall1(ball1)
, myBall2(ball2)
, myBall3(ball3)
, myOpponentBall1(opponentBall1)
, myOpponentBall2(opponentBall2)
, myOpponentBall3(opponentBall3)
, myDirection(direction)
, myIsAttacking(isAttacking)
, myEjectsBall(ejectsBall)
{
}

BallMove::BallMove(const BallMove& other)
: myBall1(other.myBall1)
, myBall2(other.myBall2)
, myBall3(other.myBall3)
, myOpponentBall1(other.myOpponentBall1)
, myOpponentBall2(other.myOpponentBall2)
, myOpponentBall3(other.myOpponentBall3)
, myDirection(other.myDirection)
, myIsAttacking(other.myIsAttacking)
, myEjectsBall(other.myEjectsBall)
{
}

BallMove::~BallMove()
{
}

BallMove& BallMove::operator= (const BallMove& other)
{
  myBall1 = other.myBall1;
  myBall2 = other.myBall2;
  myBall3 = other.myBall3;
  myOpponentBall1 = other.myOpponentBall1;
  myOpponentBall2 = other.myOpponentBall2;
  myOpponentBall3 = other.myOpponentBall3;
  myDirection = other.myDirection;
  myIsAttacking = other.myIsAttacking;
  myEjectsBall = other.myEjectsBall;

  return *this;
}
