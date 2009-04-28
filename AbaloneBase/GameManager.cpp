#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayerMonteCarlo.h"
#include "ComputerPlayerAlphaBeta.h"
#include "ComputerPlayerAlphaBetaIterativeDeepening.h"
#include "ComputerPlayerRandomMoves.h"
#include "ComputerPlayerEvaluateNextMove.h"
#include "BallMove.h"
#include "Output.h"

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
, myMaxNumberOfTurns(INT_MAX)
, myPlyCount(0)
, myBallMovesSize(0)
, myCheckAttackingOnlyAtBorder(false)
{
  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    myBallMoves[i] = new BallMove;
  }
}

GameManager::~GameManager()
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }
  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    delete myBallMoves[i];
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
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
    myPlayer1 = new ComputerPlayerAlphaBetaIterativeDeepening(this, namePlayer1 != "" ? namePlayer1 : "Player 1", BoardField::BLACK_BALL);
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
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
    myPlayer2 = new ComputerPlayerAlphaBetaIterativeDeepening(this, namePlayer2 != "" ? namePlayer2 : "Player 2", BoardField::WHITE_BALL);
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

  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;

  GetOpponentFields(direction, ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);
  BoardField::Ball opponentBall = BoardField::BLACK_BALL;
  if (ball1->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  if (GetAxisOfBalls(ball1, ball2) == pushAxis) {
    // -> O|X or -> OO|X or -> OOO|X
    if (opponentField1) {
      if (opponentField1->GetBall() == BoardField::NO_BALL) {
        ballMoves[ballMovesSize]->Init(direction, false, false, ball1, ball2, ball3, 0);
        ++ballMovesSize;
      }
      else if (opponentField1->GetBall() == opponentBall) {
        if (ball2 && (!opponentField2 || opponentField2->GetBall() == BoardField::NO_BALL))
          // -> OO|0 or -> OO|0X or -> OOO|0 or -> OOO|0X
        {
          ballMoves[ballMovesSize]->Init(direction, true, !opponentField2, ball1, ball2, ball3, 1);
          ++ballMovesSize;
        }

        else if (ball3 && opponentField2 && opponentField2->GetBall() == opponentBall
          && (!opponentField3 || (opponentField3 && opponentField3->GetBall() == BoardField::NO_BALL)))
          // -> OOO|00 or -> OOO|00X
        {
          ballMoves[ballMovesSize]->Init(direction, true, !opponentField3, ball1, ball2, ball3, 2);
          ++ballMovesSize;
        }
      }
    }
  }
  // sidesteps
  else {
    // since we use the opponentField1 as help variable since it is not used otherwise in this else block
    opponentField1 = GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
    if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL)
    {
      if (ball2) {
        opponentField1 = GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction);
        if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL)
        {
          if (ball3) {
            opponentField1 = GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction);
            if (opponentField1 && opponentField1->GetBall() == BoardField::NO_BALL)
            {
              ballMoves[ballMovesSize]->Init(direction, false, false, ball1, ball2, ball3, 0);
              ++ballMovesSize;
            }
          }
          else {
            ballMoves[ballMovesSize]->Init(direction, false, false, ball1, ball2, ball3, 0);
            ++ballMovesSize;
          }
        }
      }
      else {
        ballMoves[ballMovesSize]->Init(direction, false, false, ball1, ball2, ball3, 0);
        ++ballMovesSize;
      }
    }
  }
}

