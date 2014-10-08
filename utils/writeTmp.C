#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

void command(string str) {

  ofstream out("tmp", ios::out | ios::trunc );

  if(out) {
    out << str;
    out.close();
  }
  else {
    cout<<" Error "<<endl;
    abort();
  }
  
  gROOT->ProcessLine(".q");

}
