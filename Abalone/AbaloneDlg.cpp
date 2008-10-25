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


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

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
}

void CAbaloneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAbaloneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
ON_COMMAND(ID_NEWGAME, &CAbaloneDlg::OnNewGame)
ON_UPDATE_COMMAND_UI(ID_NEWGAME, &CAbaloneDlg::OnUpdateNewGame)
END_MESSAGE_MAP()


// CAbaloneDlg-Meldungshandler

BOOL CAbaloneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

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

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
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

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CAbaloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

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
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CAbaloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAbaloneDlg::DrawBoard()
{
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
  // Außenlinien zeichnen

  // links unten nach rechts unten (1A nach 5A)
  pDC->MoveTo(bottomLeft);
  pDC->LineTo(bottomRight);
  currentPoint.y = bottomLeft.y;
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomLeft.x + i * radius / 4;
    gameBoard->GetBoardField(i, 0)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // rechts unten nach rechts mitte (5A nach 9E)
  pDC->MoveTo(bottomRight);
  pDC->LineTo(middleRight);
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomRight.x + i * radius / 8;
    currentPoint.y = bottomRight.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(i+4, i)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // rechts mitte nach rechts oben (9E nach 9I)
  pDC->MoveTo(middleRight);
  pDC->LineTo(topRight);
  // Punkte berechnen
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = middleRight.x - i * radius / 8;
    currentPoint.y = middleRight.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(8, i+4)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // links oben nach rechts oben (5I nach 9I)
  pDC->MoveTo(topLeft);
  pDC->LineTo(topRight);
  currentPoint.y = topLeft.y;
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = topLeft.x + i * radius / 4;
    gameBoard->GetBoardField(4+i, 8)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // links mitte nach links oben (1E nach 5I)
  pDC->MoveTo(middleLeft);
  pDC->LineTo(topLeft);
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = middleLeft.x + i * radius / 8;
    currentPoint.y = middleLeft.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(i, 4+i)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  // links unten nach links mitte
  pDC->MoveTo(bottomLeft);
  pDC->LineTo(middleLeft);
  for (int i = 0; i < 5; ++i) {
    currentPoint.x = bottomLeft.x - i * radius / 8;
    currentPoint.y = bottomLeft.y - i * triangleHeight / 4;
    gameBoard->GetBoardField(0, i)->SetCoordinates(currentPoint);
    pDC->Ellipse(currentPoint.x-BOARD_POINT_RADIUS, currentPoint.y-BOARD_POINT_RADIUS,
      currentPoint.x+BOARD_POINT_RADIUS, currentPoint.y+BOARD_POINT_RADIUS);
  }

  int xLeft = 0;
  int xRight = 0;
  CPoint leftPoint;
  CPoint rightPoint;
  // Querlinien zeichnen
  for (int y = 0; y < 9; ++y) {
    xLeft = y < 4 ? 0 : y - 4;
    xRight = y < 4 ? 4 + y : 8;
    
    leftPoint = gameBoard->GetBoardField(xLeft, y)->GetCoordinates();
    rightPoint = gameBoard->GetBoardField(xRight, y)->GetCoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);

    // draw points
    currentPoint.y = leftPoint.y;

    for (int x = xLeft; x < xRight; ++x) {
      currentPoint.x = leftPoint.x + (x - xLeft) * (rightPoint.x - leftPoint.x) / (xRight - xLeft);
      gameBoard->GetBoardField(x, y)->SetCoordinates(currentPoint);
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
    
    leftPoint = gameBoard->GetBoardField(x, yTop)->GetCoordinates();
    rightPoint = gameBoard->GetBoardField(x, yBottom)->GetCoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  // Linien von links unten nach rechts oben zeichnen
  for (int x = 0; x < 5; ++x) {
    yTop = 8 - x;
    yBottom = 0;
    
    leftPoint = gameBoard->GetBoardField(x, yBottom)->GetCoordinates();
    rightPoint = gameBoard->GetBoardField(8, yTop)->GetCoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  for (int y = 0; y < 5; ++y) {
    xLeft = 0;
    xRight = 8 - y;
    
    leftPoint = gameBoard->GetBoardField(xLeft, y)->GetCoordinates();
    rightPoint = gameBoard->GetBoardField(xRight, 8)->GetCoordinates();
    pDC->MoveTo(leftPoint);
    pDC->LineTo(rightPoint);
  }

  DrawBalls();
//   DrawBall(0, 3, BALL_COLOR_WHITE);
//   myGameManager->GetGameBoard()->GetBoardField(0, 3)->SetBall(BoardField::WHITE_BALL);
//   DrawBall(5, 3, BALL_COLOR_BLACK);
//   myGameManager->GetGameBoard()->GetBoardField(5, 3)->SetBall(BoardField::BLACK_BALL);
//   DrawBall(5, 4, BALL_COLOR_WHITE);
//   myGameManager->GetGameBoard()->GetBoardField(5, 4)->SetBall(BoardField::WHITE_BALL);
//   DrawBall(4, 4, BALL_COLOR_WHITE);
//   myGameManager->GetGameBoard()->GetBoardField(4, 4)->SetBall(BoardField::WHITE_BALL);
//   DrawBall(4, 3, BALL_COLOR_WHITE);
//   myGameManager->GetGameBoard()->GetBoardField(4, 3)->SetBall(BoardField::WHITE_BALL);

  pDC->SelectObject(oldBrush);
}

void CAbaloneDlg::DrawBall(long x, long y, BallColor color)
{
  CDC* pDC = GetDC();
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  CPoint p = gameBoard->GetBoardField(x, y)->GetCoordinates();

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
  
  int ballRadius = static_cast<int>(GetBoardRadius() / 4 * 0.45);

  pDC->Ellipse(p.x - ballRadius, p.y - ballRadius, p.x + ballRadius, p.y + ballRadius);

  pDC->SelectObject(oldBrush);
  pDC->SelectObject(oldPen);
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

void CAbaloneDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (myGameManager->GetPlayerForNextTurn()->GetPlayerType() == Player::PLAYER_TYPE_HUMAN) {
    // only react on the mouse click if it is a human player's turn

    GameBoard* gameBoard = myGameManager->GetGameBoard();
    BoardField* field(0);
    CPoint fieldPoint;
    double ballRadius = GetBoardRadius() / 4 * 0.45;

    for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
      for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
        if (gameBoard->GetBoardFieldExist(x, y)) {
          field = gameBoard->GetBoardField(x, y);
          fieldPoint = field->GetCoordinates();

          if (pow(double(point.x - fieldPoint.x), 2) + pow(double(point.y - fieldPoint.y), 2) < pow(ballRadius, 2) && field->GetBall() != BoardField::NO_BALL)
          {
            if (field->GetBall() == BoardField::WHITE_BALL) {
              if (!field->IsSelected()) {
                DrawBall(x, y, BALL_COLOR_WHITE_SELECTED);
                field->SetIsSelected(true);
              }
              else {
                DrawBall(x, y, BALL_COLOR_WHITE);
                field->SetIsSelected(false);
              }

            }
            else if (field->GetBall() == BoardField::BLACK_BALL) {
              if (!field->IsSelected()) {
                DrawBall(x, y, BALL_COLOR_BLACK_SELECTED);
                field->SetIsSelected(true);
              }
              else {
                DrawBall(x, y, BALL_COLOR_BLACK);
                field->SetIsSelected(false);
              }
            }
          }
        }
      }
    }
  }
}

