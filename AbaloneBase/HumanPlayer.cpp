#include "AbaloneBase.h"

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const CString& name)
: Player(name)
{
}

HumanPlayer::~HumanPlayer()
{
}

Player::PlayerType HumanPlayer::GetType() const
{
  return PLAYER_TYPE_HUMAN;
}
