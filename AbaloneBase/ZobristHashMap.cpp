#include "AbaloneBase.h"

#include "ZobristHashMap.h"
#include "mtrand.h"
#include "Output.h"
#include "HashMapEntry.h"

ZobristHashMap::ZobristHashMap()
{
  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  MTRand_int32 irand(init, length); // 32-bit int generator 

  ULONG64 upperBits = 0;
  ULONG64 lowerBits = 0;

  for (int i = 0; i < TWO_POW_20; ++i) {
    myHashMap[i] = new HashMapEntry;
  }

  // initialization
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      for (int colour = 0; colour < 3; ++colour) {
        upperBits = irand();
        upperBits = upperBits << 32;
        lowerBits = irand();
        upperBits += lowerBits;

        myHashKeys[x][y][colour] = upperBits;
      }
    }
  }
}

ZobristHashMap::~ZobristHashMap()
{
}
