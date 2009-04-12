#pragma once

#include "BallMove.h"

class ABALONE_BASE_DLLINTERFACE HashMapEntry {
public:
  enum ValueType { UNKOWN = 0, EXACT, UPPER_BOUND, LOWER_BOUND };

  HashMapEntry(ULONG64 lock);
  HashMapEntry(byte treeDepth, int value, ValueType valueType, ULONG64 lock, BallMove* move);
  ~HashMapEntry();

  byte GetDepth() const;
  int GetValue() const;
  ValueType GetValueType() const;
  ULONG64 GetLock() const;

private:
  byte myTreeDepth;
  int myValue;
  ValueType myValueType;
  ULONG64 myLock;
  BallMove myMove;
};

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
