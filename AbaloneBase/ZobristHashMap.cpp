#include "AbaloneBase.h"

#include "ZobristHashMap.h"
#include "mtrand.h"
#include "Output.h"
#include "HashMapEntry.h"
#include "GameManager.h"

ZobristHashMap::ZobristHashMap()
{
  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  MTRand_int32 irand(init, length); // 32-bit int generator 

  ULONG64 upperBits = 0;
  ULONG64 lowerBits = 0;

  for (int i = 0; i < TWO_POW_20; ++i) {
    myHashMap[i] = 0;
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

ULONG64 ZobristHashMap::CalcHashKey(const GameBoard* gameBoard) const
{
  BoardField* field = 0;
  ULONG64 ret = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field = gameBoard->GetBoardField(x, y);

      if (field) {
        ret ^= myHashKeys[x][y][field->GetBall()];
      }
    }
  }

  return ret;
}

void ZobristHashMap::RecalcHashKeyDoMove(ULONG64& currentHash, BallMove* move, GameManager* gameManager)
{
  BoardField* lastBall = 0;
  BoardField* ball1 = 0;
  BoardField* ball2 = 0;
  BoardField* ball3 = 0;
  BoardField* field = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

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

  if (move->GetOpponentBalls() != 0) {
    if (ball1->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }
    else {
      opponentBall = BoardField::BLACK_BALL;
    }
    
  }

  // opponent's balls
  if (move->GetOpponentBalls() > 0) {
    BoardField* opponentField1 = 0;
    BoardField* opponentField2 = 0;
    BoardField* opponentField3 = 0;
    BoardField* lastOppBall = 0;
    gameManager->GetOpponentFields(move->GetDirection(), ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);

    if (move->GetOpponentBalls() == 2) {
      lastOppBall = opponentField2;
    }
    else if (move->GetOpponentBalls() == 1) {
      lastOppBall = opponentField1;
    }

    field = gameManager->GetNextFieldInDirection(lastOppBall->GetFieldCoordinates(), move->GetDirection());
    if (field) {
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
    }
  }

  BallAxis pushAxis = NO_VALID_AXIS;
  switch (move->GetDirection()) {
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
    switch (move->GetDirection()) {
    case UPLEFT:
    case LEFT:
    case DOWNLEFT:
      field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ball1->GetBall());
      currentHash ^= GetHashKey(lastBall->GetFieldCoordinates(), lastBall->GetBall());
      currentHash ^= GetHashKey(lastBall->GetFieldCoordinates(), BoardField::NO_BALL);
      break;
    case UPRIGHT:
    case RIGHT:
    case DOWNRIGHT:
      field = gameManager->GetNextFieldInDirection(lastBall->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ball1->GetBall());
      currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), lastBall->GetBall());
      currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), BoardField::NO_BALL);
      break;
    }
  }
  else {
    // sidemove
    field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection());
    currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
    currentHash ^= GetHashKey(field->GetFieldCoordinates(), ball1->GetBall());
    currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), ball1->GetBall());
    currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), BoardField::NO_BALL);

    if (ball2) {
      field = gameManager->GetNextFieldInDirection(ball2->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ball2->GetBall());
      currentHash ^= GetHashKey(ball2->GetFieldCoordinates(), ball2->GetBall());
      currentHash ^= GetHashKey(ball2->GetFieldCoordinates(), BoardField::NO_BALL);
    }

    if (ball3) {
      field = gameManager->GetNextFieldInDirection(ball3->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), ball3->GetBall());
      currentHash ^= GetHashKey(ball3->GetFieldCoordinates(), ball3->GetBall());
      currentHash ^= GetHashKey(ball3->GetFieldCoordinates(), BoardField::NO_BALL);
    }
  }
}

void ZobristHashMap::RecalcHashKeyUndoMove(ULONG64& currentHash, BallMove* move, GameManager* gameManager)
{
  BoardField* lastBall = 0;
  BoardField* ball1 = 0;
  BoardField* ball2 = 0;
  BoardField* ball3 = 0;
  BoardField* field = 0;

  BoardField::Ball opponentBall = BoardField::NO_BALL;

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

  if (move->GetOpponentBalls() != 0) {
    if (gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection())->GetBall() == BoardField::BLACK_BALL) {
      opponentBall = BoardField::WHITE_BALL;
    }
    else {
      opponentBall = BoardField::BLACK_BALL;
    }
  }

  // opponent's balls
  if (move->GetOpponentBalls() > 0) {
    BoardField* opponentField1 = 0;
    BoardField* opponentField2 = 0;
    BoardField* opponentField3 = 0;
    BoardField* lastOppBall = 0;
    gameManager->GetOpponentFields(move->GetDirection(), ball1, ball2, ball3, opponentField1, opponentField2, opponentField3);

    if (move->GetOpponentBalls() == 2) {
      lastOppBall = opponentField2;
    }
    else if (move->GetOpponentBalls() == 1) {
      lastOppBall = opponentField1;
    }

    field = gameManager->GetNextFieldInDirection(lastOppBall->GetFieldCoordinates(), move->GetDirection());
    if (field) {
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
    }
  }

  // own balls
  BallAxis pushAxis = NO_VALID_AXIS;
  switch (move->GetDirection()) {
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


  if (gameManager->GetAxisOfBalls(ball1, ball2) == pushAxis) {
    switch (move->GetDirection()) {
    case UPLEFT:
    case LEFT:
    case DOWNLEFT:
      field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), field->GetBall());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);

      currentHash ^= GetHashKey(lastBall->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(lastBall->GetFieldCoordinates(), field->GetBall());
      break;
    case UPRIGHT:
    case RIGHT:
    case DOWNRIGHT:
      field = gameManager->GetNextFieldInDirection(lastBall->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), field->GetBall());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), opponentBall);

      currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), field->GetBall());

      break;
    }
  }
  else {
    // sidemove
    field = gameManager->GetNextFieldInDirection(ball1->GetFieldCoordinates(), move->GetDirection());
    currentHash ^= GetHashKey(field->GetFieldCoordinates(), field->GetBall());
    currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
    currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), BoardField::NO_BALL);
    currentHash ^= GetHashKey(ball1->GetFieldCoordinates(), field->GetBall());

    if (ball2) {
      field = gameManager->GetNextFieldInDirection(ball2->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), field->GetBall());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(ball2->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(ball2->GetFieldCoordinates(), field->GetBall());
    }

    if (ball3) {
      field = gameManager->GetNextFieldInDirection(ball3->GetFieldCoordinates(), move->GetDirection());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), field->GetBall());
      currentHash ^= GetHashKey(field->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(ball3->GetFieldCoordinates(), BoardField::NO_BALL);
      currentHash ^= GetHashKey(ball3->GetFieldCoordinates(), field->GetBall());
    }
  }
}
