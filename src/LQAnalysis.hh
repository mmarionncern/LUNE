#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include "modules/NeuralNetTTLQ.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class LQAnalysis : public UseTree 
{
public:

  LQAnalysis();
  ~LQAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  void modifySkimming();
  void endAnalysis();

  float modWeightDDB();

  //analysis functions
  bool tauID(size_t idx);
  bool muonID(size_t idx);
  bool elecID(size_t idx);

  bool elLooseID(size_t idx);
  bool muonLooseID(size_t idx);

  bool passHLT();
  bool triggerDecision(size_t ihlt, string ref);
  
  int findZDecayChannel();
  bool isGenParticle(TLorentzVector* v4, int pdgId);
  
  int genParticle(int pdgId);
  int matchGenParticle(TLorentzVector* v4);
  int findIdxGenParticle(TLorentzVector* v4, string l);
  
  bool isElAcc(float pt, float eta);

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

  vector<size_t> jIdx_;
  vector<float> jetPt_;
  

  map<int,double> vtx_;

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

  float mT2();

  // fake rate
  float frStd(float pt1);
  float frMM(float pt1, float pt2);
  float frKK(float pt1, float pt2);
  float nFTStd_;
  float nFTMM_;
  float nFTKK_;
  TLorentzVector tauSL_;
  TLorentzVector tauSLIdx_;

  //============================================
  float ptL_;
  float ptT_;
  float mTb_;
  float mTj_;
  float TmassL_;
  float TmassJ_;
  float dPhiLM_;
  float STbjet_;
  float w_;
  
  IClassifierReader* CreateNN();
  IClassifierReader* NN;

  float efft;
  int N;


  ClassDef(LQAnalysis,0)
};
