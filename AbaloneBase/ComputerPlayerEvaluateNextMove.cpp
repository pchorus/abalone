#include "AbaloneBase.h"

#include "ComputerPlayerEvaluateNextMove.h"

#include "GameManager.h"
#include "GameBoard.h"

#include "Output.h"

ComputerPlayerEvaluateNextMove::ComputerPlayerEvaluateNextMove(GameManager* gameManager, const CString& name, BoardField::Ball ball)
:ComputerPlayer(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE)
, mySimGameManager(new GameManager)
, myBallMovesSize(0)
, myCurrentPlayer(0)
{
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);

  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    myBallMoves[i] = new BallMove;
  }

  if (ball == BoardField::BLACK_BALL) {
    myCurrentPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer1());
  }
  else {
    myCurrentPlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer2());
  }
}

ComputerPlayerEvaluateNextMove::~ComputerPlayerEvaluateNextMove()
{
  if (mySimGameManager) {
    delete mySimGameManager;
    mySimGameManager = 0;
  }

  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    delete myBallMoves[i];
  }
}

BallMove ComputerPlayerEvaluateNextMove::CalculateNextMove()
{
  BallMove retMove;

  myBallMovesSize = 0;

  DWORD time = 0;
  DWORD start = 0;
  DWORD end = 0;

  start = GetTickCount();

  // these doubles have to be initialized with the smallest
  // number they can take (or even smaller),
  // TODO: define a range of possible ratings
  int bestRating = INT_MIN;
  int newRating = INT_MIN;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    startPlayer = Player::PLAYER_TWO;
  }

  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());
  mySimGameManager->SetStartPlayer(startPlayer);

  // TODO: perhaps first of all add the moves with three balls
  // because they are more promising
  mySimGameManager->AddPossibleMovesOneBall(myCurrentPlayer, myBallMoves, myBallMovesSize);
  mySimGameManager->AddPossibleMovesTwoBalls(myCurrentPlayer, myBallMoves, myBallMovesSize);
  mySimGameManager->AddPossibleMovesThreeBalls(myCurrentPlayer, myBallMoves, myBallMovesSize);

  for (int i = 0; i < myBallMovesSize; ++i) {
    newRating = SimulateMove(myBallMoves[i]);
    if (newRating > bestRating) {
      // TODO: improvement: assignment from pointer to pointer and after the loop,
      // one assignment by value, so we have only one copy of a BallMove
      bestRating = newRating;
      retMove = *myBallMoves[i];
    }
  }

  ASSERT(retMove.HasBalls());
  ASSERT(retMove.GetDirection() != NO_VALID_DIRECTION);

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

  time = end - start;

  CString out("EvaluateNextMove\n");
  CString str;
  str.Format("  CalculateNextMove: %d\n", time);
  out += str;
  str.Format("  Possible Moves:    %d\n", myBallMovesSize);
  out += str;
  Output::Message(out, false, true);

//   out = GetGameManager()->GetGameBoard()->ToString();
//   Output::Message2(out, false, true);

  return retMove;
}

int ComputerPlayerEvaluateNextMove::SimulateMove(BallMove* ballMove) const
{
  int ret = 0;

  mySimGameManager->DoMove(ballMove);
  ret = mySimGameManager->EvaluateBoard(myCurrentPlayer, 1);
  mySimGameManager->UndoMove(ballMove);

  return ret;
}
