#ifndef _HistoMan_
#define _HistoMan_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include "utils/Format.cc"

using namespace std;

//typedef pair<string,string> legend;
//typedef map<string,hObs> systM;
//typedef map<string,hObs>::const_iterator itSystM;


class HistoManager {

private:
  
  //histograms
  map< string, hObs > _variables;
  map< string, hObs >::iterator _itVar; 
  map< string, hObs >::const_iterator _cItVar; 

  //backgrounds
  vector<map< string, hObs > > _varDDBkg;
  size_t _nDDBkg;
  vector<int> _ddStatus;
  vector<pair<string,float> > _csData;

  //initialization
  bool _initsequence;
  
  vector<string> _dsNames;
  size_t _nds;
  
  string _hname;
  
  //==============================================
  //Functions

public:

  //cosntructor & destructor

  HistoManager();
  ~HistoManager();

  //initilaization
  void configAnalysis(vector<string> datasets, int nddBkg, 
		      vector<int> ddStatus, vector<pair<string,float> > csData);
  
  void startFilling() {_initsequence=false;};
  bool getInitStatus() {return _initsequence;};
  void initStatus() {_initsequence=true;};

  //reset
  void reset();

  //Histogram access and booking
  
  void addVariable(string var, int nBin, float min, float max, 
		   string Xleg, bool prof=false, string type="m");
  void addVariable(string var, int nBin, vector<float> bins,
		   string Xleg, bool prof=false, string type="m");
  void addVariable(string var, int nBinX, float minX, float maxX,
		   int nBinY, float minY,
		   float maxY, string Xleg,string Yleg,
		   bool prof=false, string type="m");
  void addVariable(string var, int nBinX, vector<float> binsX, int nBinY,
		   vector<float> binsY,
		   string Xleg,string Yleg,bool prof=false, string type="m");
  
  void fill(string var, int ds, float val, float weight=1., int isDDBkg=0);
  void fill(string var, int ds, float valx, float valy, float weight, int isDDBkg=0);
  void fill(string var, string type, float value, float weight=1.,string dir="");
  
  void copyHisto( string var, int ds, TH1* h, string xtitle );

  TH1* getHisto(string obs, int ds);
  const hObs* getHObs(string obs);
  
  vector<string> getObservables();
  
  //Prepare observables
  hObs preparehObs(string var, int nbinX, vector<float> bins, 
		   string Xleg,string Yleg,
		   string type, bool prof,
		   int nbinsY=-1, 
		   vector<float> binsY=vector<float>(0,0) );
  

  //data driven backgrounds
  void fillDDbkg();
  void fillCSbkg();

  
  //saving plots
  // void SaveCanvas(string path,TCanvas* c);
  // void SaveRoot(string path,TCanvas* c);
  void savePlots(string path,const TCanvas* c,string advname="");

  //systematic uncertainties
  vector<systM> getSystObs(string obs);
  systM findSysts(string var,string type);

  ClassDef(HistoManager,0)
};

#endif
  
