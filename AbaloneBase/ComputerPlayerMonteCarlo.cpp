#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"


static const size_t GAMES_TO_SIMULATE = 25;
static const int MAX_NUMBER_OF_TURNS_PER_SIM_GAME = 200;

static const double LOST_BALLS_EVALUATION_WEIGHT        = 0.4;
static const double CENTER_DISTANCE_EVALUATION_WEIGHT   = 0.2;
static const double GROUPING_EVALUATION_WEIGHT          = 0.4;

ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, mySimGameManager(new GameManager)
, myNoPossibleMoves(0)
{
  mySimGameManager->SetPlayers("SimPlayer1", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, "SimPlayer2", Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES, Player::PLAYER_NONE);
}

ComputerPlayerMonteCarlo::~ComputerPlayerMonteCarlo()
{
  if (mySimGameManager) {
    delete mySimGameManager;
    mySimGameManager = 0;
  }
}

BallMove ComputerPlayerMonteCarlo::CalculateNextMove()
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

  AddPossibleMovesOneBall(ballMoves);
  AddPossibleMovesTwoBalls(ballMoves);
  AddPossibleMovesThreeBalls(ballMoves);

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator ballMoveIterator;

  myNoPossibleMoves = ballMoves.size();
  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    newRating = SimulateGamesWithMove(*ballMoveIterator);
    if (newRating > bestRating) {
      bestRating = newRating;
      ret = **ballMoveIterator;
    }
  }

  for (ballMoveIterator = ballMoves.begin(); ballMoveIterator != ballMoves.end(); ++ballMoveIterator) {
    delete *ballMoveIterator;
  }

  ASSERT(ret.GetBalls()->size() != 0);
  ASSERT(ret.GetDirection() != NO_VALID_DIRECTION);

  end = GetTickCount();

  time = end - start;

  CString str;
  CString out;
  out = GetName() + ":\n";
  str.Format("  CalculateNextMove: %d\n", time);
  out += str;
  str.Format("  Possible Moves:    %d\n", ballMoves.size());
  out += str;
  str.Format("  Simulated Games:   %d\n", GetGamesToSimulate());
  out += str;
  Output::Message(out, false, true);

  out = GetGameManager()->GetGameBoard()->ToString();
  out += GetName() + ":\n";
  Output::Message2(out, false, true);

  return ret;
}

Player::PlayerType ComputerPlayerMonteCarlo::GetType() const
{
  return PLAYER_TYPE_COMPUTER_MONTE_CARLO;
}

double ComputerPlayerMonteCarlo::SimulateGamesWithMove(BallMove* ballMove) const
{
  double ret = 0;
  double rating = 0;
  // generic number of simulated games
  size_t gamesToSimulate = GetGamesToSimulate();
  // alternative: fixed number of simulated games
//  size_t gamesToSimulate = GAMES_TO_SIMULATE;

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;
  std::vector<BoardField*>::const_iterator i;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    startPlayer = Player::PLAYER_TWO;
  }

  mySimGameManager->SetMaxNumberOfTurns(MAX_NUMBER_OF_TURNS_PER_SIM_GAME);

  for (size_t run = 0; run < gamesToSimulate; ++run) {
    // copy current real situation to the gameboard for simulation
    mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());

    for (i = ballMove->GetBalls()->begin(); i != ballMove->GetBalls()->end(); ++i) {
      mySimGameManager->AddSelectedBall(mySimGameManager->GetGameBoard()->GetBoardField((*i)->GetFieldCoordinates()));
    }

    mySimGameManager->ResetLostBalls();
    mySimGameManager->SetStartPlayer(startPlayer);
    mySimGameManager->MoveBallsInDirection(ballMove->GetDirection());
    mySimGameManager->SetGameStarted(true);
    // with the next method call the algorithm starts
    mySimGameManager->TurnIsOver();

//    Output::Message2(mySimGameManager->GetGameBoard()->ToString(), false, true);

    rating = EvaluateSimGame();
    ret += rating;
  }

  return ret;
}

bool ComputerPlayerMonteCarlo::IsMoveAllowed(Direction direction, std::vector<BoardField*>* balls) const
{
  bool ret = true;

  if (GetCenterDistanceRatio(direction, balls) > 0) {
    ret = false;
  }
  else if (balls->size() == 1 && CheckSingleBallMoveForLoneliness(direction, balls)) {
    ret = false;
  }

  return ret;
}

bool ComputerPlayerMonteCarlo::CheckSingleBallMoveForLoneliness(Direction direction, std::vector<BoardField*>* balls) const
{
  // TODO: improvement: only disallow such a move if the marble wasn't alone before the move
  GameBoard* gameBoard = GetGameManager()->GetGameBoard();
  CPoint newFieldCoord = GetGameManager()->GetNextFieldCoordinatesInDirection((*(balls->begin()))->GetFieldCoordinates(), direction);
  CPoint checkCoord;
  // check the fields around the new field for fellow balls
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, UPLEFT);
  if (direction != DOWNRIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, UPRIGHT);
  if (direction != DOWNLEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, LEFT);
  if (direction != RIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, RIGHT);
  if (direction != LEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, DOWNLEFT);
  if (direction != UPRIGHT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(newFieldCoord, DOWNRIGHT);
  if (direction != UPLEFT && gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    return false;
  }
  return true;
}