void GameManager::MoveBallsInDirection(Direction direction)
{
  // we are allowed to pass 0 as opponentBalls, because it is only used in UndoMove
  BallMove move(direction, false, false, mySelectedBall1, mySelectedBall2, mySelectedBall3, 0);
  DoMove(&move);

  CString msg;
  msg.Format("Human\nMove: %s\n\n", move.ToString());
  Output::Message(msg, false, true);
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
  Direction direction = move->GetDirection();
  BoardField* field = 0;

  BoardField* opponentField1 = 0;
  BoardField* opponentField2 = 0;
  BoardField* opponentField3 = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  GetOpponentFields(direction, ball1, ball2, ball3,
    opponentField1, opponentField2, opponentField3);

  opponentBall = BoardField::BLACK_BALL;
  if (ball1->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  // first, opponent balls are moved
  if (opponentField1 && opponentField1->GetBall() == opponentBall) {
    if (opponentField2 && opponentField2->GetBall() == opponentBall) {
      field = GetNextFieldInDirection(opponentField2->GetFieldCoordinates(), direction);
      if (field) {
        field->SetBall(opponentField2->GetBall());
        opponentField2->SetBall(BoardField::NO_BALL);
        opponentField2->SetIsSelected(false);
      }
      else {
        AddLostBall(opponentBall);
      }
    }
    field = GetNextFieldInDirection(opponentField1->GetFieldCoordinates(), direction);
    if (field) {
      field->SetBall(opponentField1->GetBall());
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
    GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction)->SetBall(ball1->GetBall());
    ball1->SetBall(BoardField::NO_BALL);
    ball1->SetIsSelected(false);

    if (ball2) {
      GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction)->SetBall(ball2->GetBall());
      ball2->SetBall(BoardField::NO_BALL);
      ball2->SetIsSelected(false);
    }

    if (ball3) {
      GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction)->SetBall(ball3->GetBall());
      ball3->SetBall(BoardField::NO_BALL);
      ball3->SetIsSelected(false);
    }
    break;
  case UPRIGHT:
  case RIGHT:
  case DOWNRIGHT:
    if (ball3) {
      GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction)->SetBall(ball3->GetBall());
      ball3->SetBall(BoardField::NO_BALL);
      ball3->SetIsSelected(false);
    }

    if (ball2) {
      GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction)->SetBall(ball2->GetBall());
      ball2->SetBall(BoardField::NO_BALL);
      ball2->SetIsSelected(false);
    }

    GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction)->SetBall(ball1->GetBall());
    ball1->SetBall(BoardField::NO_BALL);
    ball1->SetIsSelected(false);

    break;
  }
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

  Direction direction = move->GetDirection();
  int opponentBalls = move->GetOpponentBalls();
  move->GetBalls(ball1, ball2, ball3);
  GetOpponentFields(direction, ball1, ball2, ball3, opponentBall1, opponentBall2, opponentBall3);
  CPoint coord;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

  opponentBall = BoardField::BLACK_BALL;
  if (GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction)->GetBall() == BoardField::BLACK_BALL) {
    opponentBall = BoardField::WHITE_BALL;
  }

  switch (direction) {
  case UPLEFT:
  case LEFT:
  case DOWNLEFT:
    // own balls
    if (ball3) {
      field = GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction);
      ball3->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    if (ball2) {
      field = GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction);
      ball2->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    field = GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
    ball1->SetBall(field->GetBall());
    field->SetBall(BoardField::NO_BALL);
    field->SetIsSelected(false);
    break;
  case UPRIGHT:
  case RIGHT:
  case DOWNRIGHT:
    // own balls
    field = GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
    ball1->SetBall(field->GetBall());
    field->SetBall(BoardField::NO_BALL);
    field->SetIsSelected(false);

    if (ball2) {
      field = GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction);
      ball2->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }

    if (ball3) {
      field = GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction);
      ball3->SetBall(field->GetBall());
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    break;
  }

  // opponent balls
  if (opponentBalls >= 1) {
    opponentBall1->SetBall(opponentBall);
    field = GetNextFieldInDirection(opponentBall1->GetFieldCoordinates(), direction);
    if (field) {
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    else {
      RemoveLostBall(opponentBall);
    }
  }

  if (opponentBalls >= 2) {
    opponentBall2->SetBall(opponentBall);
    field = GetNextFieldInDirection(opponentBall2->GetFieldCoordinates(), direction);
    if (field) {
      field->SetBall(BoardField::NO_BALL);
      field->SetIsSelected(false);
    }
    else {
      RemoveLostBall(opponentBall);
    }
  }
}

