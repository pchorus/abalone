#pragma once

//////////////////////////////////////////////////////////////////////////
//
//  class ComputerPlayerAlphaBetaIterativeDeepening
//    Extension to the standard Alpha-Beta player:
//    it uses iterative deepening
//
//////////////////////////////////////////////////////////////////////////

#include "ComputerPlayerAlphaBeta.h"

class ABALONE_BASE_DLLINTERFACE ComputerPlayerAlphaBetaIterativeDeepening : public ComputerPlayerAlphaBeta
{
public:
  // constructor / destructor
  ComputerPlayerAlphaBetaIterativeDeepening(GameManager* gameManager, const CString& name, BoardField::Ball ball);
  virtual ~ComputerPlayerAlphaBetaIterativeDeepening();

  virtual BallMove CalculateNextMove();
  virtual void CheckTime();

  DWORD GetSecondsForGame() const;
  DWORD GetLeftSecondsForGame() const;
  void SetSecondsForGame(DWORD seconds);

private:
  DWORD myMilliSecondsForGame;
  DWORD myLeftMilliSecondsForGame;
  DWORD myStart;
};

inline void ComputerPlayerAlphaBetaIterativeDeepening::SetSecondsForGame(DWORD seconds)
{
  myMilliSecondsForGame = seconds*1000;
  myLeftMilliSecondsForGame = seconds*1000;
}

inline DWORD ComputerPlayerAlphaBetaIterativeDeepening::GetSecondsForGame() const
{
  return myMilliSecondsForGame / 1000;
}

inline DWORD ComputerPlayerAlphaBetaIterativeDeepening::GetLeftSecondsForGame() const
{
  return myLeftMilliSecondsForGame / 1000;
}
