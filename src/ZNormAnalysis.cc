#include "src/ZNormAnalysis.hh"

using namespace std;

ClassImp(ZNormAnalysis)

ZNormAnalysis::ZNormAnalysis():
UseTree() {

  loadPUWeights();

  levels_.push_back( "Presel" );
  levels_.push_back( "MultJet" );
 
  _dbm->loadDb("elID","elIDMedium.db");
  _dbm->loadDb("elTrig","TrigEff_MediumID_Ele27WP80_2D.root","h_eff");

  bTagSFTool_ = new BTagWeight(1);
  bTagDB_ = new BTagDB("CSVL","mean");
  
}

ZNormAnalysis::~ZNormAnalysis() {

}

void
ZNormAnalysis::bookHistograms() {

  for(size_t ilvl=0;ilvl<levels_.size();ilvl++) {

    //leptons
    _hm->addVariable("el1Pt"+levels_[ilvl],500,0,500,"p_{T}(e1) [GeV] ");
    _hm->addVariable("el1Eta"+levels_[ilvl],120,-3,3,"#eta(e1)  ");
    _hm->addVariable("el1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e1) [rad] ");
    _hm->addVariable("el1Charge"+levels_[ilvl],3,-1.5,1.5,"Q_{e1} ");
  
    _hm->addVariable("el2Pt"+levels_[ilvl],500,0,500,"p_{T}(e2) [GeV] ");
    _hm->addVariable("el2Eta"+levels_[ilvl],120,-3,3,"#eta(e2)  ");
    _hm->addVariable("el2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e2) [rad] ");
    _hm->addVariable("el2Charge"+levels_[ilvl],3,-1.5,1.5,"Q_{e2} ");
    
    //jets
    _hm->addVariable("jet1Pt"+levels_[ilvl],500,0,500,"p_{T}(j1) [GeV] ");
    _hm->addVariable("jet1Eta"+levels_[ilvl],120,-3,3,"#eta(j1)  ");
    _hm->addVariable("jet1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(j1) [rad] ");
  
    _hm->addVariable("jet2Pt"+levels_[ilvl],500,0,500,"p_{T}(j2) [GeV] ");
    _hm->addVariable("jet2Eta"+levels_[ilvl],120,-3,3,"#eta(j2)  ");
    _hm->addVariable("jet2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(j2) [rad] ");

    //di-e system

    _hm->addVariable("eeCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e}#times Q_{#tau} ");
    
    _hm->addVariable("eeMass"+levels_[ilvl],1000,0,1000,"M_{ee} [GeV] ");
    _hm->addVariable("eePt"+levels_[ilvl],1000,0,1000,"p_{T}(ee) [GeV] ");
    _hm->addVariable("eeY"+levels_[ilvl],120,-3,3,"Y(ee) ");
    _hm->addVariable("eePhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(ee) [rad] ");
    _hm->addVariable("eeDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(e1,e2) [rad] ");

    //met
    _hm->addVariable("metPt"+levels_[ilvl],500,0,500,"#slash{E}_{T} [GeV] ");
    _hm->addVariable("metPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#slash{E}_{T}) [rad] ");

    //e+met
    _hm->addVariable("e1METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(e1,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("e1METPt"+levels_[ilvl],1000,0,1000,"p_{T}(e1,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("e1METDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(e1,#slash{E}_{T}) [rad] ");

    _hm->addVariable("e2METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(e2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("e2METPt"+levels_[ilvl],1000,0,1000,"p_{T}(e2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("e2METDPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#Delta#phi(e2,#slash{E}_{T}) [rad] ");
    
    //multiplicity
    _hm->addVariable("nVertex"+levels_[ilvl],50,0,50,"vertex multiplicity "); 
    _hm->addVariable("nEl"+levels_[ilvl],10,0,10,"electron multiplicity "); 
    _hm->addVariable("nTau"+levels_[ilvl],10,0,10,"tau multiplicity "); 
    _hm->addVariable("nJet"+levels_[ilvl],10,0,10,"jet multiplicity "); 
    _hm->addVariable("nBJet"+levels_[ilvl],10,0,10,"b-jet multiplicity "); 
    
  }

}

void
ZNormAnalysis::modifyWeight() {

  if(!_isData) {
    float w=puWeight(_vc->getF("PileUpInteractionsTrue") );
    _w *= w;
  }
  
}


void
ZNormAnalysis::analyze() {
  
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

  size_t nTau = _vc->getSize("HPSTauPt");
  size_t nEl  = _vc->getSize("ElectronPt");
 
  nGEl_=0;
  nGTau_=0;
  
  el1L_.SetPtEtaPhiM(0.00001,0,0,0.0005);
  el2L_.SetPtEtaPhiM(0.00001,0,0,0.0005);
  jetL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetSL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBSL_.SetPtEtaPhiE(0.00001,0,0,0.);

  float pttmp=0;
  float pttmp2=0;
  for(size_t ie=0;ie<nEl;ie++) {

    if( _au->simpleCut(elecID(ie)) ) {

      tmpJet_.SetPtEtaPhiE( _vc->getD("ElectronPt",ie), 
			    _vc->getD("ElectronEta",ie),
			    _vc->getD("ElectronPhi",ie), 0.0005 );
      
      if( tmpJet_.Pt() > pttmp ) {
	el1L_ = tmpJet_;
	pttmp = el1L_.Pt();
	el1Idx_ = ie;
	nGEl_++;
      }
      if( tmpJet_.Pt() > pttmp2 && tmpJet_.Pt() < pttmp &&  
	  _vc->getI("ElectronCharge",el1Idx_)!=_vc->getI("ElectronCharge",ie) ) {
	el2L_ = tmpJet_;
	pttmp2 = el2L_.Pt();
	el2Idx_ = ie;
	nGEl_++;
      }
      
    }//ele ID
  }//for iel
  
  for(size_t it=0;it<nTau;it++) {
    if( _au->simpleCut(tauID(it)) ) {
      tmpJet_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
			    _vc->getD("HPSTauEta",it),
			    _vc->getD("HPSTauPhi",it), 1.78 );
      
      float dRel1 = el1L_.DeltaR( tmpJet_ );
      float dRel2 = el2L_.DeltaR( tmpJet_ );
      bool isSameVtx = _vc->getI("ElectronVtxIndex",el1Idx_)==_vc->getI("HPSTauVtxIndex",it);
    
      if(_au->simpleCut<double>(dRel1,0.5,">") ) {
	if(_au->simpleCut<double>(dRel2,0.5,">") ) {
	  if( _au->simpleCut(isSameVtx) ) {
	    nGTau_++;
	  }
	}
      }
    }
  }//for itau

  // float dRel = elL_.DeltaR( muL_ );
  // cout<<" -->
  
  //
  // ==============================

  //========================== Jets
  //
  size_t nJets  =_vc->getSize("PFJetPt");
  nGJet_=0;
  nGBJet_=0;
  pttmp=0;
  pttmp2=0;
  float pttmpb=0;
  float pttmpb2=0;
  //int NJnT=0;
  jetInfos_.clear();
  for(size_t ij=0;ij<nJets;ij++) {
    
    tmpJet_.SetPtEtaPhiE( _vc->getD("PFJetPt",ij),
			  _vc->getD("PFJetEta",ij),
			  _vc->getD("PFJetPhi",ij),
			  _vc->getD("PFJetEnergy",ij) );

    float dRel1 = tmpJet_.DeltaR( el1L_ );
    float dRel2 = tmpJet_.DeltaR( el2L_ );
    
    if(!_au->simpleCut<double>( tmpJet_.Pt(), 30 , ">") ) continue;
    if(!_au->simpleCut<double>( fabs(tmpJet_.Eta()), 3, "<") ) continue;
    if(!_au->simpleCut<float>(dRel1, 0.5, ">") ) continue;

    if(!_au->simpleCut<float>(dRel2, 0.5, ">") ) continue;

    nGJet_++;
    
    storeBTagInfos( tmpJet_.Pt(), tmpJet_.Eta(), 
		    _vc->getD("PFJetCombinedSecondaryVertexBTag",ij),
		    _vc->getI("PFJetPartonFlavour",ij) );
    

    if( tmpJet_.Pt() > pttmp ) {
      jetL_ = tmpJet_;
      pttmp = jetL_.Pt();
      jetLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmp2 && tmpJet_.Pt() < pttmp ) {
      jetSL_ = tmpJet_;
      pttmp2 = jetSL_.Pt();
      jetSLIdx_ = ij;
    }
    
    if(!_au->simpleCut<double>(_vc->getD("PFJetCombinedSecondaryVertexBTag",ij), 0.244, ">") ) continue; //0.679 = medium, 0.244=loose

    nGBJet_++;
    
    if( tmpJet_.Pt() > pttmpb ) {
      jetBL_ = tmpJet_;
      pttmpb = jetBL_.Pt();
      jetBLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmpb2 && tmpJet_.Pt() < pttmpb ) {
      jetBSL_ = tmpJet_;
      pttmpb2 = jetBSL_.Pt();
      jetBSLIdx_ = ij;
    }
    
  }// ijet
  
  if(!_isData)
    _w *=_dbm->getDBValue ("elID", fabs(el1L_.Eta()), el1L_.Pt() )*_dbm->getDBValue ("elID", fabs(el2L_.Eta()), el2L_.Pt() );;

  if(!_au->makeCut<int>( nGEl_, 1 , ">" ,_ids,"el multiplicity",_w) ) return;

  if(_isData) {
    if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
  }
  else {
    _w *=_dbm->getDBValue ("elTrig", el1L_.Pt(), fabs(el1L_.Eta()) );
    if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  }
  
  zCand_ = el1L_ + el2L_;
  

  if(!_au->makeCut<float>( zCand_.M() , 60 , "[]" ,_ids,"mass cut",_w, 120) ) return;
  
  
  met_.SetMagPhi( _vc->getD("PFMETType01XYCor"), _vc->getD("PFMETPhiType01XYCor") );
 
  fillPlots( "Presel");
  
  if(!_au->makeCut<int>( nGJet_, 2 , ">=" ,_ids,"jet multiplicity",_w) ) return;

  fillPlots( "MultJet");
  
}




bool ZNormAnalysis::tauID(size_t idx) {


  if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", idx)), 2.1, "<")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", idx), 30, ">")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauDecayModeFindingDiscr",idx),1,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauAgainstElectronMVADiscr", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstMuonLoose2", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr", idx),1.,"=")) return false; // != means inverted isolation
  // if(!_au->simpleCut<double>(_vc->getD("HPSTauMediumCombinedIsolationDeltaBetaCorrDiscr", idx),0.,"!=")) return false;
  return true;
}


