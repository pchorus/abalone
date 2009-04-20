#include "AbaloneBase.h"
#include "ComputerPlayerAlphaBetaIterativeDeepening.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"

static const int MAX_TREE_DEPTH = 10;
static const int DEFAULT_MILLISECONDS_PER_MOVE = 600000;
static const int AVG_PLY_PER_GAME_PER_PLAYER = 44;

ComputerPlayerAlphaBetaIterativeDeepening::ComputerPlayerAlphaBetaIterativeDeepening(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayerAlphaBeta(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING)
, myMilliSecondsForGame(DEFAULT_MILLISECONDS_PER_MOVE)
, myLeftMilliSecondsForGame(DEFAULT_MILLISECONDS_PER_MOVE)
, myStart(0)
{
  SetTreeDepth(MAX_TREE_DEPTH);
}

ComputerPlayerAlphaBetaIterativeDeepening::~ComputerPlayerAlphaBetaIterativeDeepening()
{
}

BallMove ComputerPlayerAlphaBetaIterativeDeepening::CalculateNextMove()
{
  DWORD start = GetTickCount();
  // TODO: currently we only have an alarm timer, another timer which takes the
  // ratio between the times for the different tree depth is of interest.
  BallMove currentBestMove;
  BallMove retMove;
  bool foundMove = false;
  myKeepInvestigating = true;

  DWORD end = 0;

  myStart = GetTickCount();

  int alpha = INT_MIN;
  int beta = INT_MAX;
  int value = 0;
  int currentBestValue = 0;
  int retBestValue = 0;

  myTreeDepth[NORMAL] = 1;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  while (myKeepInvestigating && myTreeDepth[NORMAL] <= MAX_TREE_DEPTH) {
    myNodeCounter = 1;
    myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1] = 0;

    alpha = INT_MIN;
    beta = INT_MAX;
    myStartQSCounter = 0;
    myLeafNodesQuiescent = 0;

    myHashMap.UnInit();
    myCurrentHashKey = myHashMap.CalcHashKey(mySimGameManager->GetGameBoard());

    // best move from previous iteration into the movelist
    if (foundMove) {
      *(myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][0]) = retMove;
      myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1] = 1;
    }

    mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[NORMAL][myTreeDepth[NORMAL]-1], myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1]);

    for (int i = 0; i < myBallMovesSize[NORMAL][myTreeDepth[NORMAL]-1] && myKeepInvestigating; ++i) {
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
        currentBestMove = *myBallMoves[NORMAL][myTreeDepth[NORMAL]-1][i];
        currentBestValue = value;
        alpha = value;
        foundMove = true;
      }
    }

    if (myKeepInvestigating) {
      // search with the current depth is finished, so we have a new best move
      retMove = currentBestMove;
      retBestValue = currentBestValue;
      ++myTreeDepth[NORMAL];
    }
  }

  // TODO: it happened that no move was found. In that case we should at least choose a
  // move randomly or one with 3 balls in direction to the center
  // save the move chosen on the simboard to prevent loops in the game

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

  end = GetTickCount();
  if ((end - myStart) < myLeftMilliSecondsForGame) {
    myLeftMilliSecondsForGame = myLeftMilliSecondsForGame - (GetTickCount() - myStart);
  }
  else {
    myLeftMilliSecondsForGame = 0;
  }
  
  CString msg;
  msg.Format("AB ID\nMove: %s\nDepth: %d\nTime: %d\nNodes: %d\nValueBestMove: %d\nInserts: %d\nReuses: %d\n\n", retMove.ToString(), --myTreeDepth[NORMAL], GetTickCount()-start, myNodeCounter, retBestValue, myHashMap.myInserts, myHashMap.myReUseEntries);
//  Output::Message(msg, false, true);

  return retMove;
}

void ComputerPlayerAlphaBetaIterativeDeepening::CheckTime()
{
  if ((GetTickCount() - myStart) > myLeftMilliSecondsForGame / AVG_PLY_PER_GAME_PER_PLAYER) {
    myKeepInvestigating = false;
  }
}
