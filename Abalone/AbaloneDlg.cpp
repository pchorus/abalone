// AbaloneDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "Abalone.h"
#include "AbaloneDlg.h"
#include "NewGameDlg.h"

#include <math.h>

#include "AbaloneBase/GameManager.h"
#include "AbaloneBase/GameBoard.h"
#include "AbaloneBase/Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// global variables to start the computer player calculations in another thread
GameManager* gameManager = 0;


// this method must be called after a player finished his turn
UINT TurnIsOver(LPVOID pParam)
{
  gameManager->TurnIsOver(pParam);

  // TODO: PostMessage is called out of gameManager->TurnIsOver
  // it could be more expensive for MonteCarlo. If that is the case
  // we put this PostMessage here in the code and take it out of the GameManager

  //  ::PostMessage((HWND)pParam, WM_COMPUTER_CALC_FINISHED, 0, 0);
  return 0;
}


// CAboutDlg-Dialogfeld f�r Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAbaloneDlg-Dialogfeld


static const int BOARD_POINT_RADIUS = 4;

CAbaloneDlg::CAbaloneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAbaloneDlg::IDD, pParent)
, myGameManager(new GameManager)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  gameManager = myGameManager;
}

void CAbaloneDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_NAME_PLAYER1, myEditNamePlayer1);
  DDX_Control(pDX, IDC_EDIT_NAME_PLAYER2, myEditNamePlayer2);
  DDX_Control(pDX, IDC_STATIC_PLAYER1, myStaticNamePlayer1);
  DDX_Control(pDX, IDC_STATIC_PLAYER2, myStaticNamePlayer2);
  DDX_Control(pDX, IDC_STATIC_PLAYERS_TURN, myStaticPlayersTurn);
  DDX_Control(pDX, IDC_BTN_MOVE_UPLEFT, myBtnMoveUpLeft);
  DDX_Control(pDX, IDC_BTN_MOVE_UPRIGHT, myBtnMoveUpRight);
  DDX_Control(pDX, IDC_BTN_MOVE_LEFT, myBtnMoveLeft);
  DDX_Control(pDX, IDC_BTN_MOVE_RIGHT, myBtnMoveRight);
  DDX_Control(pDX, IDC_BTN_MOVE_DOWNLEFT, myBtnMoveDownLeft);
  DDX_Control(pDX, IDC_BTN_MOVE_DOWNRIGHT, myBtnMoveDownRight);
  DDX_Control(pDX, IDC_STATIC_LOST_BALLS, myGroupLostBalls);
}

BEGIN_MESSAGE_MAP(CAbaloneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_LBUTTONDOWN()
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_NEWGAME, &CAbaloneDlg::OnNewGame)
  ON_UPDATE_COMMAND_UI(ID_NEWGAME, &CAbaloneDlg::OnUpdateNewGame)
  ON_COMMAND(ID_CLOSE, &CAbaloneDlg::OnClose)
  ON_UPDATE_COMMAND_UI(ID_CLOSE, &CAbaloneDlg::OnUpdateClose)
  ON_BN_CLICKED(IDC_BTN_MOVE_UPLEFT, &CAbaloneDlg::OnBtnMoveUpLeft)
  ON_BN_CLICKED(IDC_BTN_MOVE_UPRIGHT, &CAbaloneDlg::OnBtnMoveUpRight)
  ON_BN_CLICKED(IDC_BTN_MOVE_LEFT, &CAbaloneDlg::OnBtnMoveLeft)
  ON_BN_CLICKED(IDC_BTN_MOVE_RIGHT, &CAbaloneDlg::OnBtnMoveRight)
  ON_BN_CLICKED(IDC_BTN_MOVE_DOWNLEFT, &CAbaloneDlg::OnBtnMoveDownLeft)
  ON_BN_CLICKED(IDC_BTN_MOVE_DOWNRIGHT, &CAbaloneDlg::OnBtnMoveDownRight)
  ON_MESSAGE(WM_COMPUTER_CALC_FINISHED, &CAbaloneDlg::ComputerCalcFinished)
END_MESSAGE_MAP()


