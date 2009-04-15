#include "AbaloneBase.h"

#include "BallMove.h"

#include "BoardField.h"
#include "GameManager.h"

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

CString BallMove::ToString() const
{
  CString ret;
  BoardField* lastBall(0);
  BallAxis pushAxis = NO_VALID_AXIS;
  switch (myDirection) {
  case UPLEFT:
  case DOWNRIGHT:
    pushAxis = UPPERLEFT_TO_DOWNRIGHT;
    break;
  case UPRIGHT:
  case DOWNLEFT:
    pushAxis = DOWNLEFT_TO_UPPERRIGHT;
    break;
  case LEFT:
  case RIGHT:
    pushAxis = HORIZONTAL;
    break;
  }
  BallAxis axis = GameManager::GetAxisOfBalls(myBall1, myBall2);

  if (pushAxis == axis || !myBall2) {
    // Inline Move (or single ball move)
    switch (myDirection) {
      case UPLEFT:
      case LEFT:
      case DOWNLEFT:
        if (myBall3) {
          lastBall = myBall3;
        }
        else if (myBall2) {
          lastBall = myBall2;
        }
        else {
          lastBall = myBall1;
        }
        break;
      case UPRIGHT:
      case RIGHT:
      case DOWNRIGHT:
        lastBall = myBall1;
        break;
    }
    CPoint coord = lastBall->GetFieldCoordinates();
    CPoint newCoord = GameManager::GetNextFieldCoordinatesInDirection(coord, myDirection);
    ret.Format("%s%s", BoardField::ToString(coord), BoardField::ToString(newCoord));
  }
  else {
    // side move
    if (myBall3) {
      lastBall = myBall3;
    }
    else if (myBall2) {
      lastBall = myBall2;
    }
    else {
      lastBall = myBall1;
    }
    CPoint newCoord = GameManager::GetNextFieldCoordinatesInDirection(myBall1->GetFieldCoordinates(), myDirection);
    ret.Format("%s%s%s", BoardField::ToString(myBall1->GetFieldCoordinates()), BoardField::ToString(lastBall->GetFieldCoordinates()),
    BoardField::ToString(newCoord));
  }

  return ret;
}
