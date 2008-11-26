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

  Direction GetDirection() const;
  void SetDirection(Direction direction);

  BallMove& operator= (const BallMove& other);

private:
  std::vector<BoardField*>* myBalls;
  Direction myDirection;
};

inline std::vector<BoardField*>* BallMove::GetBalls() const
{
  return myBalls;
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
