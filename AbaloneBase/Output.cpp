#include "AbaloneBase.h"

#include "Output.h"

#define AI_DOC_APP_NAME       _T("Abalone")

namespace Output {
  static FILE* myMessageFile(0);
  static FILE* myErrorFile(0);
  static CString myOutputDir;


  void Write(const CString& message, bool messageBox, bool log, const CString& outputFile)
  {
    CString time;
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    CString messageCaption;
    UINT messageFlags = 0;
    errno_t err = 0;

    if (outputFile == LOG_FILE_MESSAGE) {
      messageCaption = LOG_MESSAGE;
      if (log) {
        err = fopen_s(&myMessageFile, myOutputDir + "\\" + LOG_FILE_MESSAGE, _T("a"));
        if (err == 0) {
          fputs(message, myMessageFile);
          fclose(myMessageFile);
          myMessageFile = 0;
        }
      }
    }
    else if (outputFile == LOG_FILE_MESSAGE_2) {
      messageCaption = LOG_MESSAGE;
      if (log) {
        err = fopen_s(&myMessageFile, myOutputDir + "\\" + LOG_FILE_MESSAGE_2, _T("a"));
        if (err == 0) {
          fputs(message, myMessageFile);
          fclose(myMessageFile);
          myMessageFile = 0;
        }
      }
    }
    else if (outputFile == LOG_FILE_ERROR) {
      messageCaption = LOG_ERROR;
      if (log) {
        err = fopen_s(&myErrorFile, myOutputDir + "\\" + LOG_FILE_ERROR, _T("a"));
        if (err == 0) {
          fputs(message, myErrorFile);
          fclose(myErrorFile);
          myErrorFile = 0;
        }
      }
    }

    if (messageBox) {
      messageFlags = messageCaption == LOG_MESSAGE ? MB_ICONINFORMATION : MB_ICONERROR;
      messageFlags |= MB_OK;
      ::MessageBox(GetActiveWindow(), message, messageCaption, messageFlags);
    }
  }

  void Init(const CString& outputDir)
  {
    myOutputDir = outputDir;
  }

  void Message(const CString& message, bool messageBox, bool log)
  {
    Write(message, messageBox, log, LOG_FILE_MESSAGE);
  }

  void Message2(const CString& message, bool messageBox, bool log)
  {
    Write(message, messageBox, log, LOG_FILE_MESSAGE_2);
  }

  void Error(const CString& message, bool messageBox, bool log)
  {
    Write(message, messageBox, log, LOG_FILE_ERROR);
  }

  bool YesNo(const CString& message)
  {
    UINT messageFlags = MB_YESNO | MB_ICONQUESTION;
    int ret = IDNO;
    HWND hWnd = GetActiveWindow();
    if (hWnd) {
      ret = ::MessageBox(hWnd, message, AI_DOC_APP_NAME, messageFlags);
    }
    return ret == IDYES;
  }
}