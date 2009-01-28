#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayerMonteCarlo.h"
#include "ComputerPlayerAlphaBeta.h"
#include "ComputerPlayerRandomMoves.h"
#include "BallMove.h"

#include <algorithm>
#include <fstream>
#include <string>


GameManager::GameManager()
: myGameBoard(new GameBoard)
, myIsGameStarted(false)
, myPlayer1(0)
, myPlayer2(0)
, myNextTurn(0)
, myLostBallsPlayer1(0)
, myLostBallsPlayer2(0)
, mySelectedBalls(new std::vector<BoardField*>)
, myMaxNumberOfTurns(0)
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
  if (mySelectedBalls) {
    delete mySelectedBalls;
  }
}

void GameManager::SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2, Player::PlayerNumber startPlayer)
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }

  ASSERT(typePlayer1 != Player::PLAYER_TYPE_NONE && typePlayer2 != Player::PLAYER_TYPE_NONE);

  if (typePlayer1 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer1 = new HumanPlayer(namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
  }
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
    myPlayer1 = new ComputerPlayerMonteCarlo(this, namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
  }
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
    myPlayer1 = new ComputerPlayerAlphaBeta(this, namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
  }
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES) {
    myPlayer1 = new ComputerPlayerRandomMoves(this, namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
  }

  if (typePlayer2 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer2 = new HumanPlayer(namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
  }
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
    myPlayer2 = new ComputerPlayerMonteCarlo(this, namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
  }
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
    myPlayer2 = new ComputerPlayerAlphaBeta(this, namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
  }
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES) {
    myPlayer2 = new ComputerPlayerRandomMoves(this, namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
  }

  myLostBallsPlayer1 = 0;
  myLostBallsPlayer2 = 0;
  // at the beginning of the game, the method turnisover must be called, because if player1
  // is a computer player it should take his next move automatically
  // so here myNextTurn must be Player2 if Player1 starts and vice versa
  if (startPlayer == Player::PLAYER_ONE) {
    myNextTurn = myPlayer2;
  }
  else {
    myNextTurn = myPlayer1;
  }
}

bool GameManager::CanSelectBall(BoardField* field) const
{
  bool ret = false;
  BoardField* selectedField1 = 0;
  CPoint selectedFieldPoint1;
  BoardField* selectedField2 = 0;
  CPoint selectedFieldPoint2;

  CPoint fieldPoint = field->GetFieldCoordinates();

  if (mySelectedBalls->size() < 3) {
    if (mySelectedBalls->empty()) {
      ret = true;
    }
    else if (mySelectedBalls->size() == 1) {
      std::vector<BoardField*>::const_iterator iter;
      for (iter = mySelectedBalls->begin(); iter != mySelectedBalls->end(); ++iter) {
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
    else if (mySelectedBalls->size() == 2) {
      selectedField1 = *mySelectedBalls->begin();
      selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
      selectedField2 = *(++mySelectedBalls->begin());
      selectedFieldPoint2 = selectedField2->GetFieldCoordinates();

      if (GetAxisOfBalls(mySelectedBalls) == HORIZONTAL
        && fieldPoint.y == selectedFieldPoint1.y
        && fieldPoint.y == selectedFieldPoint2.y
        && (fieldPoint.x == selectedFieldPoint1.x-1 || fieldPoint.x == selectedFieldPoint2.x+1))
      {
        ret = true;
      }
      else if (GetAxisOfBalls(mySelectedBalls) == DOWNLEFT_TO_UPPERRIGHT
        && ((fieldPoint.x == selectedFieldPoint1.x-1 && fieldPoint.x == selectedFieldPoint2.x-2
             && fieldPoint.y == selectedFieldPoint1.y-1 && fieldPoint.y == selectedFieldPoint2.y-2)
        || fieldPoint.x == selectedFieldPoint1.x+2 && fieldPoint.x == selectedFieldPoint2.x+1
             && fieldPoint.y == selectedFieldPoint1.y+2 && fieldPoint.y == selectedFieldPoint2.y+1))
      {
        ret = true;
      }
      else if (GetAxisOfBalls(mySelectedBalls) == UPPERLEFT_TO_DOWNRIGHT
        && fieldPoint.x == selectedFieldPoint1.x
        && fieldPoint.x == selectedFieldPoint2.x
        && (fieldPoint.y == selectedFieldPoint1.y+1 || fieldPoint.y == selectedFieldPoint2.y-1))
      {
        ret = true;
      }
    }
  }

  return ret;
}

bool GameManager::CanDeselectBall(BoardField* field) const
{
  bool ret = true;

  if (mySelectedBalls->size() == 3 && *(++mySelectedBalls->begin()) == field) {
    // the middle ball can't be deselect
    ret = false;
  }
  return ret;
}

void GameManager::AddSelectedBall(BoardField* field)
{
  mySelectedBalls->push_back(field);
  field->SetIsSelected(true);
  SortSelectedBalls();
}

void GameManager::RemoveSelectedBall(BoardField* field)
{
  std::vector<BoardField*>::iterator iter;
  for (iter = mySelectedBalls->begin(); iter != mySelectedBalls->end(); ++iter) {
    if (field == *iter) {
      mySelectedBalls->erase(iter);
      break;
    }
  }
  field->SetIsSelected(false);
}

void GameManager::ClearSelectedBalls()
{
  std::vector<BoardField*>::const_iterator iter;
  for (iter = mySelectedBalls->begin(); iter != mySelectedBalls->end(); ++iter) {
    (*iter)->SetIsSelected(false);
  }

  mySelectedBalls->clear();
}

BOOL GameManager::IsPossibleDirection(Direction direction, std::vector<BoardField*>* balls) const
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

  if (!balls) {
    balls = mySelectedBalls;
  }

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

  if (balls->size() >= 1) {

    GetSelectedAndOpponentFields(direction, balls, field1, field2, field3, opponentField1, opponentField2, opponentField3);
    opponentBall = BoardField::BLACK_BALL;
    if (field1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }

    if (GetAxisOfBalls(balls) == pushAxis) {
      // -> O|X or -> OO|X or -> OOO|X
      if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL) {
        ret = TRUE;
      }

      else if (balls->size() >= 2 && opponentField1 && opponentField1->GetBall() == opponentBall
        && (!opponentField2 || opponentField2->GetBall() == BoardField::NO_BALL))
        // -> OO|0 or -> OO|0X or -> OOO|0 or -> OOO|0X
      {
        ret = TRUE;
      }

      else if (balls->size() >= 3 && opponentField1 && opponentField2
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
      if (balls->size() >= 2) {
        fieldCoord2 = field2->GetFieldCoordinates();
      }
      if (balls->size() >= 3) {
        fieldCoord3 = field3->GetFieldCoordinates();
      }

      if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))
        && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))->GetBall() == BoardField::NO_BALL)
      {
        if (balls->size() >= 2) {
          if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))
            && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))->GetBall() == BoardField::NO_BALL)
          {
            if (balls->size() >= 3) {
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

void GameManager::MoveBallsInDirection(Direction direction)
{
  BoardField* field = 0;
  BoardField* newField = 0;
  CPoint coord;
  CPoint hlpCoord;

  BoardField* selectedField1 = 0;
  BoardField* selectedField2 = 0;
  BoardField* selectedField3 = 0;
  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  GetSelectedAndOpponentFields(direction, mySelectedBalls, selectedField1, selectedField2, selectedField3,
                               opponentField1, opponentField2, opponentField3);
  opponentBall = BoardField::BLACK_BALL;
  if (selectedField1->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  // first, opponent balls are moved
  if (opponentField1 && opponentField1->GetBall() == opponentBall) {
    if (opponentField2 && opponentField2->GetBall() == opponentBall) {
      if (opponentField3 && opponentField3->GetBall() == opponentBall) {
        if (GetGameBoard()->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(opponentField3->GetFieldCoordinates(), direction))) {
          GetGameBoard()->GetBoardField(GetNextFieldCoordinatesInDirection(opponentField3->GetFieldCoordinates(), direction))
            ->SetBall(opponentField3->GetBall());
          opponentField3->SetBall(BoardField::NO_BALL);
          opponentField3->SetIsSelected(false);
        }
        else {
          AddLostBall(opponentBall);
        }
      }
      if (GetGameBoard()->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(opponentField2->GetFieldCoordinates(), direction))) {
        GetGameBoard()->GetBoardField(GetNextFieldCoordinatesInDirection(opponentField2->GetFieldCoordinates(), direction))
          ->SetBall(opponentField2->GetBall());
        opponentField2->SetBall(BoardField::NO_BALL);
        opponentField2->SetIsSelected(false);
      }
      else {
        AddLostBall(opponentBall);
      }
    }
    if (GetGameBoard()->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(opponentField1->GetFieldCoordinates(), direction))) {
      GetGameBoard()->GetBoardField(GetNextFieldCoordinatesInDirection(opponentField1->GetFieldCoordinates(), direction))
        ->SetBall(opponentField1->GetBall());
      opponentField1->SetBall(BoardField::NO_BALL);
      opponentField1->SetIsSelected(false);
    }
    else {
      AddLostBall(opponentBall);
    }
  }

  std::vector<BoardField*>::iterator i;
  std::vector<BoardField*>::reverse_iterator ri;

  switch (direction) {
  case UPLEFT:
  case LEFT:
  case DOWNLEFT:
    for (i = mySelectedBalls->begin(); i != mySelectedBalls->end(); ++i) {
      field = *i;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    break;
  case UPRIGHT:
  case RIGHT:
  case DOWNRIGHT:
    for (ri = mySelectedBalls->rbegin(); ri != mySelectedBalls->rend(); ++ri) {
      field = *ri;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    break;
  }
  mySelectedBalls->clear();
}

BallAxis GameManager::GetAxisOfBalls(const std::vector<BoardField*>* const ballFields) const
{
  BallAxis ret = NO_VALID_AXIS;

  BoardField* selectedField1 = 0;
  CPoint selectedFieldPoint1;
  BoardField* selectedField2 = 0;
  CPoint selectedFieldPoint2;

  // because of validation done before, it is sufficient to only test the relative position
  // between two balls, even if three balls are selected
  if (ballFields->size() > 1) {
    selectedField1 = *ballFields->begin();
    selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
    selectedField2 = *(++ballFields->begin());
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
  std::sort(mySelectedBalls->begin(), mySelectedBalls->end(), BoardFieldSort());
}

void GameManager::GetSelectedAndOpponentFields(Direction direction, std::vector<BoardField*>* balls, BoardField*& selectedField1,
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

  if (balls->size() >= 1) {

    selectedField1 = *balls->begin();
    fieldCoord1 = selectedField1->GetFieldCoordinates();
    lastFieldCoord = fieldCoord1;

    if (balls->size() >= 2) {
      selectedField2 = *(++balls->begin());
      fieldCoord2 = selectedField2->GetFieldCoordinates();
      lastFieldCoord = fieldCoord2;
    }
    if (balls->size() >= 3) {
      selectedField3 = *(++(++balls->begin()));
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
      opponentField1 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x, lastFieldCoord.y-1)
        ? myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-1) : 0;
      opponentField2 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x, lastFieldCoord.y-2)
        ? myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-2) : 0;
      opponentField3 = myGameBoard->GetBoardFieldExist(lastFieldCoord.x, lastFieldCoord.y-3)
        ? myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-3) : 0;
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

void GameManager::AddLostBall(BoardField::Ball ball)
{
  if (ball == BoardField::BLACK_BALL) {
    ++myLostBallsPlayer1;
  }
  else if (ball == BoardField::WHITE_BALL) {
    ++myLostBallsPlayer2;
  }
}

void GameManager::AddPossibleMovesOneBall(std::vector<BallMove*>& ballMoves)
{
  BoardField* field = 0;
  std::vector<BoardField*> ballFields;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      if (myGameBoard->GetBoardFieldExist(x, y)) {
        field = myGameBoard->GetBoardField(x, y);
        if (field->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field);
          CheckDirections(ballFields, ballMoves);
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesTwoBalls(std::vector<BallMove*>& ballMoves)
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  std::vector<BoardField*> ballFields;

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x+1, y+1)) {
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x+1, y+1);
        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x+1, y)) {
        ballFields.clear();
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x+1, y);
        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(ballFields, ballMoves);
        }
      }
      // check balls from upleft to downright
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x, y-1)) {
        ballFields.clear();
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x, y-1);
        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(ballFields, ballMoves);
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesThreeBalls(std::vector<BallMove*>& ballMoves)
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  BoardField* field3 = 0;
  std::vector<BoardField*> ballFields;

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x+1, y+1) && myGameBoard->GetBoardFieldExist(x+2, y+2)) {
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x+1, y+1);
        field3 = myGameBoard->GetBoardField(x+2, y+2);
        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall() && field3->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x+1, y) && myGameBoard->GetBoardFieldExist(x+2, y)) {
        ballFields.clear();
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x+1, y);
        field3 = myGameBoard->GetBoardField(x+2, y);

        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall() && field3->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(ballFields, ballMoves);
        }
      }
      // check balls from upleft to downright
      if (myGameBoard->GetBoardFieldExist(x, y) && myGameBoard->GetBoardFieldExist(x, y-1) && myGameBoard->GetBoardFieldExist(x, y-2)) {
        ballFields.clear();
        field1 = myGameBoard->GetBoardField(x, y);
        field2 = myGameBoard->GetBoardField(x, y-1);
        field3 = myGameBoard->GetBoardField(x, y-2);

        if (field1->GetBall() == myNextTurn->GetBall() && field2->GetBall() == myNextTurn->GetBall() && field3->GetBall() == myNextTurn->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(ballFields, ballMoves);
        }
      }
    }
  }
}

