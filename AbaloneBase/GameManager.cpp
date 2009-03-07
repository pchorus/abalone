#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayerMonteCarlo.h"
#include "ComputerPlayerAlphaBeta.h"
#include "ComputerPlayerRandomMoves.h"
#include "ComputerPlayerEvaluateNextMove.h"
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
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE) {
    myPlayer1 = new ComputerPlayerEvaluateNextMove(this, namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
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
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE) {
    myPlayer2 = new ComputerPlayerEvaluateNextMove(this, namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
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

BOOL GameManager::IsPossibleDirection(Direction direction, bool& isAttacking, bool& ejectsBall, std::vector<BoardField*>* balls, std::vector<BoardField*>* opponentBalls) const
{
  BOOL ret = FALSE;

  isAttacking = false;

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
        if (opponentBalls) {
          opponentBalls->push_back(opponentField1);
        }
        ejectsBall = !opponentField2;
        isAttacking = true;
        ret = TRUE;
      }

      else if (balls->size() >= 3 && opponentField1 && opponentField2
        && opponentField1->GetBall() == opponentBall && opponentField2->GetBall() == opponentBall
        && (!opponentField3 || (opponentField3 && opponentField3->GetBall() == BoardField::NO_BALL)))
        // -> OOO|00 or -> OOO|00X
      {
        if (opponentBalls) {
          opponentBalls->push_back(opponentField1);
          opponentBalls->push_back(opponentField2);
        }

        ejectsBall = !opponentField3;
        isAttacking = true;
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

void GameManager::DoMove(BallMove* move)
{
  std::vector<BoardField*>::const_iterator i;

  for (i = move->GetBalls()->begin(); i != move->GetBalls()->end(); ++i) {
    AddSelectedBall(GetGameBoard()->GetBoardField((*i)->GetFieldCoordinates()));
  }

  MoveBallsInDirection(move->GetDirection());
}

void GameManager::UndoMove(BallMove* move)
{
  std::vector<BoardField*>* balls = move->GetBalls();
  std::vector<BoardField*>* opponentBalls = move->GetOpponentBalls();
  std::vector<BoardField*>::iterator i;
  std::vector<BoardField*>::reverse_iterator ri;
  BoardField* field = 0;
  CPoint coord;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  opponentBall = BoardField::BLACK_BALL;
  coord = GetNextFieldCoordinatesInDirection((*balls->begin())->GetFieldCoordinates(), move->GetDirection());
  if (GetGameBoard()->GetBoardField(coord)->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  SortBalls(move->GetOpponentBalls());

  switch (move->GetDirection()) {
  case UPLEFT:
  case LEFT:
  case DOWNLEFT:
    // own balls
    for (ri = balls->rbegin(); ri != balls->rend(); ++ri) {
      field = *ri;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      // we have to get the boardfield new, because it may be that the field in the collection
      // belongs to the sim game board and not to this board
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    // opponent balls
    for (ri = opponentBalls->rbegin(); ri != opponentBalls->rend(); ++ri) {
      field = *ri;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(opponentBall);

      if (GetGameBoard()->GetBoardFieldExist(coord)) {
        GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
        GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
      }
      else {
        RemoveLostBall(opponentBall);
      }
    }
    break;
  case UPRIGHT:
  case RIGHT:
  case DOWNRIGHT:
    // own balls
    for (i = balls->begin(); i != balls->end(); ++i) {
      field = *i;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    // opponent balls
    for (i = opponentBalls->begin(); i != opponentBalls->end(); ++i) {
      field = *i;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(opponentBall);

      if (GetGameBoard()->GetBoardFieldExist(coord)) {
        GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
        GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
      }
      else {
        RemoveLostBall(opponentBall);
      }
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

void GameManager::SortBalls(std::vector<BoardField*>* balls)
{
  std::sort(balls->begin(), balls->end(), BoardFieldSort());
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

CPoint GameManager::GetNextFieldCoordinatesInDirection(const CPoint& fieldCoord, Direction direction) const
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

double GameManager::CalcLostBallsRatio(const Player* player) const
{
  double ret = GetLostBallsPlayer1() - GetLostBallsPlayer2();
  if (player == GetPlayer1()) {
    ret *= -1;
  }

  return ret;
}

double GameManager::CalcAvgCenterDistance(const Player* player) const
{
  double ret = 0.;
  int centerDistance = 0;
  BoardField::Ball playersBall = player->GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (myGameBoard->GetBoardFieldExist(x, y)) {
        currentField = myGameBoard->GetBoardField(x, y);

        if (currentField->GetBall() == playersBall) {
          centerDistance += CalcCenterDistance(currentField->GetFieldCoordinates());
        }
      }
    }
  }

  // we return the average center distance of each ball on the board,
  // so we have to exclude the balls which are already lost
  if (player == GetPlayer1()) {
    ret = centerDistance / double(14 - GetLostBallsPlayer1());
  }
  else {
    ret = centerDistance / double(14 - GetLostBallsPlayer2());
  }
  return ret;
}

double GameManager::CalcAvgGrouping(const Player* player) const
{
  double ret = 0.;
  int grouping = 0;
  BoardField::Ball playersBall = player->GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (myGameBoard->GetBoardFieldExist(x, y)) {
        currentField = myGameBoard->GetBoardField(x, y);

        if (currentField->GetBall() == playersBall) {
          grouping += CalcGroupingField(player, currentField->GetFieldCoordinates());
        }
      }
    }
  }

  // we return the average center distance of each ball on the board,
  // so we have to exclude the balls which are already lost
  if (player == GetPlayer1()) {
    ret = grouping / double(14 - GetLostBallsPlayer1());
  }
  else {
    ret = grouping / double(14 - GetLostBallsPlayer2());
  }
  return ret;
}

double GameManager::CalcAttackingPowerOnOpponent(const Player* player) const
{
  // TODO: Player is not longer needed
  double ret = 0.;
  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);
  std::vector<BallMove*>::iterator i;

  BallMove* currentMove = 0;
  const ComputerPlayer* computerPlayer = dynamic_cast<const ComputerPlayer*>(player);

  AddPossibleMovesTwoBalls(computerPlayer, ballMoves);
  AddPossibleMovesThreeBalls(computerPlayer, ballMoves);

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    currentMove = *i;

    if (currentMove->IsAttacking()) {
      ++ret;
    }
  }

  ret = ret / (double)ballMoves.size();

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}

double GameManager::CalcAttackedByOpponent(const Player* player) const
{
  double ret = 0.;
  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);
  std::vector<BallMove*>::iterator i;

  BallMove* currentMove = 0;
  const ComputerPlayer* computerPlayer = 0;

  // computerPlayer should be the opponent
  if (player == myPlayer1) {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer2);
  }
  else {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer1);
  }

  AddPossibleMovesTwoBalls(computerPlayer, ballMoves);
  AddPossibleMovesThreeBalls(computerPlayer, ballMoves);

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    currentMove = *i;

    if (currentMove->IsAttacking()) {
      ++ret;
    }
  }

  // TODO: the absolute number of the opponent's attacking moves
  // is returned
//  ret = ret / (double)ballMoves.size();

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}

int GameManager::CalcCenterDistance(CPoint coord) const
{
  int ret = 0;
  int hlp = 0;
  CPoint centerCoord(4, 4);

  while ((coord.x < centerCoord.x && coord.y < centerCoord.y)
    || (coord.x > centerCoord.x && coord.y > centerCoord.y))
  {
    // extra effort
    // transformation: turn the game board counter clockwise until the
    // marble is in the down right or upper left corner of the board
    hlp = coord.y;
    coord.y = coord.x;
    coord.x += 4 - hlp;
  }
  ret = abs(coord.x - centerCoord.x) + abs(coord.y - centerCoord.y);

  return ret;
}

int GameManager::CalcGroupingField(const Player* player, CPoint coord) const
{
  int ret = 0;

  // TODO: perhaps this method can be implemented with the help of a loop over
  // the Direction enum's values

  CPoint checkCoord;
  // check the fields around the new field for fellow balls
  checkCoord = GetNextFieldCoordinatesInDirection(coord, UPLEFT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
  }
  checkCoord = GetNextFieldCoordinatesInDirection(coord, UPRIGHT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
  }
  checkCoord = GetNextFieldCoordinatesInDirection(coord, LEFT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
  }
  checkCoord = GetNextFieldCoordinatesInDirection(coord, RIGHT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
  }
  checkCoord = GetNextFieldCoordinatesInDirection(coord, DOWNLEFT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
  }
  checkCoord = GetNextFieldCoordinatesInDirection(coord, DOWNRIGHT);
  if (myGameBoard->GetBoardFieldExist(checkCoord) && myGameBoard->GetBoardField(checkCoord)->GetBall() == player->GetBall()) {
    ++ret;
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

void GameManager::RemoveLostBall(BoardField::Ball ball)
{
  if (ball == BoardField::BLACK_BALL) {
    --myLostBallsPlayer1;
  }
  else if (ball == BoardField::WHITE_BALL) {
    --myLostBallsPlayer2;
  }
}

void GameManager::TurnIsOver(LPVOID pParam)
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
    if (pParam)
      ::PostMessage((HWND)pParam, WM_COMPUTER_CALC_FINISHED, 0, 0);
  }
}

void GameManager::AddPossibleMovesOneBall(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const
{
  BoardField* field = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = GetGameBoard();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      if (gameBoard->GetBoardFieldExist(x, y)) {
        field = gameBoard->GetBoardField(x, y);
        if (field->GetBall() == player->GetBall()) {
          ballFields.push_back(field);
          CheckDirections(player, &ballFields, ballMoves);
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesTwoBalls(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = GetGameBoard();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      ballFields.clear();

      // check balls from downleft to upright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y+1)) {
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y+1);
        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y);
        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }
      // check balls from up left to downright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x, y-1)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x, y-1);
        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesThreeBalls(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  BoardField* field3 = 0;
  std::vector<BoardField*> ballFields;
  GameBoard* gameBoard = GetGameBoard();

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
        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall() && field3->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }

      // check balls from left to right
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x+1, y) && gameBoard->GetBoardFieldExist(x+2, y)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x+1, y);
        field3 = gameBoard->GetBoardField(x+2, y);

        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall() && field3->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }
      // check balls from upleft to downright
      if (gameBoard->GetBoardFieldExist(x, y) && gameBoard->GetBoardFieldExist(x, y-1) && gameBoard->GetBoardFieldExist(x, y-2)) {
        ballFields.clear();
        field1 = gameBoard->GetBoardField(x, y);
        field2 = gameBoard->GetBoardField(x, y-1);
        field3 = gameBoard->GetBoardField(x, y-2);

        if (field1->GetBall() == player->GetBall() && field2->GetBall() == player->GetBall() && field3->GetBall() == player->GetBall()) {
          ballFields.push_back(field1);
          ballFields.push_back(field2);
          ballFields.push_back(field3);

          CheckDirections(player, &ballFields, ballMoves);
        }
      }
    }
  }
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

void GameManager::CheckDirections(const ComputerPlayer* player, std::vector<BoardField*>* ballFields, std::vector<BallMove*>& ballMoves) const
{
  bool isAttacking = false;
  bool ejectsBall = false;

  std::vector<BoardField*>* opponentFields = new std::vector<BoardField*>;

  if (IsPossibleDirection(UPLEFT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(UPLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPLEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (IsPossibleDirection(UPRIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(UPRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(UPRIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (IsPossibleDirection(LEFT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(LEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(LEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (IsPossibleDirection(RIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(RIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(RIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (IsPossibleDirection(DOWNLEFT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(DOWNLEFT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNLEFT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }
  if (IsPossibleDirection(DOWNRIGHT, isAttacking, ejectsBall, ballFields, opponentFields) && player->IsMoveAllowed(DOWNRIGHT, ballFields)) {
    ballMoves.push_back(CreateBallMove(DOWNRIGHT, isAttacking, ejectsBall, ballFields, opponentFields));
    opponentFields->clear();
  }

  delete opponentFields;
}

BallMove* GameManager::CreateBallMove(Direction direction, bool isAttacking, bool ejectsBall, std::vector<BoardField*>* ballFields, std::vector<BoardField*>* opponentFields) const
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
