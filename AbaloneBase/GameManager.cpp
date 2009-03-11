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
, mySelectedBall1(0)
, mySelectedBall2(0)
, mySelectedBall3(0)
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

  if (!mySelectedBall3) {
    if (!mySelectedBall1) {
      ret = true;
    }
    // only one selected ball
    else if (!mySelectedBall2) {
      selectedField1 = mySelectedBall1;
      selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
      if ((selectedFieldPoint1.x - fieldPoint.x == 0 && abs(selectedFieldPoint1.y - fieldPoint.y) == 1)
        || (selectedFieldPoint1.y - fieldPoint.y == 0 && abs(selectedFieldPoint1.x - fieldPoint.x) == 1)
        || (selectedFieldPoint1.x - fieldPoint.x == 1 && selectedFieldPoint1.y - fieldPoint.y == 1)
        || (fieldPoint.x - selectedFieldPoint1.x == 1 && fieldPoint.y - selectedFieldPoint1.y == 1))
      {
        ret = true;
      }
    }
    // two selected balls
    else {
      selectedField1 = mySelectedBall1;
      selectedFieldPoint1 = selectedField1->GetFieldCoordinates();
      selectedField2 = mySelectedBall2;
      selectedFieldPoint2 = selectedField2->GetFieldCoordinates();

      if (GetAxisOfBalls(selectedField1, selectedField2) == HORIZONTAL
        && fieldPoint.y == selectedFieldPoint1.y
        && fieldPoint.y == selectedFieldPoint2.y
        && (fieldPoint.x == selectedFieldPoint1.x-1 || fieldPoint.x == selectedFieldPoint2.x+1))
      {
        ret = true;
      }
      else if (GetAxisOfBalls(selectedField1, selectedField2) == DOWNLEFT_TO_UPPERRIGHT
        && ((fieldPoint.x == selectedFieldPoint1.x-1 && fieldPoint.x == selectedFieldPoint2.x-2
             && fieldPoint.y == selectedFieldPoint1.y-1 && fieldPoint.y == selectedFieldPoint2.y-2)
        || fieldPoint.x == selectedFieldPoint1.x+2 && fieldPoint.x == selectedFieldPoint2.x+1
             && fieldPoint.y == selectedFieldPoint1.y+2 && fieldPoint.y == selectedFieldPoint2.y+1))
      {
        ret = true;
      }
      else if (GetAxisOfBalls(selectedField1, selectedField2) == UPPERLEFT_TO_DOWNRIGHT
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

  // three selected balls
  if (mySelectedBall3 && mySelectedBall2 == field) {
    // the middle ball can't be deselect
    ret = false;
  }
  return ret;
}

void GameManager::AddSelectedBall(BoardField* field)
{
  if (!mySelectedBall1) {
    mySelectedBall1 = field;
  }
  else if (!mySelectedBall2) {
    mySelectedBall2 = field;
  }
  else if (!mySelectedBall3) {
    mySelectedBall3 = field;
  }
  field->SetIsSelected(true);
  SortSelectedBalls();
}

void GameManager::RemoveSelectedBall(BoardField* field)
{
  if (mySelectedBall1 == field) {
    mySelectedBall1 = mySelectedBall2;
    mySelectedBall2 = mySelectedBall3;
    mySelectedBall3 = 0;
  }
  else if (mySelectedBall2 == field) {
    mySelectedBall2 = mySelectedBall3;
    mySelectedBall3 = 0;
  }
  else if (mySelectedBall3 == field) {
    mySelectedBall3 = 0;
  }
  field->SetIsSelected(false);
}

void GameManager::ClearSelectedBalls()
{
  if (mySelectedBall1) {
    mySelectedBall1->SetIsSelected(false);
    mySelectedBall1 = 0;
  }
  if (mySelectedBall2) {
    mySelectedBall2->SetIsSelected(false);
    mySelectedBall2 = 0;
  }
  if (mySelectedBall3) {
    mySelectedBall3->SetIsSelected(false);
    mySelectedBall3 = 0;
  }
}

void GameManager::IsPossibleDirection(Direction direction, BoardField* ball1, BoardField* ball2, BoardField* ball3, BallMove** ballMoves, int& ballMovesSize) const
{
  CPoint fieldCoord1;
  CPoint fieldCoord2;
  CPoint fieldCoord3;
  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;

  BoardField* opponentBall1 = 0;
  BoardField* opponentBall2 = 0;
  BoardField* opponentBall3 = 0;

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

  if (ball1) {
    GetOpponentFields(direction, ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);
    opponentBall = BoardField::BLACK_BALL;
    if (ball1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }

    if (GetAxisOfBalls(ball1, ball2) == pushAxis) {
      // -> O|X or -> OO|X or -> OOO|X
      if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL) {
        ballMoves[ballMovesSize] = new BallMove(direction, false, false, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
        ++ballMovesSize;
      }

      else if (ball2 && opponentField1 && opponentField1->GetBall() == opponentBall
        && (!opponentField2 || opponentField2->GetBall() == BoardField::NO_BALL))
        // -> OO|0 or -> OO|0X or -> OOO|0 or -> OOO|0X
      {
        opponentBall1 = opponentField1;
        ballMoves[ballMovesSize] = new BallMove(direction, true, !opponentField2, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
        ++ballMovesSize;
      }

      else if (ball3 && opponentField1 && opponentField2
        && opponentField1->GetBall() == opponentBall && opponentField2->GetBall() == opponentBall
        && (!opponentField3 || (opponentField3 && opponentField3->GetBall() == BoardField::NO_BALL)))
        // -> OOO|00 or -> OOO|00X
      {
        opponentBall1 = opponentField1;
        opponentBall2 = opponentField2;
        ballMoves[ballMovesSize] = new BallMove(direction, true, !opponentField3, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
        ++ballMovesSize;
      }
    }
    // sidesteps
    else {
      fieldCoord1 = ball1->GetFieldCoordinates();
      if (ball2) {
        fieldCoord2 = ball2->GetFieldCoordinates();
      }
      if (ball3) {
        fieldCoord3 = ball3->GetFieldCoordinates();
      }

      if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))
        && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))->GetBall() == BoardField::NO_BALL)
      {
        if (ball2) {
          if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))
            && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))->GetBall() == BoardField::NO_BALL)
          {
            if (ball3) {
              if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord3, direction))
                && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord3, direction))->GetBall() == BoardField::NO_BALL)
              {
                ballMoves[ballMovesSize] = new BallMove(direction, false, false, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
                ++ballMovesSize;
              }
            }
            else {
              ballMoves[ballMovesSize] = new BallMove(direction, false, false, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
              ++ballMovesSize;
            }
          }
        }
        else {
          ballMoves[ballMovesSize] = new BallMove(direction, false, false, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
          ++ballMovesSize;
        }
      }
    }
  }
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

  if (!selectedField1 && !selectedField2 && !selectedField3) {
    if (mySelectedBall1) {
      selectedField1 = mySelectedBall1;
      if (mySelectedBall2) {
        selectedField2 = mySelectedBall2;
      }
      if (mySelectedBall3) {
        selectedField3 = mySelectedBall3;
      }
    }
  }

  GetOpponentFields(direction, selectedField1, selectedField2, selectedField3,
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

  switch (direction) {
  case UPLEFT:
  case LEFT:
  case DOWNLEFT:
    field = mySelectedBall1;
    coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
    GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
    field->SetBall(BoardField::NO_BALL);
    field->SetIsSelected(false);

    if (mySelectedBall2) {
      field = mySelectedBall2;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    if (mySelectedBall3) {
      field = mySelectedBall3;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    break;
  case UPRIGHT:
  case RIGHT:
  case DOWNRIGHT:
    if (mySelectedBall3) {
      field = mySelectedBall3;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    if (mySelectedBall2) {
      field = mySelectedBall2;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
      GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    field = mySelectedBall1;
    coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), direction);
    GetGameBoard()->GetBoardField(coord)->SetBall(field->GetBall());
    field->SetBall(BoardField::NO_BALL);
    field->SetIsSelected(false);

    break;
  }

  mySelectedBall1 = 0;
  mySelectedBall2 = 0;
  mySelectedBall3 = 0;
}

void GameManager::DoMove(BallMove* move)
{
  BoardField* ball1 = 0;
  BoardField* ball2 = 0;
  BoardField* ball3 = 0;
  move->GetBalls(ball1, ball2, ball3);

  mySelectedBall1 = GetGameBoard()->GetBoardField(ball1->GetFieldCoordinates());
  if (ball2)
    mySelectedBall2 = GetGameBoard()->GetBoardField(ball2->GetFieldCoordinates());

  if (ball3)
    mySelectedBall3 = GetGameBoard()->GetBoardField(ball3->GetFieldCoordinates());

  MoveBallsInDirection(move->GetDirection());
}

void GameManager::UndoMove(BallMove* move)
{
  BoardField* field(0);
  BoardField* ball1(0);
  BoardField* ball2(0);
  BoardField* ball3(0);
  BoardField* opponentBall1(0);
  BoardField* opponentBall2(0);
  BoardField* opponentBall3(0);

  move->GetBalls(ball1, ball2, ball3);
  move->GetOpponentBalls(opponentBall1, opponentBall2, opponentBall3);
  CPoint coord;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  opponentBall = BoardField::BLACK_BALL;
  coord = GetNextFieldCoordinatesInDirection(ball1->GetFieldCoordinates(), move->GetDirection());
  if (GetGameBoard()->GetBoardField(coord)->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  SortBalls(ball1, ball2, ball3);

  switch (move->GetDirection()) {
  case UPLEFT:
  case LEFT:
  case DOWNLEFT:
    // own balls
    if (ball3) {
      field = ball3;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      // we have to get the boardfield new, because it may be that the field in the collection
      // belongs to the sim game board and not to this board
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    if (ball2) {
      field = ball2;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      // we have to get the boardfield new, because it may be that the field in the collection
      // belongs to the sim game board and not to this board
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    field = ball1;
    coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
    // we have to get the boardfield new, because it may be that the field in the collection
    // belongs to the sim game board and not to this board
    GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
    GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
    GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);

    // opponent balls
    if (opponentBall3) {
      field = opponentBall3;
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

    if (opponentBall2) {
      field = opponentBall2;
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

    if (opponentBall1) {
      field = opponentBall1;
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
    
    field = ball1;
    coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
    GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
    GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
    GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);

    if (ball2) {
      field = ball2;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    if (ball3) {
      field = ball3;
      coord = GetNextFieldCoordinatesInDirection(field->GetFieldCoordinates(), move->GetDirection());
      GetGameBoard()->GetBoardField(field->GetFieldCoordinates())->SetBall(GetGameBoard()->GetBoardField(coord)->GetBall());
      GetGameBoard()->GetBoardField(coord)->SetBall(BoardField::NO_BALL);
      GetGameBoard()->GetBoardField(coord)->SetIsSelected(false);
    }

    // opponent balls
    if (opponentBall1) {
      field = opponentBall1;
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

    if (opponentBall2) {
      field = opponentBall2;
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

    if (opponentBall3) {
      field = opponentBall3;
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

  mySelectedBall1 = 0;
  mySelectedBall2 = 0;
  mySelectedBall3 = 0;
}

BallAxis GameManager::GetAxisOfBalls(const BoardField* ball1, const BoardField* ball2) const
{
  BallAxis ret = NO_VALID_AXIS;

  CPoint selectedFieldPoint1;
  CPoint selectedFieldPoint2;

  // because of validation done before, it is sufficient to only test the relative position
  // between two balls, even if three balls are selected
  if (ball2) {
    selectedFieldPoint1 = ball1->GetFieldCoordinates();
    selectedFieldPoint2 = ball2->GetFieldCoordinates();
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

void GameManager::SortSelectedBalls()
{
  SortBalls(mySelectedBall1, mySelectedBall2, mySelectedBall3);
}

void GameManager::SortBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3)
{
  BoardField* hlp(0);
  CPoint coord1;
  CPoint coord2;
  CPoint coord3;
  if (ball2) {
    coord1 = ball1->GetFieldCoordinates();
    coord2 = ball2->GetFieldCoordinates();

    if (!(coord1.x < coord2.x || (coord1.x == coord2.x && coord1.y > coord2.y))) {
      hlp = ball1;
      ball1 = ball2;
      ball2 = hlp;
    }
  }

  if (ball3) {
    coord3 = ball3->GetFieldCoordinates();

    // ball3 is the most left one
    if (!(coord1.x < coord3.x || (coord1.x == coord3.x && coord1.y > coord3.y))) {
      hlp = ball3;
      ball3 = ball2;
      ball2 = ball1;
      ball1 = hlp;
    }
    // ball3 is in the middle
    else if (!(coord2.x < coord3.x || (coord2.x == coord3.x && coord2.y > coord3.y))) {
      hlp = ball3;
      ball3 = ball2;
      ball2 = hlp;
    }

  }
}

void GameManager::GetOpponentFields(Direction direction, const BoardField* selectedField1,
                                               const BoardField* selectedField2, const BoardField* selectedField3,
                                               BoardField*& opponentField1, BoardField*& opponentField2,
                                               BoardField*& opponentField3) const
{
  opponentField1 = 0;
  opponentField2 = 0;
  opponentField3 = 0;

  CPoint lastFieldCoord(0, 0);

  CPoint fieldCoord1 = selectedField1->GetFieldCoordinates();
  if (selectedField3) {
    lastFieldCoord = selectedField3->GetFieldCoordinates();
  }
  else if (selectedField2) {
    lastFieldCoord = selectedField2->GetFieldCoordinates();
  }
  else if (selectedField1) {
    lastFieldCoord = fieldCoord1;
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
  double ret = 0.;
  BallMove* ballMoves[BALL_MOVES_ARRAY_SIZE];
  int ballMovesSize = 0;

  BallMove* currentMove = 0;
  const ComputerPlayer* computerPlayer = dynamic_cast<const ComputerPlayer*>(player);

  AddPossibleMovesTwoBalls(computerPlayer, ballMoves, ballMovesSize);
  AddPossibleMovesThreeBalls(computerPlayer, ballMoves, ballMovesSize);

  for (int i = 0; i < ballMovesSize; ++i) {
    currentMove = ballMoves[i];

    if (currentMove->IsAttacking()) {
      ++ret;
    }
  }

  ret = ret / (double)ballMovesSize;

  for (int i = 0; i < ballMovesSize; ++i) {
    delete ballMoves[i];
  }

  return ret;
}

double GameManager::CalcAttackedByOpponent(const Player* player) const
{
  double ret = 0.;
  BallMove* ballMoves[BALL_MOVES_ARRAY_SIZE];
  int ballMovesSize = 0;

  BallMove* currentMove = 0;
  const ComputerPlayer* computerPlayer = 0;

  // computerPlayer should be the opponent
  if (player == myPlayer1) {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer2);
  }
  else {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer1);
  }

  AddPossibleMovesTwoBalls(computerPlayer, ballMoves, ballMovesSize);
  AddPossibleMovesThreeBalls(computerPlayer, ballMoves, ballMovesSize);

  for (int i = 0; i < ballMovesSize; ++i) {
    currentMove = ballMoves[i];

    if (currentMove->IsAttacking()) {
      ++ret;
    }
  }

  // TODO: the absolute number of the opponent's attacking moves
  // is returned
//  ret = ret / (double)ballMoves.size();

  for (int i = 0; i < ballMovesSize; ++i) {
    delete ballMoves[i];
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

  mySelectedBall1 = 0;
  mySelectedBall2 = 0;
  mySelectedBall3 = 0;
  int turnCount = 0;

  while (myNextTurn->GetType() != Player::PLAYER_TYPE_HUMAN && (myMaxNumberOfTurns == 0 || turnCount < myMaxNumberOfTurns)
    && myLostBallsPlayer1 < 6 && myLostBallsPlayer2 < 6)
  {
    computerPlayer = static_cast<ComputerPlayer*>(myNextTurn);

    computerPlayer->TakeNextTurn();
    myNextTurn = (myNextTurn == myPlayer1) ? myPlayer2 : myPlayer1;
    mySelectedBall1 = 0;
    mySelectedBall2 = 0;
    mySelectedBall3 = 0;

    ++turnCount;
    if (pParam)
      ::PostMessage((HWND)pParam, WM_COMPUTER_CALC_FINISHED, 0, 0);
  }
}

void GameManager::AddPossibleMovesOneBall(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const
{
  BoardField* field = 0;
  GameBoard* gameBoard = GetGameBoard();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {

      if (gameBoard->GetBoardFieldExist(x, y)) {
        field = gameBoard->GetBoardField(x, y);
        if (field->GetBall() == player->GetBall()) {
          CheckDirections(field, 0, 0, ballMoves, ballMovesSize);
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesTwoBalls(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  GameBoard* gameBoard = GetGameBoard();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        field1 = gameBoard->GetBoardField(x, y);
        if (field1->GetBall() == player->GetBall()) {
          // check balls from downleft to upright
          if (gameBoard->GetBoardFieldExist(x+1, y+1)) {
            field2 = gameBoard->GetBoardField(x+1, y+1);
            if (field2->GetBall() == player->GetBall()) {
              CheckDirections(field1, field2, 0, ballMoves, ballMovesSize);
            }
          }

          // check balls from left to right
          if (gameBoard->GetBoardFieldExist(x+1, y)) {
            field2 = gameBoard->GetBoardField(x+1, y);
            if (field2->GetBall() == player->GetBall()) {
              CheckDirections(field1, field2, 0, ballMoves, ballMovesSize);
            }
          }
          // check balls from up left to downright
          if (gameBoard->GetBoardFieldExist(x, y-1)) {
            field2 = gameBoard->GetBoardField(x, y-1);
            if (field2->GetBall() == player->GetBall()) {
              CheckDirections(field1, field2, 0, ballMoves, ballMovesSize);
            }
          }
        }
      }
    }
  }
}

void GameManager::AddPossibleMovesThreeBalls(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const
{
  BoardField* field1 = 0;
  BoardField* field2 = 0;
  BoardField* field3 = 0;
  GameBoard* gameBoard = GetGameBoard();

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        field1 = gameBoard->GetBoardField(x, y);
        if (field1->GetBall() == player->GetBall()) {
          // check balls from downleft to upright
          if (gameBoard->GetBoardFieldExist(x+1, y+1)) {
            field2 = gameBoard->GetBoardField(x+1, y+1);
            if (field2->GetBall() == player->GetBall()) {
              if (gameBoard->GetBoardFieldExist(x+2, y+2)) {
                field3 = gameBoard->GetBoardField(x+2, y+2);
                if (field3->GetBall() == player->GetBall()) {
                  CheckDirections(field1, field2, field3, ballMoves, ballMovesSize);
                }
              }
            }
          }

          // check balls from left to right
          if (gameBoard->GetBoardFieldExist(x+1, y)) {
            field2 = gameBoard->GetBoardField(x+1, y);
            if (field2->GetBall() == player->GetBall()) {
              if (gameBoard->GetBoardFieldExist(x+2, y)) {
                field3 = gameBoard->GetBoardField(x+2, y);
                if (field3->GetBall() == player->GetBall()) {
                  CheckDirections(field1, field2, field3, ballMoves, ballMovesSize);
                }
              }
            }
          }

          // check balls from upleft to downright
          if (gameBoard->GetBoardFieldExist(x, y-1)) {
            field2 = gameBoard->GetBoardField(x, y-1);
            if (field2->GetBall() == player->GetBall()) {
              if (gameBoard->GetBoardFieldExist(x, y-2)) {
                field3 = gameBoard->GetBoardField(x, y-2);
                if (field3->GetBall() == player->GetBall()) {
                  CheckDirections(field1, field2, field3, ballMoves, ballMovesSize);
                }
              }
            }
          }
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

void GameManager::CheckDirections(BoardField* ball1, BoardField* ball2, BoardField* ball3, BallMove** ballMoves, int& ballMovesSize) const
{
  IsPossibleDirection(UPLEFT, ball1, ball2, ball3, ballMoves, ballMovesSize);
  IsPossibleDirection(UPRIGHT, ball1, ball2, ball3, ballMoves, ballMovesSize);
  IsPossibleDirection(LEFT, ball1, ball2, ball3, ballMoves, ballMovesSize);
  IsPossibleDirection(RIGHT, ball1, ball2, ball3, ballMoves, ballMovesSize);
  IsPossibleDirection(DOWNLEFT, ball1, ball2, ball3, ballMoves, ballMovesSize);
  IsPossibleDirection(DOWNRIGHT, ball1, ball2, ball3, ballMoves, ballMovesSize);
}

BOOL GameManager::IsPossibleDirection(Direction direction) const
{
  BOOL ret = FALSE;

  CPoint fieldCoord1;
  CPoint fieldCoord2;
  CPoint fieldCoord3;
  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  BoardField* ball1(0);
  BoardField* ball2(0);
  BoardField* ball3(0);

  if (mySelectedBall1) {
    ball1 = mySelectedBall1;
    if (mySelectedBall2) {
      ball2 = mySelectedBall2;
    }
    if (mySelectedBall3) {
      ball3 = mySelectedBall3;
    }
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

  if (ball1) {
    GetOpponentFields(direction, ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);
    opponentBall = BoardField::BLACK_BALL;
    if (ball1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }

    if (GetAxisOfBalls(ball1, ball2) == pushAxis) {
      // -> O|X or -> OO|X or -> OOO|X
      if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL) {
        ret = TRUE;
      }

      else if (ball2 && opponentField1 && opponentField1->GetBall() == opponentBall
        && (!opponentField2 || opponentField2->GetBall() == BoardField::NO_BALL))
        // -> OO|0 or -> OO|0X or -> OOO|0 or -> OOO|0X
      {
        ret = TRUE;
      }

      else if (ball3 && opponentField1 && opponentField2
        && opponentField1->GetBall() == opponentBall && opponentField2->GetBall() == opponentBall
        && (!opponentField3 || (opponentField3 && opponentField3->GetBall() == BoardField::NO_BALL)))
        // -> OOO|00 or -> OOO|00X
      {
        ret = TRUE;
      }
    }
    // sidesteps
    else {
      fieldCoord1 = ball1->GetFieldCoordinates();
      if (ball2) {
        fieldCoord2 = ball2->GetFieldCoordinates();
      }
      if (ball3) {
        fieldCoord3 = ball3->GetFieldCoordinates();
      }

      if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))
        && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord1, direction))->GetBall() == BoardField::NO_BALL)
      {
        if (ball2) {
          if (myGameBoard->GetBoardFieldExist(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))
            && myGameBoard->GetBoardField(GetNextFieldCoordinatesInDirection(fieldCoord2, direction))->GetBall() == BoardField::NO_BALL)
          {
            if (ball3) {
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
