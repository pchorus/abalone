#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameManager
//    Contains the GameBoard and handles the two Players
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"

class GameBoard;

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

  void TurnIsOver(Player* currentPlayer);

  inline void SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2);

private:
  GameBoard* myGameBoard;
  bool myIsGameStarted;
  Player* myPlayer1;
  Player* myPlayer2;
  Player* myNextTurn;
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

inline void GameManager::TurnIsOver(Player* currentPlayer)
{
  myNextTurn = (currentPlayer == myPlayer1) ? myPlayer2 : myPlayer1;
}

inline bool GameManager::IsGameStarted() const
{
  return myIsGameStarted;
}

inline void GameManager::SetGameStarted(bool started)
{
  myIsGameStarted = started;
}
