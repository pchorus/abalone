#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"


static const size_t GAMES_TO_SIMULATE = 25;
static const int MAX_NUMBER_OF_TURNS_PER_SIM_GAME = 200;


ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball, Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO)
, mySimGameManager(new GameManager)
, myBallMovesSize(0)
, myCurrentPlayer(0)
{
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);

  for (int i = 0; i < BALL_MOVES_ARRAY_SIZE; ++i) {
    myBallMoves[i] = new BallMove;
  }

  if (GetGameManager()->IsFirstPlayersTurn()) {
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

  CString out("MonteCarlo\n");
  CString str;
  str.Format("  CalculateNextMove: %d\n", time);
  out += str;
  str.Format("  Possible Moves:    %d\n", myBallMovesSize);
  out += str;
  str.Format("  Simulated Games:   %d\n", GetGamesToSimulate());
  out += str;
  Output::Message(out, false, true);

  return retMove;
}

double ComputerPlayerMonteCarlo::SimulateGamesWithMove(BallMove* ballMove) const
{
  double ret = 0.;
  double rating = 0.;
  // generic number of simulated games
  size_t gamesToSimulate = GetGamesToSimulate();
  // alternative: fixed number of simulated games
//  size_t gamesToSimulate = GAMES_TO_SIMULATE;

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    startPlayer = Player::PLAYER_TWO;
  }

  mySimGameManager->SetMaxNumberOfTurns(MAX_NUMBER_OF_TURNS_PER_SIM_GAME);

  for (size_t run = 0; run < gamesToSimulate; ++run) {
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

    rating = EvaluateSimGame();
    ret += rating;
  }

  return ret;
}

double ComputerPlayerMonteCarlo::EvaluateSimGame() const
{
  ComputerPlayer* simGamePlayer = 0;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    simGamePlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer1());
  }
  else {
    simGamePlayer = static_cast<ComputerPlayer*>(mySimGameManager->GetPlayer2());
  }

  double lostBallsRating = mySimGameManager->CalcLostBallsRatio(simGamePlayer);
  // best ratio:  +6
  // worst ratio: -6
  lostBallsRating = (lostBallsRating + 6.) / 12.;

  double centerDistanceRating = mySimGameManager->CalcAvgCenterDistance(simGamePlayer);
  // 1.3 = 1.0 (1.357 is the best value to achieve with all 14 marbles)
  // 4.0 =  0.0 (4.0 => every marble is on the game board's border)
  centerDistanceRating = 1. - ((centerDistanceRating - 1.3) / 2.7);

  double groupingRating = mySimGameManager->CalcAvgGrouping(simGamePlayer);
  // 4.14 = 1.0 : all marbles are in a huge single group
  // 0.0  = 0.0 : no marble has any neighboring fellow marbles
  groupingRating /= 4.1;

  // TODO: attacking powers should be calculated without calling AddPossibleMoves
//   double attackingPowerRating = mySimGameManager->CalcAttackingPowerOnOpponent(simGamePlayer);
// 
//   double attackedByOpponent = mySimGameManager->CalcAttackedByOpponent(simGamePlayer);
//   // 0 attacks  = 1.0
//   // 10 attacks = 0.0
//   if (attackedByOpponent > 10.)
//     attackedByOpponent = 10.;
//   attackedByOpponent = (10. - attackedByOpponent) * 0.1;

  double evaluation = LOST_BALLS_EVALUATION_WEIGHT  * lostBallsRating
    + CENTER_DISTANCE_EVALUATION_WEIGHT             * centerDistanceRating
    + GROUPING_EVALUATION_WEIGHT                    * groupingRating;
//     + ATTACKING_POWER_EVALUATION_WEIGHT             * attackingPowerRating
//     + ATTACKED_BY_OPPONENT_EVALUATION_WEIGHT        * attackedByOpponent;

  return evaluation;
}