void CAbaloneDlg::OnNewGame()
{
  NewGameDlg newGameDlg;

  if (newGameDlg.DoModal() == IDOK) {
    // new game starts
    GameBoard* gameBoard = myGameManager->GetGameBoard();

    gameBoard->Reset();

    if (newGameDlg.GetStartFormationStr() == START_FORMATION_STR_STANDARD) {
      SetBallsStandardFormation();
    }
    else if (newGameDlg.GetStartFormationStr() == START_FORMATION_STR_BELGIAN_DAISY) {
      SetBallsBelgianDaisyFormation();
    }

    InvalidateRect(GetBoardRect());
    // TODO: now start the game
  }
}

void CAbaloneDlg::OnUpdateNewGame(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
}

void CAbaloneDlg::DrawBalls()
{
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  BoardField* field = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        field = gameBoard->GetBoardField(x, y);
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

void CAbaloneDlg::SetBallsStandardFormation()
{
  GameBoard* gameBoard = myGameManager->GetGameBoard();

  // set balls in the standard formation
  // white balls
  for (int x = 0; x < 6; ++x) {
    for (int y = 0; y < 2; ++y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        gameBoard->GetBoardField(x, y)->SetBall(BoardField::BLACK_BALL);
      }
    }
  }
  gameBoard->GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(3, 2)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(4, 2)->SetBall(BoardField::BLACK_BALL);

  // black balls
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = BOARD_FIELDS_ROW-1; y > BOARD_FIELDS_ROW-3; --y) {
      if (gameBoard->GetBoardFieldExist(x, y)) {
        gameBoard->GetBoardField(x, y)->SetBall(BoardField::WHITE_BALL);
      }
    }
  }
  gameBoard->GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(5, 6)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(6, 6)->SetBall(BoardField::WHITE_BALL);
}

void CAbaloneDlg::SetBallsBelgianDaisyFormation()
{
  GameBoard* gameBoard = myGameManager->GetGameBoard();
  gameBoard->GetBoardField(0, 0)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(0, 1)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(1, 0)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(1, 1)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(1, 2)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(2, 1)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);

  gameBoard->GetBoardField(3, 0)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(3, 1)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 0)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 1)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 2)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(5, 1)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(5, 2)->SetBall(BoardField::WHITE_BALL);

  gameBoard->GetBoardField(3, 6)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(3, 7)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 7)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(4, 8)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(5, 7)->SetBall(BoardField::WHITE_BALL);
  gameBoard->GetBoardField(5, 8)->SetBall(BoardField::WHITE_BALL);

  gameBoard->GetBoardField(6, 6)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(6, 7)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(7, 6)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(7, 7)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(7, 8)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(8, 7)->SetBall(BoardField::BLACK_BALL);
  gameBoard->GetBoardField(8, 8)->SetBall(BoardField::BLACK_BALL);
}