// CAbaloneDlg-Meldungshandler

BOOL CAbaloneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzuf�gen des Men�befehls "Info..." zum Systemmen�.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol f�r dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Gro�es Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

  myBtnMoveUpLeft.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_UP_LEFT_ARROW)));
  myBtnMoveUpRight.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_UP_RIGHT_ARROW)));
  myBtnMoveLeft.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LEFT_ARROW)));
  myBtnMoveRight.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_RIGHT_ARROW)));
  myBtnMoveDownLeft.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DOWN_LEFT_ARROW)));
  myBtnMoveDownRight.SetIcon(::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DOWN_RIGHT_ARROW)));
  
	myStaticPlayersTurn.SetWindowText("");
  DisableDirectionButtons();

	return TRUE;  // Geben Sie TRUE zur�ck, au�er ein Steuerelement soll den Fokus erhalten
}

void CAbaloneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfl�che "Minimieren" hinzuf�gen, ben�tigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. F�r MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgef�hrt.

void CAbaloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Ger�tekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
    DrawBoard();
    if (myGameManager->IsGameStarted()) {
      DrawBallsAsidePlayerNames();
      DrawLostBalls();
      CString message;

      if (myGameManager->GetLostBallsPlayer1() == 6 || myGameManager->GetLostBallsPlayer2() == 6) {
        if (myGameManager->GetLostBallsPlayer1() == 6) {
          message = myGameManager->GetPlayer2()->GetName() + " wins!";
        }
        else if (myGameManager->GetLostBallsPlayer2() == 6) {
          message = myGameManager->GetPlayer1()->GetName() + " wins!";
        }
        AfxMessageBox(message, MB_ICONINFORMATION | MB_OK);
        myGameManager->SetGameStarted(false);
      }
    }
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, w�hrend der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CAbaloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAbaloneDlg::DrawBoard()
{
  CString hlp;
  CRect rect;
  GetClientRect(&rect);
  CDC* pDC = GetDC();
//  CPen linePen(PS_SOLID, 50, RGB(0,0,0));
  //CPen* oldPen = dc.SelectObject(&linePen);

  CRect boardRect(rect);
  if (boardRect.right > boardRect.bottom) {
    boardRect.right = boardRect.bottom;
  }
  else {
    boardRect.bottom = boardRect.right;
  }

  pDC->FillSolidRect(boardRect, RGB(255, 255, 255));

  CBrush brush(RGB(0, 0, 0));
  CBrush* oldBrush = pDC->SelectObject(&brush);

  int radius = GetBoardRadius();
  CPoint middle;
  middle.x = middle.y = boardRect.right / 2;
  long triangleHeight = LONG(radius * sqrt(3./4.));

  CPoint bottomLeft;
  CPoint bottomRight;
  CPoint topLeft;
  CPoint topRight;
  CPoint middleLeft;
  CPoint middleRight;
  CPoint currentPoint;

  GameBoard* gameBoard = myGameManager->GetGameBoard();

  // Initialize points
  bottomLeft.x = middle.x - radius / 2;
  bottomLeft.y = middle.y + triangleHeight;
  bottomRight.x = middle.x + radius / 2;
  bottomRight.y = middle.y + triangleHeight;
  middleLeft.x = middle.x - radius;
  middleLeft.y = middle.y;
  middleRight.x = middle.x + radius;
  middleRight.y = middle.y;
  topLeft.x = middle.x - radius / 2;
  topLeft.y = middle.y - triangleHeight;
  topRight.x = middle.x + radius / 2;
  topRight.y = middle.y - triangleHeight;
  // Au�enlinien zeichnen

  // links unten nach rechts unten (1A nach 5A)
  currentPoint.y = bottomLeft.y;
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomLeft.x + i * radius / 4;
    gameBoard->GetBoardField(i, 0)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);

    // Label
    CRect rect(currentPoint.x+8, currentPoint.y + 25, currentPoint.x + 28, currentPoint.y + 60);
    hlp.Format("%d", i+1);
    pDC->DrawText(hlp, rect, 0);
  }

  // rechts unten nach rechts mitte (5A nach 9E)
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomRight.x + i * radius / 8;
    currentPoint.y = bottomRight.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(i+4, i)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);

    // Label
    CRect rect(currentPoint.x+8, currentPoint.y + 25, currentPoint.x + 28, currentPoint.y + 60);
    hlp.Format("%d", i+5);
    pDC->DrawText(hlp, rect, 0);
  }

  // rechts mitte nach rechts oben (9E nach 9I)
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = middleRight.x - i * radius / 8;
    currentPoint.y = middleRight.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(8, i+4)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // links oben nach rechts oben (5I nach 9I)
  currentPoint.y = topLeft.y;
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = topLeft.x + i * radius / 4;
    gameBoard->GetBoardField(4+i, 8)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // links mitte nach links oben (1E nach 5I)
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = middleLeft.x + i * radius / 8;
    currentPoint.y = middleLeft.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(i, 4+i)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);

    // Label
    CRect rect(currentPoint.x - 40, currentPoint.y - 7, currentPoint.x - 20, currentPoint.y + 20);
    hlp = char(69+i);
    pDC->DrawText(hlp, rect, 0);
  }

  // links unten nach links mitte
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomLeft.x - i * radius / 8;
    currentPoint.y = bottomLeft.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(0, i)->SetGUICoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);

    // Label
    CRect rect(currentPoint.x - 40, currentPoint.y - 7, currentPoint.x - 20, currentPoint.y + 20);
    hlp = char(65+i);
    pDC->DrawText(hlp, rect, 0);
  }

  int xLeft = 0;
  int xRight = 0;
  CPoint leftPoint;
  CPoint rightPoint;
  // Querlinien zeichnen
  for (int y = 0; y < 9; ++y) {
    xLeft = y < 4 ? 0 : y - 4;
    xRight = y < 4 ? 4 + y : 8;
    
    leftPoint = gameBoard->GetBoardField(xLeft, y)->GetGUICoordinates();
    rightPoint = gameBoard->GetBoardField(xRight, y)->GetGUICoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);

    // draw points
    currentPoint.y = leftPoint.y;

    for (int x = xLeft; x < xRight; ++x) {
      currentPoint.x = leftPoint.x + (x - xLeft) * (rightPoint.x - leftPoint.x) / (xRight - xLeft);
      gameBoard->GetBoardField(x, y)->SetGUICoordinates(currentPoint);
      pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
        currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);

    }
  }

  // Linien von rechts unten nach links oben zeichnen
  int yTop = 0;
  int yBottom = 0;
  for (int x = 0; x < 9; ++x) {
    yTop = x < 4 ? 4 + x : 8;
    yBottom = x < 4 ? 0 : x - 4;
    
    leftPoint = gameBoard->GetBoardField(x, yTop)->GetGUICoordinates();
    rightPoint = gameBoard->GetBoardField(x, yBottom)->GetGUICoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  // Linien von links unten nach rechts oben zeichnen
  for (int x = 0; x < 5; ++x) {
    yTop = 8 - x;
    yBottom = 0;
    
    leftPoint = gameBoard->GetBoardField(x, yBottom)->GetGUICoordinates();
    rightPoint = gameBoard->GetBoardField(8, yTop)->GetGUICoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  for (int y = 0; y < 5; ++y) {
    xLeft = 0;
    xRight = 8 - y;
    
    leftPoint = gameBoard->GetBoardField(xLeft, y)->GetGUICoordinates();
    rightPoint = gameBoard->GetBoardField(xRight, 8)->GetGUICoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  DrawBalls();

  pDC->SelectObject(oldBrush);

  ReleaseDC(pDC);
}

void CAbaloneDlg::DrawBall(long x, long y, BallColor color)
{
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  CPoint p = gameBoard->GetBoardField(x, y)->GetGUICoordinates();

  DrawBall(p, color);
}

void CAbaloneDlg::DrawBall(CPoint point, BallColor color, int radius)
{
  CDC* pDC = GetDC();

  COLORREF colref(RGB(0, 0, 0));
  if (color == BALL_COLOR_WHITE) {
    colref = RGB(255, 255, 255);
  }
  else if (color == BALL_COLOR_BLACK_SELECTED) {
    colref = RGB(128, 128, 128);
  }
  else if (color == BALL_COLOR_WHITE_SELECTED) {
    colref = RGB(192, 192, 192);
  }

  CBrush brush(colref);
  CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
  CBrush* oldBrush = pDC->SelectObject(&brush);
  CPen* oldPen = pDC->SelectObject(&pen);

  int ballRadius = radius == -1 ? static_cast<int>(GetBoardRadius() / 4 * 0.45) : radius;

  pDC->Ellipse(point.x - ballRadius, point.y - ballRadius, point.x + ballRadius, point.y + ballRadius);

  pDC->SelectObject(oldBrush);
  pDC->SelectObject(oldPen);

  ReleaseDC(pDC);
}

CRect CAbaloneDlg::GetBoardRect() const
{
  CRect boardRect;
  GetClientRect(&boardRect);

  if (boardRect.right > boardRect.bottom) {
    boardRect.right = boardRect.bottom;
  }
  else {
    boardRect.bottom = boardRect.right;
  }

  return boardRect;
}

void CAbaloneDlg::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
  if (myGameManager->IsGameStarted() && myGameManager->GetPlayerForNextTurn()->GetType() == Player::PLAYER_TYPE_HUMAN) {
    // only react on the mouse click if it is a human player's turn

    GameBoard* gameBoard = myGameManager->GetGameBoard();
    BoardField* field(0);
    CPoint fieldPoint;
    double ballRadius = GetBoardRadius() / 4 * 0.45;
    BoardField::Ball nextTurnsBallColor = myGameManager->IsFirstPlayersTurn() ? BoardField::BLACK_BALL : BoardField::WHITE_BALL;

    for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
      for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
        field = gameBoard->GetBoardField(x, y);
        if (field) {
          fieldPoint = field->GetGUICoordinates();

          if (pow(double(point.x - fieldPoint.x), 2) + pow(double(point.y - fieldPoint.y), 2) < pow(ballRadius, 2) && field->GetBall() != BoardField::NO_BALL)
          {
            if (field->GetBall() == BoardField::WHITE_BALL && nextTurnsBallColor == BoardField::WHITE_BALL) {
              if (!field->IsSelected()) {
                if (!myGameManager->CanSelectBall(field)) {
                  myGameManager->ClearSelectedBalls();
                }
                DrawBall(x, y, BALL_COLOR_WHITE_SELECTED);
                myGameManager->AddSelectedBall(field);
              }
              else if (myGameManager->CanDeselectBall(field)) {
                DrawBall(x, y, BALL_COLOR_WHITE);
                myGameManager->RemoveSelectedBall(field);
              }
              Invalidate();
              EnableDirectionButtons();
            }
            else if (field->GetBall() == BoardField::BLACK_BALL && nextTurnsBallColor == BoardField::BLACK_BALL) {
              if (!field->IsSelected()) {
                if (!myGameManager->CanSelectBall(field)) {
                  myGameManager->ClearSelectedBalls();
                }
                DrawBall(x, y, BALL_COLOR_BLACK_SELECTED);
                myGameManager->AddSelectedBall(field);
              }
              else if (myGameManager->CanDeselectBall(field)) {
                DrawBall(x, y, BALL_COLOR_BLACK);
                myGameManager->RemoveSelectedBall(field);
              }
              Invalidate();
              EnableDirectionButtons();
            }
          }
        }
      }
    }
  }
}

