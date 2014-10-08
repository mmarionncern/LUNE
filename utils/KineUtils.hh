#ifndef _KineUtil_
#define _KineUtil_

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>

#include <TMath.h>

class KineUtils {

public:

  static float dR(float e1, float e2, float p1, float p2, bool conv=false);
  static float phi( float x, float y );
  static float dPhi( float phi1, float phi2 );

};

#endif
