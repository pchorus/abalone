#pragma once

#include "BallMove.h"

class HashMapEntry {
public:
  enum ValueType { UNKOWN, EXACT, UPPER_BOUND, LOWER_BOUND };

  HashMapEntry(ULONG64 lock);
  ~HashMapEntry();

  ULONG64 GetLock() const;

private:
  byte myTreeDepth;
  int myValue;
  ValueType myValueType;
  ULONG64 myLock;
  BallMove myMove;
};

inline ULONG64 HashMapEntry::GetLock() const
{
  return myLock;
}
