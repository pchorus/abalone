// Abalone.cpp : Definiert das Klassenverhalten f�r die Anwendung.
//

#include "stdafx.h"
#include "Abalone.h"
#include "AbaloneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAbaloneApp

BEGIN_MESSAGE_MAP(CAbaloneApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAbaloneApp-Erstellung

CAbaloneApp::CAbaloneApp()
{
}


// Das einzige CAbaloneApp-Objekt

CAbaloneApp theApp;


// CAbaloneApp-Initialisierung

BOOL CAbaloneApp::InitInstance()
{
	// InitCommonControlsEx() ist f�r Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder h�her zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden m�chten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Gr��e
	// der ausf�hrbaren Datei verringern m�chten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// �ndern Sie den Registrierungsschl�ssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: �ndern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CAbaloneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: F�gen Sie hier Code ein, um das Schlie�en des
		//  Dialogfelds �ber "OK" zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: F�gen Sie hier Code ein, um das Schlie�en des
		//  Dialogfelds �ber "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zur�ckliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
