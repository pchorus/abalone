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

void ComputerPlayer::TakeNextTurn()
{
  CalculateNextMove();
}
