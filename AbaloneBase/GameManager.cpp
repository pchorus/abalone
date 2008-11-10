#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

#include <algorithm>

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
      std::vector<BoardField*>::const_iterator iter;
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
  mySelectedBalls.push_back(field);
  field->SetIsSelected(true);
  SortSelectedBalls();
}

void GameManager::RemoveSelectedBall(BoardField* field)
{
  std::vector<BoardField*>::iterator iter;
  for (iter = mySelectedBalls.begin(); iter != mySelectedBalls.end(); ++iter) {
    if (field == *iter) {
      mySelectedBalls.erase(iter);
      break;
    }
  }
  field->SetIsSelected(false);
}

void GameManager::ClearSelectedBalls()
{
  std::vector<BoardField*>::const_iterator iter;
  for (iter = mySelectedBalls.begin(); iter != mySelectedBalls.end(); ++iter) {
    (*iter)->SetIsSelected(false);
  }

  mySelectedBalls.clear();
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

BOOL GameManager::IsPossibleDirection(Direction direction) const
{
  BOOL ret = FALSE;

  BoardField* field1 = 0;
  CPoint fieldCoord1;
  BoardField* field2 = 0;
  CPoint fieldCoord2;
  BoardField* field3 = 0;
  CPoint fieldCoord3;
  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;
  BoardField::Ball opponentBall = BoardField::NO_BALL;

  BallAxis pushAxis = NO_VALID_AXIS;
  switch (direction) {
  case UPLEFT:
  case DOWNRIGHT:
    pushAxis = UPPERLEFT_TO_DOWNRIGHT;
    break;
  case UPRIGHT:
  case DOWNLEFT:
    pushAxis = DOWNLEFT_TO_UPPERRIGHT;
    break;
  case LEFT:
  case RIGHT:
    pushAxis = HORIZONTAL;
    break;
  }

  if (mySelectedBalls.size() >= 1) {

    GetSelectedAndOpponentFields(direction, field1, field2, field3, opponentField1, opponentField2, opponentField3);
    opponentBall = BoardField::BLACK_BALL;
    if (field1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }

    if (GetAxisOfSelectedBalls() == pushAxis) {
      // -> O|X or -> OO|X or -> OOO|X
      if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL) {
        ret = TRUE;
      }

      else if (mySelectedBalls.size() >= 2 && opponentField1 && opponentField1->GetBall() == opponentBall
        && (!opponentField2 || opponentField2->GetBall() == BoardField::NO_BALL))
        // -> OO|0 or -> OO|0X or -> OOO|0 or -> OOO|0X
      {
        ret = TRUE;
      }

      else if (mySelectedBalls.size() >= 3 && opponentField1 && opponentField2
        && opponentField1->GetBall() == opponentBall && opponentField2->GetBall() == opponentBall
        && (!opponentField3 || (opponentField3 && opponentField3->GetBall() == BoardField::NO_BALL)))
        // -> OOO|00 or -> OOO|00X
      {
        ret = TRUE;
      }
    }
    // sidesteps
    else {
      fieldCoord1 = field1->GetFieldCoordinates();
      if (mySelectedBalls.size() >= 2) {
        fieldCoord2 = field2->GetFieldCoordinates();
      }
      if (mySelectedBalls.size() >= 3) {
        fieldCoord3 = field3->GetFieldCoordinates();
      }

      if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))
        && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))->GetBall() == BoardField::NO_BALL)
      {
        if (mySelectedBalls.size() >= 2) {
          if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))
            && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))->GetBall() == BoardField::NO_BALL)
          {
            if (mySelectedBalls.size() >= 3) {
              if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord3, direction))
                && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord3, direction))->GetBall() == BoardField::NO_BALL)
              {
                  ret = TRUE;
              }
            }
            else {
              ret = TRUE;
            }
          }
        }
        else {
          ret = TRUE;
        }
      }
    }
  }

  return ret;
}

