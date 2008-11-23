#include "AbaloneBase.h"

#include "Player.h"

Player::Player(const CString& name, BoardField::Ball ball)
: myName(name)
, myBall(ball)
{
}

Player::~Player()
{
}
