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

enum Direction { UPLEFT, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWNRIGHT };
// the axis on which the selected balls are lying
enum BallAxis { NO_VALID_AXIS, HORIZONTAL, DOWNLEFT_TO_UPPERRIGHT, UPPERLEFT_TO_DOWNRIGHT };

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
  bool CanDeselectBall(BoardField* field) const;
  void AddSelectedBall(BoardField* field);
  void RemoveSelectedBall(BoardField* field);
  void ClearSelectedBalls();

  void TurnIsOver();

  int GetLostBallsPlayer1() const;
  int GetLostBallsPlayer2() const;

  void SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2);

private:

  // help methods
  BallAxis GetAxisOfSelectedBalls() const;
  void GetSelectedAndOpponentFields(Direction direction, BoardField*& selectedField1, BoardField*& selectedField2,
    BoardField*& selectedField3, BoardField*& opponentField1, BoardField*& opponentField2, BoardField*& opponentField3) const;
  CPoint GetNextFieldCoordinatesInDirection(CPoint& fieldCoord, Direction direction) const;

  void AddLostBall(BoardField::Ball ball);


  // sorts the vector of selected balls, such that they are
  // in the array as on the board from left to right
  void SortSelectedBalls();

  // members
  GameBoard* myGameBoard;
  bool myIsGameStarted;
  Player* myPlayer1;
  Player* myPlayer2;
  Player* myNextTurn;
  int myLostBallsPlayer1;
  int myLostBallsPlayer2;
  std::vector<BoardField*> mySelectedBalls;
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

inline int GameManager::GetLostBallsPlayer1() const
{
  return myLostBallsPlayer1;
}

inline int GameManager::GetLostBallsPlayer2() const
{
  return myLostBallsPlayer2;
}
