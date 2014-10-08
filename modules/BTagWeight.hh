#ifndef BTAGWEIGHT3
#define BTAGWEIGHT3
#include <math.h>
#include <iostream>
#include <vector>

struct JetInfo {
  JetInfo(float mceff,float datasf,float t=0.) : eff(mceff), sf(datasf) , tag(t){}
  float eff;
  float sf;
  int tag;
};
  

class BTagWeight {
 public:

  BTagWeight(int nTaggers) : taggers(nTaggers) {}
    
  virtual bool filter(std::vector<int> tags);
  float weight(std::vector<std::vector<JetInfo> > jets);
 private:
    int taggers;
  
  //ClassDef(BTagWeight,0)
};

#endif
