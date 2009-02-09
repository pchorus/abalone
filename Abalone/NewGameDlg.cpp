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

  CheckRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER1, IDC_RADIO_HUMAN_PLAYER1);
  CheckRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER2, IDC_RADIO_HUMAN_PLAYER2);

  myStartFormationComboBox.AddString(START_FORMATION_STR_STANDARD);
  myStartFormationComboBox.AddString(START_FORMATION_STR_BELGIAN_DAISY);

  // add file titles to combobox
  CFileFind fileFinder;
  CString fileCStr;
  BOOL working = fileFinder.FindFile("formations\\*.txt");

  while (working) {
    working = fileFinder.FindNextFile();

    fileCStr = fileFinder.GetFileTitle();
    if (!fileCStr.IsEmpty() && fileCStr != _T("."))
      myStartFormationComboBox.AddString(fileCStr);
  }

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

  int radioTypePlayer1 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER1);
  int radioTypePlayer2 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER2);

  Player::PlayerType typePlayer1 = Player::PLAYER_TYPE_NONE;
  Player::PlayerType typePlayer2 = Player::PLAYER_TYPE_NONE;

  switch (radioTypePlayer1) {
    case IDC_RADIO_HUMAN_PLAYER1:
      typePlayer1 = Player::PLAYER_TYPE_HUMAN;
      break;
    case IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER1:
      typePlayer1 = Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO;
      break;
    case IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER1:
      typePlayer1 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA;
      break;
    case IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER1:
      typePlayer1 = Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
      break;
    case IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER1:
      typePlayer1 = Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
      break;
  }

  switch (radioTypePlayer2) {
    case IDC_RADIO_HUMAN_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_HUMAN;
      break;
    case IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO;
      break;
    case IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA;
      break;
    case IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
      break;
    case IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
      break;
  }

  myGameManager->SetPlayers(namePlayer1, typePlayer1, namePlayer2, typePlayer2);

  CDialog::OnOK();
}
