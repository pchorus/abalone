#pragma once
#include "afxwin.h"


static const CString START_FORMATION_STR_STANDARD = "Standard";
static const CString START_FORMATION_STR_BELGIAN_DAISY = "Belgian Daisy";

// NewGameDlg-Dialogfeld

class NewGameDlg : public CDialog
{
	DECLARE_DYNAMIC(NewGameDlg)

public:
	NewGameDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~NewGameDlg();

// Dialogfelddaten
	enum { IDD = IDD_NEWGAME_DIALOG };

  // getter and setter
  CString GetStartFormationStr() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
  afx_msg void OnSelchangeComboStartFormation();

  CComboBox myStartFormationComboBox;
  CString myStartFormationStr;
};

inline CString NewGameDlg::GetStartFormationStr() const
{
  return myStartFormationStr;
}
