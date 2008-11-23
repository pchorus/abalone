#include "AbaloneBase.h"

#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: Player(name, ball)
, myGameManager(gameManager)
{
}

ComputerPlayer::~ComputerPlayer()
{
}

Player::PlayerType ComputerPlayer::GetType() const
{
  return PLAYER_TYPE_COMPUTER;
}

void ComputerPlayer::TakeNextTurn()
{
  CalculateNextMove();
}
