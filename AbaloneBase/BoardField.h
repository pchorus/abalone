#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class BoardField
//    Represents a single field on the game board.
//
//////////////////////////////////////////////////////////////////////////

class ABALONE_BASE_DLLINTERFACE BoardField {
public:

  enum Ball { WHITE_BALL = 0, BLACK_BALL, NO_BALL };

  // constructor / destructor
  BoardField();
  BoardField(int fieldX, int fieldY);
  ~BoardField();

  CPoint GetGUICoordinates() const;
  void SetGUICoordinates(CPoint point);
  void SetGUICoordinates(int x, int y);
  CPoint GetFieldCoordinates() const;
  void SetFieldCoordinates(CPoint point);
  void SetFieldCoordinates(int x, int y);

  Ball GetBall() const;
  void SetBall(Ball ball);
  bool IsSelected() const;
  void SetIsSelected(bool isSelected);

  CString ToString() const;
  static CString ToString(CPoint coord);

private:
  // GUICoordinates in the framewindow
  CPoint myGUICoordinates;
  CPoint myFieldCoordinates;
  Ball myBall;
  bool myIsSelected;
};

inline CPoint BoardField::GetGUICoordinates() const
{
  return myGUICoordinates;
}

inline void BoardField::SetGUICoordinates(CPoint point)
{
  myGUICoordinates = point;
}

inline void BoardField::SetGUICoordinates(int x, int y)
{
  myGUICoordinates.x = x;
  myGUICoordinates.y = y;
}

inline CPoint BoardField::GetFieldCoordinates() const
{
  return myFieldCoordinates;
}

inline void BoardField::SetFieldCoordinates(CPoint point)
{
  myFieldCoordinates = point;
}

inline void BoardField::SetFieldCoordinates(int x, int y)
{
  myFieldCoordinates.x = x;
  myFieldCoordinates.y = y;
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