bool ZNormAnalysis::elecID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 30 , ">=" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronSCEta",idx)), 2.1 , "<=" )
     && !_au->simpleCut<double>(fabs(_vc->getD("ElectronSCEta",idx)), 1.4444,"][",1.56 ) ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDMedium",idx)==1023);//1023->iso, 895=antiiso
}


void ZNormAnalysis::fillPlots(string lvl) {
  
  float eeY = 0.5*log( (zCand_.E()+zCand_.Pz())/(zCand_.E()-zCand_.Pz()) );

  float tmassE1  = sqrt(2*el1L_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),el1L_.Phi() ))));
  float tmassE2 = sqrt(2*el2L_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),el2L_.Phi() )))); 
  

  float ptE1MET  = (met_ + el1L_.Vect().XYvector()).Mod() ;
  float ptE2MET = (met_ + el2L_.Vect().XYvector()).Mod() ;
  

  float dphiE1MET  = met_.DeltaPhi( el1L_.Vect().XYvector() );
  float dphiE2MET = met_.DeltaPhi( el2L_.Vect().XYvector() );
 

  //======
  fill("el1Pt"+lvl, el1L_.Pt() , _w );
  fill("el1Eta"+lvl, el1L_.Eta() , _w );
  fill("el1Phi"+lvl, el1L_.Phi() , _w );
  fill("el1Charge"+lvl, _vc->getI("ElectronCharge",el1Idx_) , _w );
 

  fill("el2Pt"+lvl, el2L_.Pt() , _w );
  fill("el2Eta"+lvl, el2L_.Eta() , _w );
  fill("el2Phi"+lvl, el2L_.Phi() , _w ); 
  fill("el2Charge"+lvl, _vc->getI("ElectronCharge",el2Idx_) , _w );

  fill("jet1Pt"+lvl, jetL_.Pt() , _w );
  fill("jet1Eta"+lvl, jetL_.Eta() , _w );
  fill("jet1Phi"+lvl, jetL_.Phi() , _w );

  fill("jet2Pt"+lvl, jetSL_.Pt() , _w );
  fill("jet2Eta"+lvl, jetSL_.Eta() , _w );
  fill("jet2Phi"+lvl, jetSL_.Phi() , _w ); 

  fill("bjet1Pt"+lvl, jetBL_.Pt() , _w );
  fill("bjet1Eta"+lvl, jetBL_.Eta() , _w );
  fill("bjet1Phi"+lvl, jetBL_.Phi() , _w );

  fill("bjet2Pt"+lvl, jetBSL_.Pt() , _w );
  fill("bjet2Eta"+lvl, jetBSL_.Eta() , _w );
  fill("bjet2Phi"+lvl, jetBSL_.Phi() , _w ); 
  
  fill("metPt"+lvl, met_.Mod() , _w );
  fill("metPhi"+lvl, TVector2::Phi_mpi_pi( met_.Phi() ) , _w );
  
  fill("eeCharge"+lvl, _vc->getI("ElectronCharge",el2Idx_)*_vc->getI("ElectronCharge",el2Idx_) , _w ); 
  fill("eeMass"+lvl, zCand_.M() , _w ); 
  fill("eePt"+lvl, zCand_.Pt() , _w ); 
  fill("eeY"+lvl, eeY , _w ); 
  fill("eePhi"+lvl, zCand_.Phi() , _w ); 
  fill("eeDPhi"+lvl, el1L_.Vect().XYvector().DeltaPhi( el2L_.Vect().XYvector() ) , _w ); 
  
  
  fill("e1METTMass"+lvl, tmassE1 , _w ); 
  fill("e1METPt"+lvl, ptE1MET , _w ); 
  fill("e1METDPhi"+lvl, dphiE1MET , _w ); 

  fill("e2METTMass"+lvl, tmassE2 , _w ); 
  fill("e2METPt"+lvl, ptE2MET , _w ); 
  fill("e2METDPhi"+lvl, dphiE2MET , _w ); 
  
  fill("nVertex"+lvl, nGoodVtx_ , _w ); 
  fill("nEl"+lvl, nGEl_ , _w ); 
  fill("nTau"+lvl, nGTau_ , _w ); 
  fill("nJet"+lvl, nGJet_ , _w ); 
  fill("nBJet"+lvl, nGBJet_ , _w ); 

  fill("decHLT"+lvl, passHLT(), _w );


}




