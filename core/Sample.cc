#include "core/Sample.hh"

using namespace std;

ClassImp(Sample)

Sample::Sample() {
}

Sample::Sample(string name, int n, float xSect, float kfact, float eqLumi) {

  _name = name;
  _nEvents=n;
  _xSection=xSect;
  _kFactor=kfact;
  _eqLumi=eqLumi;

}

Sample::~Sample() {
}