void CAbaloneDlg::OnNewGame()
{
  NewGameDlg newGameDlg(myGameManager, this);

  if (newGameDlg.DoModal() == IDOK) {
    // new game starts
    GameBoard* gameBoard = myGameManager->GetGameBoard();

    gameBoard->Reset();
    myGameManager->SetBallFormation(newGameDlg.GetStartFormationStr());


    // Initialize the controls
    CString namePlayer = "Player 1";
    if (myGameManager->GetPlayer1()->GetType() == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
      namePlayer += " (Monte Carlo)";
    }
    else if (myGameManager->GetPlayer1()->GetType() == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
      namePlayer += " (Alpha Beta)";
    }
    else if (myGameManager->GetPlayer1()->GetType() == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
      namePlayer += " (Alpha Beta IterDeep)";
    }
    else if (myGameManager->GetPlayer1()->GetType() == Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES) {
      namePlayer += " (Random Moves)";
    }
    namePlayer += ":";
    myStaticNamePlayer1.SetWindowText(namePlayer);

    namePlayer = "Player 2";
    if (myGameManager->GetPlayer2()->GetType() == Player::PLAYER_TYPE_COMPUTER_MONTE_CARLO) {
      namePlayer += " (Monte Carlo)";
    }
    else if (myGameManager->GetPlayer2()->GetType() == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA) {
      namePlayer += " (Alpha Beta)";
    }
    else if (myGameManager->GetPlayer2()->GetType() == Player::PLAYER_TYPE_COMPUTER_ALPHA_BETA_ITERATIVE_DEEPENING) {
      namePlayer += " (Alpha Beta IterDeep)";
    }
    else if (myGameManager->GetPlayer2()->GetType() == Player::PLAYER_TYPE_COMPUTER_RANDOM_MOVES) {
      namePlayer += " (Random Moves)";
    }
    namePlayer += ":";
    myStaticNamePlayer2.SetWindowText(namePlayer);

    myEditNamePlayer1.SetWindowText(myGameManager->GetPlayer1()->GetName());
    myEditNamePlayer2.SetWindowText(myGameManager->GetPlayer2()->GetName());

    myStaticPlayersTurn.SetWindowText("It is " + myGameManager->GetPlayer1()->GetName() + "'s turn!");

    myGameManager->SetGameStarted(true);

    AfxBeginThread(TurnIsOver, GetSafeHwnd());
    Invalidate();
  }
}

