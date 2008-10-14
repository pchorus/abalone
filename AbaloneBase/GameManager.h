#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameManager
//    Contains the GameBoard and handles the two Players
//
//////////////////////////////////////////////////////////////////////////

class GameBoard;
class Player;

class ABALONE_BASE_DLLINTERFACE GameManager {

public:
  // contructor / destructor
  GameManager();
  ~GameManager();

  // getter and setter
  GameBoard* GetGameBoard() const;
  Player* GetPlayerForNextTurn() const;

  void TurnIsOver(Player* currentPlayer);

private:
  GameBoard* myGameBoard;
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

inline void GameManager::TurnIsOver(Player* currentPlayer)
{
  myNextTurn = (currentPlayer == myPlayer1) ? myPlayer2 : myPlayer1;
}
