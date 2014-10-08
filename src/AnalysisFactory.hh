#ifndef __ANFact__
#define __ANFact__

#include <iostream>
#include <string>

#include "core/UseTree.hh"

using namespace std;


class AnalysisFactory {

public:
  
  static UseTree* get(string type);

  ClassDef(AnalysisFactory,0)

};
#endif
