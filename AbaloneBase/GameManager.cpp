#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

GameManager::GameManager()
: myGameBoard(new GameBoard)
, myIsGameStarted(false)
, myPlayer1(0)
, myPlayer2(0)
, myNextTurn(0)
{
}

GameManager::~GameManager()
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }
}

void GameManager::SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2)
{
  if (myPlayer1) {
    delete myPlayer1;
  }
  if (myPlayer2) {
    delete myPlayer2;
  }

  ASSERT(typePlayer1 != Player::PLAYER_TYPE_NONE && typePlayer2 != Player::PLAYER_TYPE_NONE);

  if (typePlayer1 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer1 = new HumanPlayer(namePlayer1 != "" ? namePlayer1 : "Player 1");
  }
  else {
    myPlayer1 = new ComputerPlayer(namePlayer1 != "" ? namePlayer1 : "Player 1");
  }

  if (typePlayer2 == Player::PLAYER_TYPE_HUMAN) {
    myPlayer2 = new HumanPlayer(namePlayer2 != "" ? namePlayer2 : "Player 2");
  }
  else {
    myPlayer2 = new ComputerPlayer(namePlayer2 != "" ? namePlayer2 : "Player 2");
  }

  myNextTurn = myPlayer1;
}
