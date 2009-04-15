#pragma once

#include "GameBoard.h"
#include "HashMapEntry.h"

class BallMove;
class GameManager;

static const int TWO_POW_20 = 1048576;

class ABALONE_BASE_DLLINTERFACE ZobristHashMap {
public:
  ZobristHashMap();
  ~ZobristHashMap();

  ULONG64 GetHashKey(int x, int y, BoardField::Ball ball) const;
  ULONG64 GetHashKey(CPoint point, BoardField::Ball ball) const;
  HashMapEntry* GetEntry(ULONG64 key) const;

  ULONG64 CalcHashKey(const GameBoard* gameBoard) const;
  void RecalcHashKey(ULONG64& currentHash, BallMove* move, GameManager* gameManager);

  void Insert(ULONG64 key, byte depth, int value, HashMapEntry::ValueType valueType, BallMove* move);
  HashMapEntry* Check(ULONG64 key);

  void UnInit();

  unsigned int myInserts;
  unsigned int myReUseEntries;

private:
  ULONG64 myHashKeys[BOARD_FIELDS_COLUMN][BOARD_FIELDS_ROW][2];
  ULONG64 myHashKeyPlayer1IsNext;
  ULONG64 myHashKeyPlayer2IsNext;
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

inline void ZobristHashMap::Insert(ULONG64 key, byte depth, int value, HashMapEntry::ValueType valueType, BallMove* move)
{
  if (!myHashMap[key % TWO_POW_20]->IsInitialized()) {
    ++myInserts;
    myHashMap[key % TWO_POW_20]->Init(depth, value, valueType, key, move);
  }
  // strategy to substitute: if the node in the hashmap is deeper in the tree
  // than the new one, it is substituted, is it at the same depth it is also
  // substituted since the most recent node is often the better one
  else if (myHashMap[key % TWO_POW_20]->GetDepth() <= depth) {
    myHashMap[key % TWO_POW_20]->Init(depth, value, valueType, key, move);
  }
}

inline HashMapEntry* ZobristHashMap::Check(ULONG64 key)
{
  HashMapEntry* entry = myHashMap[key % TWO_POW_20];
  if (entry->IsInitialized() && entry->GetLock() == key) {
    // match
    ++myReUseEntries;
    return entry;
  }
  return 0;
}
