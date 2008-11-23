#include "AbaloneBase.h"

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const CString& name, BoardField::Ball ball)
: Player(name, ball)
{
}

HumanPlayer::~HumanPlayer()
{
}

Player::PlayerType HumanPlayer::GetType() const
{
  return PLAYER_TYPE_HUMAN;
}
