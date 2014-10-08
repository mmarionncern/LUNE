#ifndef _UseTree_
#define _UseTree_


#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TObjString.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TPad.h"
#include "TMath.h"
#include "TROOT.h"
#include "TProfile.h"
#include "TEllipse.h"
#include "TArrow.h"
#include "TPaveText.h"
#include "TPad.h"
#include "TPolyLine.h"
#include "TMarker.h"
#include "RooHistError.h"
#include "RooHist.h"

// #include "boost/timer.hpp"
// #include "boost/progress.hpp"

#include "core/AnaConfig.hh"
#include "core/Dataset.hh"
#include "core/HistoManager.hh"
#include "core/VarClass.hh"
#include "core/AccessHisto.hh"
#include "display/Display.hh"
#include "utils/AnaUtils.hh"

#include "database/DataBaseManager.hh"


#include <time.h>

using namespace std;


class UseTree {

  //======== variables ============
  
private:

  bool _init;
 
  bool _recompute;

  bool _treeAnalysis;

  vector<string> _dsnames;

  map< std::pair<int,int> , std::pair<string,int> > _events;
  map< std::pair<int,int> , std::pair<string,int> >::iterator _itEvt;
  vector<int> _evtsInFile;

  int _nDC;

  size_t _testNMax;

  string _currentObs;

  string _unc;
  int _uDir;
  float _wBack;
  vector<string> _uncSrcs;
  vector<int> _uncDirs;
  map<string, bool > _uType;
  
protected:

  HistoManager* _hm;
  VarClass* _vc;

  DataBaseManager* _dbm;

  AnaUtils* _au;

  string _ids; //the current dataset
  int _inds; //the current dataset
  int _ie; //the current event
  int _nevts; //current number of events in tree
  float _w; //the current global weight

  bool _isData;
  int _isDDBkg;

  bool _skim;
  bool _fullSkim;


public:

  AccessHisto ah;
  AnaConfig anConf;
  Display dp;

  //========= functions ============

private:

  void configure();

  bool checkDoubleCount();
  
  void globalAnalyze();

  void reset();

  void setHistograms(); //bypass for histo reading

  void applySystVar(SystST s);
  float applySystDBVar(SystST s, string db, float v1, float v2, float v3, float v4,
		       float v5,float v6,float v7,float v8,float v9, float v10);
  //void reinitVars( vector<string> mvars );

  //Skimming functions
  void initSkimming();
  void finalizeSkimming();
  TFile* _oFile;
  string _wDs;
  bool _storeTuple;
  TTree* _skimTree;
  TH1I* _hnSkim;
protected:

  virtual void bookHistograms();
  
  virtual void analyze();
  virtual void endAnalysis();

  virtual void modifyWeight();

  virtual void modifySkimming();
  template < typename T > void addSkimBranch(string name,T* val) {
    _skimTree->Branch( name.c_str(), val );
  };
  void fillSkimTree() { if(_ie!=-1) _skimTree->Fill();};
  

  void fill(string var, float valx, float weight=1.);
  void fill(string var, float valx, float valy, float weight);
  void fillUnc(string var, string unc, float val, float weight, string dir="");
  void fillUnc(string var, string unc, float val, float weight,
	       float wup, float wdo);

  //uncertainties
  void addSystSource(string name, int dir, string type, vector<string> modVar, 
		     float val, bool wUnc=false);
  void addSystSource(string name, int dir, string type, vector<string> modVar,
		     string db, string hname, bool wUnc=false);
  void addWSystSource(string name, int dir, string type, float val);
  void addWSystSource(string name, int dir, string type, string db, string hname);

  float getDBVal(string db, float v1=-100000, float v2=-100000, float v3=-100000,
		 float v4=-100000, float v5=-100000,float v6=-100000,
		 float v7=-100000, float v8=-100000, float v9=-100000,
		 float v10=-100000);

  virtual float modWeightDDB();

  bool mif(bool b);
 
public:

  UseTree();
  ~UseTree();

  void isNonTreeAnalysis() { _treeAnalysis=false; anConf.isHistoAnalysis();};
 
  void configureAnalysis(bool skim, bool invCut, vector<string> nM1Var);

  void doAnalysis();

  void noFullSkim() {_fullSkim=false;};
  
  bool isInitStatus();
  void unlockStatus();

  void refresh();

  void savePlot(string path, string advname="");
  void producePlots(string path);

  void getStatistics();
  void drawStatistics();
  void getYield(string ds, string lvl);

  void drawEfficiencies(string o1);
  void drawROCcurves(string o1);
  void multiROCcurves();

  void drawRatio(string o1, string o2 );
  void drawResiduals(string o1);
  void drawSignificance(string o1);
  void drawCumulativePlots(string o1);
  void drawDetailSyst(bool cumul);

  void getIntegral(float x1, float x2, float y1=0, float y2=1000000);

  void saveDataMCRatio(string fname,string hname);
  void saveHistosLQ(string o1);
  void saveHistos(string o1);

  void getDoubleCounting() {cout<<" number of double counted events : "<<_nDC<<endl; };

  void launchTest(size_t nMax);

  ClassDef(UseTree,0)

};

#endif