int ComputerPlayerMonteCarlo::GetCenterDistanceRatio(Direction direction, std::vector<BoardField*>* balls) const
{
  int ret = 0;
  CPoint currentCoord;
  CPoint centerCoord(4, 4);
  CPoint afterMoveCoord;

  // current Manhattan distance between balls and the game board's center
  for (std::vector<BoardField*>::iterator i = balls->begin(); i != balls->end(); ++i) {
    currentCoord = (*i)->GetFieldCoordinates();
    afterMoveCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(currentCoord, direction);
    ret += CalcCenterDistance(afterMoveCoord) - CalcCenterDistance(currentCoord);
  }

  return ret;
}

double ComputerPlayerMonteCarlo::EvaluateSimGame() const
{
  double lostBallsRating = CalcLostBallsRatioSimGame();
  // best ratio:  +6
  // worst ratio: -6
  lostBallsRating = (lostBallsRating + 6.) / 12.;

  double centerDistanceRating = CalcAvgCenterDistanceSimGame();
  // 1.3 = 1.0 (1.357 is the best value to achieve with all 14 marbles)
  // 4.0 =  0.0 (4.0 => every marble is on the game board's border)
  centerDistanceRating = 1. - ((centerDistanceRating - 1.3) / 2.7);

  double groupingRating = CalcAvgGroupingSimGame();
  // 4.14 = 1.0 : all marbles are in a huge single group
  // 0.0  = 0.0 : no marble has any neighboring fellow marbles
  groupingRating /= 4.1;

  double evaluation = LOST_BALLS_EVALUATION_WEIGHT      * lostBallsRating
                    + CENTER_DISTANCE_EVALUATION_WEIGHT * centerDistanceRating
                    + GROUPING_EVALUATION_WEIGHT        * groupingRating;

  return evaluation;
}

double ComputerPlayerMonteCarlo::CalcLostBallsRatioSimGame() const
{
  double ret = mySimGameManager->GetLostBallsPlayer1() - mySimGameManager->GetLostBallsPlayer2();
  if (GetGameManager()->IsFirstPlayersTurn()) {
    ret *= -1;
  }

  return ret;
}

double ComputerPlayerMonteCarlo::CalcAvgCenterDistanceSimGame() const
{
  double ret = 0.;
  int centerDistance = 0;
  BoardField::Ball playersBall = GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  GameBoard* gameBoard = mySimGameManager->GetGameBoard();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        currentField = gameBoard->GetBoardField(x, y);

        if (currentField->GetBall() == playersBall) {
          centerDistance += CalcCenterDistance(currentField->GetFieldCoordinates());
        }
      }
    }
  }

  // we return the average center distance of each ball on the board,
  // so we have to exclude the balls which are already lost
  if (GetGameManager()->IsFirstPlayersTurn()) {
    ret = centerDistance / double(14 - mySimGameManager->GetLostBallsPlayer1());
  }
  else {
    ret = centerDistance / double(14 - mySimGameManager->GetLostBallsPlayer2());
  }
  return ret;
}

double ComputerPlayerMonteCarlo::CalcAvgGroupingSimGame() const
{
  double ret = 0.;
  int grouping = 0;
  BoardField::Ball playersBall = GetBall();
  BoardField* currentField = 0;
  CPoint centerCoord(4, 4);

  GameBoard* gameBoard = mySimGameManager->GetGameBoard();

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        currentField = gameBoard->GetBoardField(x, y);

        if (currentField->GetBall() == playersBall) {
          grouping += CalcGroupingSimGameField(currentField->GetFieldCoordinates());
        }
      }
    }
  }

  // we return the average center distance of each ball on the board,
  // so we have to exclude the balls which are already lost
  if (GetGameManager()->IsFirstPlayersTurn()) {
    ret = grouping / double(14 - mySimGameManager->GetLostBallsPlayer1());
  }
  else {
    ret = grouping / double(14 - mySimGameManager->GetLostBallsPlayer2());
  }
  return ret;
}

int ComputerPlayerMonteCarlo::CalcCenterDistance(CPoint coord) const
{
  int ret = 0;
  int hlp = 0;
  CPoint centerCoord(4, 4);

  while ((coord.x < centerCoord.x && coord.y < centerCoord.y)
    || (coord.x > centerCoord.x && coord.y > centerCoord.y))
  {
    // extra effort
    // transformation: turn the game board counter clockwise until the
    // marble is in the down right or upper left corner of the board
    hlp = coord.y;
    coord.y = coord.x;
    coord.x += 4 - hlp;
  }
  ret = abs(coord.x - centerCoord.x) + abs(coord.y - centerCoord.y);

  return ret;
}

int ComputerPlayerMonteCarlo::CalcGroupingSimGameField(CPoint coord) const
{
  int ret = 0;

  // TODO: perhaps this method can be implemented with the help of a loop over
  // the Direction enum's values


  GameBoard* gameBoard = mySimGameManager->GetGameBoard();
  CPoint checkCoord;
  // check the fields around the new field for fellow balls
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, UPLEFT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, UPRIGHT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, LEFT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, RIGHT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, DOWNLEFT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  checkCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(coord, DOWNRIGHT);
  if (gameBoard->GetBoardFieldExist(checkCoord) && gameBoard->GetBoardField(checkCoord)->GetBall() == GetBall()) {
    ++ret;
  }
  return ret;
}
