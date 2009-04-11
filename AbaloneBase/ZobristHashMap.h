#pragma once

#include "GameBoard.h"

class HashMapEntry;
class BallMove;
class GameManager;

static const int TWO_POW_20 = 1048576;

class ZobristHashMap {
public:
  ZobristHashMap();
  ~ZobristHashMap();

  ULONG64 GetHashKey(int x, int y, BoardField::Ball ball) const;
  ULONG64 GetHashKey(CPoint point, BoardField::Ball ball) const;
  HashMapEntry* GetEntry(ULONG64 key) const;

  ULONG64 CalcHashKey(const GameBoard* gameBoard) const;
  void RecalcHashKeyDoMove(ULONG64& currentHash, BallMove* move, GameManager* gameManager);
  void RecalcHashKeyUndoMove(ULONG64& currentHash, BallMove* move, GameManager* gameManager);

  void Insert(ULONG64 key, HashMapEntry* entry);

private:
  ULONG64 myHashKeys[BOARD_FIELDS_COLUMN][BOARD_FIELDS_ROW][3];
  // size: 2^20
  HashMapEntry* myHashMap[TWO_POW_20];
};

inline ULONG64 ZobristHashMap::GetHashKey(int x, int y, BoardField::Ball ball) const
{
  return myHashKeys[x][y][ball];
}

inline ULONG64 ZobristHashMap::GetHashKey(CPoint point, BoardField::Ball ball) const
{
  return myHashKeys[point.x][point.y][ball];
}

inline void ZobristHashMap::Insert(ULONG64 key, HashMapEntry* entry)
{
  myHashMap[key % TWO_POW_20] = entry;
}
