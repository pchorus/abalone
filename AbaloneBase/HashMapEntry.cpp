#include "AbaloneBase.h"

#include "HashMapEntry.h"

HashMapEntry::HashMapEntry()
: myTreeDepth(0)
, myValue(0)
, myValueType(UNKOWN)
, myLock(0)
, myMove()
{
}

HashMapEntry::~HashMapEntry()
{
}
