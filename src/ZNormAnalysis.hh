#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class ZNormAnalysis : public UseTree 
{
public:

  ZNormAnalysis();
  ~ZNormAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  float modWeightDDB();

  //analysis functions
  bool tauID(size_t idx);
  bool elecID(size_t idx);
  
  bool passHLT();
  bool triggerDecision(size_t ihlt, string ref);
  
  int findZDecayChannel();
  
  void fillPlots(string lvl);

  vector<string> levels_;

  //multiplicity
  int nGoodVtx_;
  int nGEl_;
  int nGMu_;
  int nGTau_;
  int nGJet_;
  int nGBJet_;

  size_t el1Idx_;
  size_t el2Idx_;
  size_t jetLIdx_;
  size_t jetSLIdx_;
  size_t jetBLIdx_;
  size_t jetBSLIdx_;

  //4-vectors
  TLorentzVector el1L_;
  TLorentzVector el2L_;
  TLorentzVector jetL_;
  TLorentzVector jetSL_;
  TLorentzVector jetBL_;
  TLorentzVector jetBSL_;

  TLorentzVector tmpJet_;

  TLorentzVector zCand_;

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
  
  ClassDef(ZNormAnalysis,0)
};
