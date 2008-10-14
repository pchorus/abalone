#include "AbaloneBase.h"

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer()
{
}

HumanPlayer::~HumanPlayer()
{
}

Player::PlayerType HumanPlayer::GetPlayerType() const
{
  return PLAYER_TYPE_HUMAN;
}
