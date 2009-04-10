#pragma once

#include "GameBoard.h"

class HashMapEntry;

static const int TWO_POW_20 = 1048576;

class ZobristHashMap {
public:
  ZobristHashMap();
  ~ZobristHashMap();

  ULONG64 GetHashKey(int x, int y, BoardField::Ball ball) const;
  HashMapEntry* GetEntry(ULONG64 key) const;

private:
  ULONG64 myHashKeys[BOARD_FIELDS_COLUMN][BOARD_FIELDS_ROW][3];
  // size: 2^20
  HashMapEntry* myHashMap[TWO_POW_20];
};

inline ULONG64 ZobristHashMap::GetHashKey(int x, int y, BoardField::Ball ball) const
{
  return myHashKeys[x][y][ball];
}