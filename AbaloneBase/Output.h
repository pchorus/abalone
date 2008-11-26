//===========================================================================
//
// Klasse AIOutput
//   Klasse zum Protokollieren und Ausgeben von Ereignissen und Fehlern
//
//===========================================================================
#pragma once

#define LOG_FILE_MESSAGE             _T("Abalog.msg")
#define LOG_FILE_ERROR               _T("Abalog.err")

#define LOG_MESSAGE                  _T("Meldung")
#define LOG_ERROR                    _T("Fehler")

namespace Output {
  ABALONE_BASE_DLLINTERFACE void Init(const CString& outputDir);
  ABALONE_BASE_DLLINTERFACE void Message(const CString& message, bool messageBox, bool log);
  ABALONE_BASE_DLLINTERFACE void Error(const CString& message, bool messageBox, bool log);
  ABALONE_BASE_DLLINTERFACE bool YesNo(const CString& message);
}