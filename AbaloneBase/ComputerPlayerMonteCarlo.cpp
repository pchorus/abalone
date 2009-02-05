#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"
#include "Output.h"


static const size_t GAMES_TO_SIMULATE = 25;
static const int MAX_NUMBER_OF_TURNS_PER_SIM_GAME = 200;

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

  int bestRating = INT_MIN;
  int newRating = INT_MIN;

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

int ComputerPlayerMonteCarlo::SimulateGamesWithMove(BallMove* ballMove) const
{
  int ret = 0;
  int rating = 0;
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

    rating = mySimGameManager->GetLostBallsPlayer1() - mySimGameManager->GetLostBallsPlayer2();
    if (GetGameManager()->IsFirstPlayersTurn()) {
      rating *= -1;
    }
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
  CPoint centerCoord = GetGameManager()->GetGameBoard()->GetBoardField(4, 4)->GetFieldCoordinates();
  CPoint afterMoveCoord;

  // current Manhattan distance between balls and the game board's center
  for (std::vector<BoardField*>::iterator i = balls->begin(); i != balls->end(); ++i) {
    currentCoord = (*i)->GetFieldCoordinates();
    afterMoveCoord = GetGameManager()->GetNextFieldCoordinatesInDirection(currentCoord, direction);

    ret += (abs(afterMoveCoord.x - centerCoord.x) + abs(afterMoveCoord.y - centerCoord.y))
         - (abs(currentCoord.x - centerCoord.x) + abs(currentCoord.y - centerCoord.y));
  }

  return ret;
}
