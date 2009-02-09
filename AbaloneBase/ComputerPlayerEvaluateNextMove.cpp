#include "AbaloneBase.h"

#include "ComputerPlayerEvaluateNextMove.h"

ComputerPlayerEvaluateNextMove::ComputerPlayerEvaluateNextMove(GameManager* gameManager, const CString& name, BoardField::Ball ball)
:ComputerPlayer(gameManager, name, ball)
{

}

ComputerPlayerEvaluateNextMove::~ComputerPlayerEvaluateNextMove()
{
}

BallMove ComputerPlayerEvaluateNextMove::CalculateNextMove()
{
  BallMove move;
  return move;
}

Player::PlayerType ComputerPlayerEvaluateNextMove::GetType() const
{
  return PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
}
