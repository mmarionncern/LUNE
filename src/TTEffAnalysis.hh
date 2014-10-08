#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class TTEffAnalysis : public UseTree 
{
public:

  TTEffAnalysis();
  ~TTEffAnalysis();

  void advOptions(bool osCharge=true, bool lqdSel=false, string unc="");

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  void modifySkimming();
  void endAnalysis();

  float modWeightDDB();

  //analysis functions
  bool passHLT();
  bool triggerDecision(size_t ihlt, string ref);
  
  vector<int> findDecayChannel();
  bool isGenParticle(TLorentzVector* v4, int pdgId);
  
  int genParticle(int pdgId);
  int matchGenParticle(TLorentzVector* v4);
  int findIdxGenParticle(TLorentzVector* v4, string l);
  
  void fillPlots(string lvl);

  vector<string> levels_;

  //multiplicity
  int nGoodVtx_;
  int nGEl_;
  int nGMu_;
  int nGTau_;
  int nGJet_;
  int nGBJet_;

  size_t elIdx_;
  size_t muIdx_;
  size_t muSLIdx_;
  size_t tauIdx_;
  size_t jetLIdx_;
  size_t jetSLIdx_;
  size_t jetBLIdx_;
  size_t jetBSLIdx_;

  bool secLepOS_;

  //4-vectors
  TLorentzVector elL_;
  TLorentzVector muL_;
  TLorentzVector muSL_;
  TLorentzVector tauL_;
  TLorentzVector jetL_;
  TLorentzVector jetSL_;
  TLorentzVector jetBL_;
  TLorentzVector jetBSL_;

  TLorentzVector tmpJet_;

  TLorentzVector etau_;
  TLorentzVector ej1_;
  TLorentzVector ej2_;
  TLorentzVector tauj1_;
  TLorentzVector tauj2_;
  TLorentzVector ebj1_;
  TLorentzVector ebj2_;
  TLorentzVector taubj1_;
  TLorentzVector taubj2_;

  TVector2 met_;

  //btagging
  vector<vector<JetInfo> > jetInfos_;
  BTagWeight* bTagSFTool_;
  BTagDB* bTagDB_;

  void storeBTagInfos(float pt, float eta, float disc, int flav);
  float getBTagWeight();

  //Weights and corrections
  TH1F* puweights_;
  void loadPUWeights();
  float puWeight(float trueNint);
  
  // fake rate
  float fr();
  float frRealTau();
  float mcFRTauRes_;
  float mcFR_;
  float dFR_;

  TLorentzVector tauSL_;
  TLorentzVector tauSLIdx_;

  //void fillFRShape(vector<TLorentzVector> aitaus);

 
  bool tauES_,tauER_,jetES_,jetER_;
  int var_;

  //============================================
  float STbjet_;

  vector<float> jIdx_;
  vector<float> jetPt_;

  bool osCharge_;
  bool lqdSel_;

  
  bool isElAcc(TLorentzVector p);
  bool isMuAcc(TLorentzVector p);
  bool isHTauAcc(TLorentzVector p);

  bool isInAcceptance(vector<genPart> vp, int& dT);
  vector<genPart> findDaughters();
 std::map<int,std::string> decayChan;
  
  ClassDef(TTEffAnalysis,0)
};
