// NewGameDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Abalone.h"
#include "NewGameDlg.h"

#include "AbaloneBase/GameManager.h"
#include "AbaloneBase/ComputerPlayerMonteCarlo.h"
#include "AbaloneBase/ComputerPlayerAlphaBeta.h"
#include "AbaloneBase/ComputerPlayerAlphaBetaIterativeDeepening.h"
#include "AbaloneBase/Output.h"


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
  DDX_Control(pDX, IDC_STATIC_PLAYER1_SIM_GAMES, myStaticPlayer1MCSimGames);
  DDX_Control(pDX, IDC_STATIC_PLAYER1_NO_OF_TURNS_PER_SIM_GAME, myStaticPlayer1MCTurnsPerSimGame);
  DDX_Control(pDX, IDC_STATIC_PLAYER1_TREE_DEPTH, myStaticPlayer1ABTreeDepth);
  DDX_Control(pDX, IDC_STATIC_PLAYER1_AB_TIME_FOR_GAME, myStaticPlayer1ABTimeForGame);
  DDX_Control(pDX, IDC_STATIC_PLAYER1_EVALUATION, myStaticPlayer1Evaluation);
  DDX_Control(pDX, IDC_EDIT_PLAYER1_SIM_GAMES, myEditPlayer1MCSimGames);
  DDX_Control(pDX, IDC_EDIT_PLAYER1_NO_OF_TURNS_PER_SIM_GAME, myEditPlayer1MCTurnsPerSimGame);
  DDX_Control(pDX, IDC_EDIT_PLAYER1_TIME_FOR_GAME, myEditPlayer1ABTimeForGame);
  DDX_Control(pDX, IDC_EDIT_PLAYER1_TREE_DEPTH, myEditPlayer1ABTreeDepth);
  DDX_Control(pDX, IDC_EDIT_PLAYER1_EVALUATION, myEditPlayer1Evaluation);
  DDX_Control(pDX, IDC_STATIC_PLAYER2_SIM_GAMES, myStaticPlayer2MCSimGames);
  DDX_Control(pDX, IDC_STATIC_PLAYER2_NO_OF_TURNS_PER_SIM_GAME, myStaticPlayer2MCTurnsPerSimGame);
  DDX_Control(pDX, IDC_STATIC_PLAYER2_AB_TIME_FOR_GAME, myStaticPlayer2ABTimeForGame);
  DDX_Control(pDX, IDC_STATIC_PLAYER2_TREE_DEPTH, myStaticPlayer2ABTreeDepth);
  DDX_Control(pDX, IDC_STATIC_PLAYER2_EVALUATION, myStaticPlayer2Evaluation);
  DDX_Control(pDX, IDC_EDIT_PLAYER2_SIM_GAMES, myEditPlayer2MCSimGames);
  DDX_Control(pDX, IDC_EDIT_PLAYER2_NO_OF_TURNS_PER_SIM_GAME, myEditPlayer2MCTurnsPerSimGame);
  DDX_Control(pDX, IDC_EDIT_PLAYER2_TIME_FOR_GAME, myEditPlayer2ABTimeForGame);
  DDX_Control(pDX, IDC_EDIT_PLAYER2_TREE_DEPTH, myEditPlayer2ABTreeDepth);
  DDX_Control(pDX, IDC_EDIT_PLAYER2_EVALUATION, myEditPlayer2Evaluation);
  DDX_Control(pDX, IDC_EDIT_MAX_NO_OF_MOVES, myEditMaxNoOfMoves);
  DDX_Control(pDX, IDC_CHECK_ITERATIVE_DEEPENING1, myCheckPlayer1IterativeDeepening);
  DDX_Control(pDX, IDC_CHECK_ITERATIVE_DEEPENING2, myCheckPlayer2IterativeDeepening);
  DDX_Control(pDX, IDC_CHECK_TRANSPOSITION_TABLE1, myCheckPlayer1TranspositionTable);
  DDX_Control(pDX, IDC_CHECK_TRANSPOSITION_TABLE2, myCheckPlayer2TranspositionTable);
  DDX_Control(pDX, IDC_CHECK_QUIESCENCE_SEARCH1, myCheckPlayer1QuiescenceSearch);
  DDX_Control(pDX, IDC_CHECK_QUIESCENCE_SEARCH2, myCheckPlayer2QuiescenceSearch);
}


