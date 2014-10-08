
//C++ libs
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <TLorentzVector.h>
#include <TVector2.h>

#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

#include "utils/KineUtils.hh"

class MT2Functions {

public:

  MT2Functions();
  ~MT2Functions();

  float mT2(TLorentzVector mVa, TLorentzVector mVb, TVector2 met);

private:
 
  TLorentzVector _mVa;
  TLorentzVector _mVb;
  TVector2 _met;



};
