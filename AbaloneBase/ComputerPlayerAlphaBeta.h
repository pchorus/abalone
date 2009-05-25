#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerAlphaBeta
//    Implementation of abstract base class ComputerPlayer.
//    Uses the Alpha-Beta algorithm to calculate the next move.
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayer.h"
#include "ZobristHashMap.h"


static const int DEFAULT_TREE_DEPTH = 5;
static const int DEFAULT_TREE_DEPTH_QS = 30;


class ABALONE_BASE_DLLINTERFACE ComputerPlayerAlphaBeta : public ComputerPlayer {
public:

  enum NormalOrQuiescence { NORMAL = 0, QS };
  // constructor / destructor
  ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball, Player::PlayerType type = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA);
  virtual ~ComputerPlayerAlphaBeta();

  virtual BallMove CalculateNextMove();
  virtual void CheckTime();

  int GetTreeDepth() const;
  void SetTreeDepth(int treeDepth);
  int GetUsedEvaluation() const;
  void SetUsedEvaluation(int eval);
  bool GetUseTranspositionTable() const;
  void SetUseTranspositionTable(bool useTT);
  bool GetUseKillerMoves() const;
  void SetUseKillerMoves(bool useKM);
  bool GetUseQuiescenceSearch() const;
  void SetUseQuiescenceSearch(bool useQS);

protected:
  // alpha beta search methods
  int Max(NormalOrQuiescence noq, int depth, int alpha, int beta);
  int Min(NormalOrQuiescence noq, int depth, int alpha, int beta);

  // alpha beta search methods using transposition tables
  int MaxTT(NormalOrQuiescence noq, int depth, int alpha, int beta);
  int MinTT(NormalOrQuiescence noq, int depth, int alpha, int beta);

  void InsertKillerMove(int depth, const BallMove* move);
  void UnInitKillerMoves();

  void CopyCurrentPV(int depth);
  CString GetPrincipalVariation() const;
  GameManager* mySimGameManager;

  ComputerPlayer* myMaxPlayer;
  int myTreeDepth[2];

  // contains normal ballMoves and ballMoves for Quiescence search
  BallMove*** myBallMoves[2];
  int* myBallMovesSize[2];
  BallMove*** myPVMoves;
  unsigned int myNodeCounter;
  bool myKeepInvestigating;
  bool myUseTranspositionTable;
  bool myUseQuiescenceSearch;
  bool myUseKillerMoves;
  ZobristHashMap myHashMap;
  ULONG64 myCurrentHashKey;
  int myStartQSCounter;
  int myLeafNodesQuiescent;

private:
  void DeleteBallMoves();
  void DeleteBallMovesQS();

  ComputerPlayer* myMinPlayer;
  int myUsedEvaluation;
  BallMove*** myKillerMoves;
  int* myKillerMovesSize;
  int* myKillerMovesNextInsertIdx;
};

inline int ComputerPlayerAlphaBeta::GetTreeDepth() const
{
  return myTreeDepth[NORMAL];
}

inline int ComputerPlayerAlphaBeta::GetUsedEvaluation() const
{
  return myUsedEvaluation;
}

inline void ComputerPlayerAlphaBeta::SetUsedEvaluation(int eval)
{
  myUsedEvaluation = eval;
}

inline bool ComputerPlayerAlphaBeta::GetUseTranspositionTable() const
{
  return myUseTranspositionTable;
}

inline void ComputerPlayerAlphaBeta::SetUseTranspositionTable(bool useTT)
{
  myUseTranspositionTable = useTT;
}

inline bool ComputerPlayerAlphaBeta::GetUseKillerMoves() const
{
  return myUseKillerMoves;
}

inline void ComputerPlayerAlphaBeta::SetUseKillerMoves(bool useKM)
{
  myUseKillerMoves = useKM;
}

inline bool ComputerPlayerAlphaBeta::GetUseQuiescenceSearch() const
{
  return myUseQuiescenceSearch;
}

inline void ComputerPlayerAlphaBeta::SetUseQuiescenceSearch(bool useQS)
{
  myUseQuiescenceSearch = useQS;
}

inline void ComputerPlayerAlphaBeta::InsertKillerMove(int depth, const BallMove* move)
{
  for (int i = 0; i < myKillerMovesSize[depth-1]; ++i) {
    if (*myKillerMoves[depth-1][i] == *move)
      return;
  }

  *myKillerMoves[depth-1][myKillerMovesNextInsertIdx[depth-1]] = *move;
  myKillerMovesNextInsertIdx[depth-1] = ++myKillerMovesNextInsertIdx[depth-1] % KILLER_MOVES_ARRAY_SIZE;
  if (myKillerMovesSize[depth-1] < KILLER_MOVES_ARRAY_SIZE)
    ++myKillerMovesSize[depth-1];
}

inline void ComputerPlayerAlphaBeta::CopyCurrentPV(int depth)
{
  if (depth > 1) {
    // copy principal variation of the subtree to the current tree
    for (int i = 0; i <= depth-2; ++i) {
      *(myPVMoves[depth-1][i]) = *(myPVMoves[depth-2][i]);
    }
  }
}
