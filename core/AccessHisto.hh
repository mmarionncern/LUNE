#ifndef _AccessHisto_
#define _AccessHisto_

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TKey.h>
#include <TObject.h>
#include <TString.h>


class AccessHisto {

public:

  AccessHisto();
  ~AccessHisto();

  void configure(std::string filename, std::string var, std::string xtitle);
  void access();
  TH1* getHistogram(size_t chan);
  std::string getXtitle() {return _xtitle;};
  std::string getVar() {return _var;};
  
  size_t getNHistos() {return _histos.size();}

private:

  std::string _filename;
  std::string _xtitle;
  std::string _var;
  
  std::vector<TH1*> _histos;

  
  ClassDef(AccessHisto,0);
};


#endif
