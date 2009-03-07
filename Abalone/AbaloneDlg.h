// AbaloneDlg.h : Headerdatei
//

#pragma once

#include "AbaloneBase/BoardField.h"
#include "afxwin.h"

class GameManager;

// CAbaloneDlg-Dialogfeld
class CAbaloneDlg : public CDialog
{
// Konstruktion
public:
	CAbaloneDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_ABALONE_DIALOG };

  enum BallColor { BALL_COLOR_WHITE = 0,
                   BALL_COLOR_BLACK = 1,
                   BALL_COLOR_BLACK_SELECTED = 2,
                   BALL_COLOR_WHITE_SELECTED = 3
                 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnNewGame();
  afx_msg void OnUpdateNewGame(CCmdUI *pCmdUI);
  afx_msg void OnClose();
  afx_msg void OnUpdateClose(CCmdUI *pCmdUI);
  afx_msg void OnBtnMoveUpLeft();
  afx_msg void OnBtnMoveUpRight();
  afx_msg void OnBtnMoveLeft();
  afx_msg void OnBtnMoveRight();
  afx_msg void OnBtnMoveDownLeft();
  afx_msg void OnBtnMoveDownRight();
  afx_msg LRESULT ComputerCalcFinished(WPARAM wParam, LPARAM lParam);

  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

  // Spielfeld malen
  void DrawBoard();
  void DrawBalls();
  // draws a ball on the gameboard with x and y as board coordinates
  void DrawBall(long x, long y, BallColor color);
  // draws a ball at the point in the ClientRect, radius for the balls on the board
  // is calculated, otherwise you can assign a radius in the parameter
  void DrawBall(CPoint point, BallColor color, int radius = -1);
  void DrawBallsAsidePlayerNames();
  void DrawLostBalls();
  // enable/disable direction buttons
  void EnableDirectionButtons();
  // disable direction buttons
  void DisableDirectionButtons();

private:
  CRect GetBoardRect() const;
  int GetBoardRadius() const;

  // the GameManager
  GameManager* myGameManager;

  // Controls
  CStatic myStaticNamePlayer1;
  CStatic myStaticNamePlayer2;
  CStatic myGroupLostBalls;
  CEdit myEditNamePlayer1;
  CEdit myEditNamePlayer2;
  CStatic myStaticPlayersTurn;
  CButton myBtnMoveUpLeft;
  CButton myBtnMoveUpRight;
  CButton myBtnMoveLeft;
  CButton myBtnMoveRight;
  CButton myBtnMoveDownLeft;
  CButton myBtnMoveDownRight;
};

inline int CAbaloneDlg::GetBoardRadius() const
{
  return (GetBoardRect().right - 100) / 2;
}
