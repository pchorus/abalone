#pragma once

#include "BallMove.h"

class HashMapEntry {
public:
  enum ValueType { UNKOWN, EXACT, UPPER_BOUND, LOWER_BOUND };

  HashMapEntry();
  ~HashMapEntry();

private:
  byte myTreeDepth;
  int myValue;
  ValueType myValueType;
  ULONG64 myLock;
  BallMove myMove;
};
