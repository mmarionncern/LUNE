#include "src/TestUncAnalysis.hh"

using namespace std;

ClassImp(TestUncAnalysis)

TestUncAnalysis::TestUncAnalysis():
UseTree() {

  _dbm->loadDb("ZeeW","ZEMPTW.root","weights");
  
  vector<string> mpts;
  mpts.push_back("pt_m1");
  mpts.push_back("pt_m2");
  addSystSource("MES",SystUtils::kNone,"*", mpts, 1.03 );
  
  vector<string> mpt;
  mpt.push_back("qTZ");
  addSystSource("ZeeW",SystUtils::kNone,"+", mpt, "ZEMPTW.root:qtZ", "weights");
  
  addWSystSource("tru",SystUtils::kNone,"%", 0.1);


}

TestUncAnalysis::~TestUncAnalysis() {

}


void
TestUncAnalysis::bookHistograms() {

  _hm->addVariable("qT",400,0,400,"q_{T} [GeV]");
  _hm->addVariable("mass",1600,0,400,"M_{#mu#mu} [GeV]");
  _hm->addVariable( "phi",128,0,TMath::Pi()*2,"#phi(Z) [rad]" );

  _hm->addVariable("ptl1",400,0,200,"p_{T}(#mu_{1}) [GeV]");
  _hm->addVariable("ptl2",400,0,200,"p_{T}(#mu_{2}) [GeV]");
  _hm->addVariable("etal1",120,-3,3,"#eta(#mu_{1}) ");
  _hm->addVariable("etal2",120,-3,3,"#eta(#mu_{2}) ");
  _hm->addVariable("phil1",128,0,TMath::Pi()*2,"#phi(#mu_{1}) [rad]");
  _hm->addVariable("phil2",128,0,TMath::Pi()*2,"#phi(#mu_{2}) [rad]");
}

void
TestUncAnalysis::modifyWeight() {
}


void
TestUncAnalysis::analyze() {

  //nVtx selection, I don't store statistic informations
  // a simpel if may not work properly since the program 
  //loop one time before reading the tree to identify which
  // variables are needed
  
  if(!_au->simpleCut<int>( _vc->getI("nVtx"),1,">=")) return;
 

  //pdgId
  bool pdgId1 = fabs(_vc->getI("pdgId_m1"))==13;
  bool pdgId2 = fabs(_vc->getI("pdgId_m2"))==13;
  //if(!_au->makeCut(pdgId1 && pdgId2, _ids, "lepton pdgId sel",_w)) return;
   
  if(!mif(pdgId1 && pdgId2 ) ) return;

  //leptons
  bool pt1 = _vc->getF("pt_m1")>20;
  bool pt2 = _vc->getF("pt_m2")>20;

  //cout<<_ie<<"   "<<_vc->getF("pt_m1")<<" / "<<_vc->getF("pt_m2")<<" -> "<<pt1<<"  "<<pt2<<endl;

  //first selection, keep the statistic information for later.
  //if(!_au->makeCut(pt1 && pt2, _ids, "lepton pt sel",_w)) return;

  _l1.SetPtEtaPhiM(_vc->getF("pt_m1"), _vc->getF("eta_m1"), _vc->getF("phi_m1"), 0);
  _l2.SetPtEtaPhiM(_vc->getF("pt_m2"), _vc->getF("eta_m2"), _vc->getF("phi_m2"), 0);
  
  //
  //cout<<_ie<<"  "<<" -> "<<_vc->getF("pt_m1")<<" / "<<_vc->getF("pt_m2")<<" => "
  //<<_vc->getF("pt_m1")*1.5<<"  "<<_vc->getF("pt_m1")*0.5<<endl;

  // //leptons
  bool eta1 = fabs(_vc->getF("eta_m1"))>1.5;
  bool eta2 = fabs(_vc->getF("eta_m2"))>1.5;
  //cout<<_ie<<"  "<<(_l1+_l2).Pt()<<"   "<<_w;
  _w *= getDBVal("ZeeW", (_l1+_l2).Pt() );
  //cout<<" => "<<_w<<endl;

  if(!_au->makeCut<float>((_l1+_l2).M(),60,"[]", _ids, "mass cut",_w,120) ) return;
 

  fill("qT", (_l1+_l2).Pt(), _w );
  fill("mass", (_l1+_l2).M(), _w );
  fill("ptl1", _vc->getF("pt_m1"), _w );
  fill("ptl2", _vc->getF("pt_m2"), _w );
  fill("etal1", _vc->getF("eta_m1"), _w );
  fill("etal2", _vc->getF("eta_m2"), _w );
  fill("phil1", _vc->getF("phi_m1"), _w );
  fill("phil2", _vc->getF("phi_m2"), _w );
 
  // fillUnc("ptl1","MES", _vc->getF("pt_m1")*0.94, _w, "Do" );
  // fillUnc("ptl1","MES", _vc->getF("pt_m1")*1.06, _w, "Up" );

}
