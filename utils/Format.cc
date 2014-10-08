#ifndef _Format_
#define _Format_

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "TH1.h"
#include <TLorentzVector.h>

struct hObs {

  std::vector<TH1*> hs;
  
  int nBX;
  int nBY;
  std::vector<float> binsX;
  std::vector<float> binsY;
  
  std::string htype;
  
  std::string titleX;
  std::string titleY;
  std::string name;
  std::string type;
};

struct genPart {
  int pdgId;
  int idx;
  int mothIdx;
  int mothId;
  TLorentzVector p4;
};

typedef std::map<std::string,TH1*> systM;
typedef std::map<std::string,TH1*>::iterator itSystM;
typedef std::map<std::string,TH1*>::const_iterator citSystM;

typedef std::map<std::string,std::pair<std::string,std::vector<std::vector<float> > > > fitM;
typedef std::map<std::string,std::pair<std::string,std::vector<std::vector<float> > > >::iterator itFitM;
typedef std::map<std::string,std::pair<std::string,std::vector<std::vector<float> > > >::const_iterator citFitM;



struct SystST{

  std::string type; //+ * %
  float val; //variation
  std::string db; //database name
  int dir; //direction
  std::vector<std::string> vars; //input dependencies
  std::vector<std::string> modVar; //variables to be modified

};

#endif
