#ifndef _AnaConfig_
#define _AnaConfig_


#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>
#include <sstream>

#include "core/Dataset.hh"

using namespace std;

class AnaConfig {

private:
  
  string _dir;
  string _path;
  
  string _treeName;
  string _hname;

  map<int, string > _numDS;
  map<int, string >::iterator _itNDS;
  map<string, Dataset* > _datasets;
  map<string, Dataset* >::const_iterator _itDs;
  
  
  map<string,float> _kFactors;
  map<string,float> _xSecLumis;
  map<string,float>::iterator _itKF;
  map<string,float>::iterator _itXS;
  
  bool _useXSect;

  double _lumi;
  bool _runFilter;
  int _runNum;

  vector<string> _ddCuts;
  vector<pair<string, float> > _csData;

  bool _noData;

  size_t _testNMax;

  bool _skiptree;

  

public:

  map<string, Dataset* >::iterator itDs;
 

public:

  AnaConfig();
  ~AnaConfig();

  void reset();

  void configureLumi(map<string,float> Lum ,
		     map<string,float> Kfac,
		     float l,bool useXS=true );
  void configureData(bool runfilter, int runnum,bool MCOnly);
  void configureNames(string dir, string treeName, string hName);

  float getWeight(int channel, int evt);
  float getWeight(string chan, int evt);

  string getSample(int channel, int evt);
  string getSample(string chan, int evt);
  
  vector<string > getDSNames();

  vector<string> getDDCuts(){return _ddCuts;};

  vector<int> getDDDSStatus();  
  vector<pair<string, float> > getCSData();

  string findDS(int channel);
  int findChan(string ds);

  void addSample(string str, string sname, int col );

  bool passRunFilter(int run);

  Dataset* getDataset(string name);
  // TTree* getTree(string name) {return _datasets[ name ]->getTree();};
  // int getNEvents(string name) {return _datasets[ name ]->getNEvents();};
  
  bool isNoDataAna();

  string getHName();
  
  void setNMax(size_t testNMax);

  void isHistoAnalysis();

};

#endif