void CAbaloneDlg::OnUpdateNewGame(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
}

void CAbaloneDlg::OnClose()
{
  CDialog::OnCancel();
}

void CAbaloneDlg::OnUpdateClose(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
}

void CAbaloneDlg::DrawBalls()
{
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  BoardField* field = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      field = gameBoard->GetBoardField(x, y);
      if (field) {
        if (field->GetBall() == BoardField::BLACK_BALL) {
          DrawBall(x, y, field->IsSelected() ? BALL_COLOR_BLACK_SELECTED : BALL_COLOR_BLACK);
        }
        else if (field->GetBall() == BoardField::WHITE_BALL) {
          DrawBall(x, y, field->IsSelected() ? BALL_COLOR_WHITE_SELECTED : BALL_COLOR_WHITE);
        }
      }
    }
  }
}

void CAbaloneDlg::DrawBallsAsidePlayerNames()
{
  CRect topHlp;
  CRect bottomHlp;

  // player 1
  myStaticNamePlayer1.GetWindowRect(&topHlp);
  myEditNamePlayer1.GetWindowRect(&bottomHlp);
  ScreenToClient(&topHlp);
  ScreenToClient(&bottomHlp);

  CPoint point;
  int radius = (bottomHlp.bottom - topHlp.top - 10) / 2;

  point.x = topHlp.left - radius - 20;
  point. y = (bottomHlp.bottom + topHlp.top) / 2;
  DrawBall(point, BALL_COLOR_BLACK, radius);

  // player 2
  myStaticNamePlayer2.GetWindowRect(&topHlp);
  myEditNamePlayer2.GetWindowRect(&bottomHlp);
  ScreenToClient(&topHlp);
  ScreenToClient(&bottomHlp);
  point.x = topHlp.left - radius - 20;
  point. y = (bottomHlp.bottom + topHlp.top) / 2;
  DrawBall(point, BALL_COLOR_WHITE, radius);

  // current player
  myStaticPlayersTurn.GetWindowRect(&topHlp);
  ScreenToClient(&topHlp);
  point.x = topHlp.left - radius - 20;
  point. y = (topHlp.bottom + topHlp.top) / 2;

  BallColor color;
  if (myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()) {
    color = BALL_COLOR_BLACK;
  }
  else {
    color = BALL_COLOR_WHITE;
  }

  DrawBall(point, color, radius);
}

