#pragma once
#include "afxwin.h"

class GameManager;

// NewGameDlg-Dialogfeld

class NewGameDlg : public CDialog
{
	DECLARE_DYNAMIC(NewGameDlg)

public:
	NewGameDlg(GameManager* gameManager, CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~NewGameDlg();

// Dialogfelddaten
	enum { IDD = IDD_NEWGAME_DIALOG };

  // getter and setter
  CString GetStartFormationStr() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  virtual BOOL OnInitDialog();
  virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
  afx_msg void OnSelchangeComboStartFormation();
  afx_msg void OnRadioPlayer1Changed();
  afx_msg void OnRadioPlayer2Changed();
  afx_msg void OnCheckIterativeDeepeningPlayer1Changed();
  afx_msg void OnCheckIterativeDeepeningPlayer2Changed();

  GameManager* myGameManager;
  CStatic myStaticPlayer1MCSimGames;
  CStatic myStaticPlayer1MCTurnsPerSimGame;
  CStatic myStaticPlayer1ABTimeForGame;
  CStatic myStaticPlayer1ABTreeDepth;
  CStatic myStaticPlayer1Evaluation;
  CEdit myEditPlayer1MCSimGames;
  CEdit myEditPlayer1MCTurnsPerSimGame;
  CEdit myEditPlayer1ABTimeForGame;
  CEdit myEditPlayer1ABTreeDepth;
  CEdit myEditPlayer1Evaluation;

  CStatic myStaticPlayer2MCSimGames;
  CStatic myStaticPlayer2MCTurnsPerSimGame;
  CStatic myStaticPlayer2ABTimeForGame;
  CStatic myStaticPlayer2ABTreeDepth;
  CStatic myStaticPlayer2Evaluation;
  CEdit myEditPlayer2MCSimGames;
  CEdit myEditPlayer2MCTurnsPerSimGame;
  CEdit myEditPlayer2ABTimeForGame;
  CEdit myEditPlayer2ABTreeDepth;
  CEdit myEditPlayer2Evaluation;

  CButton myCheckPlayer1IterativeDeepening;
  CButton myCheckPlayer2IterativeDeepening;

  CEdit myEditMaxNoOfMoves;
  CComboBox myStartFormationComboBox;
  CString myStartFormationStr;
};

inline CString NewGameDlg::GetStartFormationStr() const
{
  return myStartFormationStr;
}
