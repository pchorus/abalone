#include "AbaloneBase.h"
#include "ComputerPlayerAlphaBetaIterativeDeepening.h"

#include "GameManager.h"
#include "GameBoard.h"

static const int MAX_TREE_DEPTH = 10;
static const int DEFAULT_MILLISECONDS_PER_MOVE = 1200000;
static const int AVG_PLIES_PER_GAME = 87;

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
  // TODO: currently we only have an alarm timer, another timer which takes the
  // ratio between the times for the different tree depth is of interest.
  BallMove retMove;
  bool foundMove = false;
  myKeepInvestigating = true;

  DWORD end = 0;

  myStart = GetTickCount();

  int alpha = INT_MIN;
  int beta = INT_MAX;
  int value = 0;

  myTreeDepth = 2;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  while (myKeepInvestigating && myTreeDepth <= MAX_TREE_DEPTH) {
    myNodeCounter = 1;
    myBallMovesSize[myTreeDepth-1] = 0;

    myHashMap.UnInit();
    myCurrentHashKey = myHashMap.CalcHashKey(mySimGameManager->GetGameBoard());

    // best move from previous iteration into the movelist
    if (foundMove) {
      *(myBallMoves[myTreeDepth-1][0]) = retMove;
      myBallMovesSize[myTreeDepth-1] = 1;
    }

    mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);

    for (int i = 0; i < myBallMovesSize[myTreeDepth-1] && myKeepInvestigating; ++i) {
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
        foundMove = true;
      }
    }
    ++myTreeDepth;
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

  end = GetTickCount();
  if ((end - myStart) < myLeftMilliSecondsForGame) {
    myLeftMilliSecondsForGame = myLeftMilliSecondsForGame - (GetTickCount() - myStart);
  }
  else {
    myLeftMilliSecondsForGame = 0;
  }
  
  return retMove;
}

void ComputerPlayerAlphaBetaIterativeDeepening::CheckTime()
{
  if ((GetTickCount() - myStart) > myLeftMilliSecondsForGame / AVG_PLIES_PER_GAME) {
    myKeepInvestigating = false;
  }
}
