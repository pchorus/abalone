#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameBoard
//    Represents the game board the players are playing on.
//    Contains all BoardFields.
//
//////////////////////////////////////////////////////////////////////////

static const int BOARD_FIELDS_ROW = 9;
static const int BOARD_FIELDS_COLUMN = 9;

#include "BoardField.h"

class ABALONE_BASE_DLLINTERFACE GameBoard {

public:
  // constructor / destructor
  GameBoard();
  ~GameBoard();

  // getter and setter
  BoardField* GetBoardField(int x, int y) const;

private:
  // Tests whether the BoardField with coordinates x and y exists or not
  bool GetBoardFieldExist(int x, int y) const;

  // the BoardFields
  BoardField* myFieldPoints[BOARD_FIELDS_ROW][BOARD_FIELDS_COLUMN];
};

inline BoardField* GameBoard::GetBoardField(int x, int y) const
{
  ASSERT(GetBoardFieldExist(x, y));
  return myFieldPoints[x][y];
}
