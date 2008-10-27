#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

GameManager::GameManager()
: myGameBoard(new GameBoard)
, myIsGameStarted(false)
, myPlayer1(0)
, myPlayer2(0)
, myNextTurn(0)
{
}

GameManager::~GameManager()
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }
}

void GameManager::SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2)
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }

  ASSERT(typePlayer1 != Player::PLAYER_TYPE_NONE && typePlayer2 != Player::PLAYER_TYPE_NONE);

  if (typePlayer1 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer1 = new HumanPlayer(namePlayer1 != "" ? namePlayer1 : "Player 1");
  }
  else {
    myPlayer1 = new ComputerPlayer(namePlayer1 != "" ? namePlayer1 : "Player 1");
  }

  if (typePlayer2 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer2 = new HumanPlayer(namePlayer2 != "" ? namePlayer2 : "Player 2");
  }
  else {
    myPlayer2 = new ComputerPlayer(namePlayer2 != "" ? namePlayer2 : "Player 2");
  }

  myNextTurn = myPlayer1;
}

bool GameManager::CanSelectBall(BoardField* field) const
{
  bool ret = false;
  BoardField* selectedField1 = 0;
  CPoint selectedFieldPoint1;
  BoardField* selectedField2 = 0;
  CPoint selectedFieldPoint2;

  CPoint fieldPoint = field->GetFieldCoordinates();

  if (mySelectedBalls.size() < 3) {
    if (mySelectedBalls.empty()) {
      ret = true;
    }
    else if (mySelectedBalls.size() == 1) {
      std::set<BoardField*>::const_iterator iter;
      for (iter = mySelectedBalls.begin(); iter != mySelectedBalls.end(); ++iter) {
        selectedField1 = *iter;
        selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
        if ((selectedFieldPoint1.x - fieldPoint.x == 0 && abs(selectedFieldPoint1.y - fieldPoint.y) == 1)
          || (selectedFieldPoint1.y - fieldPoint.y == 0 && abs(selectedFieldPoint1.x - fieldPoint.x) == 1)
          || (selectedFieldPoint1.x - fieldPoint.x == 1 && selectedFieldPoint1.y - fieldPoint.y == 1)
          || (fieldPoint.x - selectedFieldPoint1.x == 1 && fieldPoint.y - selectedFieldPoint1.y == 1))
        {
          ret = true;
          break;
        }
      }
    }
    else if (mySelectedBalls.size() == 2) {
      selectedField1 = *mySelectedBalls.begin();
      selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
      selectedField2 = *(++mySelectedBalls.begin());
      selectedFieldPoint2 = selectedField2->GetFieldCoordinates();

      if ((selectedFieldPoint1.x == selectedFieldPoint2.x && selectedFieldPoint1.x == fieldPoint.x)
        || (selectedFieldPoint1.y == selectedFieldPoint2.y && selectedFieldPoint1.y == fieldPoint.y)
        ||
          ((selectedFieldPoint1.x - selectedFieldPoint2.x == 1 && selectedFieldPoint1.y - selectedFieldPoint2.y == 1)
          || (selectedFieldPoint1.x - selectedFieldPoint2.x == -1 && selectedFieldPoint1.y - selectedFieldPoint2.y == -1))
          && (   (selectedFieldPoint1.x - fieldPoint.x == 1 && selectedFieldPoint1.y - fieldPoint.y == 1)
              || (selectedFieldPoint1.x - fieldPoint.x == -1 && selectedFieldPoint1.y - fieldPoint.y == -1)
              || (selectedFieldPoint2.x - fieldPoint.x == 1 && selectedFieldPoint2.y - fieldPoint.y == 1)
              || (selectedFieldPoint2.x - fieldPoint.x == -1 && selectedFieldPoint2.y - fieldPoint.y == -1))
        ) {
        ret = true;
      }
    }
  }

  return ret;
}

void GameManager::AddSelectedBall(BoardField* field)
{
  mySelectedBalls.insert(field);
  field->SetIsSelected(true);
}

void GameManager::RemoveSelectedBall(BoardField* field)
{
  mySelectedBalls.erase(field);
  field->SetIsSelected(false);
}

void GameManager::ClearSelectedBalls()
{
  std::set<BoardField*>::const_iterator iter;
  for (iter = mySelectedBalls.begin(); iter != mySelectedBalls.end(); ++iter) {
    (*iter)->SetIsSelected(false);
  }

  mySelectedBalls.clear();
}

BOOL GameManager::IsPossibleDirection(Direction direction) const
{
  BOOL ret = FALSE;
  switch (direction) {
  case UPLEFT:      ret = IsUpLeftPossible();      break;
  case UPRIGHT:     ret = IsUpRightPossible();     break;
  case LEFT:        ret = IsLeftPossible();        break;
  case RIGHT:       ret = IsRightPossible();       break;
  case DOWNLEFT:    ret = IsDownLeftPossible();    break;
  case DOWNRIGHT:   ret = IsDownRightPossible();   break;
  }
  return ret;
}

