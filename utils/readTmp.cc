#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;


string readTmp() {

  ifstream in("tmp", ios::in );

  string str;
  if(in) {
    in >> str;
    in.close();
  }
  else {
    cout<<" Error "<<endl;
    abort();
  }
  
  return str;
}
