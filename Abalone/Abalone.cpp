// Abalone.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"

#include <string>
#include <fstream>
#include <iostream>

#include "Abalone.h"
#include "AbaloneDlg.h"

#include "AbaloneBase/Output.h"
#include "AbaloneBase/GameManager.h"
#include "AbaloneBase/GameBoard.h"
#include "AbaloneBase/ComputerPlayerMonteCarlo.h"
#include "AbaloneBase/ComputerPlayerAlphaBeta.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAbaloneApp

BEGIN_MESSAGE_MAP(CAbaloneApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAbaloneApp-Erstellung

CAbaloneApp::CAbaloneApp()
: myIsBatch(false)
, myIsHelp(false)
, myCmdLineParams()
{
}


// Das einzige CAbaloneApp-Objekt

CAbaloneApp theApp;


// CAbaloneApp-Initialisierung

BOOL CAbaloneApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

  time_t sec;
  time(&sec);
  srand( (unsigned) sec);
  CHAR* buffer = new CHAR[256];
  GetCurrentDirectory((DWORD)256, buffer);
  Output::Init(buffer);

  // process the program parameters
  ParseParam();

  if (myIsHelp) {
    ShowHelpText();
  }
  else if (myIsBatch) {
    PlayBatchGame();
  }
  else {
	  CAbaloneDlg dlg;
	  m_pMainWnd = &dlg;
	  INT_PTR nResponse = dlg.DoModal();
	  if (nResponse == IDOK)
	  {
		  // TODO: Fügen Sie hier Code ein, um das Schließen des
		  //  Dialogfelds über "OK" zu steuern
	  }
	  else if (nResponse == IDCANCEL)
	  {
		  // TODO: Fügen Sie hier Code ein, um das Schließen des
		  //  Dialogfelds über "Abbrechen" zu steuern
	  }
  }

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}

void CAbaloneApp::ParseParam()
{
  CString key;
  CString value;

  CString cmdLine = m_lpCmdLine;

  int idx = 0;
  CString token = cmdLine.Tokenize("-", idx);

  while (idx != -1) {
    token.Trim();

    if (token.Find('=') != -1) {
      int idxEq = 0;
      key = token.Tokenize("=", idxEq);
      value = token.Tokenize("=", idxEq);
      myCmdLineParams[key] = value;
    }
    else if (token == "Batch") {
      myIsBatch = true;
    }
    else if (token == "?") {
      myIsHelp = true;
    }

    token = cmdLine.Tokenize("-", idx);
  }
}