BallAxis GameManager::GetAxisOfBalls(const BoardField* ball1, const BoardField* ball2)
{
  BallAxis ret = NO_VALID_AXIS;

  // because of validation done before, it is sufficient to only test the relative position
  // between two balls, even if three balls are selected
  if (ball2) {
    CPoint selectedFieldPoint1 = ball1->GetFieldCoordinates();
    CPoint selectedFieldPoint2 = ball2->GetFieldCoordinates();
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
//   opponentField1 = 0;
//   opponentField2 = 0;
//   opponentField3 = 0;

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
    opponentField1 = myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+1);
    opponentField2 = myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+2);
    opponentField3 = myGameBoard->GetBoardField(fieldCoord1.x, fieldCoord1.y+3);
    break;
  case UPRIGHT:
    opponentField1 = myGameBoard->GetBoardField(lastFieldCoord.x+1, lastFieldCoord.y+1);
    opponentField2 = myGameBoard->GetBoardField(lastFieldCoord.x+2, lastFieldCoord.y+2);
    opponentField3 = myGameBoard->GetBoardField(lastFieldCoord.x+3, lastFieldCoord.y+3);
    break;
  case LEFT:
    opponentField1 = myGameBoard->GetBoardField(fieldCoord1.x-1, fieldCoord1.y);
    opponentField2 = myGameBoard->GetBoardField(fieldCoord1.x-2, fieldCoord1.y);
    opponentField3 = myGameBoard->GetBoardField(fieldCoord1.x-3, fieldCoord1.y);
    break;
  case RIGHT:
    opponentField1 = myGameBoard->GetBoardField(lastFieldCoord.x+1, lastFieldCoord.y);
    opponentField2 = myGameBoard->GetBoardField(lastFieldCoord.x+2, lastFieldCoord.y);
    opponentField3 = myGameBoard->GetBoardField(lastFieldCoord.x+3, lastFieldCoord.y);
    break;
  case DOWNLEFT:
    opponentField1 = myGameBoard->GetBoardField(fieldCoord1.x-1, fieldCoord1.y-1);
    opponentField2 = myGameBoard->GetBoardField(fieldCoord1.x-2, fieldCoord1.y-2);
    opponentField3 = myGameBoard->GetBoardField(fieldCoord1.x-3, fieldCoord1.y-3);
    break;
  case DOWNRIGHT:
    opponentField1 = myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-1);
    opponentField2 = myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-2);
    opponentField3 = myGameBoard->GetBoardField(lastFieldCoord.x, lastFieldCoord.y-3);
    break;
  }
}

CPoint GameManager::GetNextFieldCoordinatesInDirection(const CPoint& fieldCoord, Direction direction)
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
  double ret = myLostBallsPlayer1 - myLostBallsPlayer2;
  if (player == myPlayer1) {
    ret *= -1;
  }

  return ret;
}

int GameManager::CalcCenterDistance(const Player* player) const
{
  int centerDistance = 0;
  BoardField::Ball playersBall = player->GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      currentField = myGameBoard->GetBoardField(x, y);
      if (currentField) {
        if (currentField->GetBall() == playersBall) {
          centerDistance += CalcCenterDistance(currentField->GetFieldCoordinates());
        }
      }
    }
  }

  return centerDistance;
}

int GameManager::CalcGrouping(const Player* player) const
{
  int grouping = 0;
  BoardField::Ball playersBall = player->GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      currentField = myGameBoard->GetBoardField(x, y);
      if (currentField) {
        if (currentField->GetBall() == playersBall) {
          grouping += CalcGroupingField(player, currentField->GetFieldCoordinates());
        }
      }
    }
  }

  return grouping;
}

int GameManager::CalcOpposingGrouping(const Player* player) const
{
  const ComputerPlayer* computerPlayer = 0;

  // computerPlayer should be the opponent
  if (player == myPlayer1) {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer2);
  }
  else {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer1);
  }
  return CalcGrouping(computerPlayer);
}

