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
  GameBoard(const GameBoard& other);
  ~GameBoard();

  // getter and setter
  BoardField* GetBoardField(int x, int y) const;
  BoardField* GetBoardField(CPoint point) const;
  // Resets all BoardFields to initial state
  void Reset();

  CString ToString();

  void CopyBoardFields(const GameBoard* other);
  GameBoard& operator=(const GameBoard& other);

private:
  // Tests whether the BoardField with coordinates x and y exists or not
  bool GetBoardFieldExist(int x, int y) const;
  bool GetBoardFieldExist(CPoint point) const;
  // the BoardFields
  // the real game board is surrounded by three rows of BoardField NULL pointers
  // to indicate that there is no legal field
  BoardField* myFieldPoints[BOARD_FIELDS_COLUMN+6][BOARD_FIELDS_ROW+6];
};

inline BoardField* GameBoard::GetBoardField(int x, int y) const
{
//  ASSERT(GetBoardFieldExist(x, y));
  return myFieldPoints[x+3][y+3];
}

inline BoardField* GameBoard::GetBoardField(CPoint point) const
{
//  ASSERT(GetBoardFieldExist(point.x, point.y));
  return myFieldPoints[point.x+3][point.y+3];
}

inline bool GameBoard::GetBoardFieldExist(int x, int y) const
{
  return !(y > x + 4 || y < x - 4 || y < 0 || y > 8 || x < 0 || x > 8);
}

// inline bool GameBoard::GetBoardFieldExist(int x, int y) const
// {
//   return (y <= x + 4 && y >= x - 4 && y >= 0 && y <= 8 && x >= 0 && x <= 8);
// }

inline bool GameBoard::GetBoardFieldExist(CPoint point) const
{
  return GetBoardFieldExist(point.x, point.y);
}