void GameManager::CheckDirections(std::vector<BoardField*> ballFields, std::vector<BallMove*>& ballMoves)
{
  if (IsPossibleDirection(UPLEFT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(UPLEFT, ballFields));
  }
  if (IsPossibleDirection(UPRIGHT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(UPRIGHT, ballFields));
  }
  if (IsPossibleDirection(LEFT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(LEFT, ballFields));
  }
  if (IsPossibleDirection(RIGHT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(RIGHT, ballFields));
  }
  if (IsPossibleDirection(DOWNLEFT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNLEFT, ballFields));
  }
  if (IsPossibleDirection(DOWNRIGHT, &ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNRIGHT, ballFields));
  }
}

void GameManager::TurnIsOver()
{
  ASSERT(myNextTurn);
  ComputerPlayer* computerPlayer = 0;
  myNextTurn = (myNextTurn == myPlayer1) ? myPlayer2 : myPlayer1;
  mySelectedBalls->clear();
  int turnCount = 0;

  while (myNextTurn->GetType() != Player::PLAYER_TYPE_HUMAN && (myMaxNumberOfTurns == 0 || turnCount < myMaxNumberOfTurns)
    && myLostBallsPlayer1 < 6 && myLostBallsPlayer2 < 6)
  {
    computerPlayer = static_cast<ComputerPlayer*>(myNextTurn);

    if (computerPlayer) {
      computerPlayer->TakeNextTurn();
      myNextTurn = (myNextTurn == myPlayer1) ? myPlayer2 : myPlayer1;
      mySelectedBalls->clear();
    }
    else {
      // this should never happen, because if the PlayerType is 
      ASSERT(false);
    }
    ++turnCount;
  }
}

