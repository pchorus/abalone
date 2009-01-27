#include "AbaloneBase.h"

#include "GameBoard.h"

#include <fstream>
#include <string>

GameBoard::GameBoard()
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y] = new BoardField(x, y);
    }
  }
}

GameBoard::GameBoard(const GameBoard& other)
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y] = new BoardField(x, y);
      *myFieldPoints[x][y] = *other.GetBoardField(x, y);
    }
  }
}

GameBoard::~GameBoard()
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      delete myFieldPoints[x][y];
    }
  }
}

bool GameBoard::GetBoardFieldExist(int x, int y) const
{
  bool ret = true;

  if (y > x + 4 || y < x - 4 || y < 0 || y > 8 || x < 0 || x > 8) {
    ret = false;
  }

  return ret;
}

bool GameBoard::GetBoardFieldExist(CPoint point) const
{
  return GetBoardFieldExist(point.x, point.y);
}

void GameBoard::Reset()
{

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myFieldPoints[x][y]->SetBall(BoardField::NO_BALL);
      myFieldPoints[x][y]->SetIsSelected(false);
    }
  }
}

CString GameBoard::ToString()
{
  CString line = "";
  for (int y = 8; y >= 0; --y) {
    for (int x = 0; x < 9; ++x) {
      if (myFieldPoints[x][y]->GetBall() == BoardField::NO_BALL) {
        line += "-";
      }
      else if (myFieldPoints[x][y]->GetBall() == BoardField::WHITE_BALL) {
        line += "W";
      }
      else if (myFieldPoints[x][y]->GetBall() == BoardField::BLACK_BALL) {
        line += "B";
      }
      line += " ";
    }

    line += "\n";
  }

  return line;
}

GameBoard& GameBoard::operator=(const GameBoard& other)
{
  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      *myFieldPoints[x][y] = *other.myFieldPoints[x][y];
    }
  }

  return *this;
}

void GameBoard::CopyBoardFields(const GameBoard* other)
{
  BoardField* myField = 0;
  BoardField* othersField = 0;

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = 0; y < BOARD_FIELDS_ROW; ++y) {
      myField = myFieldPoints[x][y];
      othersField = other->myFieldPoints[x][y];

      myField->SetBall(othersField->GetBall());
      myField->SetFieldCoordinates(othersField->GetFieldCoordinates());
      myField->SetGUICoordinates(othersField->GetGUICoordinates());
      myField->SetIsSelected(othersField->IsSelected());
    }
  }
}

void GameBoard::SetBallFormation(const CString& formation)
{
  if (formation == START_FORMATION_STR_STANDARD) {
    SetBallsStandardFormation();
  }
  else if (formation == START_FORMATION_STR_BELGIAN_DAISY) {
    SetBallsBelgianDaisyFormation();
  }
  else {
    SetBallsCustomFormation(formation);
  }
}

void GameBoard::SetBallsStandardFormation()
{
  // set balls in the standard formation
  for (int x = 0; x < 6; ++x) {
    for (int y = 0; y < 2; ++y) {
      if (GetBoardFieldExist(x, y)) {
        GetBoardField(x, y)->SetBall(BoardField::BLACK_BALL);
      }
    }
  }
  GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(3, 2)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(4, 2)->SetBall(BoardField::BLACK_BALL);

  for (int x = 0; x < BOARD_FIELDS_COLUMN; ++x) {
    for (int y = BOARD_FIELDS_ROW-1; y > BOARD_FIELDS_ROW-3; --y) {
      if (GetBoardFieldExist(x, y)) {
        GetBoardField(x, y)->SetBall(BoardField::WHITE_BALL);
      }
    }
  }
  GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(5, 6)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(6, 6)->SetBall(BoardField::WHITE_BALL);
}

void GameBoard::SetBallsBelgianDaisyFormation()
{
  GetBoardField(0, 0)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(0, 1)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(1, 0)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(1, 1)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(1, 2)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(2, 1)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(2, 2)->SetBall(BoardField::BLACK_BALL);

  GetBoardField(3, 0)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(3, 1)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 0)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 1)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 2)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(5, 1)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(5, 2)->SetBall(BoardField::WHITE_BALL);

  GetBoardField(3, 6)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(3, 7)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 6)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 7)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(4, 8)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(5, 7)->SetBall(BoardField::WHITE_BALL);
  GetBoardField(5, 8)->SetBall(BoardField::WHITE_BALL);

  GetBoardField(6, 6)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(6, 7)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(7, 6)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(7, 7)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(7, 8)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(8, 7)->SetBall(BoardField::BLACK_BALL);
  GetBoardField(8, 8)->SetBall(BoardField::BLACK_BALL);
}

void GameBoard::SetBallsCustomFormation(const CString& formation)
{
  if (!formation.IsEmpty()) {
    int curPos = 0;
    int curLine = 9;
    int curColumn = 0;
    int boardY = 0;
    CString file = "formations\\" + formation += ".txt";
    CString cStr;
    CString curStr;
    std::ifstream input;
    input.open(file, std::ios_base::in);

    std::string str;

    while (!input.eof()) {
      std::getline(input, str);
      --curLine;
      curColumn = 0;
      curPos = 0;

      if (!str.empty()) {
        cStr = str.data();
        cStr.Trim();
        curStr = cStr.Tokenize(_T(" "), curPos);

        while (!curStr.IsEmpty()) {
          if (curLine > 4) {
            boardY = curLine - 4 + curColumn;
          }
          else {
            boardY = curColumn;
          }

          switch (curStr.GetAt(0))
          {
          case 'w':
          case 'W':
            GetBoardField(boardY, curLine)->SetBall(BoardField::WHITE_BALL);
            break;
          case 'b':
          case 'B':
            GetBoardField(boardY, curLine)->SetBall(BoardField::BLACK_BALL);
            break;
          }

          curStr = cStr.Tokenize(_T(" "), curPos);
          ++curColumn;
        }
      }
    }

    if (input.is_open()) {
      input.close();
    }
  }
}
