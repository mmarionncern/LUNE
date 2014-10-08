#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <math.h>

#include <TMath.h>
#include <TROOT.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <TLine.h>

using namespace std;

class LeptonDisplay {


public:

  LeptonDisplay();
  ~LeptonDisplay();

  void fill(float l1eta, float l1phi, float l1charge,
	    vector<float> phPt, vector<float> phEta, vector<float> phPhi,
	    vector<float> chPt, vector<float> chEta, vector<float> chPhi,
	    vector<float> nhPt, vector<float> nhEta, vector<float> nhPhi );

  void nextLepton();
  void previousLepton();
  void showLepton(int evt, int lep);
  
  float corPhi(float phic, float phir);

private:
  void drawCanvas();

  TH2F* _h;
  TH2F* _hPh;
  TH2F* _hCh;
  TH2F* _hNh;

  TCanvas* _c;

  int _evt;
  int _lep;

  vector<float> _lEta;
  vector<float> _lPhi;
  vector<float> _lCharge;

  vector<vector<float> > _phPt;
  vector<vector<float> > _phEta;
  vector<vector<float> > _phPhi;
  vector<vector<float> > _chPt;
  vector<vector<float> > _chEta;
  vector<vector<float> > _chPhi;
  vector<vector<float> > _nhPt;
  vector<vector<float> > _nhEta;
  vector<vector<float> > _nhPhi;


  bool _initCanvas;

};
