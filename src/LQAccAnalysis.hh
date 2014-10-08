#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include "modules/BTagDB.hh"
#include "modules/BTagWeight.hh"

#include <TLorentzVector.h>
#include <TVector2.h>


class LQAccAnalysis : public UseTree 
{
public:

  LQAccAnalysis();
  ~LQAccAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  void endAnalysis();
  void modifySkimming();
  //analysis functions

  bool isElAcc(TLorentzVector p);
  bool isMuAcc(TLorentzVector p);
  bool isHTauAcc(TLorentzVector p);

  bool isInAcceptance(vector<genPart> vp, int& dT);
  vector<genPart> findDaughters();

  std::map<int,std::string> decayChan;

  ClassDef(LQAccAnalysis,0)
};
