#include "AbaloneBase.h"

#include "ComputerPlayerAlphaBeta.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "HashMapEntry.h"
#include "Output.h"

static const unsigned int AB_TIME_CHECK_INTERVAL_NODES = 200;

ComputerPlayerAlphaBeta::ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball, Player::PlayerType type)
: ComputerPlayer(gameManager, name, ball, type)
, mySimGameManager(new GameManager)
, myMaxPlayer(0)
, myMinPlayer(0)
, myUsedEvaluation(1)
, myNodeCounter(0)
, myKeepInvestigating(true)
, myUseTranspositionTable(false)
, myUseQuiescenceSearch(false)
, myHashMap()
, myStartQSCounter(0)
, myLeafNodesQuiescent(0)
{
  myBallMovesSize[NORMAL] = 0;
  myBallMovesSize[QS] = 0;

  myTreeDepth[NORMAL] = DEFAULT_TREE_DEPTH;
  myTreeDepth[QS] = DEFAULT_TREE_DEPTH_QS;

  SetTreeDepth(myTreeDepth[NORMAL]);
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);

  // initialize array for quiescence search
  myBallMovesSize[QS] = new int[myTreeDepth[QS]];
  myBallMoves[QS] = new BallMove**[myTreeDepth[QS]];

  for (int i = 0; i < myTreeDepth[QS]; ++i) {
    myBallMoves[QS][i] = new BallMove* [BALL_MOVES_ARRAY_SIZE_QS];
    for (int j = 0; j < BALL_MOVES_ARRAY_SIZE_QS; ++j) {
      myBallMoves[QS][i][j] = new BallMove;
    }
    myBallMovesSize[QS][i] = 0;
  }

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
  DeleteBallMovesQS();
}

void ComputerPlayerAlphaBeta::SetTreeDepth(int treeDepth)
{
  DeleteBallMoves();

  myTreeDepth[NORMAL] = treeDepth;
  myBallMovesSize[NORMAL] = new int[myTreeDepth[NORMAL]];
  myBallMoves[NORMAL] = new BallMove**[myTreeDepth[NORMAL]];

  for (int i = 0; i < myTreeDepth[NORMAL]; ++i) {
    myBallMoves[NORMAL][i] = new BallMove* [BALL_MOVES_ARRAY_SIZE];
    for (int j = 0; j < BALL_MOVES_ARRAY_SIZE; ++j) {
      myBallMoves[NORMAL][i][j] = new BallMove;
    }
    myBallMovesSize[NORMAL][i] = 0;
  }
}

void ComputerPlayerAlphaBeta::DeleteBallMoves()
{
  if (myBallMovesSize[NORMAL]) {
    delete[] myBallMovesSize[NORMAL];

    for (int i = 0; i < myTreeDepth[NORMAL]; ++i) {
      for (int j = 0; j < BALL_MOVES_ARRAY_SIZE; ++j) {
        delete myBallMoves[NORMAL][i][j];
      }
      delete[] myBallMoves[NORMAL][i];
    }
    delete[] myBallMoves[NORMAL];
  }
}

void ComputerPlayerAlphaBeta::DeleteBallMovesQS()
{
  if (myBallMovesSize[QS]) {
    delete[] myBallMovesSize[QS];

    for (int i = 0; i < myTreeDepth[QS]; ++i) {
      for (int j = 0; j < BALL_MOVES_ARRAY_SIZE_QS; ++j) {
        delete myBallMoves[QS][i][j];
      }
      delete[] myBallMoves[QS][i];
    }
    delete[] myBallMoves[QS];
  }
}

BallMove ComputerPlayerAlphaBeta::CalculateNextMove()
{
  DWORD start = GetTickCount();
  BallMove retMove;
  myKeepInvestigating = true;

  myNodeCounter = 1;

  myStartQSCounter = 0;
  myLeafNodesQuiescent = 0;

  int alpha = INT_MIN;
  int beta = INT_MAX;
  int value = 0;

  myHashMap.UnInit();

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1] = 0;

  myCurrentHashKey = myHashMap.CalcHashKey(mySimGameManager->GetGameBoard());

  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);
  mySimGameManager->OrderMoves(0, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);

  for (int i = 0; i < myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i]);
    if (myUseTranspositionTable) {
      value = MinTT(NORMAL, myTreeDepth[NORMAL]-1, alpha, beta);
    }
    else {
      value = Min(NORMAL, myTreeDepth[NORMAL]-1, alpha, beta);
    }

    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i]);
    if (value >= beta) {
      break;
    }
    if (value > alpha) {
      retMove = *myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i];
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

  CString msg;
  msg.Format("AB\nMove: %s\nTime: %d\nNodes: %d\nValueBestMove: %d\nInserts: %d\nReuses: %d\nQS starts: %d\nQuiescent Leaves: %d\n\n", retMove.ToString(), GetTickCount()-start, myNodeCounter, value, myHashMap.myInserts, myHashMap.myReUseEntries, myStartQSCounter, myLeafNodesQuiescent);
  Output::Message(msg, false, true);

  return retMove;
}

