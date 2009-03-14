#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"

BallMove::BallMove()
: myBall1(0)
, myBall2(0)
, myBall3(0)
, myOpponentBalls(0)
, myDirection(NO_VALID_DIRECTION)
, myIsAttacking(false)
, myEjectsBall(false)
{
}

BallMove::BallMove(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, int opponentBalls)
: myBall1(ball1)
, myBall2(ball2)
, myBall3(ball3)
, myOpponentBalls(opponentBalls)
, myDirection(direction)
, myIsAttacking(isAttacking)
, myEjectsBall(ejectsBall)
{
}

BallMove::BallMove(const BallMove& other)
: myBall1(other.myBall1)
, myBall2(other.myBall2)
, myBall3(other.myBall3)
, myOpponentBalls(other.myOpponentBalls)
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
  myOpponentBalls = other.myOpponentBalls;
  myDirection = other.myDirection;
  myIsAttacking = other.myIsAttacking;
  myEjectsBall = other.myEjectsBall;

  return *this;
}
