#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class BoardField
//    Represents a single field on the game board.
//
//////////////////////////////////////////////////////////////////////////

class ABALONE_BASE_DLLINTERFACE BoardField {
public:

  enum Ball { NO_BALL, WHITE_BALL, BLACK_BALL };

  // constructor / destructor
  BoardField();
  ~BoardField();

  CPoint GetCoordinates() const;
  void SetCoordinates(CPoint point);
  void SetCoordinates(int x, int y);
  Ball GetBall() const;
  void SetBall(Ball ball);
  bool IsSelected() const;
  void SetIsSelected(bool isSelected);

private:
  CPoint myCoordinates;
  Ball myBall;
  bool myIsSelected;
};

inline CPoint BoardField::GetCoordinates() const
{
  return myCoordinates;
}

inline void BoardField::SetCoordinates(CPoint point)
{
  myCoordinates = point;
}

inline void BoardField::SetCoordinates(int x, int y)
{
  myCoordinates.x = x;
  myCoordinates.y = y;
}

inline BoardField::Ball BoardField::GetBall() const
{
  return myBall;
}

inline void BoardField::SetBall(Ball ball)
{
  myBall = ball;
}

inline bool BoardField::IsSelected() const
{
  return myIsSelected;
}

inline void BoardField::SetIsSelected(bool isSelected)
{
  myIsSelected = isSelected;
}
