#include "AbaloneBase.h"

#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(const CString& name)
: Player(name)
{
}

ComputerPlayer::~ComputerPlayer()
{
}

Player::PlayerType ComputerPlayer::GetType() const
{
  return PLAYER_TYPE_COMPUTER;
}
