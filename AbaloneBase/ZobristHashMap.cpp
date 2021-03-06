#include "AbaloneBase.h"

#include "ZobristHashMap.h"
#include "mtrand.h"
#include "Output.h"
#include "HashMapEntry.h"
#include "GameManager.h"
#include "BallMove.h"

ZobristHashMap::ZobristHashMap()
: myInserts(0)
, myReUseEntries(0)
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
      for (int colour = 0; colour < 2; ++colour) {
        upperBits = irand();
        upperBits = upperBits << 32;
        lowerBits = irand();
        upperBits += lowerBits;

        myHashKeys[x][y][colour] = upperBits;
      }
    }
  }

  upperBits = irand();
  upperBits = upperBits << 32;
  lowerBits = irand();
  upperBits += lowerBits;
  myHashKeyPlayer1IsNext = upperBits;

  upperBits = irand();
  upperBits = upperBits << 32;
  lowerBits = irand();
  upperBits += lowerBits;
  myHashKeyPlayer2IsNext = upperBits;
}

ZobristHashMap::~ZobristHashMap()
{
  for (int i = 0; i < TWO_POW_20; ++i) {
    delete myHashMap[i];
  }
}

ULONG64 ZobristHashMap::CalcHashKey(const GameBoard* gameBoard) const
{
  BoardField* field = 0;
  ULONG64 ret = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field = gameBoard->GetBoardField(x, y);

      if (field && field->GetBall() != BoardField::NO_BALL) {
        ret ^= myHashKeys[x][y][field->GetBall()];
      }
    }
  }

  // initial next player is player1
  ret ^= myHashKeyPlayer1IsNext;

  return ret;
}

void ZobristHashMap::RecalcHashKey(ULONG64& currentHash, BallMove* move, GameManager* gameManager)
{
  BoardField* lastBall = 0;
  BoardField* ball1 = 0;
  BoardField* ball2 = 0;
  BoardField* ball3 = 0;
  BoardField* field = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;
  BoardField::Ball ownBall = BoardField::NO_BALL;

  Direction direction = move->GetDirection();
  move->GetBalls(ball1, ball2, ball3);

  if (ball3) {
    lastBall = ball3;
  }
  else if (ball2) {
    lastBall = ball2;
  }
  else {
    lastBall = ball1;
  }

  if (ball1->GetBall() != BoardField::NO_BALL) {
    // for DoMove the ownBall is one of the ballfields
    ownBall = ball1->GetBall();
  }
  else {
    // for UndoMove the ownBall is not one of the ballfields but one of the fields where it was moved to
    ownBall = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection())->GetBall();
  }

  // opponent's balls
  if (move->GetOpponentBalls() > 0) {
    if (ownBall == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }
    else {
      opponentBall = BoardField::BLACK_BALL;
    }

    BoardField* opponentField1 = 0;
    BoardField* opponentField2 = 0;
    BoardField* opponentField3 = 0;
    BoardField* lastOppBall = 0;
    gameManager->GetOpponentFields(direction, ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);

    if (move->GetOpponentBalls() == 2) {
      lastOppBall = opponentField2;
    }
    else if (move->GetOpponentBalls() == 1) {
      lastOppBall = opponentField1;
    }

    field = gameManager->GetNextFieldInDirection(lastOppBall->GetFieldCoordinates(), direction);
    if (field) {
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
    }
  }

  BallAxis pushAxis = NO_VALID_AXIS;
  switch (direction) {
  case UPLEFT:
  case DOWNRIGHT:
    pushAxis = UPPERLEFT_TO_DOWNRIGHT;
    break;
  case UPRIGHT:
  case DOWNLEFT:
    pushAxis = DOWNLEFT_TO_UPPERRIGHT;
    break;
  case LEFT:
  case RIGHT:
    pushAxis = HORIZONTAL;
    break;
  }

  // own balls
  if (gameManager->GetAxisOfBalls(ball1, ball2) == pushAxis) {
    switch (direction) {
    case UPLEFT:
    case LEFT:
    case DOWNLEFT:
      field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
      if (opponentBall != BoardField::NO_BALL)
        currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ownBall);
      currentHash ^= GetHashKey(lastBall->GetFieldCoordinates(), ownBall);
      break;
    case UPRIGHT:
    case RIGHT:
    case DOWNRIGHT:
      field = gameManager->GetNextFieldInDirection(lastBall->GetFieldCoordinates(), direction);
      if (opponentBall != BoardField::NO_BALL)
        currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ownBall);
      currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), ownBall);
      break;
    }
  }
  else {
    // sidemove
    field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), direction);
    currentHash ^= GetHashKey(field->GetFieldCoordinates(), ownBall);
    currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), ownBall);

    if (ball2) {
      field = gameManager->GetNextFieldInDirection(ball2->GetFieldCoordinates(), direction);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ownBall);
      currentHash ^= GetHashKey(ball2->GetFieldCoordinates(), ownBall);
    }

    if (ball3) {
      field = gameManager->GetNextFieldInDirection(ball3->GetFieldCoordinates(), direction);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ownBall);
      currentHash ^= GetHashKey(ball3->GetFieldCoordinates(), ownBall);
    }
  }

  // the player to move next changes
  currentHash ^= myHashKeyPlayer1IsNext;
  currentHash ^= myHashKeyPlayer2IsNext;
}

void ZobristHashMap::UnInit()
{
  for (int i = 0; i < TWO_POW_20; ++i) {
    myHashMap[i]->SetInitialized(false);
  }
  myInserts = 0;
  myReUseEntries = 0;
}
