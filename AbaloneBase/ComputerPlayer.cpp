#include "AbaloneBase.h"

#include "ComputerPlayer.h"

#include "GameManager.h"
#include "GameBoard.h"

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

  GetGameManager()->DoMove(&move);
}

void ComputerPlayer::AddPossibleMovesOneBall(std::vector<BallMove*>& ballMoves) const
{
  BoardField* field = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = myGameManager->GetGameBoard();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      if (gameBoard->GetBoardFieldExist(x, y)) {
        field = gameBoard->GetBoardField(x, y);
        if (field->GetBall() == myGameManager->GetPlayerForNextTurn()->GetBall()) {
          ballFields.push_back(field);
          CheckDirections(&ballFields, ballMoves);
        }
      }
    }
  }
}

void ComputerPlayer::AddPossibleMovesTwoBalls(std::vector<BallMove*>& ballMoves) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = myGameManager->GetGameBoard();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y+1)) {
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y+1);
        if (field1->GetBall() == GetBall() && field2->GetBall() == GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(&ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y);
        if (field1->GetBall() == GetBall() && field2->GetBall() == GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(&ballFields, ballMoves);
        }
      }
      // check balls from upleft to downright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x, y-1)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x, y-1);
        if (field1->GetBall() == GetBall() && field2->GetBall() == GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(&ballFields, ballMoves);
        }
      }
    }
  }
}

void ComputerPlayer::AddPossibleMovesThreeBalls(std::vector<BallMove*>& ballMoves) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  BoardField* field3 = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  Player* playerNextTurn = myGameManager->GetPlayerForNextTurn();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y+1) && gameBoard->GetBoardFieldExist(x+2, y+2)) {
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y+1);
        field3 = gameBoard->GetBoardField(x+2, y+2);
        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall() && field3->GetBall() == playerNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(&ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y) && gameBoard->GetBoardFieldExist(x+2, y)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y);
        field3 = gameBoard->GetBoardField(x+2, y);

        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall() && field3->GetBall() == playerNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(&ballFields, ballMoves);
        }
      }
      // check balls from upleft to downright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x, y-1) && gameBoard->GetBoardFieldExist(x, y-2)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x, y-1);
        field3 = gameBoard->GetBoardField(x, y-2);

        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall() && field3->GetBall() == playerNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(&ballFields, ballMoves);
        }
      }
    }
  }
}

void ComputerPlayer::CheckDirections(std::vector<BoardField*>* ballFields, std::vector<BallMove*>& ballMoves) const
{
  bool isAttacking = false;
  bool ejectsBall = false;

  std::vector<BoardField*>* opponentFields = new std::vector<BoardField*>;

  if (myGameManager->IsPossibleDirection(UPLEFT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(UPLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPLEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (myGameManager->IsPossibleDirection(UPRIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(UPRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPRIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (myGameManager->IsPossibleDirection(LEFT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(LEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(LEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (myGameManager->IsPossibleDirection(RIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(RIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(RIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (myGameManager->IsPossibleDirection(DOWNLEFT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(DOWNLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNLEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (myGameManager->IsPossibleDirection(DOWNRIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && IsMoveAllowed(DOWNRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNRIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }

  delete opponentFields;
}

BallMove* ComputerPlayer::CreateBallMove(Direction direction, bool isAttacking, bool ejectsBall, std::vector<BoardField*>* ballFields, std::vector<BoardField*>* opponentFields) const
{
  BallMove* ballMove = new BallMove;

  std::vector<BoardField*>::iterator i;
  i = ballFields->begin();

  if (ballFields->size() >= 1) {
    ballMove->AddBall(*i);
  }
  if (ballFields->size() >= 2) {
    ++i;
    ballMove->AddBall(*i);
  }
  if (ballFields->size() >= 3) {
    ++i;
    ballMove->AddBall(*i);
  }

  i = opponentFields->begin();

  if (opponentFields->size() >= 1) {
    ballMove->AddOpponentBall(*i);
  }
  if (opponentFields->size() >= 2) {
    ++i;
    ballMove->AddOpponentBall(*i);
  }
  if (opponentFields->size() >= 3) {
    ++i;
    ballMove->AddOpponentBall(*i);
  }

  ballMove->SetDirection(direction);
  ballMove->SetIsAttacking(isAttacking);
  ballMove->SetEjectsBall(ejectsBall);

  return ballMove;
}

bool ComputerPlayer::IsMoveAllowed(Direction /*direction*/, std::vector<BoardField*>* /*balls*/) const
{
  return true;
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
