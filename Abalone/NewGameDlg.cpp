// NewGameDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Abalone.h"
#include "NewGameDlg.h"

#include "AbaloneBase/GameManager.h"


// NewGameDlg-Dialogfeld

IMPLEMENT_DYNAMIC(NewGameDlg, CDialog)

NewGameDlg::NewGameDlg(GameManager* gameManager, CWnd* pParent /*=NULL*/)
: CDialog(NewGameDlg::IDD, pParent)
, myGameManager(gameManager)
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

void NewGameDlg::OnOK()
{
  // initialize the players
  CString namePlayer1;
  CString namePlayer2;

  GetDlgItem(IDC_EDIT_NAME1)->GetWindowText(namePlayer1);
  GetDlgItem(IDC_EDIT_NAME2)->GetWindowText(namePlayer2);

  int typePlayer1 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_PLAYER1);
  int typePlayer2 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_PLAYER2);

  myGameManager->SetPlayers(
    namePlayer1,
    typePlayer1 == IDC_RADIO_HUMAN_PLAYER1 ? Player::PLAYER_TYPE_HUMAN : Player::PLAYER_TYPE_COMPUTER,
    namePlayer2,
    typePlayer2 == IDC_RADIO_HUMAN_PLAYER2 ? Player::PLAYER_TYPE_HUMAN : Player::PLAYER_TYPE_COMPUTER
    );


  CDialog::OnOK();
}