void  
ZNormAnalysis::loadPUWeights() {
  
  TFile * file = new TFile("/home/mmarionn/Documents/CMS/dev/LUNE/data/rootFiles/puWeightsLQ.root","READ"); //puWeightsS10
  puweights_ = (TH1F*)file->Get("pileup");

}

float 
ZNormAnalysis::puWeight(float trueNint) {
  return puweights_->GetBinContent( puweights_->GetXaxis()->FindBin( trueNint)  );
}





bool 
ZNormAnalysis::passHLT() {

  bool pass=false;
  
  for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
      pass = triggerDecision(ih, "HLT_Ele27_WP80");
      if(pass) return true;
  }
  return false;
}

bool
ZNormAnalysis::triggerDecision(size_t ihlt, string ref) {

  if(!_au->simpleCut<size_t>(_vc->getS("HLTInsideDatasetTriggerNames",ihlt).find(ref),-1,"!=") ) return false;
  if(!_au->simpleCut<int>(_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt),1,"=")) return false;
  if(!_au->simpleCut(_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt) ) ) return false;
  return true;
}

int
ZNormAnalysis::findZDecayChannel() {

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


float
ZNormAnalysis::modWeightDDB() {
    return 1.;
}


void
ZNormAnalysis::storeBTagInfos(float pt, float eta, float disc, int flav) {
  
  float eff = bTagDB_->getBTagEff(pt,eta,disc,flav);
  float sf = bTagDB_->getBTagSF(pt,eta,flav);

  JetInfo jetinfo(eff,sf);
  vector<JetInfo> jetInfoForAllOPs;
  jetInfoForAllOPs.push_back(jetinfo);
  jetInfos_.push_back(jetInfoForAllOPs);
}

float
ZNormAnalysis::getBTagWeight() {
  return bTagSFTool_->weight(jetInfos_);
}

