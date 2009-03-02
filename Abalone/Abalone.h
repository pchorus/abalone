// Abalone.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole

#include "AbaloneBase/Player.h"
#include <map>

// CAbaloneApp:
// Siehe Abalone.cpp für die Implementierung dieser Klasse
//

class CAbaloneApp : public CWinApp
{
public:
	CAbaloneApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()

private:
  void ParseParam();
  void PlayBatchGame();
  Player::PlayerType GetPlayerType(const CString& playerType);
  bool FormationFileExists();
  void ShowHelpText();

  bool myIsBatch;
  bool myIsHelp;
  std::map<CString, CString> myCmdLineParams;
};

extern CAbaloneApp theApp;