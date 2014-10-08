#include "core/UseTree.hh"

#include "utils/Format.cc"

class ZTauAnalysis : public UseTree 
{
public:

  ZTauAnalysis();
  ~ZTauAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  void endAnalysis();
  void modifySkimming();
  //analysis functions

  bool tauID(size_t it);
  bool isTauInAcc( TLorentzVector);
  void isInAcc( vector<genPart> vp , bool& acc1, bool& acc2, int& tauH );

  bool muonID(size_t im);
  bool elecID(size_t idx); 

  TLorentzVector tau1_;
  TLorentzVector tau2_;
  TLorentzVector tmpTau_;
  
  float effm;
  float effmID;
  float effmIso;
  int N;

  ClassDef(ZTauAnalysis,0)
};