BEGIN_MESSAGE_MAP(NewGameDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_COMBO_START_FORMATION, &NewGameDlg::OnSelchangeComboStartFormation)
  ON_BN_CLICKED(IDC_RADIO_HUMAN_PLAYER1, &NewGameDlg::OnRadioPlayer1Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER1, &NewGameDlg::OnRadioPlayer1Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER1, &NewGameDlg::OnRadioPlayer1Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER1, &NewGameDlg::OnRadioPlayer1Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER1, &NewGameDlg::OnRadioPlayer1Changed)
  ON_BN_CLICKED(IDC_RADIO_HUMAN_PLAYER2, &NewGameDlg::OnRadioPlayer2Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER2, &NewGameDlg::OnRadioPlayer2Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER2, &NewGameDlg::OnRadioPlayer2Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER2, &NewGameDlg::OnRadioPlayer2Changed)
  ON_BN_CLICKED(IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER2, &NewGameDlg::OnRadioPlayer2Changed)
  ON_BN_CLICKED(IDC_CHECK_ITERATIVE_DEEPENING1, &NewGameDlg::OnCheckIterativeDeepeningPlayer1Changed)
  ON_BN_CLICKED(IDC_CHECK_ITERATIVE_DEEPENING2, &NewGameDlg::OnCheckIterativeDeepeningPlayer2Changed)
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

  myStaticPlayer1MCSimGames.EnableWindow(FALSE);
  myStaticPlayer1MCTurnsPerSimGame.EnableWindow(FALSE);
  myStaticPlayer1ABTimeForGame.EnableWindow(FALSE);
  myStaticPlayer1ABTreeDepth.EnableWindow(FALSE);
  myStaticPlayer1Evaluation.EnableWindow(FALSE);

  myEditPlayer1MCSimGames.EnableWindow(FALSE);
  myEditPlayer1MCTurnsPerSimGame.EnableWindow(FALSE);
  myEditPlayer1ABTimeForGame.EnableWindow(FALSE);
  myEditPlayer1ABTreeDepth.EnableWindow(FALSE);
  myEditPlayer1MCSimGames.SetWindowText("100");
  myEditPlayer1MCTurnsPerSimGame.SetWindowText("200");
  myEditPlayer1ABTimeForGame.SetWindowText("1200");
  myEditPlayer1ABTreeDepth.SetWindowText("5");
  myEditPlayer1Evaluation.EnableWindow(FALSE);
  myEditPlayer1Evaluation.SetWindowText("1");

  myStaticPlayer2MCSimGames.EnableWindow(FALSE);
  myStaticPlayer2MCTurnsPerSimGame.EnableWindow(FALSE);
  myStaticPlayer2ABTimeForGame.EnableWindow(FALSE);
  myStaticPlayer2ABTreeDepth.EnableWindow(FALSE);
  myStaticPlayer2Evaluation.EnableWindow(FALSE);

  myEditPlayer2MCSimGames.EnableWindow(FALSE);
  myEditPlayer2MCTurnsPerSimGame.EnableWindow(FALSE);
  myEditPlayer2ABTimeForGame.EnableWindow(FALSE);
  myEditPlayer2ABTreeDepth.EnableWindow(FALSE);
  myEditPlayer2MCSimGames.SetWindowText("100");
  myEditPlayer2MCTurnsPerSimGame.SetWindowText("200");
  myEditPlayer2ABTimeForGame.SetWindowText("1200");
  myEditPlayer2ABTreeDepth.SetWindowText("5");
  myEditPlayer2Evaluation.EnableWindow(FALSE);
  myEditPlayer2Evaluation.SetWindowText("1");

  myCheckPlayer1IterativeDeepening.SetCheck(BST_CHECKED);
  myCheckPlayer2IterativeDeepening.SetCheck(BST_CHECKED);
  myCheckPlayer1IterativeDeepening.EnableWindow(FALSE);
  myCheckPlayer2IterativeDeepening.EnableWindow(FALSE);

  myCheckPlayer1QuiescenceSearch.SetCheck(BST_CHECKED);
  myCheckPlayer2QuiescenceSearch.SetCheck(BST_CHECKED);
  myCheckPlayer1QuiescenceSearch.EnableWindow(FALSE);
  myCheckPlayer2QuiescenceSearch.EnableWindow(FALSE);

  myCheckPlayer1TranspositionTable.SetCheck(BST_CHECKED);
  myCheckPlayer2TranspositionTable.SetCheck(BST_CHECKED);
  myCheckPlayer1TranspositionTable.EnableWindow(FALSE);
  myCheckPlayer2TranspositionTable.EnableWindow(FALSE);

  myEditMaxNoOfMoves.SetWindowText("300");

  return ret;
}

