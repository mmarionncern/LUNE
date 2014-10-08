#include "core/UseTree.hh"

#include "utils/KineUtils.hh"

#include <TVector2.h>

class MVAConfigAnalysis : public UseTree 
{
public:

  MVAConfigAnalysis();
  ~MVAConfigAnalysis();

private:

  void bookHistograms();
  void modifyWeight();
  void analyze();
  
 
  TVector2 _met;
  TVector2 _genMet;
  TVector2 _pfMet;
  TVector2 _noPUMet;
  TVector2 _mvaMet;

  float _sumEt;

  float _metpara;
  float _metperp;
  
  ClassDef(MVAConfigAnalysis,0)
};
