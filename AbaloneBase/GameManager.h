#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameManager
//    Contains the GameBoard and handles the two Players
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "BoardField.h"
#include <vector>

class GameBoard;
class BallMove;
class ComputerPlayer;

// the axis on which the selected balls are lying
enum BallAxis { NO_VALID_AXIS, HORIZONTAL, DOWNLEFT_TO_UPPERRIGHT, UPPERLEFT_TO_DOWNRIGHT };

class ABALONE_BASE_DLLINTERFACE GameManager {

public:
  // constructor / destructor
  GameManager();
  ~GameManager();

  // getter and setter
  GameBoard* GetGameBoard() const;
  Player* GetPlayerForNextTurn() const;
  Player* GetPlayer1() const;
  Player* GetPlayer2() const;
  bool IsGameStarted() const;

  void SetStartPlayer(Player::PlayerNumber startPlayer);
  void SetGameStarted(bool started);
  void SetMaxNumberOfTurns(int maxTurns);

  BOOL IsPossibleDirection(Direction direction, bool& isAttacking, bool& ejectsBall, std::vector<BoardField*>* balls = 0, std::vector<BoardField*>* opponentBalls = 0) const;
  void MoveBallsInDirection(Direction direction);
  void DoMove(BallMove* move);
  void UndoMove(BallMove* move);

  bool IsFirstPlayersTurn() const;

  bool CanSelectBall(BoardField* field) const;
  bool CanDeselectBall(BoardField* field) const;
  void AddSelectedBall(BoardField* field);
  void RemoveSelectedBall(BoardField* field);
  void ClearSelectedBalls();

  void TurnIsOver();

  void AddPossibleMovesOneBall(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const;
  void AddPossibleMovesTwoBalls(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const;
  void AddPossibleMovesThreeBalls(const ComputerPlayer* player, std::vector<BallMove*>& ballMoves) const;

  int GetLostBallsPlayer1() const;
  void SetLostBallsPlayer1(int lostBalls);
  int GetLostBallsPlayer2() const;
  void SetLostBallsPlayer2(int lostBalls);

  void ResetLostBalls();

  void SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2, Player::PlayerNumber startPlayer = Player::PLAYER_ONE);

  // sets the balls in the start formation
  void SetBallFormation(const CString& formation);

  CPoint GetNextFieldCoordinatesInDirection(const CPoint& fieldCoord, Direction direction) const;

  // methods for evaluation of the current game board situation
  // calculates the ratio of lost balls
  double CalcLostBallsRatio(const Player* player) const;
  // returns the average distance of the player's marbles to the game board's center
  // at the end of a simulated game
  double CalcAvgCenterDistance(const Player* player) const;
  // calculates the average grouping of the marbles
  // TODO: this can be calculated in the same loop as in GetAvgCenterDistanceSimGame
  // put them into one to improve performance
  double CalcAvgGrouping(const Player* player) const;
  // calculates the attacking power on the opponent
  double CalcAttackingPowerOnOpponent(const Player* player) const;
  // calculates how strong the player is attacked by the other player
  double CalcAttackedByOpponent(const Player* player) const;


  // returns the Manhattan distance of the passed coordinates to the center
  int CalcCenterDistance(CPoint coord) const;
  // returns the amount of neighbor marbles of the same color the
  // marble at coord
  int CalcGroupingField(const Player* player, CPoint coord) const;

private:
  void CheckDirections(const ComputerPlayer* player, std::vector<BoardField*>* ballFields, std::vector<BallMove*>& ballMoves) const;
  BallMove* CreateBallMove(Direction direction, bool isAttacking, bool ejectsBall, std::vector<BoardField*>* ballFields, std::vector<BoardField*>* opponentFields) const;

  // help methods
  BallAxis GetAxisOfBalls(const std::vector<BoardField*>* const ballFields) const;
  void GetSelectedAndOpponentFields(Direction direction, std::vector<BoardField*>* balls, BoardField*& selectedField1, BoardField*& selectedField2,
    BoardField*& selectedField3, BoardField*& opponentField1, BoardField*& opponentField2, BoardField*& opponentField3) const;

  void AddLostBall(BoardField::Ball ball);
  void RemoveLostBall(BoardField::Ball ball);

  // sorts the vector of selected balls, such that they are
  // in the array as on the board from left to right
  void SortSelectedBalls();
  // sorts the passed collection of balls
  void SortBalls(std::vector<BoardField*>* balls);

  // ball start formations
  void SetBallsStandardFormation();
  void SetBallsBelgianDaisyFormation();
  void SetBallsCustomFormation(const CString& formation);

  // members
  GameBoard* myGameBoard;
  bool myIsGameStarted;
  Player* myPlayer1;
  Player* myPlayer2;
  Player* myNextTurn;
  int myLostBallsPlayer1;
  int myLostBallsPlayer2;
  std::vector<BoardField*>* mySelectedBalls;
  int myMaxNumberOfTurns;
};

inline GameBoard* GameManager::GetGameBoard() const
{
  return myGameBoard;
}

inline Player* GameManager::GetPlayerForNextTurn() const
{
  return myNextTurn;
}

inline Player* GameManager::GetPlayer1() const
{
  return myPlayer1;
}

inline Player* GameManager::GetPlayer2() const
{
  return myPlayer2;
}

inline bool GameManager::IsGameStarted() const
{
  return myIsGameStarted;
}

inline void GameManager::SetGameStarted(bool started)
{
  myIsGameStarted = started;
}

inline bool GameManager::IsFirstPlayersTurn() const
{
  return myPlayer1 == myNextTurn;
}

inline int GameManager::GetLostBallsPlayer1() const
{
  return myLostBallsPlayer1;
}

inline void GameManager::SetLostBallsPlayer1(int lostBalls)
{
  myLostBallsPlayer1 = lostBalls;
}

inline int GameManager::GetLostBallsPlayer2() const
{
  return myLostBallsPlayer2;
}

inline void GameManager::SetLostBallsPlayer2(int lostBalls)
{
  myLostBallsPlayer2 = lostBalls;
}

inline void GameManager::ResetLostBalls()
{
  myLostBallsPlayer1 = 0;
  myLostBallsPlayer2 = 0;
}

inline void GameManager::SetMaxNumberOfTurns(int maxTurns)
{
  myMaxNumberOfTurns = maxTurns;
}

inline void GameManager::SetStartPlayer(Player::PlayerNumber startPlayer)
{
  if (startPlayer == Player::PLAYER_ONE) {
    myNextTurn = myPlayer2;
  }
  else if (startPlayer == Player::PLAYER_TWO) {
    myNextTurn = myPlayer1;
  }
  else {
    myNextTurn = 0;
  }
}
