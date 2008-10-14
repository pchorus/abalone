#include "AbaloneBase.h"

#include "GameManager.h"

#include "GameBoard.h"
#include "HumanPlayer.h"

GameManager::GameManager()
: myGameBoard(new GameBoard)
, myPlayer1(new HumanPlayer)
, myPlayer2(new HumanPlayer)
, myNextTurn(myPlayer1)
{
}

GameManager::~GameManager()
{
}
