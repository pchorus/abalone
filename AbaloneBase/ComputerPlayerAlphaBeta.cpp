#include "AbaloneBase.h"

#include "ComputerPlayerAlphaBeta.h"

#include "GameManager.h"
#include "GameBoard.h"

#include "Output.h"



ComputerPlayerAlphaBeta::ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA)
, mySimGameManager(new GameManager)
, myMaxPlayer(0)
, myMinPlayer(0)
, myTreeDepth(DEFAULT_TREE_DEPTH)
, myUsedEvaluation(1)
, myBallMoves(0)
, myBallMovesSize(0)
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

  DWORD time = 0;
  DWORD start = 0;
  DWORD end = 0;

  start = GetTickCount();

  double alpha = -10.;
  double beta = 10.;
  double value = 0.;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  myBallMovesSize[myTreeDepth-1] = 0;

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
    }
  }

//   end = GetTickCount();
//   time = end - start;
// 
//   CString out("AlphaBeta\n");
//   CString str;
//   str.Format("  CalculateNextMove: %d\n", time);
//   out += str;
//   str.Format("  Possible Moves:    %d\n", myBallMovesSize[myTreeDepth-1]);
//   out += str;
//   Output::Message(out, false, true);

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

double ComputerPlayerAlphaBeta::Max(int depth, double alpha, double beta)
{
  double ret = alpha;
  double value = 0.;

  if (depth == 0)
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

  myBallMovesSize[depth-1] = 0;

  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1]; ++i) {
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = Min(depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);
    if (value >= beta) {
      ret = beta;
      break;
    }
    if (value > alpha) {
      alpha = value;
      ret = alpha;
    }
  }

  return ret;
}

double ComputerPlayerAlphaBeta::Min(int depth, double alpha, double beta)
{
  double ret = beta;
  double value = 0.;
  if (depth == 0)
    return mySimGameManager->EvaluateBoard(myMaxPlayer, myUsedEvaluation);

  myBallMovesSize[depth-1] = 0;
  
  mySimGameManager->AddPossibleMovesThreeBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesTwoBalls(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);
  mySimGameManager->AddPossibleMovesOneBall(myMinPlayer, myBallMoves[depth-1], myBallMovesSize[depth-1]);

  for (int i = 0; i < myBallMovesSize[depth-1]; ++i) {
    mySimGameManager->DoMove(myBallMoves[depth-1][i]);
    value = Max(depth-1, alpha, beta);
    mySimGameManager->UndoMove(myBallMoves[depth-1][i]);
    if (value <= alpha) {
      ret = alpha;
      break;
    }
    if (value < beta) {
      beta = value;
      ret = beta;
    }
  }

  return ret;
}
