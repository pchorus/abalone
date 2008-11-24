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
  // �ffnet die Eingabe- und Ausgabedatei
  void Open();
  // Schlie�t die Eingabe- und Ausgabedatei
  void Close();
  // Pr�ft, ob die Dateien ge�ffnet sind
  bool IsOpen() const;
  // Pr�ft, ob die Dateiverarbeitung weiterhin fehlerfrei verl�uft
  bool IsGood() const;
  // Pr�ft, ob Eingabedatei zu Ende ist
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
  // Pr�ft, ob Input und Output ge�ffnet werden konnten
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
