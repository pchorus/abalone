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

  DWORD GetSecondsPerMove() const;
  void SetSecondsPerMove(DWORD seconds);

private:
  DWORD myMilliSecondsPerMove;
};

inline void ComputerPlayerAlphaBetaIterativeDeepening::SetSecondsPerMove(DWORD seconds)
{
  myMilliSecondsPerMove = seconds*1000;
}

inline DWORD ComputerPlayerAlphaBetaIterativeDeepening::GetSecondsPerMove() const
{
  return myMilliSecondsPerMove / 1000;
}
