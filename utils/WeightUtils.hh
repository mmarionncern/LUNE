#ifndef _WeightUtil_
#define _WeightUtil_


#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#include "TH1.h"
#include "TFile.h"

using namespace std;

struct wdbe {

  float xmin;
  float xmax;
  float ymin;
  float ymax;

  float weight;

};

class WeightUtils {

private:
  
  map<string, TH1*> _wHistos;
  map<string, TH1*>::iterator _itWH;
  
  map<string, wdbe > _wDB;
  map<string, wdbe >::iterator _itWDB;

  map<string, string> _wVar;
  map<string, pair<string,string> > _wVar2D;
  
  void addWeight( string wname, TH1* wh );

public:

  WeightUtils();
  ~WeightUtils();

  void reset();

  void addWeightH( string wname, string loc, string hname );
  void addWeightDB( string wname, string loc, string fname );
  
  float findWeight(string wname, float valx, float valy=-1);


  ClassDef(WeightUtils,0)  
};


#endif
