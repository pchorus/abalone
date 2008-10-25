// AbaloneDlg.h : Headerdatei
//

#pragma once

#include "AbaloneBase/BoardField.h"

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
	DECLARE_MESSAGE_MAP()

  // Spielfeld malen
  void DrawBoard();
  void DrawBalls();
  void DrawBall(long x, long y, BallColor color);

private:
  CRect GetBoardRect() const;
  int GetBoardRadius() const;
  void SetBallsStandardFormation();
  void SetBallsBelgianDaisyFormation();

  // the GameManager
  GameManager* myGameManager;
};

inline int CAbaloneDlg::GetBoardRadius() const
{
  return (GetBoardRect().right - 40) / 2;
}
