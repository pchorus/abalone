#include "AbaloneBase.h"

#include "HashMapEntry.h"

HashMapEntry::HashMapEntry()
: myIsInitialized(false)
, myTreeDepth(0)
, myValue(0)
, myValueType(UNKOWN)
, myLock(0)
{
}

HashMapEntry::HashMapEntry(byte treeDepth, int value, ValueType valueType, ULONG64 lock)
: myIsInitialized(true)
, myTreeDepth(treeDepth)
, myValue(value)
, myValueType(valueType)
, myLock(lock)
{
}

HashMapEntry::~HashMapEntry()
{
}