// NewGameDlg-Meldungshandler

void NewGameDlg::OnSelchangeComboStartFormation()
{
  myStartFormationComboBox.GetLBText(myStartFormationComboBox.GetCurSel(), myStartFormationStr);
}

void NewGameDlg::OnOK()
{
  bool ok = true;

  // initialize the players
  CString namePlayer1;
  CString namePlayer2;

  GetDlgItem(IDC_EDIT_NAME1)->GetWindowText(namePlayer1);
  GetDlgItem(IDC_EDIT_NAME2)->GetWindowText(namePlayer2);

  int radioTypePlayer1 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER1);
  int radioTypePlayer2 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER2);

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
      if (myCheckPlayer1IterativeDeepening.GetCheck() == BST_CHECKED) {
        typePlayer1 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING;
      }
      else {
        typePlayer1 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA;
      }
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
      if (myCheckPlayer2IterativeDeepening.GetCheck() == BST_CHECKED) {
        typePlayer2 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING;
      }
      else {
        typePlayer2 = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA;
      }
      break;
    case IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
      break;
    case IDC_RADIO_COMPUTER_EVALUATE_NEXT_MOVE_PLAYER2:
      typePlayer2 = Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
      break;
  }

  myGameManager->SetPlayers(namePlayer1, typePlayer1, namePlayer2, typePlayer2);

  // check the information about the players
  if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
    ComputerPlayerMonteCarlo* mcPlayer = static_cast<ComputerPlayerMonteCarlo*>(myGameManager->GetPlayer1());
    CString str;
    myEditPlayer1MCSimGames.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetSecondsForGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of simulated games for player 1.", true, false);
    }
    myEditPlayer1MCTurnsPerSimGame.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetTurnsPerSimGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of turns per simulated game for player 1.", true, false);
    }
    myEditPlayer1Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 1.", true, false);
    }
  }
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
    ComputerPlayerAlphaBeta* abPlayer = static_cast<ComputerPlayerAlphaBeta*>(myGameManager->GetPlayer1());
    abPlayer->SetUseTranspositionTable(myCheckPlayer1TranspositionTable.GetCheck() == BST_CHECKED);
    abPlayer->SetUseQuiescenceSearch(myCheckPlayer1QuiescenceSearch.GetCheck() == BST_CHECKED);
    CString str;
    myEditPlayer1ABTreeDepth.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      if (_ttoi(str) != DEFAULT_TREE_DEPTH) {
        abPlayer->SetTreeDepth(_ttoi(str));
      }
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of simulated games for player 2.", true, false);
    }
    myEditPlayer1Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 1.", true, false);
    }

  }
  else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
    ComputerPlayerAlphaBetaIterativeDeepening* abPlayer = static_cast<ComputerPlayerAlphaBetaIterativeDeepening*>(myGameManager->GetPlayer1());
    abPlayer->SetUseTranspositionTable(myCheckPlayer1TranspositionTable.GetCheck() == BST_CHECKED);
    abPlayer->SetUseQuiescenceSearch(myCheckPlayer1QuiescenceSearch.GetCheck() == BST_CHECKED);
    CString str;
    myEditPlayer1ABTimeForGame.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetSecondsForGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the allowed no. of seconds per move for player 1.", true, false);
    }
    myEditPlayer1Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 1.", true, false);
    }
  }

  if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
    ComputerPlayerMonteCarlo* mcPlayer = static_cast<ComputerPlayerMonteCarlo*>(myGameManager->GetPlayer2());
    CString str;
    myEditPlayer2MCSimGames.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetSecondsForGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of simulated games for player 2.", true, false);
    }
    myEditPlayer2MCTurnsPerSimGame.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetTurnsPerSimGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of turns per simulated game for player 2.", true, false);
    }
    myEditPlayer2Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      mcPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 2.", true, false);
    }

  }
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
    ComputerPlayerAlphaBeta* abPlayer = static_cast<ComputerPlayerAlphaBeta*>(myGameManager->GetPlayer2());
    abPlayer->SetUseTranspositionTable(myCheckPlayer2TranspositionTable.GetCheck() == BST_CHECKED);
    abPlayer->SetUseQuiescenceSearch(myCheckPlayer2QuiescenceSearch.GetCheck() == BST_CHECKED);
    CString str;
    myEditPlayer2ABTreeDepth.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      if (_ttoi(str) != DEFAULT_TREE_DEPTH) {
        abPlayer->SetTreeDepth(_ttoi(str));
      }
    }
    else {
      ok = false;
      Output::Message("Please enter the no. of simulated games for player 2.", true, false);
    }
    myEditPlayer2Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 2.", true, false);
    }
  }
  else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
    ComputerPlayerAlphaBetaIterativeDeepening* abPlayer = static_cast<ComputerPlayerAlphaBetaIterativeDeepening*>(myGameManager->GetPlayer2());
    abPlayer->SetUseTranspositionTable(myCheckPlayer2TranspositionTable.GetCheck() == BST_CHECKED);
    abPlayer->SetUseQuiescenceSearch(myCheckPlayer2QuiescenceSearch.GetCheck() == BST_CHECKED);
    CString str;
    myEditPlayer2ABTimeForGame.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetSecondsForGame(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the allowed no. of seconds per move for player 2.", true, false);
    }
    myEditPlayer2Evaluation.GetWindowText(str);
    if (!str.IsEmpty() && _ttoi(str) != 0) {
      abPlayer->SetUsedEvaluation(_ttoi(str));
    }
    else {
      ok = false;
      Output::Message("Please enter the evaluation function for player 2.", true, false);
    }
  }

  CString str;
  myEditMaxNoOfMoves.GetWindowText(str);
  if (!str.IsEmpty() && _ttoi(str) != 0) {
    myGameManager->SetMaxNumberOfTurns(_ttoi(str));
  }
  else {
    ok = false;
    Output::Message("Please enter the maximum no. of moves.", true, false);
  }

  if (ok) {
    CDialog::OnOK();
  }
}