void CAbaloneApp::PlayBatchGame()
{
  CString hlp;
  CString msg;
  CString header;
  CString namePlayer1 = "Player1 (" + myCmdLineParams["Player1"] + ")";
  CString namePlayer2 = "Player2 (" + myCmdLineParams["Player2"] + ")";

  GameManager gameManager;

  // new game starts
  GameBoard* gameBoard = gameManager.GetGameBoard();
  
  hlp = myCmdLineParams["Formation"];
  if (hlp == START_FORMATION_STR_STANDARD || hlp == START_FORMATION_STR_BELGIAN_DAISY || FormationFileExists()) {
    gameBoard->Reset();
    gameManager.SetBallFormation(hlp);

    Player::PlayerType typePlayer1 = Player::PLAYER_TYPE_NONE;
    Player::PlayerType typePlayer2 = Player::PLAYER_TYPE_NONE;

    typePlayer1 = GetPlayerType(myCmdLineParams["Player1"]);
    typePlayer2 = GetPlayerType(myCmdLineParams["Player2"]);

    if (typePlayer1 != Player::PLAYER_TYPE_NONE && typePlayer2 != Player::PLAYER_TYPE_NONE) {
      gameManager.SetPlayers(namePlayer1, typePlayer1,
                             namePlayer2, typePlayer2);

      // set the players' configuration
      if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
        ComputerPlayerMonteCarlo* mcPlayer = static_cast<ComputerPlayerMonteCarlo*>(gameManager.GetPlayer1());
        hlp = myCmdLineParams["SimGames1"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0) {
          mcPlayer->SetGamesToSimulate(_ttoi(hlp));
        }
        hlp = myCmdLineParams["TurnsPerSim1"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0) {
          mcPlayer->SetTurnsPerSimGame(_ttoi(hlp));
        }
      }
      else if (typePlayer1 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
        ComputerPlayerAlphaBeta* abPlayer = static_cast<ComputerPlayerAlphaBeta*>(gameManager.GetPlayer1());
        hlp = myCmdLineParams["TreeDepth1"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0 && _ttoi(hlp) != DEFAULT_TREE_DEPTH) {
          abPlayer->SetTreeDepth(_ttoi(hlp));
        }
      }

      if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
        ComputerPlayerMonteCarlo* mcPlayer = static_cast<ComputerPlayerMonteCarlo*>(gameManager.GetPlayer2());
        hlp = myCmdLineParams["SimGames2"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0) {
          mcPlayer->SetGamesToSimulate(_ttoi(hlp));
        }
        hlp = myCmdLineParams["TurnsPerSim2"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0) {
          mcPlayer->SetTurnsPerSimGame(_ttoi(hlp));
        }
      }
      else if (typePlayer2 == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
        ComputerPlayerAlphaBeta* abPlayer = static_cast<ComputerPlayerAlphaBeta*>(gameManager.GetPlayer2());
        hlp = myCmdLineParams["TreeDepth2"];
        if (!hlp.IsEmpty() && _ttoi(hlp) != 0 && _ttoi(hlp) != DEFAULT_TREE_DEPTH) {
          abPlayer->SetTreeDepth(_ttoi(hlp));
        }
      }

      gameManager.SetGameStarted(true);
      gameManager.SetMaxNumberOfTurns(300);
      gameManager.TurnIsOver();

      header =  "=========================\n";
      header += "===== Game is over! =====\n";
      header += "=========================\n";

      msg.Format("%s  Lost Balls %s:\t%d\n  Lost Balls %s:\t%d\n",
        header,
        namePlayer1, gameManager.GetLostBallsPlayer1(),
        namePlayer2, gameManager.GetLostBallsPlayer2());
      Output::Message(msg, false, true);
      Output::Message2(gameManager.GetGameBoard()->ToString(), false, true);
    }
    else {
      header =  "=============================================\n";
      header += "===== GAME CANCELLED, WRONG PLAYER TYPE =====\n";
      header += "=============================================\n";
      msg.Format("%s  %s\n  %s\n", header, namePlayer1, namePlayer2);
      Output::Message(msg, false, true);
    }
  }
}

Player::PlayerType CAbaloneApp::GetPlayerType(const CString& playerType)
{
  Player::PlayerType ret = Player::PLAYER_TYPE_NONE;
  
  if (playerType == "MC") {
    ret = Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO;
  }
  else if (playerType == "AB") {
    ret = Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA;
  }
  else if (playerType == "RM") {
    ret = Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES;
  }
  else if (playerType == "ENM") {
    ret = Player::PLAYER_TYPE_COMPUTER_EVALUATE_NEXT_MOVE;
  }

  ASSERT(ret != Player::PLAYER_TYPE_NONE);
  return ret;
}

bool CAbaloneApp::FormationFileExists()
{
  bool ret = true;
  CString file = "formations\\" + myCmdLineParams["Formation"] += ".txt";
  std::ifstream input;
  input.open(file, std::ios_base::in);

  if (!input.good()) {
    CString header;
    CString msg;
    header =  "=========================================================\n";
    header += "===== GAME CANCELLED, COULD NOT OPEN FORMATION FILE =====\n";
    header += "=========================================================\n";
    msg.Format("%s  Filename: %s", header, file);
    Output::Message(msg, false, true);
    ret = false;
  }
  input.close();

  return ret;
}

void CAbaloneApp::ShowHelpText()
{
  CString msg("Please use the following parameters to run the program in batch mode:\n\n");
  msg += "  -Batch\t\tenables the batch mode\n";
  msg += "  -Player1=XX\tsets the player type for player1\n";
  msg += "  -Player2=XX\tsets the player type for player2\n";
  msg += "  -Formation=XXX\tsets the start formation for the game\n";
  msg += "\n";
  msg += "\n";
  msg += "Player types:\n";
  msg += "\n";
  msg += "  - AB\tAlpha Beta search\n";
  msg += "  - MC\tMonte Carlo search\n";
  msg += "  - ENM\tEvaluate the next move\n";
  msg += "  - RM\tRandom Moves";
  msg += "\n";
  msg += "\n";
  msg += "start formations:\n";
  msg += "\n";
  msg += "  - Standard\n";
  msg += "  - Belgian Daisy\n";
  msg += "  - or custom formation, file in the \"formations\\\"-folder\n";

  AfxMessageBox(msg);
}