void CAbaloneDlg::DrawLostBalls()
{
  CRect rect;
  CPoint point;
  myGroupLostBalls.GetWindowRect(&rect);
  ScreenToClient(&rect);

  rect.left += 10;
  rect.right -= 10;
  rect.top += 10;
  rect.bottom -= 10;

  int width = (rect.right - rect.left);

  int ballRadius = width / 12 - 4;

  point.x = rect.left + ballRadius + 4;
  int left = point.x;
  point.y = rect.top + ballRadius + 10;

  for (int i = 0; i < myGameManager->GetLostBallsPlayer1(); ++i) {
    point.x = left + i * (2 * ballRadius + 8);
    DrawBall(point, BALL_COLOR_BLACK, ballRadius);
  }

  point.y = rect.bottom - ballRadius - 10;

  for (int i = 0; i < myGameManager->GetLostBallsPlayer2(); ++i) {
    point.x = left + i * (2 * ballRadius + 8);
    DrawBall(point, BALL_COLOR_WHITE, ballRadius);
  }
}

void CAbaloneDlg::EnableDirectionButtons()
{
  myBtnMoveUpLeft.EnableWindow(myGameManager->IsPossibleDirection(UPLEFT));
  myBtnMoveUpRight.EnableWindow(myGameManager->IsPossibleDirection(UPRIGHT));
  myBtnMoveLeft.EnableWindow(myGameManager->IsPossibleDirection(LEFT));
  myBtnMoveRight.EnableWindow(myGameManager->IsPossibleDirection(RIGHT));
  myBtnMoveDownLeft.EnableWindow(myGameManager->IsPossibleDirection(DOWNLEFT));
  myBtnMoveDownRight.EnableWindow(myGameManager->IsPossibleDirection(DOWNRIGHT));
}

