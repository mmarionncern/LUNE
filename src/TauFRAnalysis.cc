#include "src/TauFRAnalysis.hh"

using namespace std;

ClassImp(TauFRAnalysis)

TauFRAnalysis::TauFRAnalysis():
UseTree() {

  loadPUWeights();
  
  levels_.push_back( "Presel" );
  levels_.push_back( "Charge" );
  levels_.push_back( "JetVeto" );
  levels_.push_back( "TMass" );
  levels_.push_back( "Iso" );

  _dbm->loadDb("elID","elIDMedium.db");
  _dbm->loadDb("elTrig","TrigEff_MediumID_Ele27WP80_2D.root","h_eff");

}

TauFRAnalysis::~TauFRAnalysis() {

}


void
TauFRAnalysis::bookHistograms() {

  for(size_t ilvl=0;ilvl<levels_.size();ilvl++) {
    
    _hm->addVariable("elPt"+levels_[ilvl],500,0,500,"p_{T}(e) [GeV] ");
    _hm->addVariable("elEta"+levels_[ilvl],120,-3,3,"#eta(e)  ");
    _hm->addVariable("elPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e) [rad] ");
    _hm->addVariable("elCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e} ");

    _hm->addVariable("tauPt"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauEta"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau) [rad] ");
    _hm->addVariable("tauCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{#tau} ");
    
    _hm->addVariable("tauPtTIso"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauPtMIso"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauPtLIso"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauPtVLIso"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauPtNoIso"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauEtaTIso"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauEtaMIso"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauEtaLIso"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauEtaVLIso"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauEtaNoIso"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");

    _hm->addVariable("isoValue"+levels_[ilvl],50,0,50.,"I_{#tau} [GeV] ",true);
    _hm->addVariable("isoRelValue"+levels_[ilvl],50,0,50.,"I_{#tau}/p_{T}(#tau)  ",true);

    //met
    _hm->addVariable("metPt"+levels_[ilvl],500,0,500,"#slash{E}_{T} [GeV] ");
    _hm->addVariable("metPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#slash{E}_{T}) [rad] ");

    //e+tau variables
    _hm->addVariable("etauCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e}#times Q_{#tau} ");
    _hm->addVariable("etauMass"+levels_[ilvl],1000,0,1000,"M(e,#tau) [GeV] ");
    _hm->addVariable("etauPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#tau) [GeV] ");
    _hm->addVariable("etauY"+levels_[ilvl],120,-3,3,"Y(e,#tau) ");
    _hm->addVariable("etauPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,#tau) [rad] ");
    _hm->addVariable("etauDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(e,#tau) [rad] ");

  //e+met
    _hm->addVariable("eMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETTMassVLIso"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETTMassLIso"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETTMassMIso"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETTMassTIso"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(e,#slash{E}_{T}) [rad] ");

    //tau+met
    _hm->addVariable("tauMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(#tau,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("tauMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("tauMETDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(#tau,#slash{E}_{T}) [rad] ");

    _hm->addVariable("nVertex"+levels_[ilvl],50,0,50,"vertex multiplicity "); 
    _hm->addVariable("nEl"+levels_[ilvl],10,0,10,"electron multiplicity "); 
    _hm->addVariable("nTau"+levels_[ilvl],10,0,10,"tau multiplicity "); 

  }

}

void
TauFRAnalysis::modifyWeight() {

  if(!_isData) {
    float w=puWeight(_vc->getF("PileUpInteractionsTrue") );
    _w *= w;
  }
  
}

void
TauFRAnalysis::analyze() {


  nGoodVtx_=0;

  for(size_t ivtx = 0; ivtx < _vc->getSize("VertexRho") ; ++ivtx) {
    //double chi2 = _vc->getD("VertexChi2",ivtx);
    double ndf  = _vc->getD("VertexNDF",ivtx);
    double z    = _vc->getD("VertexZ",ivtx);
    double rho  = _vc->getD("VertexRho",ivtx);
	
    if (!(_vc->getB("VertexIsFake",ivtx) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2) ) {
      ++nGoodVtx_; 
    }
  }			
 
  if(!_au->simpleCut<int>(nGoodVtx_,1,">=")) return;
 
  //splitting Z =================================
  if(_ids.find("Z(")!=(size_t)-1) {
    int ltype = findZDecayChannel();
    if( _ids == "Z(ll)+jets" ) {
      if(!_au->simpleCut<int>(ltype,15,"!=")) return;
    }
    else {
      if(!_au->simpleCut<int>(ltype,15,"=")) return;
    }
  }
 
  //=============================================


  _au->makeCut(true,_ids,"beginning",_w);
 
  //========================= Leptons
  //
  
  nGEl_=0;
  nGTau_=0;
  secElOS_=false;
  
  elL_.SetPtEtaPhiM(0.00001,0,0,0.0005);
  tauL_.SetPtEtaPhiM(0.00001,0,0,1.78);
  
  selectElectron();
  selectTau();

  met_.SetMagPhi( _vc->getD("PFMETType01XYCor"), _vc->getD("PFMETPhiType01XYCor") );
 

  if(!_isData)
    _w *=_dbm->getDBValue ("elID", fabs(elL_.Eta()), elL_.Pt() );

  if(!_au->makeCut<int>( nGEl_, 0 , ">" ,_ids,"el multiplicity",_w) ) return;
  
  if(!_au->makeCut( !secElOS_ ,_ids,"no OS electron",_w) ) return;

  if(_isData) {
    if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
  }
  else {
    _w *=_dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
    if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  }
  
  if(!_au->makeCut<int>( nGTau_, 0 , ">" ,_ids,"tau multiplicity",_w) ) return;

  fillPlots( "Presel");
  
  if(!_au->makeCut<int>( _vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",tauIdx_), -1 , "=" ,_ids,"opposite charge",_w) ) return;
  
  fillPlots( "Charge");


  if(!_au->makeCut( jetVeto(), _ids, "jet veto", _w) ) return;
  
  fillPlots( "JetVeto");

  // float ptEMET  = (met_ + elL_.Vect().XYvector()).Mod();  
  // float dphiEMET  = met_.DeltaPhi( elL_.Vect().XYvector() );
  float tmassE  = sqrt(2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))));

  if(!_au->makeCut<float>( tmassE, 50, ">", _ids, "transverse mass cut", _w ) ) return;

  fillPlots( "TMass");
  if(!_au->makeCut<int>( _vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", tauIdx_), 1, "=", _ids, "isolation", _w ) ) return;
// if(!_au->makeCut<int>( _vc->getD("HPSTauLooseCombinedIsolationDeltaBetaCorr3Hits", tauIdx_), 1, "=", _ids, "isolation", _w ) ) return;

  fillPlots("Iso");

}

void
TauFRAnalysis::fillPlots(string lvl) {

  etau_  = elL_ + tauL_;

  float tmassE  = sqrt(2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))));
  float ptEMET  = (met_ + elL_.Vect().XYvector()).Mod() ;
  float dphiEMET  = met_.DeltaPhi( elL_.Vect().XYvector() );

  float etauY = 0.5*log( (etau_.E()+etau_.Pz())/(etau_.E()-etau_.Pz()) );


  fill("elPt"+lvl, elL_.Pt() , _w );
  fill("elEta"+lvl, elL_.Eta() , _w );
  fill("elPhi"+lvl, elL_.Phi() , _w );
  fill("elCharge"+lvl, _vc->getI("ElectronCharge",elIdx_) , _w );

  fill("tauPt"+lvl, tauL_.Pt() , _w );
  fill("tauEta"+lvl, tauL_.Eta() , _w );
  fill("tauPhi"+lvl, tauL_.Phi() , _w ); 
  fill("tauCharge"+lvl, _vc->getI("HPSTauCharge",tauIdx_) , _w );

  if(_vc->getD("HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr", tauIdx_) == 1) {
    fill("tauPtVLIso"+lvl, tauL_.Pt() , _w );
    fill("tauEtaVLIso"+lvl, tauL_.Eta() , _w );
    fill("eMETTMassVLIso"+lvl, tmassE , _w ); 
  }
  if(_vc->getD("HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr", tauIdx_) == 1) {
    fill("tauPtLIso"+lvl, tauL_.Pt() , _w );
    fill("tauEtaLIso"+lvl, tauL_.Eta() , _w );
    fill("eMETTMassLIso"+lvl, tmassE , _w ); 
    fill("metPt"+lvl, met_.Mod() , _w );
  }
  if(_vc->getD("HPSTauMediumCombinedIsolationDeltaBetaCorrDiscr", tauIdx_) == 1) {
    fill("tauPtMIso"+lvl, tauL_.Pt() , _w );
    fill("tauEtaMIso"+lvl, tauL_.Eta() , _w );
    fill("eMETTMassMIso"+lvl, tmassE , _w ); 
  }
  if(_vc->getD("HPSTauTightCombinedIsolationDeltaBetaCorrDiscr", tauIdx_) == 1) {
    fill("tauPtTIso"+lvl, tauL_.Pt() , _w );
    fill("tauEtaTIso"+lvl, tauL_.Eta() , _w );
    fill("eMETTMassTIso"+lvl, tmassE , _w ); 
  }
  
  fill("tauPtNoIso"+lvl, tauL_.Pt() , _w );
  fill("tauEtaNoIso"+lvl, tauL_.Eta() , _w );
 
  float iso = _vc->getD("HPSTauIsolationPFChargedHadrCandsPtSum",tauIdx_) +  _vc->getD("HPSTauIsolationPFGammaCandsEtSum",tauIdx_);
  fill("isoValue"+lvl, nGoodVtx_ , iso, _w );
  fill("isoRelValue"+lvl, nGoodVtx_ , iso/tauL_.Pt(), _w );

  fill("etauCharge"+lvl, _vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",tauIdx_) , _w ); 
  fill("etauMass"+lvl, etau_.M() , _w ); 
  fill("etauPt"+lvl, etau_.Pt() , _w ); 
  fill("etauY"+lvl, etauY , _w ); 
  fill("etauPhi"+lvl, etau_.Phi() , _w ); 
  fill("etauDPhi"+lvl, tauL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) , _w ); 
  
 
  fill("metPhi"+lvl, TVector2::Phi_mpi_pi( met_.Phi() ) , _w );
  
  fill("eMETTMass"+lvl, tmassE , _w ); 
  fill("eMETPt"+lvl, ptEMET , _w ); 
  fill("eMETDPhi"+lvl, dphiEMET , _w ); 

  fill("nVertex"+lvl, nGoodVtx_ , _w ); 
  fill("nEl"+lvl, nGEl_ , _w ); 
  fill("nTau"+lvl, nGTau_ , _w ); 

}



