#include "AbaloneBase.h"

#include "ComputerPlayer.h"

#include "GameManager.h"

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
  BallMove move = CalculateNextMove();
  std::vector<BoardField*>::iterator boardFieldIterator;

  for (boardFieldIterator = move.GetBalls()->begin(); boardFieldIterator != move.GetBalls()->end(); ++boardFieldIterator) {
    GetGameManager()->AddSelectedBall(*boardFieldIterator);
  }

  GetGameManager()->MoveBallsInDirection(move.GetDirection());
}
