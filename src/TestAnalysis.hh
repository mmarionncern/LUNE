#include "core/UseTree.hh"

#include <TRandom3.h>

class TestAnalysis : public UseTree 
{
public:

  TestAnalysis();
  ~TestAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();

  float modWeightDDB();

  TRandom3 rnd;

  ClassDef(TestAnalysis,0)
};
