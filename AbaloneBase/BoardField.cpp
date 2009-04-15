#include "AbaloneBase.h"

#include "BoardField.h"

BoardField::BoardField()
: myGUICoordinates(-1, -1)
, myFieldCoordinates(-1, -1)
, myBall(NO_BALL)
, myIsSelected(false)
{
}

BoardField::BoardField(int fieldX, int fieldY)
: myGUICoordinates(-1, -1)
, myFieldCoordinates(fieldX, fieldY)
, myBall(NO_BALL)
, myIsSelected(false)
{

}

BoardField::~BoardField()
{
}

CString BoardField::ToString() const
{
  return ToString(myFieldCoordinates);
}

CString BoardField::ToString(CPoint coord)
{
  CString ret;
  ret.Format("%c%d", char('A'+coord.y), coord.x+1);
  return ret;
}
