#include "AbaloneBase.h"

#include "ComputerPlayer.h"

#include "GameManager.h"
#include "GameBoard.h"

ComputerPlayer::ComputerPlayer(GameManager* gameManager, const CString& name, BoardField::Ball ball, PlayerType playerType)
: Player(name, ball, playerType)
, myGameManager(gameManager)
{
}

ComputerPlayer::~ComputerPlayer()
{
}

void ComputerPlayer::TakeNextTurn()
{
  BallMove move = CalculateNextMove();
  GetGameManager()->DoMove(&move);
}

bool ComputerPlayer::CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const
{
  // TODO: improvement: only disallow such a move if the marble wasn't alone before the move

  // TODO: perhaps this method can be implemented with the help of a loop over
  // the Direction enum's values

  GameBoard* gameBoard = GetGameManager()->GetGameBoard();
  CPoint newFieldCoord = GetGameManager()->GetNextFieldCoordinatesInDirection((*(balls->begin()))->GetFieldCoordinates(), direction);
  CPoint checkCoord;
  // check the fields around the new field for fellow balls
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, UPLEFT);
  if (direction != DOWNRIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, UPRIGHT);
  if (direction != DOWNLEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, LEFT);
  if (direction != RIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, RIGHT);
  if (direction != LEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, DOWNLEFT);
  if (direction != UPRIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, DOWNRIGHT);
  if (direction != UPLEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  return true;
}

int ComputerPlayer::GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const
{
  int ret = 0;
  CPoint currentCoord;
  CPoint centerCoord(4, 4);
  CPoint afterMoveCoord;

  // current Manhattan distance between balls and the game board's center
  for (std::vector<BoardField*>::iterator i = balls->begin(); i != balls->end(); ++i) {
    currentCoord = (*i)->GetFieldCoordinates();
    afterMoveCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(currentCoord, direction);
    ret += GetGameManager()->CalcCenterDistance(afterMoveCoord) - GetGameManager()->CalcCenterDistance(currentCoord);
  }

  return ret;
}
