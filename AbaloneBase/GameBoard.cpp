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