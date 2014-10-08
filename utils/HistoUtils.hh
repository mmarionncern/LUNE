#ifndef _HistoUtil_
#define _HistoUtil_


#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#include "TF1.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TPolyLine.h"

using namespace std;

class HistoUtils {

  //Functions

public:

  //Conversion functions
  static TH1F* convertGraphToHisto(TGraph* graph, int NbinX, double mX, double MX,
				   string name);
 static TH1F* convertGraphToHisto(TGraph* graph, int NbinX, double BinX[],
				  string name);
  static TH2F* convertGraphToHisto(TGraph* graph, int NbinX, double mX, double MX,
				   int NbinY, double mY, double MY,string name);
  
  static TH2F* convertGraphToHisto(TGraph* graph, int NbinX, double BinX[],
				   int NbinY, double BinY[],string name);
  
  static TProfile* convertGraphToProfile(TGraph* graph, int NbinX, double BinX[],
					 string name,string opt);

  static TProfile* convertGraphToProfile(TGraph* graph, int NbinX, double mX, double MX,
					 string name,string opt);

  static vector<TGraph*> convertGraphToErrorProfile(TGraph* graph, int NbinX, double mX, double MX );

  static vector<TGraph*> convertGraphToErrorProfile(TGraph* graph, int NbinX, int BinVar[]);

  // static vector<TProfile*> convertAllProfileVarBin(vector<TGraph*> graphs2d, int n, int VarBin[],
  // 					   vector<string> names,string k,string opt);
  
  static TGraphAsymmErrors* convertHistoToGraph(TH1* h, float datnorm=1, bool diff=false);

  static TGraphAsymmErrors* ratioHistoToGraph(TH1* hd, TH1* hmc, string opt="nP");

  static TGraphAsymmErrors* residualGraph(TH1* hd, TH1* hmc, string opt="nP");

  //reduction function (empty bin / point removal)

  static TGraphAsymmErrors* graphReduction(TGraph* graph, int NbinX, double mX, double MX,
					   string name,string opt);
  
  static TGraphAsymmErrors* graphReduction(TGraph* graph, int NbinX, int BinVar[],
					   string name,string opt);
  
  static vector<TH1F*> histoReduction(TH1F* source,TH1F* MC);

  static void cleanGraph(TGraph*& g);

  //Stacking function

  static vector<TH2F*> stack2DHistos(vector<TH2F*> Histos2d,
					 vector<string> datanames,
					 map<string,float> weights);


  //rebinning histogram
  static void rebin1DHisto(TH1*& h, int binning);

  //overflow, underflow
  static void addOverflowBin(TH1*& h, float max);
  static void addUnderflowBin(TH1*& h, float min);

  //differential normalization
  static void changeToDiffHisto(TH1*& h);

  //rebinning
  static void manualRebin(TH1*& h, int bin);
  static void manualCompleteRebin(TH1*& h, int nBin, float xmin, float xmax);
  
  //statistics and histograms
  static TH1F* significance(TH1F* hs, TH1F* hb);
  static TH2F* significance(TH2F* hs, TH2F* hb);

  //efficiency cuvre from the histo
  static TGraphAsymmErrors* efficiency(TH1* h);

  //ROC curve
  static TGraphAsymmErrors* curveROC(TH1* s, TH1* b);

  //bands uncertainties
  static TPolyLine* GetSystBand(vector<float> xs, vector<float> yl, vector<float> yh, float xmin, float xmax);

  //copy the histogram style
  static void copyStyle(TH1* href, TH1*& hmod, bool axisOnly=false);

  //manual cloning ( keep style)
  static void manualClone(TH1* href, TH1*& mod);

  //autoBinning
  static int autoBinning(float xm, float xM, int nTB, float Xm, float XM);


  //extraction
  static vector<vector<float> > getHistoUpperCoordinates(TH1* h);
  static vector<vector<float> > getHistoUpperCoordinatesWithError(TH1* h, float dNorm=1.);
  static vector<vector<float> > getGraphUpperCoordinatesWithError(TGraphAsymmErrors* g);
  
  static float getHistoYlow(TH1* h, float xm=-10000000000., float xM=10000000000.);
  static float getGraphYlowWithError(TGraphAsymmErrors* g, float xm=-10000000000.,
				     float xM=10000000000.);
  static float getGraphYhighWithError(TGraphAsymmErrors* g, float xm=-10000000000.,
				      float xM=10000000000.);
  static float getHistoYlowWithError(TH1* h, float xm=-10000000000.,
				     float xM=10000000000.,string opt="P");
  static float getHistoYhighWithError(TH1* h, float xm=-10000000000.,
				      float xM=10000000000.,string opt="P");
  
  static float getMaxDiff(TH1* hd, TH1* hm, float xm=-10000000000.,
			  float xM=10000000000.);

  static std::vector<float> getXbinning(TH1* h );


  ClassDef(HistoUtils,0)

};

#endif
