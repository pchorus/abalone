#include "AbaloneBase.h"

#include "ComputerPlayerAlphaBeta.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "HashMapEntry.h"

static const unsigned int TIME_CHECK_INTERVAL = 200;

ComputerPlayerAlphaBeta::ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball, Player::PlayerType type)
: ComputerPlayer(gameManager, name, ball, type)
, mySimGameManager(new GameManager)
, myMaxPlayer(0)
, myMinPlayer(0)
, myTreeDepth(DEFAULT_TREE_DEPTH)
, myUsedEvaluation(1)
, myBallMoves(0)
, myBallMovesSize(0)
, myNodeCounter(0)
, myKeepInvestigating(true)
, myUseTranspositionTable(false)
, myHashMap()
{
  SetTreeDepth(myTreeDepth);
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);

  if (ball == BoardField::BLACK_BALL) {
    myMaxPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer1());
    myMinPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer2());
  }
  else {
    myMinPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer1());
    myMaxPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer2());
  }
}

ComputerPlayerAlphaBeta::~ComputerPlayerAlphaBeta()
{
  if (mySimGameManager) {
    delete mySimGameManager;
    mySimGameManager = 0;
  }

  DeleteBallMoves();
}

void ComputerPlayerAlphaBeta::SetTreeDepth(int treeDepth)
{
  DeleteBallMoves();

  myTreeDepth = treeDepth;
  myBallMovesSize = new int[myTreeDepth];
  myBallMoves = new BallMove**[myTreeDepth];

  for (int i = 0; i < myTreeDepth; ++i) {
    myBallMoves[i] = new BallMove* [BALL_MOVES_ARRAY_SIZE];
    for (int j = 0; j < BALL_MOVES_ARRAY_SIZE; ++j) {
      myBallMoves[i][j] = new BallMove;
    }
    myBallMovesSize[i] = 0;
  }
}

void ComputerPlayerAlphaBeta::DeleteBallMoves()
{
  if (myBallMovesSize) {
    delete[] myBallMovesSize;

    for (int i = 0; i < myTreeDepth; ++i) {
      for (int j = 0; j < BALL_MOVES_ARRAY_SIZE; ++j) {
        delete myBallMoves[i][j];
      }
      delete[] myBallMoves[i];
    }
    delete[] myBallMoves;
  }
}

BallMove ComputerPlayerAlphaBeta::CalculateNextMove()
{
  BallMove retMove;
  myKeepInvestigating = true;

  myNodeCounter = 1;

  int alpha = INT_MIN;
  int beta = INT_MAX;
  int value = 0;

  myHashMap.UnInit();

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  myBallMovesSize[myTreeDepth-1] = 0;

  myCurrentHashKey = myHashMap.CalcHashKey(mySimGameManager->GetGameBoard());

  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);

  for (int i = 0; i < myBallMovesSize[myTreeDepth-1]; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[myTreeDepth-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[myTreeDepth-1][i]);
    if (myUseTranspositionTable) {
      value = MinTT(myTreeDepth-1, alpha, beta);
    }
    else {
      value = Min(myTreeDepth-1, alpha, beta);
    }

    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[myTreeDepth-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[myTreeDepth-1][i]);
    if (value >= beta) {
      break;
    }
    if (value > alpha) {
      retMove = *myBallMoves[myTreeDepth-1][i];
      alpha = value;
    }
  }

  // retMove contains the ballfields from the simGameManager,
  // to do the move on the real game board, we have to give it the boardfields from the
  // real game manager
  BoardField* ball1 = 0;
  BoardField* ball2 = 0;
  BoardField* ball3 = 0;
  retMove.GetBalls(ball1, ball2, ball3);
  ball1 = GetGameManager()->GetGameBoard()->GetBoardField(ball1->GetFieldCoordinates());
  if (ball2)
    ball2 = GetGameManager()->GetGameBoard()->GetBoardField(ball2->GetFieldCoordinates());
  if (ball3)
    ball3 = GetGameManager()->GetGameBoard()->GetBoardField(ball3->GetFieldCoordinates());
  retMove.SetBalls(ball1, ball2, ball3);

  return retMove;
}

