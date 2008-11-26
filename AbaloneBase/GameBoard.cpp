#include "AbaloneBase.h"

#include "GameBoard.h"

GameBoard::GameBoard()
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y] = new BoardField(x, y);
    }
  }
}

GameBoard::GameBoard(const GameBoard& other)
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y] = new BoardField(x, y);
      *myFieldPoints[x][y] = *other.GetBoardField(x, y);
    }
  }
}

GameBoard::~GameBoard()
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      delete myFieldPoints[x][y];
    }
  }
}

bool GameBoard::GetBoardFieldExist(int x, int y) const
{
  bool ret = true;

  if (y > x + 4 || y < x - 4 || y < 0 || y > 8 || x < 0 || x > 8) {
    ret = false;
  }

  return ret;
}

bool GameBoard::GetBoardFieldExist(CPoint point) const
{
  return GetBoardFieldExist(point.x, point.y);
}

void GameBoard::Reset()
{

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y]->SetBall(BoardField::NO_BALL);
      myFieldPoints[x][y]->SetIsSelected(false);
    }
  }
}

CString GameBoard::ToString()
{
  CString line = "";
  for (int y = 8; y >= 0; --y) {
    for (int x = 0; x < 9; ++x) {
      if (myFieldPoints[x][y]->GetBall() == BoardField::NO_BALL) {
        line += "-";
      }
      else if (myFieldPoints[x][y]->GetBall() == BoardField::WHITE_BALL) {
        line += "W";
      }
      else if (myFieldPoints[x][y]->GetBall() == BoardField::BLACK_BALL) {
        line += "B";
      }
      line += " ";
    }

    line += "\n";
  }

  return line;
}

GameBoard& GameBoard::operator=(const GameBoard& other)
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      *myFieldPoints[x][y] = *other.myFieldPoints[x][y];
    }
  }

  return *this;
}

void GameBoard::CopyBoardFields(const GameBoard* other)
{
  BoardField* myField = 0;
  BoardField* othersField = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myField = myFieldPoints[x][y];
      othersField = other->myFieldPoints[x][y];

      myField->SetBall(othersField->GetBall());
      myField->SetFieldCoordinates(othersField->GetFieldCoordinates());
      myField->SetGUICoordinates(othersField->GetGUICoordinates());
      myField->SetIsSelected(othersField->IsSelected());
    }
  }
}
