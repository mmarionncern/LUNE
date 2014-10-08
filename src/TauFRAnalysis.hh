#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class TauFRAnalysis : public UseTree 
{
public:

  TauFRAnalysis();
  ~TauFRAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  float modWeightDDB();

  //analysis functions
  bool tauID(size_t idx);
  bool elecID(size_t idx);

  void selectElectron();
  void selectTau();

  
  bool passHLT();
  bool triggerDecision(size_t ihlt, string ref);
  
  bool jetVeto();

  int findZDecayChannel();
  bool isGenParticle(TLorentzVector* v4, int pdgId);
  
  int genParticle(int pdgId);
  int matchGenParticle(TLorentzVector* v4);

  void fillPlots(string lvl);

  vector<string> levels_;

  //multiplicity
  int nGoodVtx_;
  int nGEl_;
  int nGTau_;
  int nGJet_;
  int nGBJet_;

  size_t elIdx_;
  size_t tauIdx_;
  
  bool secElOS_;

  //4-vectors
  TLorentzVector elL_;
  TLorentzVector tauL_;
  
  TLorentzVector tmpJet_;

  TLorentzVector etau_;

  TVector2 met_;

  //Weights and corrections
  TH1F* puweights_;
  void loadPUWeights();
  float puWeight(float trueNint);
  
  ClassDef(TauFRAnalysis,0)
};
