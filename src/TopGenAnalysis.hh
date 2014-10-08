#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include <TLorentzVector.h>
#include <TVector2.h>

class TopGenAnalysis : public UseTree 
{
public:

  TopGenAnalysis();
  ~TopGenAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
  float modWeightDDB();


  size_t lLIdx_;
  size_t lSLIdx_;

  int pdgIdL_;
  int pdgIdSL_;

  TLorentzVector lL_;
  TLorentzVector lSL_;
  TLorentzVector tmp_;

  ClassDef(TopGenAnalysis,0)
};
