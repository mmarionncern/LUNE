#include "src/MVAConfigAnalysis.hh"

using namespace std;

ClassImp(MVAConfigAnalysis)

MVAConfigAnalysis::MVAConfigAnalysis():
UseTree() {

}

MVAConfigAnalysis::~MVAConfigAnalysis() {

}


void
MVAConfigAnalysis::bookHistograms() {
  
  _hm->addVariable( "MET",400,0,400,"#slash{E}_{T} [GeV]" );
  _hm->addVariable( "Phi",128,0,TMath::Pi()*2,"#phi(#slash{E}_{T}) [rad]" );
  _hm->addVariable( "METX",400,-200,200,"#slash{E}_{T,X} [GeV]" );
  _hm->addVariable( "METY",400,-200,200,"#slash{E}_{T,Y} [GeV]" );
  
  _hm->addVariable( "METpara",800,-100,100,"#slash{E}_{T,||} [GeV]" );
  _hm->addVariable( "METredpara",800,-100,100,"#slash{E}_{T,||} - |#slash{E}_{T,gen}| [GeV]" );
  _hm->addVariable( "METperp",800,-100,100,"#slash{E}_{T,#perp}   [GeV]" );

  _hm->addVariable( "METSumEt",1000,0,2000,"#sum E_{T} [GeV]" );
 
  _hm->addVariable( "DMET",400,0,400,"() [GeV]" );
  _hm->addVariable( "DPhiMET",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},#slash{E}_{T,gen}) [rad]" );
  
}


void
MVAConfigAnalysis::modifyWeight() {
}


void
MVAConfigAnalysis::analyze() {
  
  _genMet.Set( _vc->getD("genMEtTrue_x"), _vc->getD("genMEtTrue_y") );
  // _pfMet.Set( _vc->getD("pfMEt_x"), _vc->getD("pfMEt_y") );
  // _noPUMet.Set( _vc->getD("NoPuMEt_x"), _vc->getD("NoPuMEt_y") );
  // _mvaMet.Set( _vc->getD("MvaMEt_x"), _vc->getD("MvaMEt_y") );

  if(mif(_ids.find("pf")!=(size_t)-1) ) {
    _met.Set( _vc->getD("pfMEt_x"), _vc->getD("pfMEt_y") );
    _sumEt = _vc->getD("pfMEtSumEt");
  }
  if(mif(_ids.find("mva")!=(size_t)-1)) {
    _met.Set( _vc->getD("MVaMEt_x"), _vc->getD("MVaMEt_y") );
    _sumEt = _vc->getD("MVaMEtSumEt");
  }
  if(mif(_ids.find("noPu")!=(size_t)-1) ) {
    _met.Set( _vc->getD("NoPuMEt_x"), _vc->getD("NoPuMEt_y") );
    _sumEt = _vc->getD("NoPuMEtSumEt");
  }

  fill( "MET", _met.Mod() );
  fill( "Phi", _met.Phi() );
  fill( "METX", _met.X() );
  fill( "METY", _met.Y() );
  fill( "METSumEt", _sumEt );

  fill( "DMET", _genMet.Mod()-_met.Mod() );
  fill( "DPhiMET", _genMet.DeltaPhi( _met ) );
	

  _metpara = (float)(( _genMet*_met)/_genMet.Mod());;
  TVector2 m = _met.Rotate( TMath::Pi()/2);
  _metperp = (float)(( m*_genMet)/_genMet.Mod());

  fill( "METpara", _metpara );
  fill( "METredpara", _metpara - _genMet.Mod() );
  fill( "METperp", _metperp );


}

