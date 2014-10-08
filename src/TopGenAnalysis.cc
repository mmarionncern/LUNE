#include "src/TopGenAnalysis.hh"

using namespace std;

ClassImp(TopGenAnalysis)

TopGenAnalysis::TopGenAnalysis():
UseTree() {


}


TopGenAnalysis::~TopGenAnalysis() {

}

void
TopGenAnalysis::bookHistograms() {

 _hm->addVariable("eeDPhi",256,0,TMath::Pi(),"#Delta#phi(e,e) [rad] ");
 _hm->addVariable("mumuDPhi",256,0,TMath::Pi(),"#Delta#phi(#mu,#mu) [rad] ");
 _hm->addVariable("tautauDPhi",256,0,TMath::Pi(),"#Delta#phi(#tau,#tau) [rad] ");

 _hm->addVariable("emuDPhi",256,0,TMath::Pi(),"#Delta#phi(e,#mu) [rad] ");
 _hm->addVariable("etauDPhi",256,0,TMath::Pi(),"#Delta#phi(e,#tau) [rad] ");

 _hm->addVariable("mutauDPhi",256,0,TMath::Pi(),"#Delta#phi(#mu,#tau) [rad] ");


 _hm->addVariable("pdgId",41,-20.5,20.5,"pdgId ");
 _hm->addVariable("pdgIdSum",6,0.5,6.5,"pdgId categ ");
}

float
TopGenAnalysis::modWeightDDB() {
  return 1.;
}

void
TopGenAnalysis::modifyWeight() {

}



void
TopGenAnalysis::analyze() {

  lL_.SetPtEtaPhiM(0.00001,0,0,0.0);
  lSL_.SetPtEtaPhiM(0.00001,0,0,0.);
  tmp_.SetPtEtaPhiM(0.00001,0,0,0.);

  pdgIdL_ = 0;
  pdgIdSL_ = 0;
  

  float pttmp=0.;
  float pttmp2=0.;

  size_t nGen=_vc->getSize("GenParticlePt"); 
  for(size_t ig=0;ig<nGen;ig++) {

    if(!_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") &&
       !_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=") &&
       !_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") ) continue; 
   

    fill("pdgId", _vc->getI("GenParticlePdgId",ig), _w);
 
    tmp_.SetPtEtaPhiM( _vc->getD("GenParticlePt",ig),  _vc->getD("GenParticleEta",ig), 
		       _vc->getD("GenParticlePhi",ig), 0. ); 
    
    //    cout<<fabs(_vc->getI("GenParticlePdgId",ig))<<"   "<<tmp_.Pt()<<endl;

   if( _vc->getD("GenParticlePt",ig) > pttmp ) {
      lL_ = tmp_;
      pttmp = lL_.Pt();
      lLIdx_ = ig;
      pdgIdL_ = fabs(_vc->getI("GenParticlePdgId",ig));
      //cout<<" --> glou"<<"  "<<pdgIdL_<<endl;
   }
   if( _vc->getD("GenParticlePt",ig) > pttmp2 && _vc->getD("GenParticlePt",ig) < pttmp ) {
     if(fabs(_vc->getI("GenParticlePdgId",ig)) == pdgIdL_ ) {
       if(lL_.DeltaR( tmp_ ) <0.1 ) continue;
       // else
       //cout<<lL_.Pt()<<"   "<<tmp_.Pt()<<"   "<<lL_.Eta()<<"  "<<tmp_.Eta()<<endl;
     }

     // if(fabs(_vc->getI("GenParticlePdgId",ig)) == pdgIdL_ )
     //   cout<< tmp_.Pt()<<"   "<<pttmp<<"   "<<(tmp_.Pt() < pttmp)<<endl;

      lSL_ = tmp_;
      pttmp2 = lSL_.Pt();
      lSLIdx_ = ig;
      pdgIdSL_ = fabs(_vc->getI("GenParticlePdgId",ig));
      //cout<<" --> bi"<<"  "<<pdgIdSL_<<endl;
    }

  }


  
  if( pdgIdL_==0 || pdgIdSL_==0) return;

  int categ=0;
  if( (pdgIdL_==11 && pdgIdSL_==11 ) )
    fill("pdgIdSum",1,_w);
  if( (pdgIdL_==13 && pdgIdSL_==13 ) )
    fill("pdgIdSum",2,_w);
  if( (pdgIdL_==15 && pdgIdSL_==15 ) )
    fill("pdgIdSum",3,_w);
  if( (pdgIdL_==11 && pdgIdSL_==13 ) ||  (pdgIdL_==13 && pdgIdSL_==11 ) )
    fill("pdgIdSum",4,_w);
  if( (pdgIdL_==11 && pdgIdSL_==15 ) ||  (pdgIdL_==15 && pdgIdSL_==11 ) )
    fill("pdgIdSum",5,_w);
  if( (pdgIdL_==15 && pdgIdSL_==13 ) ||  (pdgIdL_==13 && pdgIdSL_==15 ) )
    fill("pdgIdSum",6,_w);

  //cout<<" glouba    -----> "<<pdgIdL_<<"   "<<pdgIdSL_<<endl;

  if( (pdgIdL_==11 && pdgIdSL_==11 ) )
    fill( "eeDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
  if( (pdgIdL_==13 && pdgIdSL_==13 ) )
    fill( "mumuDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
  if( (pdgIdL_==15 && pdgIdSL_==15 ) )
    fill( "tautauDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
  if( (pdgIdL_==11 && pdgIdSL_==13 ) ||  (pdgIdL_==13 && pdgIdSL_==11 ) )
    fill( "emuDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
  if( (pdgIdL_==11 && pdgIdSL_==15 ) ||  (pdgIdL_==15 && pdgIdSL_==11 ) )
    fill( "etauDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
  if( (pdgIdL_==15 && pdgIdSL_==13 ) ||  (pdgIdL_==13 && pdgIdSL_==15 ) )
    fill( "mutauDPhi",  fabs( lL_.Vect().XYvector().DeltaPhi( lSL_.Vect().XYvector() ) ), _w );
}
