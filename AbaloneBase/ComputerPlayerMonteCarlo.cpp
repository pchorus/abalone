#include "AbaloneBase.h"

#include "ComputerPlayerMonteCarlo.h"

#include "GameManager.h"
#include "GameBoard.h"

static const int GAMES_TO_SIMULATE = 30;
static const int MAX_NUMBER_OF_TURNS_PER_SIM_GAME = 150;

ComputerPlayerMonteCarlo::ComputerPlayerMonteCarlo(GameManager* gameManager, const CString& name, BoardField::Ball ball)
: ComputerPlayer(gameManager, name, ball)
, mySimGameManager(new GameManager)
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

BallMove ComputerPlayerMonteCarlo::CalculateNextMove() const
{
  BallMove ret;
  std::vector<BallMove*> ballMoves;

  int bestRating = INT_MIN;
  int newRating = INT_MIN;

  GetGameManager()->AddPossibleMovesOneBall(ballMoves);
  GetGameManager()->AddPossibleMovesTwoBalls(ballMoves);
  GetGameManager()->AddPossibleMovesThreeBalls(ballMoves);

  // destroy all BallMove objects which were created in the AddPossibleMoves methods
  std::vector<BallMove*>::iterator ballMoveIterator;


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

  Player::PlayerNumber startPlayer = Player::PLAYER_ONE;
  std::vector<BoardField*>::const_iterator i;

  if (GetGameManager()->IsFirstPlayersTurn()) {
    startPlayer = Player::PLAYER_TWO;
  }


  for (int run = 0; run < GAMES_TO_SIMULATE; ++run) {
    // copy current real situation to the gameboard for simulation
    mySimGameManager->GetGameBoard()->CopyBoardFields(GetGameManager()->GetGameBoard());

    for (i = ballMove->GetBalls()->begin(); i != ballMove->GetBalls()->end(); ++i) {
      mySimGameManager->AddSelectedBall(mySimGameManager->GetGameBoard()->GetBoardField((*i)->GetFieldCoordinates()));
    }

    mySimGameManager->SetStartPlayer(startPlayer);
    mySimGameManager->MoveBallsInDirection(ballMove->GetDirection());
    mySimGameManager->SetMaxNumberOfTurns(MAX_NUMBER_OF_TURNS_PER_SIM_GAME);
    mySimGameManager->SetGameStarted(true);
    mySimGameManager->TurnIsOver();

    rating = mySimGameManager->GetLostBallsPlayer1() - mySimGameManager->GetLostBallsPlayer2();
    if (GetGameManager()->IsFirstPlayersTurn()) {
      rating *= -1;
    }
    ret += rating;
  }

  return ret;
}
