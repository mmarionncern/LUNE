#include <vector>
#include <sstream>
#include <string>

#include "TROOT.h"

#include <RooRealVar.h>
#include <RooArgList.h>
#include <RooGaussian.h>
#include <RooExponential.h>
#include <RooPolynomial.h>
#include <RooGExpModel.h>
#include <RooVoigtian.h>
#include <RooCBShape.h>
#include <RooArgusBG.h>
#include <RooBreitWigner.h>
#include <RooFFTConvPdf.h>
#include <RooNumConvPdf.h>
#include <RooNovosibirsk.h>
#include <RooLandau.h>

using namespace std;
using namespace RooFit;


class FitUtils {

public:
  FitUtils();
  ~FitUtils();

  static RooGaussian* gausPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooExponential* expoPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooVoigtian* voigPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooPolynomial* polyPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooCBShape* cbPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooBreitWigner* bwPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooNovosibirsk* novoPdf(string id, vector<vector<float> > iVals, RooRealVar x);
  static RooLandau* landPdf(string id, vector<vector<float> > iVals, RooRealVar x);



};