void  
TauFRAnalysis::loadPUWeights() {
  
  TFile * file = new TFile("/home/mmarionn/Documents/CMS/dev/LUNE/data/rootFiles/puWeightsLQ.root","READ"); //puWeightsS10
  puweights_ = (TH1F*)file->Get("pileup");

}

float 
TauFRAnalysis::puWeight(float trueNint) {
  return puweights_->GetBinContent( puweights_->GetXaxis()->FindBin( trueNint)  );
}


bool 
TauFRAnalysis::passHLT() {

  bool pass=false;
  
  for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
      pass = triggerDecision(ih, "HLT_Ele27_WP80");
      if(pass) return true;
  }
  return false;
}


bool
TauFRAnalysis::triggerDecision(size_t ihlt, string ref) {

  if(!_au->simpleCut<size_t>(_vc->getS("HLTInsideDatasetTriggerNames",ihlt).find(ref),-1,"!=") ) return false;
  if(!_au->simpleCut<int>(_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt),1,"=")) return false;
  if(!_au->simpleCut(_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt) ) ) return false;
  return true;
}


bool 
TauFRAnalysis::tauID(size_t idx) {

  if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", idx)), 2.1, "<")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", idx), 30, ">")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauDecayModeFindingDiscr",idx),1,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauAgainstElectronMVADiscr", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstMuonLoose2", idx),1.,"=")) return false;
  //if(!_au->simpleCut<double>(_vc->getD("HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr", idx),1.,"=")) return false; // != means inverted isolation
  
  return true;
}


