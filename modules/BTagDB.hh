#ifndef _BTagDB_
#define _BTagDB_

#include <iostream>
#include <vector>
#include <string>

#include "TMath.h"

using namespace std;

class BTagDB {


public:
  
  BTagDB();
  BTagDB(string disc, string type);
  
  ~BTagDB();
  
  void configure(string disc, string type);

  float getBTagSF(double pt, double eta, int flav);
  float getBTagEff(double x, double eta, double disc, int flav);

private:

  string _disc;
  string _type;

  //CSVL ===============================
  float btagCSVLSF(double x, bool isb);
  float mistagCSVLSF(double x, double y);
  float btagCSVLEFF(double x, int isb);
  float mistagCSVLEFF(double x, double y);

};


#endif
