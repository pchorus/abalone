#include "AbaloneBase.h"

#include "BoardField.h"

BoardField::BoardField()
: myCoordinates(-1, -1)
, myBall(NO_BALL)
, myIsSelected(false)
{
}

BoardField::~BoardField()
{
}
