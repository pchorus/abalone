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
  BallMove(const BallMove& other);
  ~BallMove();

  std::vector<BoardField*>* GetBalls() const;
  void AddBall(BoardField* field);
  void ClearBalls();
  std::vector<BoardField*>* GetOpponentBalls() const;
  void AddOpponentBall(BoardField* field);
  void ClearOpponentBalls();

  Direction GetDirection() const;
  void SetDirection(Direction direction);

  bool IsAttacking() const;
  void SetIsAttacking(bool isAttacking);

  bool GetEjectsBall() const;
  void SetEjectsBall(bool ejectsBall);

  BallMove& operator= (const BallMove& other);

private:
  std::vector<BoardField*>* myBalls;
  std::vector<BoardField*>* myOpponentBalls;
  // direction of the move
  Direction myDirection;
  // true, if the move pushes opponent's balls
  bool myIsAttacking;
  // true, if the move pushes an opponent's ball from the game board
  bool myEjectsBall;
};

inline std::vector<BoardField*>* BallMove::GetBalls() const
{
  return myBalls;
}

inline std::vector<BoardField*>* BallMove::GetOpponentBalls() const
{
  return myOpponentBalls;
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
  myBalls->push_back(field);
}

inline void BallMove::ClearBalls()
{
  myBalls->clear();
}

inline void BallMove::AddOpponentBall(BoardField* field)
{
  myOpponentBalls->push_back(field);
}

inline void BallMove::ClearOpponentBalls()
{
  myOpponentBalls->clear();
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
