#include "AbaloneBase.h"

#include "ComputerPlayerAlphaBeta.h"

#include "GameManager.h"
#include "GameBoard.h"

#include "Output.h"

static const int TREE_DEPTH = 4;


ComputerPlayerAlphaBeta::ComputerPlayerAlphaBeta(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, mySimGameManager(new GameManager)
, myMaxPlayer(0)
, myMinPlayer(0)
{
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);

  if (GetGameManager()->IsFirstPlayersTurn()) {
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
}

Player::PlayerType ComputerPlayerAlphaBeta::GetType() const
{
  return PLAYER_TYPE_COMPUTER_ALPHA_BETA;
}

BallMove ComputerPlayerAlphaBeta::CalculateNextMove()
{
  BallMove retMove;

  double alpha = -10.;
  double beta = 10.;
  double value = 0.;

  // copy current real situation to the game board for simulation
  mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());
  mySimGameManager->SetLostBallsPlayer1(GetGameManager()->GetLostBallsPlayer1());
  mySimGameManager->SetLostBallsPlayer2(GetGameManager()->GetLostBallsPlayer2());

  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);

  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, ballMoves);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, ballMoves);
  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, ballMoves);

  std::vector<BallMove*>::iterator i;

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    mySimGameManager->DoMove(*i);
    value = Min(TREE_DEPTH-1, alpha, beta);
    mySimGameManager->UndoMove(*i);
    if (value >= beta) {
      break;
    }
    if (value > alpha) {
      retMove = **i;
      alpha = value;
    }
  }

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return retMove;
}

double ComputerPlayerAlphaBeta::Max(int depth, double alpha, double beta)
{
  double ret = alpha;
  double value = 0.;

  if (depth == 0)
    return EvaluateBoard();

  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);

  mySimGameManager->AddPossibleMovesOneBall(myMaxPlayer, ballMoves);
  mySimGameManager->AddPossibleMovesTwoBalls(myMaxPlayer, ballMoves);
  mySimGameManager->AddPossibleMovesThreeBalls(myMaxPlayer, ballMoves);

  std::vector<BallMove*>::iterator i;

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    mySimGameManager->DoMove(*i);
    value = Min(depth-1, alpha, beta);
    mySimGameManager->UndoMove(*i);
    if (value >= beta) {
      ret = beta;
      break;
    }
    if (value > alpha) {
      alpha = value;
      ret = alpha;
    }
  }

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}

double ComputerPlayerAlphaBeta::Min(int depth, double alpha, double beta)
{
  double ret = beta;
  double value = 0.;
  if (depth == 0)
    return EvaluateBoard();
  std::vector<BallMove*> ballMoves;
  ballMoves.reserve(100);
  
  GetGameManager()->AddPossibleMovesOneBall(myMinPlayer, ballMoves);
  GetGameManager()->AddPossibleMovesTwoBalls(myMinPlayer, ballMoves);
  GetGameManager()->AddPossibleMovesThreeBalls(myMinPlayer, ballMoves);

  std::vector<BallMove*>::iterator i;

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    mySimGameManager->DoMove(*i);
    value = Max(depth-1, alpha, beta);
    mySimGameManager->UndoMove(*i);
    if (value <= alpha) {
      ret = alpha;
      break;
    }
    if (value < beta) {
      beta = value;
      ret = beta;
    }
  }

  for (i = ballMoves.begin(); i != ballMoves.end(); ++i) {
    delete *i;
  }

  return ret;
}

double ComputerPlayerAlphaBeta::EvaluateBoard() const
{
  double lostBallsRating = mySimGameManager->CalcLostBallsRatio(myMaxPlayer);
  // best ratio:  +6
  // worst ratio: -6
  lostBallsRating = (lostBallsRating + 6.) / 12.;

  double centerDistanceRating = mySimGameManager->CalcAvgCenterDistance(myMaxPlayer);
  // 1.3 = 1.0 (1.357 is the best value to achieve with all 14 marbles)
  // 4.0 =  0.0 (4.0 => every marble is on the game board's border)
  centerDistanceRating = 1. - ((centerDistanceRating - 1.3) / 2.7);

  double groupingRating = mySimGameManager->CalcAvgGrouping(myMaxPlayer);
  // 4.14 = 1.0 : all marbles are in a huge single group
  // 0.0  = 0.0 : no marble has any neighboring fellow marbles
  groupingRating /= 4.1;

  double attackingPowerRating = mySimGameManager->CalcAttackingPowerOnOpponent(myMaxPlayer);
  double attackedByOpponent = mySimGameManager->CalcAttackedByOpponent(myMaxPlayer);
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

//   CString out;
//   CString str;
//   str.Format("  Lost Balls:             %f\n", lostBallsRating);
//   out += str;
//   str.Format("  Center Distance:        %f\n", centerDistanceRating);
//   out += str;
//   str.Format("  Grouping:               %f\n", groupingRating);
//   out += str;
//   str.Format("  Attacking Power:        %f\n\n", attackingPowerRating);
//   out += str;
//   str.Format("  Attacked By Opponent:   %f\n\n", attackingPowerRating);
//   out += str;
// 
//   Output::Message(out, false, true);
  return evaluation;
}