int ComputerPlayerAlphaBeta::Max(int depth, int alpha, int beta)
{
  ++myNodeCounter;

  if (depth == 0)
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

  if (myNodeCounter % TIME_CHECK_INTERVAL == 0) {
    CheckTime();
  }

  int value = 0;

  myBallMovesSize[depth-1] = 0;

  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1] && myKeepInvestigating; ++i) {
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = Min(depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);
    if (value >= beta) {
      return beta;
    }
    if (value > alpha) {
      alpha = value;
    }
  }
  return alpha;
}

int ComputerPlayerAlphaBeta::Min(int depth, int alpha, int beta)
{
  ++myNodeCounter;

  if (depth == 0)
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

  if (myNodeCounter % TIME_CHECK_INTERVAL == 0) {
    CheckTime();
  }

  int value = 0;

  myBallMovesSize[depth-1] = 0;
  
  mySimGameManager->AddPossibleMovesThreeBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1] && myKeepInvestigating; ++i) {
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = Max(depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);
    if (value <= alpha) {
      return alpha;
    }
    if (value < beta) {
      beta = value;
    }
  }
  return beta;
}

int ComputerPlayerAlphaBeta::MaxTT(int depth, int alpha, int beta)
{
  ++myNodeCounter;

  // check if we can use a transposition
  HashMapEntry* entry = myHashMap.Check(myCurrentHashKey);
  if (entry && entry->GetDepth() >= depth) {
    HashMapEntry::ValueType type = entry->GetValueType();
    int val = entry->GetValue();

    if(type == HashMapEntry::EXACT)
      return val;
    if(type == HashMapEntry::LOWER_BOUND && val > alpha)
      alpha = val;
    else if(type == HashMapEntry::UPPER_BOUND && val < beta)
      beta = val;
    if(alpha >= beta)
      return val;
  }

  if (depth == 0) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  if (myNodeCounter % TIME_CHECK_INTERVAL == 0) {
    CheckTime();
  }

  int value = 0;
  myBallMovesSize[depth-1] = 0;

  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1] && myKeepInvestigating; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[depth-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = MinTT(depth-1, alpha, beta);
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[depth-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);
    if (value >= beta) {
      myHashMap.Insert(myCurrentHashKey, (byte)depth, value, HashMapEntry::LOWER_BOUND);
      return beta;
    }
    if (value > alpha) {
      alpha = value;
    }
  }

  myHashMap.Insert(myCurrentHashKey, (byte)depth, alpha, HashMapEntry::EXACT);
  return alpha;
}

int ComputerPlayerAlphaBeta::MinTT(int depth, int alpha, int beta)
{
  ++myNodeCounter;

  // check if we can use a transposition
  HashMapEntry* entry = myHashMap.Check(myCurrentHashKey);
  if (entry && entry->GetDepth() >= depth) {
    HashMapEntry::ValueType type = entry->GetValueType();
    int val = entry->GetValue();

    if(type == HashMapEntry::EXACT)
      return val;
    if(type == HashMapEntry::LOWER_BOUND && val > alpha)
      alpha = val;
    else if(type == HashMapEntry::UPPER_BOUND && val < beta)
      beta = val;
    if(alpha >= beta)
      return val;
  }

  if (depth == 0) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  if (myNodeCounter % TIME_CHECK_INTERVAL == 0) {
    CheckTime();
  }

  int value = 0;
  myBallMovesSize[depth-1] = 0;

  mySimGameManager->AddPossibleMovesThreeBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1] && myKeepInvestigating; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[depth-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = MaxTT(depth-1, alpha, beta);
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[depth-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);

    if (value <= alpha) {
      myHashMap.Insert(myCurrentHashKey, (byte)depth, value, HashMapEntry::UPPER_BOUND);
      return alpha;
    }
    if (value < beta) {
      beta = value;
    }
  }

  myHashMap.Insert(myCurrentHashKey, (byte)depth, beta, HashMapEntry::EXACT);
  return beta;
}

void ComputerPlayerAlphaBeta::CheckTime()
{
}
