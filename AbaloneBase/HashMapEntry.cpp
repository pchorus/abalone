#include "AbaloneBase.h"

#include "HashMapEntry.h"

HashMapEntry::HashMapEntry(ULONG64 lock)
: myTreeDepth(0)
, myValue(0)
, myValueType(UNKOWN)
, myLock(lock)
, myMove()
{
}

HashMapEntry::HashMapEntry(byte treeDepth, int value, ValueType valueType, ULONG64 lock, BallMove* move)
: myTreeDepth(treeDepth)
, myValue(value)
, myValueType(valueType)
, myLock(lock)
, myMove(*move)
{
}

HashMapEntry::~HashMapEntry()
{
}
