#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameManager
//    Contains the GameBoard and handles the two Players
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include <set>

class GameBoard;
class BoardField;

enum Direction { UPLEFT, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWNRIGHT };

class ABALONE_BASE_DLLINTERFACE GameManager {

public:
  // contructor / destructor
  GameManager();
  ~GameManager();

  // getter and setter
  GameBoard* GetGameBoard() const;
  Player* GetPlayerForNextTurn() const;
  Player* GetPlayer1() const;
  Player* GetPlayer2() const;
  bool IsGameStarted() const;
  void SetGameStarted(bool started);

  BOOL IsPossibleDirection(Direction direction) const;
  void MoveBallsInDirection(Direction direction);
  bool IsFirstPlayersTurn() const;

  bool CanSelectBall(BoardField* field) const;
  void AddSelectedBall(BoardField* field);
  void RemoveSelectedBall(BoardField* field);
  void ClearSelectedBalls();

  void TurnIsOver();

  inline void SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2);

private:

  // help methods
  BOOL IsUpLeftPossible() const;
  BOOL IsUpRightPossible() const;
  BOOL IsLeftPossible() const;
  BOOL IsRightPossible() const;
  BOOL IsDownLeftPossible() const;
  BOOL IsDownRightPossible() const;

  void MoveBallsUpLeft();
  void MoveBallsUpRight();
  void MoveBallsLeft();
  void MoveBallsRight();
  void MoveBallsDownLeft();
  void MoveBallsDownRight();

  // members
  GameBoard* myGameBoard;
  bool myIsGameStarted;
  Player* myPlayer1;
  Player* myPlayer2;
  Player* myNextTurn;
  std::set<BoardField*> mySelectedBalls;
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

inline void GameManager::TurnIsOver()
{
  myNextTurn = (myNextTurn == myPlayer1) ? myPlayer2 : myPlayer1;
  mySelectedBalls.clear();
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
