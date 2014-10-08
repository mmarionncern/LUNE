#ifndef _Dataset_
#define _Dataset_

#include <vector>
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TChain.h"

#include "core/Sample.hh"

using namespace std;


class Dataset {

private:

  string _name;
 
  vector<Sample> _samples;
  vector<pair<int,int> > _events;
  vector<float> _weights;
  vector<int> _nprocs;
  
  int _color;

  TChain* _chain;

  bool _isDataDriven;
  int _isFromCS;
  bool _isGhost;

  size_t _testNMax;

  bool _skiptree;

public:

  Dataset();
  Dataset(string name, int color);

  ~Dataset();

  void setName(string name){ _name=name;};
  void setColor(int ncol){ _color=ncol;};
  
  void addSample(string sname, string path, string dir, 
		 string treeName, string hname, float xSect,
		 float kFact, float lumi, float eqLumi);
  
  void isHistoAnalysis(bool skiptree);

  //access functions
  
  string getName(){ return _name;};
  int getColor(){ return _color;};

  bool isDataset(string name){return _name==name;};
 
  bool isDataDriven(){return _isDataDriven;};
  bool isFromCS(){return _isFromCS!=0;};
  int csCode(){return _isFromCS;};

  bool isGhost(){return _isGhost;};

  string findProcess(int event);
  
  float findWeight(int event);
  
  TTree* getTree() {return _chain;};
  int getNEvents() { return _chain->GetEntries(); };

  int getNProcEvent(int evt);

  void setNMax(size_t nmax);

  vector<string> getSamples();

private:

  float computeWeight(float nEvts, float xSect, float kFact,
		      float lumi, float& eqLumi);
  
  int getNProcEvents(string path, string dir, 
		     string sname, string hname);
  

  ClassDef(Dataset,0)
  
};

#endif
