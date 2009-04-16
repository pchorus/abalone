#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class GameManager
//    Contains the GameBoard and handles the two Players
//
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "BoardField.h"

#define WM_COMPUTER_CALC_FINISHED     WM_USER + 23

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
  int GetMaxNumberOfTurns() const;
  void SetMaxNumberOfTurns(int maxTurns);

  void IsPossibleDirection(Direction direction, BoardField* ball1, BoardField* ball2, BoardField* ball3, BallMove** ballMoves, int& ballMovesSize) const;
  BOOL IsPossibleDirection(Direction direction) const;
  void MoveBallsInDirection(Direction direction);
  void DoMove(BallMove* move);
  void UndoMove(BallMove* move);

  bool IsFirstPlayersTurn() const;

  bool CanSelectBall(BoardField* field) const;
  bool CanDeselectBall(BoardField* field) const;
  void AddSelectedBall(BoardField* field);
  void RemoveSelectedBall(BoardField* field);
  void ClearSelectedBalls();

  void TurnIsOver(LPVOID pParam = 0);

  void AddPossibleMovesOneBall(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const;
  void AddPossibleMovesTwoBalls(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const;
  void AddPossibleMovesThreeBalls(const ComputerPlayer* player, BallMove** ballMoves, int& ballMovesSize) const;
  void OrderMoves(BallMove** ballMoves, int ballMoveSize) const;

  int GetLostBallsPlayer1() const;
  void SetLostBallsPlayer1(int lostBalls);
  int GetLostBallsPlayer2() const;
  void SetLostBallsPlayer2(int lostBalls);
  bool IsTerminalPosition() const;

  void ResetLostBalls();

  void SetPlayers(const CString& namePlayer1, Player::PlayerType typePlayer1,const CString& namePlayer2, Player::PlayerType typePlayer2, Player::PlayerNumber startPlayer = Player::PLAYER_ONE);

  // sets the balls in the start formation
  void SetBallFormation(const CString& formation);

  // returns the next field in the given direction
  BoardField* GetNextFieldInDirection(CPoint fieldCoord, Direction direction) const;
  static CPoint GetNextFieldCoordinatesInDirection(const CPoint& fieldCoord, Direction direction);

  int EvaluateBoard(Player* player, int evaluation) const;
  // methods for evaluation of the current game board situation
  // calculates the ratio of lost balls
  double CalcLostBallsRatio(const Player* player) const;
  // returns the average distance of the player's marbles to the game board's center
  // at the end of a simulated game
  int CalcCenterDistance(const Player* player) const;
  // calculates the average grouping of the marbles
  // TODO: this can be calculated in the same loop as in GetAvgCenterDistanceSimGame
  // put them into one to improve performance
  int CalcGrouping(const Player* player) const;
  int CalcOpposingGrouping(const Player* player) const;
  // calculates the attacking power on the opponent
  int CalcAttackingPowerOnOpponent(const Player* player) const;
  // calculates how strong the player is attacked by the other player
  int CalcAttackedByOpponent(const Player* player) const;

  // returns the Manhattan distance of the passed coordinates to the center
  int CalcCenterDistance(CPoint coord) const;
  // returns the amount of neighbor marbles of the same color the
  // marble at coord
  int CalcGroupingField(const Player* player, CPoint coord) const;

  void GetOpponentFields(Direction direction, const BoardField* selectedField1, const BoardField* selectedField2,
    const BoardField* selectedField3, BoardField*& opponentField1, BoardField*& opponentField2, BoardField*& opponentField3) const;

  static BallAxis GetAxisOfBalls(const BoardField* ball1, const BoardField* ball2);

  // checks whether a non-quiescent position is reached or a more
  // intensive search is needed
  bool IsQuiescencePosition() const;

private:
  void CheckDirections(BoardField* ball1, BoardField* ball2, BoardField* ball3, BallMove** ballMoves, int& ballMovesSize) const;

  // help methods

  void AddLostBall(BoardField::Ball ball);
  void RemoveLostBall(BoardField::Ball ball);

  // sorts the selected balls, such that they are
  // in the array as on the board from left to right
  void SortSelectedBalls();
  // sorts the passed collection of balls
  void SortBalls(BoardField*& ball1, BoardField*& ball2, BoardField*& ball3);

  // ball start formations
  void SetBallsStandardFormation();
  void SetBallsBelgianDaisyFormation();
  void SetBallsCustomFormation(const CString& formation);

  bool IsAttacking(BoardField* field1, BoardField* field2, BoardField* field3, /*BoardField* opField1, */BoardField* opField2, BoardField* opField3) const;

  // members
  GameBoard* myGameBoard;
  bool myIsGameStarted;
  Player* myPlayer1;
  Player* myPlayer2;
  Player* myNextTurn;
  int myLostBallsPlayer1;
  int myLostBallsPlayer2;
  BoardField* mySelectedBall1;
  BoardField* mySelectedBall2;
  BoardField* mySelectedBall3;
  int myMaxNumberOfTurns;

  // this collection is used for evaluating the game situation
  // so we avoid too many constructor calls
  BallMove* myBallMoves[BALL_MOVES_ARRAY_SIZE];
  int myBallMovesSize;
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

inline int GameManager::GetMaxNumberOfTurns() const
{
  return myMaxNumberOfTurns;
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

inline bool GameManager::IsAttacking(BoardField* field1, BoardField* field2, BoardField* field3, /*BoardField* opField1, */BoardField* opField2, BoardField* opField3) const
{
  // field3 field2 field1 => opField1 opField2 opField3
  if (field1 && field2 && field1->GetBall() == field2->GetBall()) {
    if (!opField2 || opField2->GetBall() == BoardField::NO_BALL) {
      return true;
    }
    else if (opField2 && opField2->GetBall() != field1->GetBall()
              && field3 && field3->GetBall() == field1->GetBall()
              && (!opField3 || opField3->GetBall() == BoardField::NO_BALL))
    {
      return true;
    }
  }

  return false;
}

inline bool GameManager::IsTerminalPosition() const
{
  return myLostBallsPlayer1 == 6 || myLostBallsPlayer2 == 6;
}