bool 
TauFRAnalysis::elecID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 30 , ">=" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronSCEta",idx)), 2.1 , "<=" )
     && !_au->simpleCut<double>(fabs(_vc->getD("ElectronSCEta",idx)), 1.4444,"][",1.56 ) ) return false;
  
  return (_vc->getI("ElectronPassEGammaIDMedium",idx)==1023);//1023->iso, 895=antiiso
}

void
TauFRAnalysis::selectElectron() {

  float pttmp=0;
  size_t nEl  = _vc->getSize("ElectronPt");
  for(size_t ie=0;ie<nEl;ie++) {

    if( _au->simpleCut(elecID(ie)) ) {
      if( _au->simpleCut<double>(_vc->getD("ElectronPt",ie),pttmp,">") ) {
	elL_.SetPtEtaPhiM( _vc->getD("ElectronPt",ie), 
			   _vc->getD("ElectronEta",ie),
			   _vc->getD("ElectronPhi",ie), 0.0005 );
	elIdx_ = ie;
	pttmp = elL_.Pt();
      }
      nGEl_++;
      
      if(elIdx_!=ie) {
	if( _vc->getI("ElectronCharge",elIdx_)* _vc->getI("ElectronCharge",ie) ==-1)
	  secElOS_=true;
      }

    }//ele ID
  }//for iel
 
}