void NewGameDlg::OnRadioPlayer1Changed()
{
  int radioTypePlayer1 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER1, IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER1);

  if (radioTypePlayer1 == IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER1 || radioTypePlayer1 == IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER1) {
    myStaticPlayer1Evaluation.EnableWindow(TRUE);
    myEditPlayer1Evaluation.EnableWindow(TRUE);
  }
  else {
    myStaticPlayer1Evaluation.EnableWindow(FALSE);
    myEditPlayer1Evaluation.EnableWindow(FALSE);
  }

  if (radioTypePlayer1 == IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER1) {
    myStaticPlayer1MCSimGames.EnableWindow(TRUE);
    myStaticPlayer1MCTurnsPerSimGame.EnableWindow(TRUE);
    myEditPlayer1MCSimGames.EnableWindow(TRUE);
    myEditPlayer1MCTurnsPerSimGame.EnableWindow(TRUE);
  }
  else {
    myStaticPlayer1MCSimGames.EnableWindow(FALSE);
    myStaticPlayer1MCTurnsPerSimGame.EnableWindow(FALSE);
    myEditPlayer1MCSimGames.EnableWindow(FALSE);
    myEditPlayer1MCTurnsPerSimGame.EnableWindow(FALSE);
  }

  if (radioTypePlayer1 == IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER1) {
    myCheckPlayer1TranspositionTable.EnableWindow(TRUE);
    myCheckPlayer1QuiescenceSearch.EnableWindow(TRUE);
    myCheckPlayer1IterativeDeepening.EnableWindow(TRUE);
    OnCheckIterativeDeepeningPlayer1Changed();
  }
  else {
    myCheckPlayer1TranspositionTable.EnableWindow(FALSE);
    myCheckPlayer1QuiescenceSearch.EnableWindow(FALSE);
    myCheckPlayer1IterativeDeepening.EnableWindow(FALSE);
    myStaticPlayer1ABTimeForGame.EnableWindow(FALSE);
    myEditPlayer1ABTimeForGame.EnableWindow(FALSE);
    myStaticPlayer1ABTreeDepth.EnableWindow(FALSE);
    myEditPlayer1ABTreeDepth.EnableWindow(FALSE);
  }
}

