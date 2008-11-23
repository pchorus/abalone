#pragma once

#include "AbaloneBase.h"

#include <vector>

class BoardField;

class ABALONE_BASE_DLLINTERFACE BallMove {
public:
  // constructor / destructor
  BallMove();
  ~BallMove();

  std::vector<BoardField*>* GetBalls();
  void AddBall(BoardField* field);

  Direction GetDirection() const;
  void SetDirection(Direction direction);


private:
  std::vector<BoardField*>* myBalls;
  Direction myDirection;
};

inline std::vector<BoardField*>* BallMove::GetBalls()
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
