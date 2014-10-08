#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <math.h>

#include <TROOT.h>
#include <TTree.h>
#include <TGraphAsymmErrors.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPad.h>
#include <TLine.h>
#include <TLatex.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <TStyle.h>
#include <RooVoigtian.h>

#include <RooMsgService.h>

typedef std::map<std::string, std::vector<std::vector<float> > > recMap;
typedef std::map<std::string, std::vector<std::vector<float> > >::iterator recMapIter;

struct internalTree{
  
  float qt_mc;
  float upara_mc;
  float uperp_mc;
  float redupara_mc;
  float metx_mc;
  float mety_mc;
  int nVtx_mc;
  float sumEt_mc;
  float w_mc;
  float wqT_mc;
  std::map<std::string, float> upa_mc;
  std::map<std::string, float> upe_mc;
  std::map<std::string, float> urpa_mc;
  std::map<std::string, float> umx_mc;
  std::map<std::string, float> umy_mc;

  float qt_d;
  float upara_d;
  float uperp_d;
  float redupara_d;
  float metx_d;
  float mety_d;
  int nVtx_d;
  float sumEt_d;
  float w_d;
  std::map<std::string, float> upa_d;
  std::map<std::string, float> upe_d;
  std::map<std::string, float> urpa_d;
  std::map<std::string, float> umx_d;
  std::map<std::string, float> umy_d;
 
};


class METFunctions {


public:

  METFunctions();
  METFunctions(std::string flav, std::vector<std::string> uncSrc);
  ~METFunctions();

  //void configMETFunctions(bool isData);

  void fillRFTree(float qT, float upara, float uperp, 
		  float redupara, int nVtx, float sumEt, float metx, float mety,
		  std::map<std::string,float> upa, std::map<std::string, float> upe,
		  std::map<std::string,float> urpa, 
		  std::map<std::string,float> umx, std::map<std::string,float> umy, 
		  float w, float wqt, bool isData);

  void configureBinning(std::vector<double> binQt, std::vector<double> binSEt/*, int* binNVtx*/);
  void configureMETTool(std::string comp, bool fNV,
			bool fSEt, bool fit, bool qTCut=false, bool unc=false);
  
  void setFlavor(std::string flav) { _flav=flav;};

  void computeResponse();
  void computeResolution();

  void computeResponse(bool unc, bool fit);
  void computeResolution(std::string comp, bool fNV, bool fSEt, bool fit, bool qTCut=false, bool unc=false);

  void checkPoint(bool resp, std::string comp, float val,bool isData, bool fNV, bool fSEt, bool fit, bool qTCut=false);

private:

  //Voigtian FWHM
  double FWHM(double sigma, double gamma);
  double FWHMError( double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg);
  
  //
  void reset();

  //core
  std::vector<float> fit(TTree* ZData, bool isData, float nVm, float nVM, 
			  float qtm, float qtM);
  void doRooFitJob(bool isData);
  void singleFit(bool isData, float val);
  void convertVals(bool isData);
  void compute(); // == doRooFit + convertVals
  
  //uncertainties
  std::vector<float> computeUncertainties( std::map<std::string, TH1*> uncs );


  //usefull functions
  static TGraphAsymmErrors* getRatio(TGraphAsymmErrors* denom, TGraphAsymmErrors* num);
  void getRatios();

  //drawing functions
  void preparePad();
  void draw();
  
  //function objects
  bool _detail;


  TTree* _dataT;
  TTree* _mcT;
  internalTree _itt;

  recMap _vals;
  std::vector<std::vector<std::vector<float> > > _uncs;

  std::string _flav;

  std::vector<double> _binsQt;
  std::vector<double> _binsSEt;
  int* _binsNVtx;
  
  std::string _comp;
  bool _vsNPV;
  bool _vsSEt;
  bool _fit;
  bool _qTCut;
  bool _unc;

  bool _resp;
  bool _isPara;

  std::vector<TGraphAsymmErrors*> _graphs;

  TCanvas* _c;
  std::vector<TPad*> _pads;

  std::vector<std::string> _uncNames;

  ClassDef(METFunctions,0);

};
