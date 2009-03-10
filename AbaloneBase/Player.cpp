#include "AbaloneBase.h"

#include "Player.h"

Player::Player(const CString& name, BoardField::Ball ball, PlayerType playerType)
: myName(name)
, myBall(ball)
, myType(playerType)
{
}

Player::~Player()
{
}