void CAbaloneDlg::DisableDirectionButtons()
{
  myBtnMoveUpLeft.EnableWindow(FALSE);
  myBtnMoveUpRight.EnableWindow(FALSE);
  myBtnMoveLeft.EnableWindow(FALSE);
  myBtnMoveRight.EnableWindow(FALSE);
  myBtnMoveDownLeft.EnableWindow(FALSE);
  myBtnMoveDownRight.EnableWindow(FALSE);
}

void CAbaloneDlg::OnBtnMoveUpLeft()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(UPLEFT);
  DisableDirectionButtons();
  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

void CAbaloneDlg::OnBtnMoveUpRight()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(UPRIGHT);
  DisableDirectionButtons();
  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

void CAbaloneDlg::OnBtnMoveLeft()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(LEFT);
  DisableDirectionButtons();
  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

void CAbaloneDlg::OnBtnMoveRight()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(RIGHT);
  DisableDirectionButtons();
  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

void CAbaloneDlg::OnBtnMoveDownLeft()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(DOWNLEFT);
  DisableDirectionButtons();
  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

void CAbaloneDlg::OnBtnMoveDownRight()
{
  Player* nextPlayer = myGameManager->GetPlayerForNextTurn() == myGameManager->GetPlayer1()
    ? myGameManager->GetPlayer2() : myGameManager->GetPlayer1();
  myStaticPlayersTurn.SetWindowText("It is " + nextPlayer->GetName() + "'s turn!");

  myGameManager->MoveBallsInDirection(DOWNRIGHT);
  DisableDirectionButtons();

  AfxBeginThread(TurnIsOver, GetSafeHwnd());
  Invalidate();
}

LRESULT CAbaloneDlg::ComputerCalcFinished(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
  EnableDirectionButtons();
  myStaticPlayersTurn.SetWindowText("It is " + myGameManager->GetPlayerForNextTurn()->GetName() + "'s turn!");
  Invalidate();

  return 0;
}

BOOL CAbaloneDlg::OnEraseBkgnd(CDC* pDC)
{
  return CDialog::OnEraseBkgnd(pDC);
}
