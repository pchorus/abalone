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

  GameManager* myGameManager;
  CComboBox myStartFormationComboBox;
  CString myStartFormationStr;
};

inline CString NewGameDlg::GetStartFormationStr() const
{
  return myStartFormationStr;
}
