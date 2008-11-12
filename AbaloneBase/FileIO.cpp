/////////////////////////////////////////////////////////////////////
//
// Klasse FileIO Implementierung
//
/////////////////////////////////////////////////////////////////////

#include "AbaloneBase.h"

#include "FileIO.h"

#include <string>

using namespace std;


FileIO::FileIO(string filename)
: myFilename(filename)
, myInput()
, myOutput()
{
}

FileIO::~FileIO()
{
  Close();
}

void FileIO::Open()
{
//   myInput.open(myFilename.data(), ios_base::in);
//   // schlägt der Aufruf der open-Funktion fehl, ist das Failbit
//   // gesetzt. Der bool-Operator der Stream-Klasse fragt dieses Bit ab
//   if (!myInput) {
//      ASSERT(false);
//   }

  // Die Ausgabedatei hat den gleichen Namen wie die Eingabedatei,
  // als Endung wird "out" angehängt
  string outFilename(myFilename);
//  outFilename += ".out";
  // Wenn die Ausgabedatei vorhanden ist, wird diese überschrieben, 
  // sonst wird eine neue Datei erzeugt
  myOutput.open(outFilename.data(), ios_base::out);

  // schlägt der Aufruf der open-Funktion fehl, ist das Failbit
  // gesetzt. Der bool-Operator der Stream-Klasse fragt dieses Bit ab
  if (!myOutput) {
    ASSERT(false);
  }
}

void FileIO::Close()
{
  if (myInput.is_open()) {
    myInput.close();
  }
  if (myOutput.is_open()) {
    myOutput.close();
  }
}

string FileIO::ReadLine()
{
  string str;
  getline(myInput, str);
  return str;
}

void FileIO::WriteLine(std::string str)
{
  myOutput << str << endl;
}