void GameManager::MoveBallsInDirection(Direction direction)
{
  // TODO: implement
  switch (direction) {
  case UPLEFT:      MoveBallsUpLeft();      break;
  case UPRIGHT:     MoveBallsUpRight();     break;
  case LEFT:        MoveBallsLeft();        break;
  case RIGHT:       MoveBallsRight();       break;
  case DOWNLEFT:    MoveBallsDownLeft();    break;
  case DOWNRIGHT:   MoveBallsDownRight();   break;
  }
  
  mySelectedBalls.clear();
  TurnIsOver();
}

BOOL GameManager::IsUpLeftPossible() const
{
  BOOL ret = FALSE;

  BoardField* field1 = 0;
  CPoint fieldCoord1;
  BoardField* field2 = 0;
  CPoint fieldCoord2;
  BoardField::Ball opponentBall = BoardField::NO_BALL;

  if (mySelectedBalls.size() == 1) {
    field1 = *mySelectedBalls.begin();
    fieldCoord1 = field1->GetFieldCoordinates();

    if (myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y + 1) &&
      myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y + 1)->GetBall() == BoardField::NO_BALL)
    {
      ret = TRUE;
    }
  }
  else if (mySelectedBalls.size() == 2) {
    field1 = *mySelectedBalls.begin();
    fieldCoord1 = field1->GetFieldCoordinates();
    field2 = *(++mySelectedBalls.begin());
    fieldCoord2 = field2->GetFieldCoordinates();
    opponentBall = BoardField::BLACK_BALL;
    if (field1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }

    if (fieldCoord1.x == fieldCoord2.x) {
      int i = max(fieldCoord1.y, fieldCoord2.y);

      if (myGameBoard->GetBoardFieldExist(fieldCoord1.x, i+1)
        && ((myGameBoard->GetBoardField(fieldCoord1.x, i+1)->GetBall() == opponentBall
        && ((myGameBoard->GetBoardFieldExist(fieldCoord1.x, i+2)
        && myGameBoard->GetBoardField(fieldCoord1.x, i+2)->GetBall() == BoardField::NO_BALL))
        || !(myGameBoard->GetBoardFieldExist(fieldCoord1.x, i+2)))
        || myGameBoard->GetBoardField(fieldCoord1.x, i+1)->GetBall() == BoardField::NO_BALL))
      {
        ret = TRUE;
      }
    }
    else if (fieldCoord1.y == fieldCoord2.y) {
      if (myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y + 1)
        && myGameBoard->GetBoardFieldExist(fieldCoord2.x, fieldCoord2.y + 1)
        && myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y + 1)->GetBall() == BoardField::NO_BALL
        && myGameBoard->GetBoardField(fieldCoord2.x, fieldCoord2.y + 1)->GetBall() == BoardField::NO_BALL)
      {
        ret = TRUE;
      }
    }
    else {
      // TODO: implement
    }
  }
  else if (mySelectedBalls.size() == 3) {
    // TODO: implement
  }

  return ret;
}

BOOL GameManager::IsUpRightPossible() const
{
  // TODO: implement
  BOOL ret = FALSE;
  return ret;
}

BOOL GameManager::IsLeftPossible() const
{
  // TODO: implement
  BOOL ret = FALSE;
  return ret;
}

BOOL GameManager::IsRightPossible() const
{
  // TODO: implement
  BOOL ret = FALSE;
  return ret;
}

BOOL GameManager::IsDownLeftPossible() const
{
  // TODO: implement
  BOOL ret = FALSE;
  return ret;
}

BOOL GameManager::IsDownRightPossible() const
{
  // TODO: implement
  BOOL ret = FALSE;
  return ret;
}

void GameManager::MoveBallsUpLeft()
{
  BoardField* field1 = 0;
  BoardField* newField = 0;
  CPoint coord1;

  if (mySelectedBalls.size() == 1) {
    field1 = *mySelectedBalls.begin();
    coord1 = field1->GetFieldCoordinates();
    ++coord1.y;
    GetGameBoard()->GetBoardField(coord1)->SetBall(field1->GetBall());
    field1->SetBall(BoardField::NO_BALL);
  }
}

void GameManager::MoveBallsUpRight()
{
  // TODO: implement
}

void GameManager::MoveBallsLeft()
{
  // TODO: implement
}

void GameManager::MoveBallsRight()
{
  // TODO: implement
}

void GameManager::MoveBallsDownLeft()
{
  // TODO: implement
}

void GameManager::MoveBallsDownRight()
{
  // TODO: implement
}
