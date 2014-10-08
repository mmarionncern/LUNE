#include "src/TestAnalysis.hh"

using namespace std;

ClassImp(TestAnalysis)

TestAnalysis::TestAnalysis():
UseTree() {

}

TestAnalysis::~TestAnalysis() {

}

void
TestAnalysis::bookHistograms() {
  //cout<<_hm<<endl;
  _hm->addVariable("pt_l1",100,0,1000,"p_{T} [GeV]");
    
}


float
TestAnalysis::modWeightDDB() {
  return 1.;
}

void
TestAnalysis::modifyWeight() {

  float w=1.;
  
  _w *= w;
}


void
TestAnalysis::analyze() {
  
 
  _au->makeCut<bool>(true,true,"=",_ids,"beginning",_w);

  //cout<<_ids<<"  "<<_inds<<"  "<<_ie<<endl;
  // cout<<_vc->getSize("MuonPt")<<endl;
  if(_vc->getSize("MuonPt")!=0) {
    //cout<<" coin "<<endl;
    fill("pt_l1", _vc->getD("MuonPt",0), _w );
    fillUnc("pt_l1","ES",_vc->getD("MuonPt",0)*rnd.Gaus(1,0.20), _w);
 
    _au->makeCut<double>(_vc->getD("MuonPt",0),20,">",_ids,"pt cut",_vc->getD("MuonPt",0)*1.02,_vc->getD("MuonPt",0)+0.98,_w);
 
    //cout<<" gloubi "<<_vc->getD("MuonPt",0)<<endl;
  }
  
}
