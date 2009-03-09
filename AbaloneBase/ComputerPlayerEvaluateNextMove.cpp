#include "AbaloneBase.h"

#include "ComputerPlayerEvaluateNextMove.h"

#include "GameManager.h"
#include "GameBoard.h"

#include "Output.h"

ComputerPlayerEvaluateNextMove::ComputerPlayerEvaluateNextMove(GameManager* gameManager, const CString& name, BoardField::Ball ball)
:ComputerPlayer(gameManager, name, ball)
, mySimGameManager(new GameManager)
{
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);
}

ComputerPlayerEvaluateNextMove::~ComputerPlayerEvaluateNextMove()
{
}

BallMove ComputerPlayerEvaluateNextMove::CalculateNextMove()
{
  BallMove ret;
  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);

  DWORD time = 0;
  DWORD start = 0;
  DWORD end = 0;

  start = GetTickCount();

  // these doubles have to be initialized with the smallest
  // number they can take (or even smaller),
  // TODO: define a range of possible ratings
  double bestRating = -1.;
  double newRating = -1.;

  // TODO: perhaps first of all add the moves with three balls
  // because they are more promising
  GetGameManager()->AddPossibleMovesOneBall(this, ballMoves);
  GetGameManager()->AddPossibleMovesTwoBalls(this, ballMoves);
  GetGameManager()->AddPossibleMovesThreeBalls(this, ballMoves);

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator ballMoveIterator;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    startPlayer = Player::PLAYER_TWO;
  }

  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());
  mySimGameManager->SetStartPlayer(startPlayer);

  CString out;
  out = GetName() + ":\n";
  Output::Message(out, false, true);

//  myNoPossibleMoves = ballMoves.size();
  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    newRating = SimulateMove(*ballMoveIterator);
    if (newRating > bestRating) {
      // TODO: improvement: assignment from pointer to pointer and after the loop,
      // one assignment by value, so we have only one copy of a BallMove
      bestRating = newRating;
      ret = **ballMoveIterator;
    }
  }

  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    delete *ballMoveIterator;
  }

  ASSERT(ret.HasBalls());
  ASSERT(ret.GetDirection() != NO_VALID_DIRECTION);

  end = GetTickCount();

  time = end - start;

  CString str;
  str.Format("  CalculateNextMove: %d\n", time);
  out += str;
  str.Format("  Possible Moves:    %d\n", ballMoves.size());
  out += str;
  Output::Message(out, false, true);

  out = GetGameManager()->GetGameBoard()->ToString();
//  Output::Message2(out, false, true);

  return ret;
}

Player::PlayerType ComputerPlayerEvaluateNextMove::GetType() const
{
  return PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
}

double ComputerPlayerEvaluateNextMove::SimulateMove(BallMove* ballMove) const
{
  double ret = 0.;

  mySimGameManager->DoMove(ballMove);
  ret = EvaluateMove();
  mySimGameManager->UndoMove(ballMove);

  return ret;
}

double ComputerPlayerEvaluateNextMove::EvaluateMove() const
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

  double attackingPowerRating = mySimGameManager->CalcAttackingPowerOnOpponent(simGamePlayer);
  double attackedByOpponent = mySimGameManager->CalcAttackedByOpponent(simGamePlayer);
  // 0 attacks  = 1.0
  // 10 attacks = 0.0
  if (attackedByOpponent > 10.)
    attackedByOpponent = 10.;
  attackedByOpponent = (10. - attackedByOpponent) * 0.1;

  // TODO: another evaluation: if you can win the game with your next move,
  // you should take it anyway
  double evaluation = LOST_BALLS_EVALUATION_WEIGHT  * lostBallsRating
    + CENTER_DISTANCE_EVALUATION_WEIGHT             * centerDistanceRating
    + GROUPING_EVALUATION_WEIGHT                    * groupingRating
    + ATTACKING_POWER_EVALUATION_WEIGHT             * attackingPowerRating
    + ATTACKED_BY_OPPONENT_EVALUATION_WEIGHT        * attackedByOpponent;

  CString out;
  CString str;
  str.Format("  Lost Balls:             %f\n", lostBallsRating);
  out += str;
  str.Format("  Center Distance:        %f\n", centerDistanceRating);
  out += str;
  str.Format("  Grouping:               %f\n", groupingRating);
  out += str;
  str.Format("  Attacking Power:        %f\n\n", attackingPowerRating);
  out += str;
  str.Format("  Attacked By Opponent:   %f\n\n", attackingPowerRating);
  out += str;

  Output::Message(out, false, true);
  return evaluation;
}
