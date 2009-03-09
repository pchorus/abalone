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

#include <vector>

class BoardField;

class ABALONE_BASE_DLLINTERFACE BallMove {
public:
  // constructor / destructor
  BallMove();
  BallMove(Direction direction, bool isAttacking, bool ejectsBall, BoardField* ball1, BoardField* ball2, BoardField* ball3, BoardField* opponentBall1, BoardField* opponentBall2, BoardField* opponentBall3);
  BallMove(const BallMove& other);
  ~BallMove();

  void GetBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3) const;
  void AddBall(BoardField* field);
  void ClearBalls();
  void GetOpponentBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3) const;
  void AddOpponentBall(BoardField* field);
  void ClearOpponentBalls();

  Direction GetDirection() const;
  void SetDirection(Direction direction);

  bool IsAttacking() const;
  void SetIsAttacking(bool isAttacking);

  bool GetEjectsBall() const;
  void SetEjectsBall(bool ejectsBall);

  BallMove& operator= (const BallMove& other);

  bool HasBalls() const;

private:
  BoardField* myBall1;
  BoardField* myBall2;
  BoardField* myBall3;
  BoardField* myOpponentBall1;
  BoardField* myOpponentBall2;
  BoardField* myOpponentBall3;
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

inline void BallMove::GetOpponentBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3) const
{
  ball1 = myOpponentBall1;
  ball2 = myOpponentBall2;
  ball3 = myOpponentBall3;
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

inline void BallMove::AddOpponentBall(BoardField* field)
{
  if (!myOpponentBall1) {
    myOpponentBall1 = field;
  }
  else if (!myOpponentBall2) {
    myOpponentBall2 = field;
  }
  else if (!myOpponentBall3) {
    myOpponentBall3 = field;
  }
}

inline void BallMove::ClearOpponentBalls()
{
  myOpponentBall1 = 0;
  myOpponentBall2 = 0;
  myOpponentBall3 = 0;
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
