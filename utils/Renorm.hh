#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"

using namespace std;
#include <TGraphAsymmErrors.h>
#include <TLorentzVector.h>

#include <TH1.h>

#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooStringVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>
#include <RooAbsBinning.h>
#include <RooArgList.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooDataHist.h>
#include <RooRandom.h>
#include <RooSimultaneous.h>
#include <RooHistPdf.h>
#include <RooHist.h>
// models
// #include <RooGaussian.h>
// #include <RooExponential.h>
// #include <RooPolynomial.h>
// #include <RooGExpModel.h>
// #include <RooVoigtian.h>
// #include <RooCBShape.h>
// #include <RooArgusBG.h>
// #include <RooBreitWigner.h>
// #include <RooFFTConvPdf.h>
// #include <RooNumConvPdf.h>
// #include <RooNovosibirsk.h>
// #include <RooLandau.h>
using namespace RooFit;
using namespace std;


class Renorm {

private:

  void doFit();
 
  vector<TH1*> _hMCs;
  TH1* _hData;
  
  map<string,bool> _conf;
  map<string,float> _weights;

public:

  Renorm();
  ~Renorm();
   map<string,bool> parse(string str);
  
  void initialize(vector<TH1*> hmcs, TH1* hdata, string str);

  map<string,float> getWeights();
  

};
