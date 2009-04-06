#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"


static const int DEFAULT_GAMES_TO_SIMULATE = 100;
static const int DEFAULT_TURNS_PER_SIM_GAME = 200;

ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO)
, mySimGameManager(new GameManager)
, myBallMovesSize(0)
, myCurrentPlayer(0)
, myGamesToSimulate(DEFAULT_GAMES_TO_SIMULATE)
, myTurnsPerSimGame(DEFAULT_TURNS_PER_SIM_GAME)
, myUsedEvaluation(1)
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

ComputerPlayerMonteCarlo::~ComputerPlayerMonteCarlo()
{
  if (mySimGameManager) {
    delete mySimGameManager;
    mySimGameManager = 0;
  }

  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    delete myBallMoves[i];
  }
}

BallMove ComputerPlayerMonteCarlo::CalculateNextMove()
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
  double bestRating = -1.;
  double newRating = -1.;

  // copy current real situation to the gameboard for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  
  mySimGameManager->AddPossibleMovesOneBall(myCurrentPlayer, myBallMoves, myBallMovesSize);
  mySimGameManager->AddPossibleMovesTwoBalls(myCurrentPlayer, myBallMoves, myBallMovesSize);
  mySimGameManager->AddPossibleMovesThreeBalls(myCurrentPlayer, myBallMoves, myBallMovesSize);

  for (int i = 0; i < myBallMovesSize; ++i) {
    newRating = SimulateGamesWithMove(myBallMoves[i]);
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

//   CString out("MonteCarlo\n");
//   CString str;
//   str.Format("  CalculateNextMove: %d\n", time);
//   out += str;
//   str.Format("  Possible Moves:    %d\n", myBallMovesSize);
//   out += str;
//   str.Format("  Simulated Games:   %d\n", GetGamesToSimulate());
//   out += str;
//  Output::Message(out, false, true);

  return retMove;
}

double ComputerPlayerMonteCarlo::SimulateGamesWithMove(BallMove* ballMove) const
{
  double ret = 0.;
  double rating = 0.;

  int gamesToSimulate = myGamesToSimulate;

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;

  ComputerPlayer* simGamePlayer = 0;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    simGamePlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer1());
    startPlayer = Player::PLAYER_TWO;
  }
  else {
    simGamePlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer2());
  }

  mySimGameManager->SetMaxNumberOfTurns(myTurnsPerSimGame);

  for (int run = 0; run < gamesToSimulate; ++run) {
    // copy current real situation to the gameboard for simulation
    mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());

    // mySimGameManager->ResetLostBalls();
    mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
    mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());
    mySimGameManager->SetStartPlayer(startPlayer);
    mySimGameManager->DoMove(ballMove);
    mySimGameManager->SetGameStarted(true);
    // with the next method call the algorithm starts
    mySimGameManager->TurnIsOver();

    rating = mySimGameManager->EvaluateBoard(simGamePlayer, myUsedEvaluation);
    ret += rating;
  }

  return ret;
}
