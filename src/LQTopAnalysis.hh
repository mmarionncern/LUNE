#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class LQTopAnalysis : public UseTree 
{
public:

  LQTopAnalysis();
  ~LQTopAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  float modWeightDDB();
  void endAnalysis();
  
  //analysis functions
  bool tauID(size_t idx);
  bool muonID(size_t idx);
  bool elecID(size_t idx);
  
  bool muonLooseID(size_t idx);
  bool elLooseID(size_t idx);
  
  bool testHLT(string hlt);
  bool passHLT();
  bool triggerDecision(size_t ihlt, string ref);
  
  void fillPlots(string lvl);

  float getPtWeight(float pt);
  float getPtWeightE(float pt);

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
  size_t tauIdx_;
  size_t jetLIdx_;
  size_t jetSLIdx_;
  size_t jetBLIdx_;
  size_t jetBSLIdx_;

  bool secLepOS_;

  //4-vectors
  TLorentzVector elL_;
  TLorentzVector muL_;
  TLorentzVector tauL_;
  TLorentzVector jetL_;
  TLorentzVector jetSL_;
  TLorentzVector jetBL_;
  TLorentzVector jetBSL_;
  
  TLorentzVector tmpJet_;

  TLorentzVector emu_;
  TLorentzVector ej1_;
  TLorentzVector ej2_;
  TLorentzVector muj1_;
  TLorentzVector muj2_;
  TLorentzVector ebj1_;
  TLorentzVector ebj2_;
  TLorentzVector mubj1_;
  TLorentzVector mubj2_;
  
  TVector2 met_;

  //btagging
  vector<vector<JetInfo> > jetInfos_;
  BTagWeight* bTagSFTool_;
  BTagDB* bTagDB_;

  void storeBTagInfos(float pt, float eta, float disc, int flav);
  float getBTagWeight();

  int findZDecayChannel();
  vector<int> findTTDecayChannel();

  //Weights and corrections
  TH1F* puweights_;
  void loadPUWeights();
  float puWeight(float trueNint);
  

  //efficiencies
  float effe;
  float effeNH;
  float effm;
  float effmHLT;
  float deffe_2;
  float deffm_2;
  float deffmHLT_2;
  int N;


  ClassDef(LQTopAnalysis,0)
};