int GameManager::CalcAttackingPowerOnOpponent(const Player* player) const
{
  int count = 0;

  BoardField* left1(0);
  BoardField* left2(0);
  BoardField* left3(0);
  BoardField* right1(0);
  BoardField* right2(0);
  BoardField* right3(0);
  GameBoard* gameBoard = myGameBoard;
  BoardField::Ball ball = player->GetBall();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      // check whether two different colored marbles are facing each other
      left1 = gameBoard->GetBoardField(x, y);
      if (left1 && left1->GetBall() != BoardField::NO_BALL) {
        // check balls from left to right
        right1 = gameBoard->GetBoardField(x+1, y);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x-1, y);
          left3 = gameBoard->GetBoardField(x-2, y);
          right2 = gameBoard->GetBoardField(x+2, y);
          right3 = gameBoard->GetBoardField(x+3, y);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              ++count;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              ++count;
            }
          }
        }

        // check balls from up left to downright
        right1 = gameBoard->GetBoardField(x, y-1);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x, y+1);
          left3 = gameBoard->GetBoardField(x, y+2);
          right2 = gameBoard->GetBoardField(x, y-2);
          right3 = gameBoard->GetBoardField(x, y-3);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              ++count;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              ++count;
            }
          }
        }
        // check balls from downleft to upright
        right1 = gameBoard->GetBoardField(x+1, y+1);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x-1, y-1);
          left3 = gameBoard->GetBoardField(x-2, y-2);
          right2 = gameBoard->GetBoardField(x+2, y+2);
          right3 = gameBoard->GetBoardField(x+3, y+3);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              ++count;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              ++count;
            }
          }
        }
      }
    }
  }
  return count;
}

int GameManager::CalcAttackedByOpponent(const Player* player) const
{
  const ComputerPlayer* computerPlayer = 0;

  // computerPlayer should be the opponent
  if (player == myPlayer1) {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer2);
  }
  else {
    computerPlayer = dynamic_cast<const ComputerPlayer*>(myPlayer1);
  }
  return CalcAttackingPowerOnOpponent(computerPlayer);
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

  BoardField* field = 0;
  // check the fields around the new field for fellow balls
  field = GetNextFieldInDirection(coord, UPLEFT);
  if (field && field->GetBall() == player->GetBall()) {
    ++ret;
  }
  field = GetNextFieldInDirection(coord,UPRIGHT);
  if (field && field->GetBall() == player->GetBall()) {
    ++ret;
  }
  field = GetNextFieldInDirection(coord, LEFT);
  if (field && field->GetBall() == player->GetBall()) {
    ++ret;
  }
  field = GetNextFieldInDirection(coord, RIGHT);
  if (field && field->GetBall() == player->GetBall()) {
    ++ret;
  }
  field = GetNextFieldInDirection(coord, DOWNLEFT);
  if (field && field->GetBall() == player->GetBall()) {
    ++ret;
  }
  field = GetNextFieldInDirection(coord, DOWNRIGHT);
  if (field && field->GetBall() == player->GetBall()) {
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

  if (myNextTurn == myPlayer1) {
    myNextTurn = myPlayer2;
  }
  else {
    myNextTurn = myPlayer1;
  }

  mySelectedBall1 = 0;
  mySelectedBall2 = 0;
  mySelectedBall3 = 0;
  myPlyCount = 0;

  while (myPlyCount < myMaxNumberOfTurns
    && myLostBallsPlayer1 < 6 && myLostBallsPlayer2 < 6 && myNextTurn->GetType() != Player::PLAYER_TYPE_HUMAN)
  {
    computerPlayer = static_cast<ComputerPlayer*>(myNextTurn);
    computerPlayer->TakeNextTurn();

    if (myNextTurn == myPlayer1) {
      myNextTurn = myPlayer2;
    }
    else {
      myNextTurn = myPlayer1;
    }
    mySelectedBall1 = 0;
    mySelectedBall2 = 0;
    mySelectedBall3 = 0;

    ++myPlyCount;

    if (pParam) {
      ::PostMessage((HWND)pParam, WM_COMPUTER_CALC_FINISHED, 0, 0);
    }
  }
}

