#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/METFunctions.hh"
#include "modules/mt2bisect.hh"
#include "modules/MT2Functions.hh"
#include "modules/LeptonDisplay.hh"

#include <TVector2.h>
#include <TRandom3.h>

class METAnalysis : public UseTree 
{
public:

  METAnalysis();
  ~METAnalysis();

  void computeResponse(bool unc=true, bool fit=false);
  void computeResolution(std::string comp, bool fNV, bool fSEt,
			 bool fit, bool qTCut=false, bool unc=true);

  void checkFit(bool resp, string comp, float val, 
		bool isData, bool fNV, bool fSEt,
		bool fit, bool qTCut);

  void setFlavor(string flav);

  void resoStudy(string metType);

  LeptonDisplay ld;

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  //analysis functions
  void prepareMETHistos(string name);

  void addMET(string tn, string name, string postfix);
  void addUnc(string tn, string name, string dir);
  void replaceString(string& str, string a, string b);

  void fillModuleMET(vector<string> metName);
  void fillUncPlots(string met, string unc, string dir );
  void fillPlots(string met);
  void fillMT2(string met);

  TVector2 phiCorrection(TVector2 met, int Nvtx, string type);

  float getRespW(double qt);

  void occupancyVars();
  bool occCleaning(string type, size_t i, TLorentzVector l);
  bool genCleaning(string type, size_t i, string gType);
  TVector2 footPrintEl(string l, TLorentzVector tl);
  

  TVector2 _qT;
  TVector2 _met;
  TVector2 _metUnc;
  float _sumEt;
  float _sumEtUnc;
  float _upara;
  float _uupara;
  float _uperp;
  float _uuperp;
  float _udphi;

  float _metpara;
  float _metperp;

  float _metx;
  float _umetx;
  float _mety;
  float _umety;
  

  TLorentzVector _l1,_l2;
  TLorentzVector _j1,_j2;

  vector<vector<string> > _mets;
  map<string, pair<string,string> > _uncMap;
  map<string, pair<string,string> >::const_iterator _itUM;

  map<string, float> _upa;
  map<string, float> _upe;
  map<string, float> _urpa;
  map<string, float> _umx;
  map<string, float> _umy;

  METFunctions* _metTools;

  float getRecoilCor(float pt, float eta, bool el, bool data);

  bool _fillPlots;

  //mt2bisect _mt2Mod;
  MT2Functions _mT2Fcts;

  TRandom3 rnd;

  //Weights and corrections
  TH1F* _puweights;
  void loadPUWeights();
  
  ClassDef(METAnalysis,0)
};
