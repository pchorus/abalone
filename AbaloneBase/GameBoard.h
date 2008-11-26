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

class FileIO;

class ABALONE_BASE_DLLINTERFACE GameBoard {

public:
  // constructor / destructor
  GameBoard();
  GameBoard(const GameBoard& other);
  ~GameBoard();

  // getter and setter
  BoardField* GetBoardField(int x, int y) const;
  BoardField* GetBoardField(CPoint point) const;
  // Tests whether the BoardField with coordinates x and y exists or not
  bool GetBoardFieldExist(int x, int y) const;
  bool GetBoardFieldExist(CPoint point) const;
  // Resets all BoardFields to initial state
  void Reset();

  void output();

  void CopyBoardFields(const GameBoard* other);

  GameBoard& operator=(const GameBoard& other);

private:
  // the BoardFields
  BoardField* myFieldPoints[BOARD_FIELDS_ROW][BOARD_FIELDS_COLUMN];
  FileIO* myFileIO;
};

inline BoardField* GameBoard::GetBoardField(int x, int y) const
{
  ASSERT(GetBoardFieldExist(x, y));
  return myFieldPoints[x][y];
}

inline BoardField* GameBoard::GetBoardField(CPoint point) const
{
  ASSERT(GetBoardFieldExist(point.x, point.y));
  return myFieldPoints[point.x][point.y];
}
