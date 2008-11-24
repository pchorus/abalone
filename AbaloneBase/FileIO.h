#pragma once

/////////////////////////////////////////////////////////////////////
//
// Klasse FileIO
//   zum Lesen der Eingabedatei und Schreiben der Ausgabedatei
//
/////////////////////////////////////////////////////////////////////

#include <fstream>

class ABALONE_BASE_DLLINTERFACE FileIO {
public:
  // Konstruktor, Parameter ist der Name der Eingabedatei
  FileIO(std::string filename);
  // Destruktor
  ~FileIO();
  // Öffnet die Eingabe- und Ausgabedatei
  void Open();
  // Schließt die Eingabe- und Ausgabedatei
  void Close();
  // Prüft, ob die Dateien geöffnet sind
  bool IsOpen() const;
  // Prüft, ob die Dateiverarbeitung weiterhin fehlerfrei verläuft
  bool IsGood() const;
  // Prüft, ob Eingabedatei zu Ende ist
  bool IsInputEOF() const;
  // Zeile aus der Eingabedatei lesen
  std::string ReadLine();
  // Zeile in die Ausgabedatei schreiben
  void WriteLine(std::string str);

private:
  // Kopierkonstruktor darf nicht benutzt werden
  FileIO(const FileIO&);
  // Zuweisungsoperator darf nicht benutzt werden
  const FileIO& operator=(const FileIO&);

  std::string myFilename;
  std::ifstream myInput;
  std::ofstream myOutput;
};

inline bool FileIO::IsOpen() const
{
  // Prüft, ob Input und Output geöffnet werden konnten
  return myInput.is_open() && myOutput.is_open();
}

inline bool FileIO::IsGood() const
{
  // Good-Bit ist nur gesetzt, wenn kein Fehlerbit gesetzt ist.
  // Ist das EOF-Bit gesetzt, ist trotzdem alles in Ordnung
  return (myInput.good() || IsInputEOF()) && myOutput.good();
}

inline bool FileIO::IsInputEOF() const
{
  return myInput.eof();
}