void GameManager::MoveBallsUpLeft()
{
  BoardField* field = 0;
  BoardField* newField = 0;
  CPoint coord;
  CPoint hlpCoord;

  // first, opponent balls are moved
  if (mySelectedBalls.size() >= 3 && GetAxisOfSelectedBalls() == UPPERLEFT_TO_DOWNRIGHT) {
    field = *mySelectedBalls.begin();
    coord = field->GetFieldCoordinates();
    hlpCoord = coord;
    hlpCoord.y += 1;
    coord.y += 2;

    if (GetGameBoard()->GetBoardField(coord)->GetBall() != BoardField::NO_BALL
      && GetGameBoard()->GetBoardField(hlpCoord)->GetBall() != BoardField::NO_BALL)
    {
      hlpCoord = coord;
      hlpCoord.y += 1;
      GetGameBoard()->GetBoardField(hlpCoord)->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
    }
  }

  if (mySelectedBalls.size() >= 2 && GetAxisOfSelectedBalls() == UPPERLEFT_TO_DOWNRIGHT) {
    field = *mySelectedBalls.begin();
    coord = field->GetFieldCoordinates();
    coord.y += 1;

    if (GetGameBoard()->GetBoardField(coord)->GetBall() != BoardField::NO_BALL) {
      hlpCoord = coord;
      hlpCoord.y += 1;
      GetGameBoard()->GetBoardField(hlpCoord)->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
    }

  }
  std::vector<BoardField*>::iterator i;
  for (i = mySelectedBalls.begin(); i != mySelectedBalls.end(); ++i) {
    field = *i;
    coord = field->GetFieldCoordinates();
    ++coord.y;
    GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
    field->SetBall(BoardField::NO_BALL);
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

BallAxis GameManager::GetAxisOfSelectedBalls() const
{
  BallAxis ret = NO_VALID_AXIS;

  BoardField* selectedField1 = 0;
  CPoint selectedFieldPoint1;
  BoardField* selectedField2 = 0;
  CPoint selectedFieldPoint2;

  // because of validation done before, it is sufficient to only test the relative position
  // between two balls, even if three balls are selected
  if (mySelectedBalls.size() > 1) {
    selectedField1 = *mySelectedBalls.begin();
    selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
    selectedField2 = *(++mySelectedBalls.begin());
    selectedFieldPoint2 = selectedField2->GetFieldCoordinates();
    if (selectedFieldPoint1.y == selectedFieldPoint2.y) {
      ret = HORIZONTAL;
    }
    else if (selectedFieldPoint1.x < selectedFieldPoint2.x
      && selectedFieldPoint1.y < selectedFieldPoint2.y)
    {
      ret = DOWNLEFT_TO_UPPERRIGHT;
    }
    else if (selectedFieldPoint1.x == selectedFieldPoint2.x
      && selectedFieldPoint1.y > selectedFieldPoint2.y)
    {
      ret = UPPERLEFT_TO_DOWNRIGHT;
    }
  }
  return ret;
}

struct BoardFieldSort
{
  bool operator()(BoardField*& left, BoardField*& right)
  {
    CPoint leftP = left->GetFieldCoordinates();
    CPoint rightP = right->GetFieldCoordinates();
    return leftP.x < rightP.x || (leftP.x == rightP.x && leftP.y > rightP.y);
  }
};

void GameManager::SortSelectedBalls()
{
  std::sort(mySelectedBalls.begin(), mySelectedBalls.end(), BoardFieldSort());
}

void GameManager::GetSelectedAndOpponentFields(Direction direction, BoardField*& selectedField1,
                                               BoardField*& selectedField2, BoardField*& selectedField3,
                                               BoardField*& opponentField1, BoardField*& opponentField2,
                                               BoardField*& opponentField3) const
{
  selectedField1 = 0;
  selectedField2 = 0;
  selectedField3 = 0;
  opponentField1 = 0;
  opponentField2 = 0;
  opponentField3 = 0;

  CPoint lastFieldCoord(0, 0);

  CPoint fieldCoord1;
  CPoint fieldCoord2;
  CPoint fieldCoord3;

  if (mySelectedBalls.size() >= 1) {

    selectedField1 = *mySelectedBalls.begin();
    fieldCoord1 = selectedField1->GetFieldCoordinates();
    lastFieldCoord = fieldCoord1;

    if (mySelectedBalls.size() >= 2) {
      selectedField2 = *(++mySelectedBalls.begin());
      fieldCoord2 = selectedField2->GetFieldCoordinates();
      lastFieldCoord = fieldCoord2;
    }
    if (mySelectedBalls.size() >= 3) {
      selectedField3 = *(++(++mySelectedBalls.begin()));
      fieldCoord3 = selectedField3->GetFieldCoordinates();
      lastFieldCoord = fieldCoord3;
    }

    // find the opponent fields:
    switch (direction) {
    case UPLEFT:
      opponentField1 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y+1)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+1) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y+2)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+2) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y+3)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+3) : 0;
      break;
    case UPRIGHT:
      opponentField1 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+1, lastFieldCoord.y+1)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+1, lastFieldCoord.y+1) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+2, lastFieldCoord.y+2)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+2, lastFieldCoord.y+2) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+3, lastFieldCoord.y+3)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+3, lastFieldCoord.y+3) : 0;
      break;
    case LEFT:
      opponentField1 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-1, fieldCoord1.y)
        ? myGameBoard->GetBoardField(fieldCoord1.x-1, fieldCoord1.y) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-2, fieldCoord1.y)
        ? myGameBoard->GetBoardField(fieldCoord1.x-2, fieldCoord1.y) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-3, fieldCoord1.y)
        ? myGameBoard->GetBoardField(fieldCoord1.x-3, fieldCoord1.y) : 0;
      break;
    case RIGHT:
      opponentField1 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+1, lastFieldCoord.y)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+1, lastFieldCoord.y) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+2, lastFieldCoord.y)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+2, lastFieldCoord.y) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x+3, lastFieldCoord.y)
        ? myGameBoard->GetBoardField(lastFieldCoord.x+3, lastFieldCoord.y) : 0;
      break;
    case DOWNLEFT:
      opponentField1 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-1, fieldCoord1.y-1)
        ? myGameBoard->GetBoardField(fieldCoord1.x-1, fieldCoord1.y-1) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-2, fieldCoord1.y-2)
        ? myGameBoard->GetBoardField(fieldCoord1.x-2, fieldCoord1.y-2) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(fieldCoord1.x-3, fieldCoord1.y-3)
        ? myGameBoard->GetBoardField(fieldCoord1.x-3, fieldCoord1.y-3) : 0;
      break;
      break;
    case DOWNRIGHT:
      opponentField1 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y-1)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y-1) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y-2)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y-2) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(fieldCoord1.x, fieldCoord1.y-3)
        ? myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y-3) : 0;
      break;
    }
  }
}

CPoint GameManager::GetNextFieldCoordinatesInDirection(CPoint& fieldCoord, Direction direction) const
{
  CPoint ret(fieldCoord);

  switch (direction) {
  case UPLEFT:
    ++ret.y;
    break;
  case UPRIGHT:
    ++ret.x;
    ++ret.y;
    break;
  case LEFT:
    --ret.x;
    break;
  case RIGHT:
    ++ret.x;
    break;
  case DOWNLEFT:
    --ret.x;
    --ret.y;
    break;
  case DOWNRIGHT:
    --ret.y;
    break;
  }
  return ret;
}
