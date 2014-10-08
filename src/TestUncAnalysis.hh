#include "core/UseTree.hh"

#include "TLorentzVector.h"

class TestUncAnalysis : public UseTree 
{
public:

  TestUncAnalysis();
  ~TestUncAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();

  TLorentzVector _l1,_l2;
  
  ClassDef(TestUncAnalysis,0)
};