void
TauFRAnalysis::selectTau() {

  float pttmp=0;
  size_t nTau = _vc->getSize("HPSTauPt");
  for(size_t it=0;it<nTau;it++) {
    if( _au->simpleCut(tauID(it)) ) {
      tmpJet_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
			    _vc->getD("HPSTauEta",it),
			    _vc->getD("HPSTauPhi",it), 1.78 );
      
      float dRel = elL_.DeltaR( tmpJet_ );
      bool isSameVtx = _vc->getI("ElectronVtxIndex",elIdx_)==_vc->getI("HPSTauVtxIndex",it);
    
      if(_au->simpleCut<double>(dRel,0.5,">") ) {
	if( _au->simpleCut(isSameVtx) ) {
	  if( _au->simpleCut<double>(_vc->getD("HPSTauPt",it),pttmp,">") ) {
	    tauL_ = tmpJet_;
	    tauIdx_ = it;
	    
	    pttmp = tauL_.Pt();
	  }
	  nGTau_++;
	}
      }
    }
  }//for itau

}


bool
TauFRAnalysis::jetVeto() {

  size_t nJets  =_vc->getSize("PFJetPt");
  for(size_t ij=0;ij<nJets;ij++) {
    
    tmpJet_.SetPtEtaPhiE( _vc->getD("PFJetPt",ij),
			  _vc->getD("PFJetEta",ij),
			  _vc->getD("PFJetPhi",ij),
			  _vc->getD("PFJetEnergy",ij) );

    float dRel = tmpJet_.DeltaR( elL_ );
    float dRtau = tmpJet_.DeltaR( tauL_ );

    if(!_au->simpleCut<double>( tmpJet_.Pt(), 30 , ">") ) continue;
    if(!_au->simpleCut<double>( fabs(tmpJet_.Eta()), 3, "<") ) continue;
    if(!_au->simpleCut<float>(dRel, 0.5, ">") ) continue;
    if(!_au->simpleCut<float>(dRtau, 0.5, ">") ) continue;

    return false;
  }

  return true;

}




float
TauFRAnalysis::modWeightDDB() {
  return 1.;
}




int
TauFRAnalysis::findZDecayChannel() {

  size_t nGen=_vc->getSize("GenParticlePt"); 
 
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),10,">") ) continue;
    
    if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") ) ) continue; 

    if(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),23,"=")) 
      return fabs(_vc->getI("GenParticlePdgId",ig));
  }

  return -1;
}
