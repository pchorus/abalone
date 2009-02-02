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

  for (boardFieldIterator = move.GetBalls()->begin(); boardFieldIterator != move.GetBalls()->end(); ++boardFieldIterator) {
    GetGameManager()->AddSelectedBall(*boardFieldIterator);
  }

  GetGameManager()->MoveBallsInDirection(move.GetDirection());
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
  Player* playerNextTurn = myGameManager->GetPlayerForNextTurn();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y+1)) {
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y+1);
        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall()) {
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
        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall()) {
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
        if (field1->GetBall() == playerNextTurn->GetBall() && field2->GetBall() == playerNextTurn->GetBall()) {
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
  if (myGameManager->IsPossibleDirection(UPLEFT, ballFields) && IsMoveAllowed(UPLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPLEFT, ballFields));
  }
  if (myGameManager->IsPossibleDirection(UPRIGHT, ballFields) && IsMoveAllowed(UPRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPRIGHT, ballFields));
  }
  if (myGameManager->IsPossibleDirection(LEFT, ballFields) && IsMoveAllowed(LEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(LEFT, ballFields));
  }
  if (myGameManager->IsPossibleDirection(RIGHT, ballFields) && IsMoveAllowed(RIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(RIGHT, ballFields));
  }
  if (myGameManager->IsPossibleDirection(DOWNLEFT, ballFields) && IsMoveAllowed(DOWNLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNLEFT, ballFields));
  }
  if (myGameManager->IsPossibleDirection(DOWNRIGHT, ballFields) && IsMoveAllowed(DOWNRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNRIGHT, ballFields));
  }
}

BallMove* ComputerPlayer::CreateBallMove(Direction direction, std::vector<BoardField*>* ballFields) const
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
  ballMove->SetDirection(direction);

  return ballMove;
}

bool ComputerPlayer::IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const
{
  return true;
}
