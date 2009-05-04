#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class BallMove
//    Represents a possible move that can be taken in a specific
//    game situation. It consists of the balls that should be moved
//    and the direction in which they are moved.
//
//////////////////////////////////////////////////////////////////////////

#include "AbaloneBase.h"

#include "GameManager.h"

class BoardField;


class ABALONE_BASE_DLLINTERFACE BallMove {
public:
  // constructor / destructor
  BallMove();
  BallMove(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, int opponentBalls);
  BallMove(const BallMove& other);
  ~BallMove();

  void Init(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, int opponentBalls);

  void GetBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3) const;
  void SetBalls(BoardField* ball1, BoardField* ball2, BoardField* ball3);
  void AddBall(BoardField* field);
  void ClearBalls();

  int GetOpponentBalls() const;
  Direction GetDirection() const;
  void SetDirection(Direction direction);

  bool IsInline() const;
  bool IsAttacking() const;
  void SetIsAttacking(bool isAttacking);

  bool GetEjectsBall() const;
  void SetEjectsBall(bool ejectsBall);

  int GetNoOfBalls() const;

  BallMove& operator= (const BallMove& other);
  bool operator== (const BallMove& other);
  bool operator!= (const BallMove& other);

  bool HasBalls() const;

  CString ToString() const;
  CString ToStringDebug() const;

  int Compare(const BallMove* other) const;

private:
  BoardField* myBall1;
  BoardField* myBall2;
  BoardField* myBall3;

  int myOpponentBalls;
  // direction of the move
  Direction myDirection;
  // true, if the move pushes opponent's balls
  bool myIsAttacking;
  // true, if the move pushes an opponent's ball from the game board
  bool myEjectsBall;
};

inline void BallMove::GetBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3) const
{
  ball1 = myBall1;
  ball2 = myBall2;
  ball3 = myBall3;
}

inline void BallMove::SetBalls(BoardField* ball1, BoardField* ball2, BoardField* ball3)
{
  myBall1 = ball1;
  myBall2 = ball2;
  myBall3 = ball3;
}

inline Direction BallMove::GetDirection() const
{
  return myDirection;
}

inline void BallMove::SetDirection(Direction direction)
{
  myDirection = direction;
}

inline void BallMove::AddBall(BoardField* field)
{
  if (!myBall1) {
    myBall1 = field;
  }
  else if (!myBall2) {
    myBall2 = field;
  }
  else if (!myBall3) {
    myBall3 = field;
  }
}

inline void BallMove::ClearBalls()
{
  myBall1 = 0;
  myBall2 = 0;
  myBall3 = 0;
}

inline int BallMove::GetOpponentBalls() const
{
  return myOpponentBalls;
}

inline bool BallMove::IsAttacking() const
{
  return myIsAttacking;
}

inline void BallMove::SetIsAttacking(bool isAttacking)
{
  myIsAttacking = isAttacking;
}

inline bool BallMove::GetEjectsBall() const
{
  return myEjectsBall;
}

inline void BallMove::SetEjectsBall(bool ejectsBall)
{
  myEjectsBall = ejectsBall;
}

inline bool BallMove::HasBalls() const
{
  return !!myBall1;
}

inline void BallMove::Init(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, int opponentBalls)
{
  myBall1 = ball1;
  myBall2 = ball2;
  myBall3 = ball3;
  myOpponentBalls = opponentBalls;
  myDirection = direction;
  myIsAttacking = isAttacking;
  myEjectsBall = ejectsBall;
}

inline bool BallMove::IsInline() const
{
  GameManager::GetAxisOfBalls(myBall1, myBall2);

  switch (myDirection) {
  case UPLEFT:
  case DOWNRIGHT:
    return GameManager::GetAxisOfBalls(myBall1, myBall2) == UPPERLEFT_TO_DOWNRIGHT;
    break;
  case UPRIGHT:
  case DOWNLEFT:
    return GameManager::GetAxisOfBalls(myBall1, myBall2) == DOWNLEFT_TO_UPPERRIGHT;
    break;
  case LEFT:
  case RIGHT:
    return GameManager::GetAxisOfBalls(myBall1, myBall2) == HORIZONTAL;
    break;
  }
  return false;
}

inline int BallMove::GetNoOfBalls() const
{
  if (myBall3)
    return 3;
  if (myBall2)
    return 2;
  return 1;
}