BallMove* GameManager::CreateBallMove(Direction direction, std::vector<BoardField*> ballFields)
{
  BallMove* ballMove = new BallMove;

  std::vector<BoardField*>::iterator i;
  i = ballFields.begin();

  if (ballFields.size() >= 1) {
    ballMove->AddBall(*i);
  }
  if (ballFields.size() >= 2) {
    ++i;
    ballMove->AddBall(*i);
  }
  if (ballFields.size() >= 3) {
    ++i;
    ballMove->AddBall(*i);
  }
  ballMove->SetDirection(direction);

  return ballMove;
}

void GameManager::SetBallFormation(const CString& formation)
{
  if (formation == START_FORMATION_STR_STANDARD) {
    SetBallsStandardFormation();
  }
  else if (formation == START_FORMATION_STR_BELGIAN_DAISY) {
    SetBallsBelgianDaisyFormation();
  }
  else {
    SetBallsCustomFormation(formation);
  }
}

void GameManager::SetBallsStandardFormation()
{
  // set balls in the standard formation
  for (int x = 0; x < 6; ++x) {
    for (int y = 0; y < 2; ++y) {
      if (myGameBoard->GetBoardFieldExist(x, y)) {
        myGameBoard->GetBoardField(x, y)->SetBall(BoardField::BLACK_BALL);
      }
    }
  }
  myGameBoard->GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(3, 2)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(4, 2)->SetBall(BoardField::BLACK_BALL);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = BOARD_FIELDS_ROW-1; y > BOARD_FIELDS_ROW-3; --y) {
      if (myGameBoard->GetBoardFieldExist(x, y)) {
        myGameBoard->GetBoardField(x, y)->SetBall(BoardField::WHITE_BALL);
      }
    }
  }
  myGameBoard->GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(5, 6)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(6, 6)->SetBall(BoardField::WHITE_BALL);
}