void GameManager::AddPossibleMovesOneBall(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const
{
  BoardField* field = 0;
  GameBoard* gameBoard = myGameBoard;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field = gameBoard->GetBoardField(x, y);
      if (field) {
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
  GameBoard* gameBoard = myGameBoard;

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field1 = gameBoard->GetBoardField(x, y);
      if (field1) {
        if (field1->GetBall() == player->GetBall()) {
          // check balls from downleft to upright
          field2 = gameBoard->GetBoardField(x+1, y+1);
          if (field2) {
            if (field2->GetBall() == player->GetBall()) {
              CheckDirections(field1, field2, 0, ballMoves, ballMovesSize);
            }
          }

          // check balls from left to right
          field2 = gameBoard->GetBoardField(x+1, y);
          if (field2) {
            if (field2->GetBall() == player->GetBall()) {
              CheckDirections(field1, field2, 0, ballMoves, ballMovesSize);
            }
          }
          // check balls from up left to downright
          field2 = gameBoard->GetBoardField(x, y-1);
          if (field2) {
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
  GameBoard* gameBoard = myGameBoard;

  // for every ball we check the movement for this ball and a neighbor ball to the upper right, right and
  // downright, so every combination of balls is checked only once
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field1 = gameBoard->GetBoardField(x, y);
      if (field1) {
        if (field1->GetBall() == player->GetBall()) {
          // check balls from downleft to upright
          field2 = gameBoard->GetBoardField(x+1, y+1);
          if (field2) {
            if (field2->GetBall() == player->GetBall()) {
              field3 = gameBoard->GetBoardField(x+2, y+2);
              if (field3) {
                if (field3->GetBall() == player->GetBall()) {
                  CheckDirections(field1, field2, field3, ballMoves, ballMovesSize);
                }
              }
            }
          }

          // check balls from left to right
          field2 = gameBoard->GetBoardField(x+1, y);
          if (field2) {
            if (field2->GetBall() == player->GetBall()) {
              field3 = gameBoard->GetBoardField(x+2, y);
              if (field3) {
                if (field3->GetBall() == player->GetBall()) {
                  CheckDirections(field1, field2, field3, ballMoves, ballMovesSize);
                }
              }
            }
          }

          // check balls from upleft to downright
          field2 = gameBoard->GetBoardField(x, y-1);
          if (field2) {
            if (field2->GetBall() == player->GetBall()) {
              field3 = gameBoard->GetBoardField(x, y-2);
              if (field3) {
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

void GameManager::AddCapturingMoves(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize)
{
  myCheckAttackingOnlyAtBorder = true;
  int opponentBalls = 0;

  BoardField* left1(0);
  BoardField* left2(0);
  BoardField* left3(0);
  BoardField* right1(0);
  BoardField* right2(0);
  BoardField* right3(0);
  GameBoard* gameBoard = myGameBoard;
  BoardField::Ball ball = player->GetBall();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      // check whether two different colored marbles are facing each other
      left1 = gameBoard->GetBoardField(x, y);
      if (left1 && left1->GetBall() != BoardField::NO_BALL) {
        // check balls from left to right
        right1 = gameBoard->GetBoardField(x+1, y);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x-1, y);
          left3 = gameBoard->GetBoardField(x-2, y);
          right2 = gameBoard->GetBoardField(x+2, y);
          right3 = gameBoard->GetBoardField(x+3, y);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              opponentBalls = 1;
              if (right2 && right2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(RIGHT, true, true, left3, left2, left1, opponentBalls);
              }
              else {
                if (left3 && left3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(RIGHT, true, true, left3, left2, left1, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(RIGHT, true, true, left2, left1, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              opponentBalls = 1;
              if (left2 && left2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(LEFT, true, true, right1, right2, right3, opponentBalls);
              }
              else {
                if (right3 && right3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(LEFT, true, true, right1, right2, right3, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(LEFT, true, true, right1, right2, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
        }

        // check balls from up left to downright
        right1 = gameBoard->GetBoardField(x, y-1);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x, y+1);
          left3 = gameBoard->GetBoardField(x, y+2);
          right2 = gameBoard->GetBoardField(x, y-2);
          right3 = gameBoard->GetBoardField(x, y-3);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              opponentBalls = 1;
              if (right2 && right2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(DOWNRIGHT, true, true, left3, left2, left1, opponentBalls);
              }
              else {
                if (left3 && left3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(DOWNRIGHT, true, true, left3, left2, left1, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(DOWNRIGHT, true, true, left2, left1, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              opponentBalls = 1;
              if (left2 && left2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(UPLEFT, true, true, right1, right2, right3, opponentBalls);
              }
              else {
                if (right3 && right3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(UPLEFT, true, true, right1, right2, right3, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(UPLEFT, true, true, right1, right2, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
        }
        // check balls from downleft to upright
        right1 = gameBoard->GetBoardField(x+1, y+1);
        if (right1 && right1->GetBall() != BoardField::NO_BALL && left1->GetBall() != right1->GetBall()) {
          // opponent balls are facing each other
          left2 = gameBoard->GetBoardField(x-1, y-1);
          left3 = gameBoard->GetBoardField(x-2, y-2);
          right2 = gameBoard->GetBoardField(x+2, y+2);
          right3 = gameBoard->GetBoardField(x+3, y+3);

          if (left1->GetBall() == ball) {
            // left balls are own balls
            if (IsAttacking(left1, left2, left3, right2, right3)) {
              opponentBalls = 1;
              if (right2 && right2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(UPRIGHT, true, true, left3, left2, left1, opponentBalls);
              }
              else {
                if (left3 && left3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(UPRIGHT, true, true, left3, left2, left1, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(UPRIGHT, true, true, left2, left1, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
          else if (right1->GetBall() == ball) {
            // right balls are own balls
            if (IsAttacking(right1, right2, right3, left2, left3)) {
              opponentBalls = 1;
              if (left2 && left2->GetBall() != BoardField::NO_BALL)
                opponentBalls = 2;

              if (opponentBalls == 2) {
                ballMoves[ballMovesSize]->Init(DOWNLEFT, true, true, right1, right2, right3, opponentBalls);
              }
              else {
                if (right3 && right3->GetBall() == ball) {
                  ballMoves[ballMovesSize]->Init(DOWNLEFT, true, true, right1, right2, right3, opponentBalls);
                  ++ballMovesSize;
                }
                ballMoves[ballMovesSize]->Init(DOWNLEFT, true, true, right1, right2, 0, opponentBalls);
              }
              ++ballMovesSize;
            }
          }
        }
      }
    }
  }
  myCheckAttackingOnlyAtBorder = false;
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
      if (myGameBoard->GetBoardField(x, y)) {
        myGameBoard->GetBoardField(x, y)->SetBall(BoardField::BLACK_BALL);
      }
    }
  }
  myGameBoard->GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(3, 2)->SetBall(BoardField::BLACK_BALL);
  myGameBoard->GetBoardField(4, 2)->SetBall(BoardField::BLACK_BALL);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = BOARD_FIELDS_ROW-1; y > BOARD_FIELDS_ROW-3; --y) {
      if (myGameBoard->GetBoardField(x, y)) {
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

  BoardField* field(0);

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

      field = GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
      if (field && field->GetBall() == BoardField::NO_BALL)
      {
        if (ball2) {
          field = GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction);
          if (field && field->GetBall() == BoardField::NO_BALL)
          {
            if (ball3) {
              field = GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction);
              if (field && field->GetBall() == BoardField::NO_BALL)
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

inline BoardField* GameManager::GetNextFieldInDirection(CPoint fieldCoord, Direction direction) const
{
  switch (direction) {
  case UPLEFT:
    return myGameBoard->GetBoardField(fieldCoord.x, fieldCoord.y+1);
  case UPRIGHT:
    return myGameBoard->GetBoardField(fieldCoord.x+1, fieldCoord.y+1);
  case LEFT:
    return myGameBoard->GetBoardField(fieldCoord.x-1, fieldCoord.y);
  case RIGHT:
    return myGameBoard->GetBoardField(fieldCoord.x+1, fieldCoord.y);
  case DOWNLEFT:
    return myGameBoard->GetBoardField(fieldCoord.x-1, fieldCoord.y-1);
  case DOWNRIGHT:
    return myGameBoard->GetBoardField(fieldCoord.x, fieldCoord.y-1);
  }

//   switch (direction) {
//   case UPLEFT:
//     return myFieldPoints[coord.x+3][coord.y+4];
//   case UPRIGHT:
//     return myFieldPoints[coord.x+4][coord.y+4];
//   case LEFT:
//     return myFieldPoints[coord.x+2][coord.y+3];
//   case RIGHT:
//     return myFieldPoints[coord.x+4][coord.y+3];
//   case DOWNLEFT:
//     return myFieldPoints[coord.x+2][coord.y+2];
//   case DOWNRIGHT:
//     return myFieldPoints[coord.x+3][coord.y+2];
//   }
  return 0;
}

int GameManager::EvaluateBoard(Player* player, int evaluation) const
{
  int ret = 0;
  if (evaluation == 1) {
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 1000;
      ret -= ownLostBalls * 1000;
    }

    // Center Distance ========================================================
    // 20 is the best value to achieve with all 14 marbles
    // 56 is the worst value: every marble is on the game board's border
    int centerDistance = CalcCenterDistance(player);
    if (centerDistance < 24) {
      ret += 400;
    }
    else if (centerDistance < 30) {
      ret += 300;
    }
    else if (centerDistance < 35) {
      ret += 200;
    }
    else if (centerDistance < 40) {
      ret += 100;
    }

    // Marbles Grouping =======================================================
    int grouping = CalcGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret += 320;
    }
    else if (grouping > 50) {
      ret += 240;
    }
    else if (grouping > 45) {
      ret += 160;
    }
    else if (grouping > 40) {
      ret += 80;
    }

    // Attacking Power ========================================================
    // TODO: Output of power
    ret += 10 * CalcAttackingPowerOnOpponent(player);

    // Defending Power ========================================================
    ret -= 10 * CalcAttackedByOpponent(player);
  }
  else if (evaluation == 2) {
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 1000;
      ret -= ownLostBalls * 1000;
    }

    // Center Distance ========================================================
    // 20 is the best value to achieve with all 14 marbles
    // 56 is the worst value: every marble is on the game board's border
    int centerDistance = CalcCenterDistance(player);
    if (centerDistance < 24) {
      ret += 400;
    }
    else if (centerDistance < 30) {
      ret += 300;
    }
    else if (centerDistance < 35) {
      ret += 200;
    }
    else if (centerDistance < 40) {
      ret += 100;
    }

    // Marbles Grouping =======================================================
    int grouping = CalcGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret += 320;
    }
    else if (grouping > 50) {
      ret += 240;
    }
    else if (grouping > 45) {
      ret += 160;
    }
    else if (grouping > 40) {
      ret += 80;
    }

    // Opposing Marbles Grouping ==============================================
    grouping = CalcOpposingGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret -= 40;
    }
    else if (grouping > 50) {
      ret -= 30;
    }
    else if (grouping > 45) {
      ret -= 20;
    }
    else if (grouping > 40) {
      ret -= 10;
    }

    // Attacking Power ========================================================
    // TODO: Output of power
    ret += 10 * CalcAttackingPowerOnOpponent(player);

    // Defending Power ========================================================
    ret -= 10 * CalcAttackedByOpponent(player);
  }
  else if (evaluation == 3) {
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 1000;
      ret -= ownLostBalls * 1000;
    }

    // Center Distance ========================================================
    // 20 is the best value to achieve with all 14 marbles
    // 56 is the worst value: every marble is on the game board's border
    int centerDistance = CalcCenterDistance(player);
    if (centerDistance < 24) {
      ret += 400;
    }
    else if (centerDistance < 30) {
      ret += 300;
    }
    else if (centerDistance < 35) {
      ret += 200;
    }
    else if (centerDistance < 40) {
      ret += 100;
    }

    // Marbles Grouping =======================================================
    int grouping = CalcGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret += 320;
    }
    else if (grouping > 50) {
      ret += 240;
    }
    else if (grouping > 45) {
      ret += 160;
    }
    else if (grouping > 40) {
      ret += 80;
    }
  }
  else if (evaluation == 4) {
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 1000;
      ret -= ownLostBalls * 1000;
    }

    // Center Distance ========================================================
    // 20 is the best value to achieve with all 14 marbles
    // 56 is the worst value: every marble is on the game board's border
    int centerDistance = CalcCenterDistance(player);
    if (centerDistance < 24) {
      ret += 400;
    }
    else if (centerDistance < 30) {
      ret += 300;
    }
    else if (centerDistance < 35) {
      ret += 200;
    }
    else if (centerDistance < 40) {
      ret += 100;
    }

    // Marbles Grouping =======================================================
    int grouping = CalcGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret += 320;
    }
    else if (grouping > 50) {
      ret += 240;
    }
    else if (grouping > 45) {
      ret += 160;
    }
    else if (grouping > 40) {
      ret += 80;
    }

    // Attacking Power ========================================================
    // TODO: Output of power
    ret += 40 * CalcAttackingPowerOnOpponent(player);

    // Defending Power ========================================================
    ret -= 40 * CalcAttackedByOpponent(player);
  }
  else if (evaluation == 5) {
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 700;
      ret -= ownLostBalls * 1000;
    }

    // Center Distance ========================================================
    // 20 is the best value to achieve with all 14 marbles
    // 56 is the worst value: every marble is on the game board's border
    int centerDistance = CalcCenterDistance(player);
    if (centerDistance < 24) {
      ret += 400;
    }
    else if (centerDistance < 30) {
      ret += 300;
    }
    else if (centerDistance < 35) {
      ret += 200;
    }
    else if (centerDistance < 40) {
      ret += 100;
    }

    // Marbles Grouping =======================================================
    int grouping = CalcGrouping(player);
    // 58 : all marbles are in a huge single group
    //  0 : no marble has any neighboring fellow marbles
    // below 40 is already not very good
    if (grouping > 55) {
      ret += 320;
    }
    else if (grouping > 50) {
      ret += 240;
    }
    else if (grouping > 45) {
      ret += 160;
    }
    else if (grouping > 40) {
      ret += 80;
    }

    // Attacking Power ========================================================
    // TODO: Output of power
    ret += 10 * CalcAttackingPowerOnOpponent(player);

    // Defending Power ========================================================
    ret -= 10 * CalcAttackedByOpponent(player);
  }
  else if (evaluation == 6) {
    // use only the ratio between the lost balls of both players
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (opponentLostBalls == 6) {
      ret += 1000000;
    }
    else if (ownLostBalls == 6) {
      ret -= 1000000;
    }
    else {
      ret += opponentLostBalls * 1000;
      ret -= ownLostBalls * 1000;
    }
  }
  else if (evaluation == 7) {
    // use only the final outcome which is win, draw or defeat
    // Lost Marbles ===========================================================
    int ownLostBalls = 0;
    int opponentLostBalls = 0;
    if (player == myPlayer1) {
      ownLostBalls = GetLostBallsPlayer1();
      opponentLostBalls = GetLostBallsPlayer2();
    }
    else {
      ownLostBalls = GetLostBallsPlayer2();
      opponentLostBalls = GetLostBallsPlayer1();
    }

    if (ownLostBalls != 6 && opponentLostBalls == 6) {
      ret += 10;
    }
    else if (ownLostBalls == 6 && opponentLostBalls != 6) {
      ret -= 10;
    }
  }

  // random factor
//  ret += ((rand() % 5) - 2);

  return ret;
}

void GameManager::OrderMoves(int startOrderAtIdx, BallMove** ballMoves, int ballMoveSize) const
{
  qsort(ballMoves, startOrderAtIdx, ballMoveSize-1);

  // only for debug
//   if (false) {
//     BallMove* current = 0;
//     CString msg;
// 
//     Output::Message(myGameBoard->ToString(), false, true);
//     for (int i = 0; i < ballMoveSize; ++i) {
//       current = ballMoves[i];
//       msg.Format("%s\n", current->ToStringDebug());
//       Output::Message(msg, false, true);
//     }
//   }
}

bool GameManager::IsQuiescencePosition(Player* player)
{
  // the position is only quiescent if no player is currently attacking the other one at
  // the border of the game board
  myCheckAttackingOnlyAtBorder = true;
  int ret = CalcAttackingPowerOnOpponent(player);/* + CalcAttackedByOpponent(player);*/
  myCheckAttackingOnlyAtBorder = false;

  return ret == 0;
}

void GameManager::qsort(BallMove** arr, int l, int r) const
{
  int i, j;
  BallMove* tmp;
  // Quicksort für Teilfelder größer als 8
  if (r-l > 8) {
    i = l - 1;
    j = r;
    for (;;) {
      while(arr[++i]->Compare(arr[r]) < 0);
      while(arr[--j]->Compare(arr[r]) > 0 && j > i);
      if( i >= j) break;
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }
    tmp = arr[i];
    arr[i] = arr[r];
    arr[r] = tmp;

    qsort(arr, l, i-1);
    qsort(arr, i+1, r);
  }
  // für Teilfelder kleiner als 9 Insertion Sort
  else {
    for (i = l+1; i <= r; ++i) {
      tmp = arr[i];
      for(j = i-1; j >= l && tmp->Compare(arr[j]) < 0; --j)
        arr[j+1] = arr[j];
      arr[j+1] = tmp;
    }
  }
}