void NewGameDlg::OnRadioPlayer2Changed()
{
  int radioTypePlayer2 = GetCheckedRadioButton(IDC_RADIO_HUMAN_PLAYER2, IDC_RADIO_COMPUTER_RANDOM_MOVES_PLAYER2);

  if (radioTypePlayer2 == IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER2 || radioTypePlayer2 == IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER2) {
    myStaticPlayer2Evaluation.EnableWindow(TRUE);
    myEditPlayer2Evaluation.EnableWindow(TRUE);
  }
  else {
    myStaticPlayer2Evaluation.EnableWindow(FALSE);
    myEditPlayer2Evaluation.EnableWindow(FALSE);
  }

  if (radioTypePlayer2 == IDC_RADIO_COMPUTER_MONTE_CARLO_PLAYER2) {
    myStaticPlayer2MCSimGames.EnableWindow(TRUE);
    myStaticPlayer2MCTurnsPerSimGame.EnableWindow(TRUE);
    myEditPlayer2MCSimGames.EnableWindow(TRUE);
    myEditPlayer2MCTurnsPerSimGame.EnableWindow(TRUE);
  }
  else {
    myStaticPlayer2MCSimGames.EnableWindow(FALSE);
    myStaticPlayer2MCTurnsPerSimGame.EnableWindow(FALSE);
    myEditPlayer2MCSimGames.EnableWindow(FALSE);
    myEditPlayer2MCTurnsPerSimGame.EnableWindow(FALSE);
  }

  if (radioTypePlayer2 == IDC_RADIO_COMPUTER_ALPHA_BETA_PLAYER2) {
    myCheckPlayer2TranspositionTable.EnableWindow(TRUE);
    myCheckPlayer2QuiescenceSearch.EnableWindow(TRUE);
    myCheckPlayer2IterativeDeepening.EnableWindow(TRUE);
    OnCheckIterativeDeepeningPlayer2Changed();
  }
  else {
    myCheckPlayer2TranspositionTable.EnableWindow(FALSE);
    myCheckPlayer2QuiescenceSearch.EnableWindow(FALSE);
    myCheckPlayer2IterativeDeepening.EnableWindow(FALSE);
    myStaticPlayer2ABTimeForGame.EnableWindow(FALSE);
    myEditPlayer2ABTimeForGame.EnableWindow(FALSE);
    myStaticPlayer2ABTreeDepth.EnableWindow(FALSE);
    myEditPlayer2ABTreeDepth.EnableWindow(FALSE);
  }
}

void NewGameDlg::OnCheckIterativeDeepeningPlayer1Changed()
{
  if (myCheckPlayer1IterativeDeepening.GetCheck() == BST_CHECKED) {
    myStaticPlayer1ABTimeForGame.EnableWindow(TRUE);
    myEditPlayer1ABTimeForGame.EnableWindow(TRUE);
    myStaticPlayer1ABTreeDepth.EnableWindow(FALSE);
    myEditPlayer1ABTreeDepth.EnableWindow(FALSE);
  }
  else {
    myStaticPlayer1ABTimeForGame.EnableWindow(FALSE);
    myEditPlayer1ABTimeForGame.EnableWindow(FALSE);
    myStaticPlayer1ABTreeDepth.EnableWindow(TRUE);
    myEditPlayer1ABTreeDepth.EnableWindow(TRUE);
  }
}

void NewGameDlg::OnCheckIterativeDeepeningPlayer2Changed()
{
  if (myCheckPlayer2IterativeDeepening.GetCheck() == BST_CHECKED) {
    myStaticPlayer2ABTimeForGame.EnableWindow(TRUE);
    myEditPlayer2ABTimeForGame.EnableWindow(TRUE);
    myStaticPlayer2ABTreeDepth.EnableWindow(FALSE);
    myEditPlayer2ABTreeDepth.EnableWindow(FALSE);
  }
  else {
    myStaticPlayer2ABTimeForGame.EnableWindow(FALSE);
    myEditPlayer2ABTimeForGame.EnableWindow(FALSE);
    myStaticPlayer2ABTreeDepth.EnableWindow(TRUE);
    myEditPlayer2ABTreeDepth.EnableWindow(TRUE);
  }
}
