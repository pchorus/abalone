#include "AbaloneBase.h"
#include "ComputerPlayerAlphaBetaIterativeDeepening.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"

static const int MAX_TREE_DEPTH = 10;
static const int DEFAULT_MILLISECONDS_PER_MOVE = 20000;

ComputerPlayerAlphaBetaIterativeDeepening::ComputerPlayerAlphaBetaIterativeDeepening(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayerAlphaBeta(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING)
, myMilliSecondsPerMove(DEFAULT_MILLISECONDS_PER_MOVE)
{
  SetTreeDepth(MAX_TREE_DEPTH);
}

ComputerPlayerAlphaBetaIterativeDeepening::~ComputerPlayerAlphaBetaIterativeDeepening()
{
}

BallMove ComputerPlayerAlphaBetaIterativeDeepening::CalculateNextMove()
{
  bool stop = false;
  bool foundMove = false;
  BallMove retMove;

  DWORD start = 0;
  DWORD end = 0;

  start = GetTickCount();

  double alpha = -10.;
  double beta = 10.;
  double value = 0.;

  myTreeDepth = 2;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  while (!stop && myTreeDepth <= MAX_TREE_DEPTH) {
    myBallMovesSize[myTreeDepth-1] = 0;

    // best move from previous iteration into the movelist
    if (foundMove) {
      *(myBallMoves[myTreeDepth-1][0]) = retMove;
      myBallMovesSize[myTreeDepth-1] = 1;
    }

    mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
    mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);
    mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[myTreeDepth-1], myBallMovesSize[myTreeDepth-1]);

    for (int i = 0; i < myBallMovesSize[myTreeDepth-1]; ++i) {
      mySimGameManager->DoMove(myBallMoves[myTreeDepth-1][i]);
      value = Min(myTreeDepth-1, alpha, beta);
      mySimGameManager->UndoMove(myBallMoves[myTreeDepth-1][i]);
      if (value >= beta) {
        break;
      }
      if (value > alpha) {
        retMove = *myBallMoves[myTreeDepth-1][i];
        alpha = value;
        foundMove = true;
      }
      end = GetTickCount();
      if ((end - start) > myMilliSecondsPerMove) {
        stop = true;
        break;
      }
    }

    end = GetTickCount();
    if ((end - start) < myMilliSecondsPerMove) {
      ++myTreeDepth;
    }
    else {
      stop = true;
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
