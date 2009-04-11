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

HashMapEntry::~HashMapEntry()
{
}