int ComputerPlayerAlphaBeta::Max(NormalOrQuiescence noq, int depth, int alpha, int beta)
{
  ++myNodeCounter;

  int value = 0;

  if (noq == QS && mySimGameManager->IsQuiescencePosition(myMaxPlayer)) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }
  else if (depth == 0 || mySimGameManager->IsTerminalPosition()) {
    if (myUseQuiescenceSearch && noq == NORMAL && !mySimGameManager->IsQuiescencePosition(myMaxPlayer)) {
      ++myStartQSCounter;
      value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

      if (value >= beta) {
        return beta;
      }

      int qsValue = Max(QS, myTreeDepth[QS]-1, alpha, beta);

      if (value > qsValue) {
        return value;
      }
      else if (qsValue >= beta) {
        return beta;
      }
      return qsValue;
    }
    if (noq == NORMAL)
      ++myLeafNodesQuiescent;
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  if (myNodeCounter % AB_TIME_CHECK_INTERVAL_NODES == 0) {
    CheckTime();
  }

  myBallMovesSize[noq][depth-1] = 0;

  if (noq == NORMAL) {
    mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->OrderMoves(0, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
  }
  else {
    mySimGameManager->AddCapturingMoves(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
    if (value >= beta) {
      return beta;
    }
    else if (value > alpha) {
      alpha = value;
    }
  }

  for (int i = 0; i < myBallMovesSize[noq][depth-1] && myKeepInvestigating; ++i) {
    mySimGameManager->DoMove(myBallMoves[noq][depth-1][i]);
    value = Min(noq, depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[noq][depth-1][i]);
    if (value >= beta) {
      return beta;
    }
    if (value > alpha) {
      alpha = value;
    }
  }
  return alpha;
}

int ComputerPlayerAlphaBeta::Min(NormalOrQuiescence noq, int depth, int alpha, int beta)
{
  ++myNodeCounter;

  int value = 0;

  if (noq == QS && mySimGameManager->IsQuiescencePosition(myMinPlayer)) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }
  else if (depth == 0 || mySimGameManager->IsTerminalPosition()) {
    if (myUseQuiescenceSearch && noq == NORMAL && !mySimGameManager->IsQuiescencePosition(myMinPlayer)) {
      ++myStartQSCounter;
      value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

      if (value <= alpha) {
        return alpha;
      }

      int qsValue = Min(QS, myTreeDepth[QS]-1, alpha, beta);
      if (value < qsValue) {
        return value;
      }
      else if (qsValue <= alpha) {
        return alpha;
      }
      return qsValue;
    }
    if (noq == NORMAL)
      ++myLeafNodesQuiescent;
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  if (myNodeCounter % AB_TIME_CHECK_INTERVAL_NODES == 0) {
    CheckTime();
  }

  myBallMovesSize[noq][depth-1] = 0;

  if (noq == NORMAL) {
    mySimGameManager->AddPossibleMovesThreeBalls(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->OrderMoves(0, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
  }
  else {
    mySimGameManager->AddCapturingMoves(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
    if (value <= alpha) {
      return alpha;
    }
    else if (value < beta) {
      beta = value;
    }
  }

  for (int i = 0; i < myBallMovesSize[noq][depth-1] && myKeepInvestigating; ++i) {
    mySimGameManager->DoMove(myBallMoves[noq][depth-1][i]);
    value = Max(noq, depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[noq][depth-1][i]);
    if (value <= alpha) {
      return alpha;
    }
    if (value < beta) {
      beta = value;
    }
  }
  return beta;
}

int ComputerPlayerAlphaBeta::MaxTT(NormalOrQuiescence noq, int depth, int alpha, int beta)
{
  ++myNodeCounter;
  BallMove* bestMove(0);
  int value = 0;
  int startOrderAtIdx = 0;

  if (noq == QS && mySimGameManager->IsQuiescencePosition(myMaxPlayer)) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }
  else if (depth == 0 || mySimGameManager->IsTerminalPosition()) {
    if (myUseQuiescenceSearch && noq == NORMAL && !mySimGameManager->IsQuiescencePosition(myMaxPlayer)) {
      ++myStartQSCounter;
      value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

      if (value >= beta) {
        return beta;
      }

      int qsValue = MaxTT(QS, myTreeDepth[QS]-1, alpha, beta);

      if (value > qsValue) {
        return value;
      }
      else if (qsValue >= beta) {
        return beta;
      }
      return qsValue;
    }
    if (noq == NORMAL)
      ++myLeafNodesQuiescent;
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  // check if we can use a transposition
  HashMapEntry* entry = myHashMap.Check(myCurrentHashKey);
  if (entry) {
    *(myBallMoves[noq][depth-1][0]) = entry->GetMove();
    myBallMovesSize[noq][depth-1] = 1;
    startOrderAtIdx = 1;

    if (entry->GetDepth() >= depth) {
      // consider the move in the hash map first
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
  }

  if (myNodeCounter % AB_TIME_CHECK_INTERVAL_NODES == 0) {
    CheckTime();
  }

  myBallMovesSize[noq][depth-1] = 0;

  if (noq == NORMAL) {
    mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->OrderMoves(startOrderAtIdx, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
  }
  else {
    mySimGameManager->AddCapturingMoves(myMaxPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
    if (value >= beta) {
      return beta;
    }
    else if (value > alpha) {
      alpha = value;
    }
  }

  for (int i = 0; i < myBallMovesSize[noq][depth-1] && myKeepInvestigating; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[noq][depth-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[noq][depth-1][i]);
    value = MinTT(noq, depth-1, alpha, beta);
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[noq][depth-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[noq][depth-1][i]);

    if (value >= beta) {
      myHashMap.Insert(myCurrentHashKey, (byte)depth, value, HashMapEntry::LOWER_BOUND, myBallMoves[noq][depth-1][i]);
      return beta;
    }
    if (value > alpha) {
      bestMove = myBallMoves[noq][depth-1][i];
      alpha = value;
    }
  }

  if (noq == NORMAL)
    myHashMap.Insert(myCurrentHashKey, (byte)depth, alpha, HashMapEntry::EXACT, bestMove);

  return alpha;
}

int ComputerPlayerAlphaBeta::MinTT(NormalOrQuiescence noq, int depth, int alpha, int beta)
{
  ++myNodeCounter;
  BallMove* bestMove(0);
  int value = 0;
  int startOrderAtIdx = 0;

  if (noq == QS && mySimGameManager->IsQuiescencePosition(myMinPlayer)) {
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }
  else if (depth == 0 || mySimGameManager->IsTerminalPosition()) {
    if (myUseQuiescenceSearch && noq == NORMAL && !mySimGameManager->IsQuiescencePosition(myMinPlayer)) {
      ++myStartQSCounter;
      value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

      if (value <= alpha) {
        return alpha;
      }

      int qsValue = MinTT(QS, myTreeDepth[QS]-1, alpha, beta);
      if (value < qsValue) {
        return value;
      }
      else if (qsValue <= alpha) {
        return alpha;
      }
      return qsValue;
    }
    if (noq == NORMAL)
      ++myLeafNodesQuiescent;
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
  }

  // check if we can use a transposition
  HashMapEntry* entry = myHashMap.Check(myCurrentHashKey);
  if (entry) {
    // consider the move in the hash map first
    *(myBallMoves[noq][depth-1][0]) = entry->GetMove();
    myBallMovesSize[noq][depth-1] = 1;
    startOrderAtIdx = 1;

    if (entry->GetDepth() >= depth) {
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
  }

  if (myNodeCounter % AB_TIME_CHECK_INTERVAL_NODES == 0) {
    CheckTime();
  }

  myBallMovesSize[noq][depth-1] = 0;

  if (noq == NORMAL) {
    mySimGameManager->AddPossibleMovesThreeBalls(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    mySimGameManager->OrderMoves(startOrderAtIdx, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
  }
  else {
    mySimGameManager->AddCapturingMoves(myMinPlayer, myBallMoves[noq][depth-1], myBallMovesSize[noq][depth-1]);
    value = mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);
    if (value <= alpha) {
      return alpha;
    }
    else if (value < beta) {
      beta = value;
    }
  }

  for (int i = 0; i < myBallMovesSize[noq][depth-1] && myKeepInvestigating; ++i) {
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[noq][depth-1][i], mySimGameManager);
    mySimGameManager->DoMove(myBallMoves[noq][depth-1][i]);
    value = MaxTT(noq, depth-1, alpha, beta);
    myHashMap.RecalcHashKey(myCurrentHashKey, myBallMoves[noq][depth-1][i], mySimGameManager);
    mySimGameManager->UndoMove(myBallMoves[noq][depth-1][i]);

    if (value <= alpha) {
      myHashMap.Insert(myCurrentHashKey, (byte)depth, value, HashMapEntry::UPPER_BOUND, myBallMoves[noq][depth-1][i]);
      return alpha;
    }
    if (value < beta) {
      bestMove = myBallMoves[noq][depth-1][i];
      beta = value;
    }
  }

  if (noq == NORMAL)
    myHashMap.Insert(myCurrentHashKey, (byte)depth, beta, HashMapEntry::EXACT, bestMove);

  return beta;
}

void ComputerPlayerAlphaBeta::CheckTime()
{
}
