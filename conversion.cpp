#include "decodage.h"

int decodeMTHO2 (QString mtho2)
{

  //-------------------------------variables
  string globalstring = mtho2.toStdString();
  stringstream sstmsb = globalstring.substr(0, 8);
  stringstream sstlsb = globalstring.substr(8, 16);
  string hexatmsb;
  string hexatlsb;
  string hexastr;
  int temp;

//--------------------------------division de la chaine globale

 

//---------------------------------conversion bin-string ==> hex-sstream
  bitset<8> setM(tmsb);
  sstmsb << hex << setM.to_ulong() << endl;

  bitset<8> setL(tlsb);
  sstlsb << hex << setL.to_ulong() << endl;


//----------------------------------verification taille chaines(sachant que cette chaine est
//plus grande de 1 charactere pour d'obscures raisons)
  if (sstmsb.str().length() == 2)
  {
    hexatmsb = '0' + sstmsb.str();
  }
  else
  {
    hexatmsb = sstmsb.str();
  }

  if (sstlsb.str().length() == 2)
  {
    hexatlsb = '0' + sstlsb.str();
  }
  else
  {
    hexatlsb = sstlsb.str();
  }

//----------------------------------concatenation (et suppression des characteres mysteres)
hexatmsb = hexatmsb.substr(0, hexatmsb.length()-1);
hexatlsb = hexatlsb.substr(0, hexatlsb.length()-1);
hexastr = hexatmsb + hexatlsb;
cout << hexastr << ' ' << hexastr.size() << endl;

//-----------------------------------conversion hexa-str ==> decimal-int

temp = strtol(hexastr.c_str(), 0, 16);
//cout << temp << endl;
//-----------------------------------------------------------------
  return temp;
}