void GameManager::SetBallsBelgianDaisyFormation()
{
  myGameBoard->GetBoardField(0, 0)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(0, 1)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(1, 0)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(1, 1)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(1, 2)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(2, 1)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);

  myGameBoard->GetBoardField(3, 0)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(3, 1)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 0)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 1)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 2)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(5, 1)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(5, 2)->SetBall(BoardField::WHITE_BALL);

  myGameBoard->GetBoardField(3, 6)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(3, 7)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 7)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(4, 8)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(5, 7)->SetBall(BoardField::WHITE_BALL);
  myGameBoard->GetBoardField(5, 8)->SetBall(BoardField::WHITE_BALL);

  myGameBoard->GetBoardField(6, 6)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(6, 7)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(7, 6)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(7, 7)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(7, 8)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(8, 7)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(8, 8)->SetBall(BoardField::BLACK_BALL);
}

void GameManager::SetBallsCustomFormation(const CString& formation)
{
  if (!formation.IsEmpty()) {
    int curPos = 0;
    int curLine = 9;
    int curColumn = 0;
    int boardX = 0;
    int countWhiteBalls = 0;
    int countBlackBalls = 0;
    CString file = "formations\\" + formation += ".txt";
    CString cStr;
    CString curStr;
    std::ifstream input;
    input.open(file, std::ios_base::in);

    std::string str;

    while (!input.eof()) {
      std::getline(input, str);
      --curLine;
      curColumn = 0;
      curPos = 0;

      if (!str.empty()) {
        cStr = str.data();
        cStr.Trim();
        curStr = cStr.Tokenize(_T(" "), curPos);

        while (!curStr.IsEmpty()) {
          if (curLine > 4) {
            boardX = curLine - 4 + curColumn;
          }
          else {
            boardX = curColumn;
          }

          switch (curStr.GetAt(0))
          {
          case 'w':
          case 'W':
            myGameBoard->GetBoardField(boardX, curLine)->SetBall(BoardField::WHITE_BALL);
            ++countWhiteBalls;
            break;
          case 'b':
          case 'B':
            myGameBoard->GetBoardField(boardX, curLine)->SetBall(BoardField::BLACK_BALL);
            ++countBlackBalls;
            break;
          }

          curStr = cStr.Tokenize(_T(" "), curPos);
          ++curColumn;
        }
      }
    }

    myLostBallsPlayer1 = 14 - countBlackBalls;
    myLostBallsPlayer2 = 14 - countWhiteBalls;
    if (input.is_open()) {
      input.close();
    }
  }
}
