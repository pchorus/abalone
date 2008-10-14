#include "AbaloneBase.h"

#include "GameBoard.h"

GameBoard::GameBoard()
{
  
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y] = new BoardField;
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
  // TODO: implementation
  return true;
}
