#pragma once

#include "BallMove.h"

class ABALONE_BASE_DLLINTERFACE HashMapEntry {
public:
  enum ValueType { UNKOWN = 0, EXACT, UPPER_BOUND, LOWER_BOUND };

  HashMapEntry();
  HashMapEntry(byte treeDepth, int value, ValueType valueType, ULONG64 lock);
  HashMapEntry(byte treeDepth, int value, ValueType valueType, ULONG64 lock, BallMove* move);
  ~HashMapEntry();

  void Init(byte treeDepth, int value, ValueType valueType, ULONG64 lock, BallMove* move);

  bool IsInitialized() const;
  void SetInitialized(bool initialized);
  byte GetDepth() const;
  int GetValue() const;
  ValueType GetValueType() const;
  ULONG64 GetLock() const;
  BallMove* GetMove() const;
  bool HasMove() const;

private:
  bool myIsInitialized;
  byte myTreeDepth;
  int myValue;
  ValueType myValueType;
  ULONG64 myLock;
  BallMove* myMove;
  bool myHasMove;
};

inline bool HashMapEntry::IsInitialized() const
{
  return myIsInitialized;
}

inline void HashMapEntry::SetInitialized(bool initialized)
{
  myIsInitialized = initialized;
}

inline byte HashMapEntry::GetDepth() const
{
  return myTreeDepth;
}

inline int HashMapEntry::GetValue() const
{
  return myValue;
}

inline HashMapEntry::ValueType HashMapEntry::GetValueType() const
{
  return myValueType;
}

inline ULONG64 HashMapEntry::GetLock() const
{
  return myLock;
}

inline void HashMapEntry::Init(byte treeDepth, int value, ValueType valueType, ULONG64 lock, BallMove* move)
{
  myIsInitialized = true;
  myTreeDepth = treeDepth;
  myValue = value;
  myValueType = valueType;
  myLock = lock;
  if (move) {
    *myMove = *move;
    myHasMove = true;
  }
  else {
    myHasMove = false;
  }
}

inline BallMove* HashMapEntry::GetMove() const
{
  return myMove;
}

inline bool HashMapEntry::HasMove() const
{
  return myHasMove;
}
