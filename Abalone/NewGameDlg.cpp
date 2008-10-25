// NewGameDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Abalone.h"
#include "NewGameDlg.h"


// NewGameDlg-Dialogfeld

IMPLEMENT_DYNAMIC(NewGameDlg, CDialog)

NewGameDlg::NewGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NewGameDlg::IDD, pParent)
{

}

NewGameDlg::~NewGameDlg()
{
}

void NewGameDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_START_FORMATION, myStartFormationComboBox);
}


BEGIN_MESSAGE_MAP(NewGameDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_COMBO_START_FORMATION, &NewGameDlg::OnSelchangeComboStartFormation)
END_MESSAGE_MAP()

BOOL NewGameDlg::OnInitDialog()
{
  BOOL ret = CDialog::OnInitDialog();

  CheckRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_PLAYER1, IDC_RADIO_HUMAN_PLAYER1);
  CheckRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_PLAYER2, IDC_RADIO_HUMAN_PLAYER2);

  myStartFormationComboBox.AddString(START_FORMATION_STR_STANDARD);
  myStartFormationComboBox.AddString(START_FORMATION_STR_BELGIAN_DAISY);

  myStartFormationComboBox.SetCurSel(myStartFormationComboBox.FindString(-1, START_FORMATION_STR_STANDARD));
  myStartFormationStr = START_FORMATION_STR_STANDARD;
  return ret;
}

// NewGameDlg-Meldungshandler

void NewGameDlg::OnSelchangeComboStartFormation()
{
  myStartFormationComboBox.GetLBText(myStartFormationComboBox.GetCurSel(), myStartFormationStr);
}
