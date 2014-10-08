#include "src/LQTopAnalysis.hh"

using namespace std;

ClassImp(LQTopAnalysis)

LQTopAnalysis::LQTopAnalysis():
UseTree() {

  loadPUWeights();

  levels_.push_back( "Presel" );
  levels_.push_back( "Charge" );
  levels_.push_back( "MultJet" );
  levels_.push_back( "BTag" );
  levels_.push_back( "Final" );

  _dbm->loadDb("elID","elIDMedium.db");
  _dbm->loadDb("muID","muIDTight.db");
  _dbm->loadDb("muIso","muIsoTight.db");
  _dbm->loadDb("elIDMC","elIDMediumMCEff.db");
  _dbm->loadDb("muIDMC","muIDTightMCEff.db");
  _dbm->loadDb("muIsoMC","muIsoTightMCEff.db");
  _dbm->loadDb("elTrig","TrigEff_MediumID_Ele27WP80_2D.root","h_eff");
  _dbm->loadDb("muTrig","muHLT.db");
  _dbm->loadDb("ptW","PTWeights.root","PTweights");
  _dbm->loadDb("muHLTD","muHLTMCEff.db");

  bTagSFTool_ = new BTagWeight(1);
  bTagDB_ = new BTagDB("CSVL","mean");

  effeNH=0.;
  effe = 0.;
  effm = 0.;
  deffe_2=0.;
  deffm_2=0;
  N=0;

}

LQTopAnalysis::~LQTopAnalysis() {

}

void
LQTopAnalysis::bookHistograms() {

  _hm->addVariable("btagWeight", 100,0, 2.,"b-tag weight ");

  for(size_t ilvl=0;ilvl<levels_.size();ilvl++) {

    //leptons
    _hm->addVariable("elPt"+levels_[ilvl],500,0,500,"p_{T}(e) [GeV] ");
    _hm->addVariable("elEta"+levels_[ilvl],240,-3,3,"#eta(e)  ");
    _hm->addVariable("elPhi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e) [rad] ");
  
    _hm->addVariable("muPt"+levels_[ilvl],500,0,500,"p_{T}(#mu) [GeV] ");
    _hm->addVariable("muEta"+levels_[ilvl],120,-3,3,"#eta(#mu)  ");
    _hm->addVariable("muPhi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#mu) [rad] ");
    _hm->addVariable("muIso"+levels_[ilvl],200,0,0.2,"I_{#mu} ");
    _hm->addVariable("muChIso"+levels_[ilvl],200,0,0.2,"I_{#mu}(ch) ");
    _hm->addVariable("muNhIso"+levels_[ilvl],200,0,0.2,"I_{#mu}(nh) ");
    _hm->addVariable("muPhIso"+levels_[ilvl],200,0,0.2,"I_{#mu}(ph) ");
    _hm->addVariable("muPuIso"+levels_[ilvl],200,0,0.2,"I_{#mu}(pu) ");
    _hm->addVariable("muIsoVsEta"+levels_[ilvl],100,-3,3,100,0,0.2,"#eta(#mu)","I_{#mu} ");
  
    _hm->addVariable("PtVsPt"+levels_[ilvl],500,0,500,500,0,500,
		     "p_{T}(e) [GeV] ","p_{T}(#tau) [GeV] ");


    // _hm->addVariable("muPtIso"+levels_[ilvl],500,0,500,"p_{T}(#mu) [GeV] ");
    // _hm->addVariable("muPtAllIso"+levels_[ilvl],500,0,500,"p_{T}(#mu) [GeV] ");
  
    // _hm->addVariable("tauPt"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    // _hm->addVariable("tauEta"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    // _hm->addVariable("tauPhi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#tau) [rad] ");

    //jets
    _hm->addVariable("jet1Pt"+levels_[ilvl],500,0,500,"p_{T}(j1) [GeV] ");
    _hm->addVariable("jet1Eta"+levels_[ilvl],120,-3,3,"#eta(j1)  ");
    _hm->addVariable("jet1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(j1) [rad] ");
  
    _hm->addVariable("jet2Pt"+levels_[ilvl],500,0,500,"p_{T}(j2) [GeV] ");
    _hm->addVariable("jet2Eta"+levels_[ilvl],120,-3,3,"#eta(j2)  ");
    _hm->addVariable("jet2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(j2) [rad] ");
  
    //jets
    _hm->addVariable("bjet1Pt"+levels_[ilvl],500,0,500,"p_{T}(b-j1) [GeV] ");
    _hm->addVariable("bjet1Eta"+levels_[ilvl],120,-3,3,"#eta(b-j1)  ");
    _hm->addVariable("bjet1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(b-j1) [rad] ");
  
    _hm->addVariable("bjet2Pt"+levels_[ilvl],500,0,500,"p_{T}(b-j2) [GeV] ");
    _hm->addVariable("bjet2Eta"+levels_[ilvl],120,-3,3,"#eta(b-j2)  ");
    _hm->addVariable("bjet2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(b-j2) [rad] ");

    //e+mu variables
    _hm->addVariable("emuCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e}#times Q_{#mu} ");
  
    _hm->addVariable("emuMass"+levels_[ilvl],1000,0,1000,"M(e,#mu) [GeV] ");
    _hm->addVariable("emuPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#mu) [GeV] ");
    _hm->addVariable("emuY"+levels_[ilvl],120,-3,3,"Y(e,#mu) ");
    _hm->addVariable("emuPhi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e,#mu) [rad] ");
    _hm->addVariable("emuDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,#mu) [rad] ");
    _hm->addVariable("emuDEta"+levels_[ilvl],100,-5.,5,"#Delta#eta(e,#mu) ");
    _hm->addVariable("emuDR"+levels_[ilvl],100,0,5.,"#DeltaR(e,#mu) ");
    _hm->addVariable("emuDEtaDPhi"+levels_[ilvl],128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#eta(e,#mu) ","#Delta#phi(e,#mu) [rad] ");
    _hm->addVariable("elEtaMuEta"+levels_[ilvl],100,-2.5,2.5,100,-2.5,2.5,"#eta_{e} ","#eta{#mu} ");

    _hm->addVariable("emuj1DPhi2D"+levels_[ilvl],128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(e,j1) [rad] ","#Delta#phi(e,#mu) [rad] ");
    _hm->addVariable("emuj2DPhi2D"+levels_[ilvl],128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(e,j2) [rad] ","#Delta#phi(e,#mu) [rad] ");

    //e+jet1
    _hm->addVariable("ej1Mass"+levels_[ilvl],1000,0,1000,"M(e,j1) [GeV] ");
    _hm->addVariable("ej1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,j1) [GeV] ");
    _hm->addVariable("ej1DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,j1) [rad] ");
    _hm->addVariable("ej1DR"+levels_[ilvl],200,0,5,"#DeltaR(e,j1) ");
    // _hm->addVariable("ej1Y"+levels_[ilvl],120,-3,3,"Y(e,j1) ");
    // _hm->addVariable("ej1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e,j1) [rad] ");

    //e+jet2
    _hm->addVariable("ej2Mass"+levels_[ilvl],1000,0,1000,"M(e,j2) [GeV] ");
    _hm->addVariable("ej2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,j2) [GeV] ");
    _hm->addVariable("ej2DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,j2) [rad] ");
    _hm->addVariable("ej2DR"+levels_[ilvl],200,0,5,"#DeltaR(e,j2) ");
    // _hm->addVariable("ej2Y"+levels_[ilvl],120,-3,3,"Y(e,j2) ");
    // _hm->addVariable("ej2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e,j2) [rad] ");

    //e+bjet1
    _hm->addVariable("ebj1Mass"+levels_[ilvl],1000,0,1000,"M(e,b-j1) [GeV] ");
    _hm->addVariable("ebj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,b-j1) [GeV] ");
    _hm->addVariable("ebj1DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,b-j1) [rad] ");
    // _hm->addVariable("ebj1Y"+levels_[ilvl],120,-3,3,"Y(e,b-j1) ");
    // _hm->addVariable("ebj1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e,b-j1) [rad] ");

    //e+bjet2
    _hm->addVariable("ebj2Mass"+levels_[ilvl],1000,0,1000,"M(e,b-j2) [GeV] ");
    _hm->addVariable("ebj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,b-j2) [GeV] ");
    _hm->addVariable("ebj2DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,b-j2) [rad] ");
    // _hm->addVariable("ebj2Y"+levels_[ilvl],120,-3,3,"Y(e,b-j2) ");
    // _hm->addVariable("ebj2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(e,b-j2) [rad] ");


    //mu+jet1
    _hm->addVariable("muj1Mass"+levels_[ilvl],1000,0,1000,"M(#mu,j1) [GeV] ");
    _hm->addVariable("muj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#mu,j1) [GeV] ");
    _hm->addVariable("muj1DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#mu,j1) [rad] ");
    _hm->addVariable("muj1DR"+levels_[ilvl],200,0,5,"#DeltaR(#mu,j1) ");
    // _hm->addVariable("muj1Y"+levels_[ilvl],120,-3,3,"Y(#mu,j1) ");
    // _hm->addVariable("muj1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#mu,j1) [rad] ");
  
    //mu+jet2
    _hm->addVariable("muj2Mass"+levels_[ilvl],1000,0,1000,"M(#mu,j2) [GeV] ");
    _hm->addVariable("muj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#mu,j2) [GeV] ");
    _hm->addVariable("muj2DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#mu,j2) [rad] ");
    _hm->addVariable("muj2DR"+levels_[ilvl],200,0,5,"#DeltaR(#mu,j2) ");
    //_hm->addVariable("muj2Y"+levels_[ilvl],120,-3,3,"Y(#mu,j2) ");
    //_hm->addVariable("muj2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#mu,j2) [rad] ");
  

    //mu+bjet1
    _hm->addVariable("mubj1Mass"+levels_[ilvl],1000,0,1000,"M(#mu,b-j1) [GeV] ");
    _hm->addVariable("mubj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#mu,b-j1) [GeV] ");
    _hm->addVariable("mubj1DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#mu,b-j1) [rad] ");
    // _hm->addVariable("mubj1Y"+levels_[ilvl],120,-3,3,"Y(#mu,b-j1) ");
    // _hm->addVariable("mubj1Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#mu,b-j1) [rad] ");
  
    //mu+bjet2
    _hm->addVariable("mubj2Mass"+levels_[ilvl],1000,0,1000,"M(#mu,b-j2) [GeV] ");
    _hm->addVariable("mubj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#mu,b-j2) [GeV] ");
    _hm->addVariable("mubj2DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#mu,b-j2) [rad] ");
    // _hm->addVariable("mubj2Y"+levels_[ilvl],120,-3,3,"Y(#mu,b-j2) ");
    // _hm->addVariable("mubj2Phi"+levels_[ilvl],256,0,TMath::Pi(),"#phi(#mu,b-j2) [rad] ");
  
    _hm->addVariable("j1j2DPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(j1,j2) [rad] ");
    _hm->addVariable("j1j2DEta"+levels_[ilvl],100,-5,5,"#Delta#eta(j1,j2) ");


    //e+met
    _hm->addVariable("eMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,#slash{E}_{T}) [rad] ");

    //mu+met
    _hm->addVariable("muMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(#mu,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("muMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(#mu,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("muMETDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#mu,#slash{E}_{T}) [rad] ");

    //jet+met
    _hm->addVariable("j1METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("j1METPt"+levels_[ilvl],1000,0,1000,"p_{T}(j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("j1METDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(j1,#slash{E}_{T}) [rad] ");
    _hm->addVariable("j2METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("j2METPt"+levels_[ilvl],1000,0,1000,"p_{T}(j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("j2METDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(j2,#slash{E}_{T}) [rad] ");

    //bjet+met
    _hm->addVariable("bj1METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(b-j1,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("bj1METPt"+levels_[ilvl],1000,0,1000,"p_{T}(b-j1,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("bj1METDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(b-j1,#slash{E}_{T}) [rad] ");
    _hm->addVariable("bj2METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(b-j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("bj2METPt"+levels_[ilvl],1000,0,1000,"p_{T}(b-j2,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("bj2METDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(b-j2,#slash{E}_{T}) [rad] ");

    //ST
    _hm->addVariable("STjet"+levels_[ilvl],1000,0,2000,"S_{T,jet} [GeV] ");
    _hm->addVariable("STbjet"+levels_[ilvl],1000,0,2000,"S_{T} [GeV] ");
    
    //multiplicity
    _hm->addVariable("nVertex"+levels_[ilvl],50,0,50,"vertex multiplicity "); 
    _hm->addVariable("nEl"+levels_[ilvl],10,0,10,"electron multiplicity "); 
    _hm->addVariable("nMu"+levels_[ilvl],10,0,10,"muon multiplicity "); 
    _hm->addVariable("nJet"+levels_[ilvl],10,0,10,"jet multiplicity "); 
    _hm->addVariable("nBJet"+levels_[ilvl],10,0,10,"b-jet multiplicity "); 

    //miscellaneous
    _hm->addVariable("decHLT"+levels_[ilvl],2,-0.5,1.5,"decision HLT "); 
    _hm->addVariable("testHLT"+levels_[ilvl],4,-0.5,3.5,"HLT "); 

    _hm->addVariable("run"+levels_[ilvl],2000,190000,210000,"run"); 

      }

  _hm->addVariable("bPdgId",21,0,21,"bPdgId"); 
  _hm->addVariable("finalMass",1000,0,1000,"M_{1} [GeV] ");

}

float
LQTopAnalysis::modWeightDDB() {
  return 1.;
}

void
LQTopAnalysis::modifyWeight() {

  if(!_isData && _ids!="Ele" && _ids!="Mu" ) {
    float w=puWeight(_vc->getF("PileUpInteractionsTrue") );
    //_w *= w;
  }
  
}


void
LQTopAnalysis::analyze() {
  

  _au->makeCut(true,_ids,"beginning",_w);

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

  //Splittint ttbar ================================
  // vector<int> pdgIds=findTTDecayChannel();
  // if(!mif(pdgIds.size()==2)) return;
  // bool dec= (pdgIds[0]==13 && pdgIds[1]==15) || (pdgIds[0]==15 && pdgIds[1]==13);
  // if(!mif(dec)) return;

  //======================================================
  _au->makeCut(true,_ids,"emu gen events",_w);
  
  nGoodVtx_=0;

  for(size_t ivtx = 0; ivtx < _vc->getSize("VertexRho") ; ++ivtx) {
    //double chi2 = _vc->getD("VertexChi2",ivtx);
    double ndf  = _vc->getD("VertexNDF",ivtx);
    double z    = _vc->getD("VertexZ",ivtx);
    double rho  = _vc->getD("VertexRho",ivtx);
	
    if (!(_vc->getB("VertexIsFake",ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2 ) {
      ++nGoodVtx_; 
    
    }
  }			

  if(!_au->makeCut<int>(nGoodVtx_,1,">=",_ids,"has good vtx",_w)) return;
  
  
  //========================= Leptons
  //

  size_t nMu  =_vc->getSize("MuonPt");
  size_t nTau = _vc->getSize("HPSTauPt");
  size_t nEl  = _vc->getSize("ElectronPt");

  nGEl_=0;
  nGMu_=0;
  nGTau_=0;
  secLepOS_=false;

  elL_.SetPtEtaPhiM(0.00001,0,0,0.0005);
  muL_.SetPtEtaPhiM(0.00001,0,0,0.106);
  tauL_.SetPtEtaPhiM(0.00001,0,0,1.78);
  jetL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetSL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBSL_.SetPtEtaPhiE(0.00001,0,0,0.);

  float pttmp=0;
  
  //========== Gen matching
  // size_t mIdx=-1;
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=")//  ||
  // 	   //_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") 
  // 	   ) ) continue;
  //   if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 24, "=") || 
  // 	 _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 15, "=") 
  // 	 )) continue;
  //   mIdx = _vc->getI("GenParticleMotherIndex",ig);
    //========== Gen matching
    for(size_t im=0;im<nMu;im++) {

      //gen matching
      // if(_au->simpleCut(!_isData) )
      // 	if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("MuonEta",im) ,_vc->getD("GenParticleEta",ig), _vc->getD("MuonPhi",im), _vc->getD("GenParticlePhi",ig)), 0.2 ,"<")) continue; 

      if( !_au->simpleCut(muonID(im)) ) continue;
    
      if( _au->simpleCut<double>(_vc->getD("MuonPt",im),pttmp,">") ) {
	muL_.SetPtEtaPhiM( _vc->getD("MuonPt",im), 
			   _vc->getD("MuonEta",im),
			   _vc->getD("MuonPhi",im), 0.106 );
	muIdx_ = im;
	pttmp = muL_.Pt();
      }
      nGMu_++;
      
    }//for imu
    //}//gen matching

    for(size_t im=0;im<nMu;im++) {
      if( _au->simpleCut(muonLooseID(im) ) ) {
	if( muIdx_!=im) {
	  //cout<<muIdx_<<"   "<<im<<endl;
	  secLepOS_=true; break;
	}
      }
    }
    

  // //cout<<elL_.Pt()<<"  //  "<<elL_.Eta()<<"   --> "<<_dbm->getDBValue ("elID", fabs(elL_.Eta()), elL_.Pt() )<<"    "<<_dbm->getDBValue("elTrig", elL_.Pt())<<endl;
  //if(!_au->simpleCut( mIdx!=(size_t)-1 ) ) return;

  pttmp=0;
  //========== Gen matching
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if( !( _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") //||
  // 	    //_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=")
  // 	     ) ) continue; 
    // if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 24, "=") || 
    // 	 _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 15, "=") 
    // 	 )) continue;
  //========== Gen matching
  for(size_t ie=0;ie<nEl;ie++) {

    //gen matching
    // if(_au->simpleCut(!_isData) )
    // 	if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("ElectronEta",ie) ,_vc->getD("GenParticleEta",ig), _vc->getD("ElectronPhi",ie), _vc->getD("GenParticlePhi",ig)), 0.2 ,"<")) continue;
      
    
    if( !_au->simpleCut(elecID(ie)) ) continue;
      
    tmpJet_.SetPtEtaPhiM( _vc->getD("ElectronPt",ie), 
			  _vc->getD("ElectronEta",ie),
			  _vc->getD("ElectronPhi",ie), 0.0005 );
      
    float dRel = muL_.DeltaR( tmpJet_ );
    //
    if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
    if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",ie)*_vc->getI("MuonCharge",muIdx_),-1,"=") ) continue;
    if( _au->simpleCut<double>(_vc->getD("ElectronPt",ie),pttmp,">") ) {
      elL_ = tmpJet_;
	   
	   
      elIdx_ = ie;
      pttmp = elL_.Pt();
    }
    nGEl_++; //FIXME
	    
  }//for iel
 
  //  }//gen matching

  for(size_t ie=0;ie<nEl;ie++) {
    if( _au->simpleCut(elLooseID(ie) ) ) {
      if( elIdx_!=ie) {
	secLepOS_=true;
	break;
      }
    }
  }
    

  pttmp=0;
 //========== Gen matching
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if( !( _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") //||
  // 	    //_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=")
  // 	     ) ) continue; 
  //   if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 24, "=") || 
  //   	 _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 15, "=") 
  //   	 )) continue;
  //========== Gen matching
  // for(size_t it=0;it<nTau;it++) {

  //   // if(_au->simpleCut(!_isData) )
  //   //   if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("HPSTauEta",it) ,_vc->getD("GenParticleEta",ig), _vc->getD("HPSTauPhi",it), _vc->getD("GenParticlePhi",ig)), 0.2 ,"<")) continue;

  //   if( _au->simpleCut(tauID(it)) ) {
  //     tmpJet_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
  // 			    _vc->getD("HPSTauEta",it),
  // 			    _vc->getD("HPSTauPhi",it), 1.78 );
      
  //     float dRel = elL_.DeltaR( tmpJet_ );
  //     bool isSameVtx = _vc->getI("ElectronVtxIndex",elIdx_)==_vc->getI("HPSTauVtxIndex",it);
  //     //cout<<_vc->getD("HPSTauPt",it)<<endl;
  //     if(_au->simpleCut<double>(dRel,0.5,">") ) {
  // 	if( _au->simpleCut(isSameVtx) ) {
  // 	  if( _au->simpleCut<double>(_vc->getD("HPSTauPt",it),pttmp,">") ) {
  // 	    tauL_ = tmpJet_;
  // 	    tauIdx_ = it;
  // 	    pttmp = tauL_.Pt();
  // 	  }
  // 	  nGEl_++;
  // 	}
  //     }
  //   }
  // }//for itau
  //}//gen matching
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
  float pttmp2=0;
  float pttmpb=0;
  float pttmpb2=0;
  int NJnT=0;
  jetInfos_.clear();
  for(size_t ij=0;ij<nJets;ij++) {
    
    tmpJet_.SetPtEtaPhiE( _vc->getD("PFJetPt",ij),
			  _vc->getD("PFJetEta",ij),
			  _vc->getD("PFJetPhi",ij),
			  _vc->getD("PFJetEnergy",ij) );

    float dRel = tmpJet_.DeltaR( elL_ );
    float dRmu = tmpJet_.DeltaR( muL_ );
    float dRtau = tmpJet_.DeltaR( tauL_ );
  
    if(!_au->simpleCut<double>( tmpJet_.Pt(), 30 , ">") ) continue;
    if(!_au->simpleCut<double>( fabs(tmpJet_.Eta()), 3, "<") ) continue;
    if(!_au->simpleCut<float>(dRel, 0.5, ">") ) continue;

    if(_au->simpleCut<float>(dRtau, 0.5, ">") ) {
      NJnT++;
    }

    if(!_au->simpleCut<float>(dRmu, 0.5, ">") ) continue;

    if(!_au->simpleCut<int>(_vc->getI("PFJetPassLooseID",ij), 1, "=") ) continue;

    nGJet_++;
    
    storeBTagInfos( tmpJet_.Pt(), tmpJet_.Eta(), 
		    _vc->getD("PFJetCombinedSecondaryVertexBTag",ij),
		    _vc->getI("PFJetPartonFlavour",ij) );

    if( tmpJet_.Pt() > pttmp ) {
      jetL_ = tmpJet_;
      pttmp = jetL_.Pt();
      jetLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmp2 && tmpJet_.Pt() < pttmp && jetLIdx_!=ij ) {
      jetSL_ = tmpJet_;
      pttmp2 = jetSL_.Pt();
      jetSLIdx_ = ij;
    }
    
    if(!_au->simpleCut<double>(_vc->getD("PFJetCombinedSecondaryVertexBTag",ij), 0.244, ">") ) continue;

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

  
 
  
  if(!_au->makeCut<int>( nGMu_, 0 , ">" ,_ids,"mu multiplicity",_w) ) return;

  if(!_isData)
    _w *=_dbm->getDBValue ("muID" , fabs(muL_.Eta()), muL_.Pt() )
      *_dbm->getDBValue ("muIso", fabs(muL_.Eta()), muL_.Pt() );

  //if(!_au->makeCut( !secLepOS_ ,_ids,"no OS lepton",_w) ) return;

  //if( fabs(elL_.Eta())<1.56 && fabs(elL_.Eta())>1.44444) cout<<" --> "<<elL_.Pt()<<"   "<<elL_.Eta()<<endl;

  //if(_isData) {
    if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
    //}
  // else {
  // //   _w *=_dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
  //   if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  // }
  
  // if(!_isData)
  //   _w *=_dbm->getDBValue ("muTrig", fabs(muL_.Eta()), muL_.Pt() );
  
  if(!_au->makeCut<int>( nGEl_, 0 , ">" ,_ids,"el multiplicity",_w) ) return;
 
  // if(!_isData)
  //   _w *=_dbm->getDBValue ("elID", fabs(elL_.Eta()), elL_.Pt() );

  //met_.SetMagPhi( _vc->getD("PFMETPatType1"), _vc->getD("PFMETPhiPatType1") );
  //met_.SetMagPhi( _vc->getD("PFMETType01XYCor"), _vc->getD("PFMETPhiType01XYCor") );
  //_w*=_dbm->getDBValue ("ptW", elL_.Pt(), muL_.Pt()/1000. );
  // cout<<_dbm->getDBValue ("ptW", elL_.Pt(), muL_.Pt()/1000. )<<endl;

  fillPlots( "Presel");

  //2 >=
  
  if(!_au->makeCut<int>( nGJet_, 2 , ">=" ,_ids,"jet multiplicity",_w) ) return;
  
  fillPlots( "MultJet");

  // if(!_isData)
  //   _w *=getBTagWeight();
  if(!_au->makeCut<int>( nGBJet_, 1 , ">=" ,_ids,"b-tagging",_w) ) return;



  bool isSameVtx=false;
  for(size_t ivtx = 0; ivtx < _vc->getSize("VertexRho") ; ++ivtx) {
    //double chi2 = _vc->getD("VertexChi2",ivtx);
    double ndf  = _vc->getD("VertexNDF",ivtx);
    double z    = _vc->getD("VertexZ",ivtx);
    double rho  = _vc->getD("VertexRho",ivtx);

    bool goodVtx = !(_vc->getB("VertexIsFake",ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2;
	
    if(_au->simpleCut(goodVtx) ) {
      bool isGVMu = (int)ivtx==_vc->getI("MuonVtxIndex",muIdx_);
      bool isGVEl = _vc->getI("ElectronVtxIndex",elIdx_)==(int)ivtx;
      isSameVtx=isGVMu && isGVEl;

      if(isSameVtx) break;
    }
  }
  
  if(!_au->makeCut(isSameVtx, _ids,"Vtx matching", _w ) ) return;

  if(!_au->makeCut<int>( _vc->getI("ElectronCharge",elIdx_)*_vc->getI("MuonCharge",muIdx_), -1 , "=" ,_ids,"opposite charge",_w) ) return;

  fillPlots( "Charge");

  effeNH += _dbm->getDBValue ("elIDMC", fabs(elL_.Eta()), elL_.Pt() );
  effe += _dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
  effm += _dbm->getDBValue ("muIDMC", fabs(muL_.Eta()), muL_.Pt() )*_dbm->getDBValue ("muIsoMC", fabs(muL_.Eta()), muL_.Pt() );
  deffe_2 += pow( _dbm->getDBErrL("elIDMC", fabs(elL_.Eta()), elL_.Pt()), 2);
  deffm_2 += pow( _dbm->getDBErrL("muIDMC", fabs(muL_.Eta()), muL_.Pt()), 2);
  effmHLT +=  _dbm->getDBValue("muHLTD", fabs(muL_.Eta()), muL_.Pt() );
  deffmHLT_2 += pow( _dbm->getDBErrL("muHLTD", fabs(muL_.Eta()), muL_.Pt()), 2);
  N++;

  if(!_au->makeCut(!secLepOS_, _ids,"veto add lepton", _w ) ) return;

 
  
  
  //cout<<jetLIdx_<<"  "<<jetSLIdx_<<" ------------ "<<jetL_.Pt()<<"/"<<jetSL_.Pt()<<"    "<<jetL_.Eta()<<"/"<<jetSL_.Eta() <<endl;

  // btagging

  fill("btagWeight", getBTagWeight(), _w );
 

  //_w *= getPtWeight( muL_.Pt() );
  //cout<<muL_.Pt()<<"   "<<getPtWeight( muL_.Pt() )<<endl;


  fillPlots( "BTag");
  
  float M11 = (elL_ +jetBL_).M();
  float M12 = (elL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  float M21 = (muL_+jetBL_).M();
  float M22 = (muL_+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
  float m1=0;//,m2=0;
  if (fabs(M11-M22) < fabs(M12-M21)) {
    m1 = M22;
    //m2 = M11;
  }else{
    m1 = M21;
    //m2 = M12;
  }
  


  // float tmpdr=0.4;
  // int pdgId=-1;
  // nGen=_vc->getSize("GenParticlePt"); 
  // if(!_isData) {
  //   for(size_t ig=0;ig<nGen;ig++) {
  //     if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),22,"<=")) ) continue;
     
  // 	 float dr = KineUtils::dR( jetBL_.Eta()  ,_vc->getD("GenParticleEta",ig), jetBL_.Phi() , _vc->getD("GenParticlePhi",ig));
  // 	 float dpt = (jetBL_.Pt()-_vc->getD("GenParticlePt",ig))/jetBL_.Pt();
 
  // 	 //if(dpt< 0.33) continue;

  // 	 if(tmpdr>dr) {
  // 	   pdgId = fabs(_vc->getI("GenParticlePdgId",ig));
  // 	   tmpdr = dr;
  // 	   if(pdgId==11) {
  // 	     cout<<dr<<"   "<<dpt<<"   "
  // 		 <<_vc->getD("GenParticleEta",ig)<<"   "<<elL_.Eta()<<"   "
  // 		 <<_vc->getD("GenParticlePhi",ig)<<"   "<<elL_.Phi()<<endl;
  // 	   }
  // 	 }
     
  //   }
  //   fill("bPdgId",pdgId,1.);
  // }


  //_w *= getPtWeight( muL_.Pt() );

  if(!_au->makeCut<float>( muL_.Pt() , 50 , ">" ,_ids,"tau pT cut",_w) ) return;
  
  fill("finalMass",m1,_w);
  
  if(!_au->makeCut<float>( m1, 250, ">", _ids,"tauB Mass",_w) ) return;  
  

  fillPlots( "Final");
 
  
}




bool LQTopAnalysis::tauID(size_t idx) {


  if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", idx)), 2.3, "<")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", idx), 30, ">")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauDecayModeFindingDiscr",idx),1,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstElectronLooseMVA3", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstMuonLoose2", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", idx),1.,"=")) return false;
  
  return true;
}

bool LQTopAnalysis::muonID(size_t idx) {

  if(!_au->simpleCut<double>( _vc->getD("MuonPt",idx), 30 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonEta",idx)), 2.1 , "<" ) ) return false;
  
  if(!_au->simpleCut<int>( _vc->getI("MuonPassID",idx), 1, "=" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonIsPF",idx), 1, "=" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonStationMatches",idx), 0, ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonBestTrackVtxDistXY",idx)), 0.2, "<" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonBestTrackVtxDistZ",idx)), 0.5, "<" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonPixelHits",idx), 0, ">" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonTrackLayersWithMeasurement",idx), 5, ">" ) ) return false;

  float iso = _vc->getD("MuonPFIsoR04ChargedHadron",idx) + 
    max( _vc->getD("MuonPFIsoR04NeutralHadron",idx) +
	 _vc->getD("MuonPFIsoR04Photon",idx) -
	 0.5*_vc->getD("MuonPFIsoR04PU",idx) ,0.);
  iso /= _vc->getD("MuonPt",idx);

  if( !_au->simpleCut<float>(iso, 0.12, "<") ) return false;

  return true;

}

bool LQTopAnalysis::muonLooseID(size_t idx) {

  if(!_au->simpleCut<double>( _vc->getD("MuonPt",idx), 20 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonEta",idx)), 2.4 , "<" ) ) return false;
  
  if(!_au->simpleCut<bool>( _vc->getI("MuonIsGlobal",idx), true, "=" ) ) return false;
  if(!_au->simpleCut<bool>( _vc->getI("MuonIsPF",idx), true, "=" ) ) return false;
  if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",elIdx_)*_vc->getI("MuonCharge",idx),-1,"=") ) return false;

  float iso = _vc->getD("MuonPFIsoR04ChargedHadron",idx) + 
    max( _vc->getD("MuonPFIsoR04NeutralHadron",idx) + _vc->getD("MuonPFIsoR04Photon",idx) - 0.5*_vc->getD("MuonPFIsoR04PU",idx) ,0.);
  iso /= _vc->getD("MuonPt",idx);

  if( !_au->simpleCut<float>(iso, 0.3, "<") ) return false;

  return true;

}




bool LQTopAnalysis::elecID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 30 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronEta",idx)), 2.5 , "<" ) ) return false;
  if(!_au->simpleCut<double>(fabs(_vc->getD("ElectronEta",idx)), 1.4442,"]![",1.56 ) ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDMedium",idx)==1023);
}

bool LQTopAnalysis::elLooseID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 20 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronEta",idx)), 2.5 , "<" ) ) return false;
  if(!_au->simpleCut<double>(fabs(_vc->getD("ElectronEta",idx)), 1.4442,"]![",1.56 ) ) return false;
  if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",idx)*_vc->getI("MuonCharge",muIdx_),-1,"=") ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDLoose",idx)==1023);
}


void LQTopAnalysis::fillPlots(string lvl) {

  emu_  = elL_ + muL_;
  ej1_  = elL_ + jetL_;
  ej2_  = elL_ + jetSL_;
  muj1_ = muL_ + jetL_;
  muj2_ = muL_ + jetSL_;

  ebj1_  = elL_ + jetBL_;
  ebj2_  = elL_ + jetBSL_;
  mubj1_ = muL_ + jetBL_;
  mubj2_ = muL_ + jetBSL_;


  float emuY = 0.5*log( (emu_.E()+emu_.Pz())/(emu_.E()-emu_.Pz()) );

  float tmassE  = sqrt(2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))));
  float tmassMu = sqrt(2*muL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),muL_.Phi() )))); 
  float tmassJ1 = sqrt(2*jetL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetL_.Phi() ))));
  float tmassJ2 = sqrt(2*jetSL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetSL_.Phi() ))));
  float tmassBJ1 = sqrt(2*jetL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetL_.Phi() ))));
  float tmassBJ2 = sqrt(2*jetSL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetSL_.Phi() ))));

  float ptEMET  = (met_ + elL_.Vect().XYvector()).Mod() ;
  float ptMuMET = (met_ + muL_.Vect().XYvector()).Mod() ;
  float ptJ1MET = (met_ + jetL_.Vect().XYvector()).Mod() ;
  float ptJ2MET = (met_ + jetSL_.Vect().XYvector()).Mod() ;
  float ptBJ1MET = (met_ + jetBL_.Vect().XYvector()).Mod() ;
  float ptBJ2MET = (met_ + jetBSL_.Vect().XYvector()).Mod() ;

  float dphiEMET  = fabs( met_.DeltaPhi( elL_.Vect().XYvector() ) );
  float dphiMuMET = fabs( met_.DeltaPhi( muL_.Vect().XYvector() ) );
  float dphiJ1MET = fabs( met_.DeltaPhi( jetL_.Vect().XYvector() ) );
  float dphiJ2MET = fabs( met_.DeltaPhi( jetSL_.Vect().XYvector() ) );
  float dphiBJ1MET = fabs( met_.DeltaPhi( jetBL_.Vect().XYvector() ) );
  float dphiBJ2MET = fabs( met_.DeltaPhi( jetBSL_.Vect().XYvector() ) );

  float dphiEJ1 = fabs( elL_.Vect().XYvector().DeltaPhi( jetL_.Vect().XYvector() ) );
  float dphiEJ2 = fabs( elL_.Vect().XYvector().DeltaPhi( jetSL_.Vect().XYvector() ) );
  float dphiEBJ1 = fabs( elL_.Vect().XYvector().DeltaPhi( jetBL_.Vect().XYvector() ) );
  float dphiEBJ2 = fabs( elL_.Vect().XYvector().DeltaPhi( jetBSL_.Vect().XYvector() ) );

  float dphiMJ1 = fabs( muL_.Vect().XYvector().DeltaPhi( jetL_.Vect().XYvector() ) );
  float dphiMJ2 = fabs( muL_.Vect().XYvector().DeltaPhi( jetSL_.Vect().XYvector() ) );
  float dphiMBJ1 = fabs( muL_.Vect().XYvector().DeltaPhi( jetBL_.Vect().XYvector() ) );
  float dphiMBJ2 = fabs( muL_.Vect().XYvector().DeltaPhi( jetBSL_.Vect().XYvector() ) );
  
  //======
  fill("elPt"+lvl, elL_.Pt() , _w );
  fill("elEta"+lvl, elL_.Eta() , _w );
  fill("elPhi"+lvl, elL_.Phi() , _w );

  fill("muPt"+lvl, muL_.Pt() , _w );
  fill("muEta"+lvl, muL_.Eta() , _w );
  fill("muPhi"+lvl, muL_.Phi() , _w ); 
  // float iso = _vc->getD("MuonPFIsoR04ChargedHadron",muIdx_) + 
  //   max( _vc->getD("MuonPFIsoR04NeutralHadron",muIdx_) + _vc->getD("MuonPFIsoR04Photon",muIdx_) - 0.5*_vc->getD("MuonPFIsoR04PU",muIdx_) ,0.);
  // iso /= _vc->getD("MuonPt",muIdx_);
  // fill("muIso"+lvl,  iso, _w);
  // fill("muChIso"+lvl,  _vc->getD("MuonPFIsoR04ChargedHadron",muIdx_)/_vc->getD("MuonPt",muIdx_), _w);
  // fill("muNhIso"+lvl,  _vc->getD("MuonPFIsoR04NeutralHadron",muIdx_)/_vc->getD("MuonPt",muIdx_), _w);
  // fill("muPhIso"+lvl,  _vc->getD("MuonPFIsoR04Photon",muIdx_)/_vc->getD("MuonPt",muIdx_), _w);
  // fill("muPuIso"+lvl,  _vc->getD("MuonPFIsoR04PU",muIdx_)/_vc->getD("MuonPt",muIdx_), _w);
  
  // fill("muIsoVsEta"+lvl, muL_.Eta(), iso, _w );

  fill("PtVsPt"+lvl, elL_.Pt(), muL_.Pt() , _w );

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
  
  //fill("emuCharge"+lvl, _vc->getI("ElectronCharge",elIdx_)*_vc->getI("MuonCharge",muIdx_) , _w ); 
  fill("emuMass"+lvl, emu_.M() , _w ); 
  fill("emuPt"+lvl, emu_.Pt() , _w ); 
  fill("emuY"+lvl, emuY , _w ); 
  fill("emuPhi"+lvl, emu_.Phi() , _w ); 
  fill("emuDPhi"+lvl, fabs(muL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) ) , _w ); 
  fill("emuDEta"+lvl, muL_.Eta() - elL_.Eta() , _w ); 
  fill("emuDR"+lvl, muL_.DeltaR( elL_ ) , _w );   
  fill("emuDEtaDPhi"+lvl, muL_.Eta() - elL_.Eta() , muL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) , _w ); 
  fill("elEtaMuEta"+lvl, elL_.Eta(), muL_.Eta() , _w ); 

  fill("emuj1DPhi2D"+lvl, dphiEJ1, fabs( muL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) ) , _w ); 
  fill("emuj2DPhi2D"+lvl, dphiEJ2, fabs( muL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) ) , _w ); 

  fill("ej1Mass"+lvl, ej1_.M() , _w ); 
  fill("ej1Pt"+lvl, ej1_.Pt() , _w ); 
  fill("ej1DPhi"+lvl, dphiEJ1 , _w ); 
  fill("ej1DR"+lvl, elL_.DeltaR( jetL_ ) , _w );   
  
  fill("ej2Mass"+lvl, ej2_.M() , _w ); 
  fill("ej2Pt"+lvl, ej2_.M() , _w ); 
  fill("ej2DPhi"+lvl, dphiEJ2 , _w ); 
  fill("ej2DR"+lvl, elL_.DeltaR( jetSL_ ) , _w );  
  
  fill("ebj1Mass"+lvl, ebj1_.M() , _w ); 
  fill("ebj1Pt"+lvl, ebj1_.Pt() , _w ); 
  fill("ebj1DPhi"+lvl, dphiEBJ1 , _w ); 
  
  fill("ebj2Mass"+lvl, ebj2_.M() , _w ); 
  fill("ebj2Pt"+lvl, ebj2_.Pt() , _w ); 
  fill("ebj2DPhi"+lvl, dphiEBJ2 , _w ); 

  fill("muj1Mass"+lvl, muj1_.M() , _w ); 
  fill("muj1Pt"+lvl, muj1_.Pt() , _w ); 
  fill("muj1DPhi"+lvl, dphiMJ1 , _w ); 
  fill("muj1DR"+lvl, muL_.DeltaR( jetL_ ) , _w );  
  
  fill("muj2Mass"+lvl, muj2_.M() , _w ); 
  fill("muj2Pt"+lvl, muj2_.Pt() , _w ); 
  fill("muj2DPhi"+lvl, dphiMJ2 , _w ); 
  fill("muj2DR"+lvl, muL_.DeltaR( jetSL_ ) , _w );  

  fill("mubj1Mass"+lvl, mubj1_.M() , _w ); 
  fill("mubj1Pt"+lvl, mubj1_.Pt() , _w ); 
  fill("mubj1DPhi"+lvl, dphiMBJ1 , _w ); 

  fill("mubj2Mass"+lvl, mubj2_.M() , _w ); 
  fill("mubj2Pt"+lvl, mubj2_.Pt() , _w ); 
  fill("mubj2DPhi"+lvl, dphiMBJ2 , _w ); 

  fill("j1j2DPhi"+lvl, fabs(jetL_.Vect().XYvector().DeltaPhi( jetSL_.Vect().XYvector() ) ) , _w ); 
  fill("j1j2DEta"+lvl, jetL_.Eta()-jetSL_.Eta() , _w ); 

  fill("eMETTMass"+lvl, tmassE , _w ); 
  fill("eMETPt"+lvl, ptEMET , _w ); 
  fill("eMETDPhi"+lvl, dphiEMET , _w ); 

  fill("muMETTMass"+lvl, tmassMu , _w ); 
  fill("muMETPt"+lvl, ptMuMET , _w ); 
  fill("muMETDPhi"+lvl, dphiMuMET , _w ); 

  fill("j1METTMass"+lvl, tmassJ1 , _w ); 
  fill("j1METPt"+lvl, ptJ1MET , _w ); 
  fill("j1METDPhi"+lvl, dphiJ1MET , _w ); 

  fill("j2METTMass"+lvl, tmassJ2 , _w ); 
  fill("j2METPt"+lvl, ptJ2MET , _w ); 
  fill("j2METDPhi"+lvl, dphiJ2MET , _w ); 
  
  fill("bj1METTMass"+lvl, tmassBJ1 , _w ); 
  fill("bj1METPt"+lvl, ptBJ1MET , _w ); 
  fill("bj1METDPhi"+lvl, dphiBJ1MET , _w ); 

  fill("bj2METTMass"+lvl, tmassBJ2 , _w ); 
  fill("bj2METPt"+lvl, ptBJ2MET , _w ); 
  fill("bj2METDPhi"+lvl, dphiBJ2MET , _w ); 
  
  fill("STjet"+lvl, elL_.Pt() + muL_.Pt() + jetL_.Pt() + jetSL_.Pt(), _w );
  fill("STbjet"+lvl, elL_.Pt() + muL_.Pt() + jetBL_.Pt() + ( (jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt() ), _w );
  
  fill("nVertex"+lvl, nGoodVtx_ , _w ); 
  fill("nEl"+lvl, nGEl_ , _w ); 
  fill("nMu"+lvl, nGMu_ , _w ); 
  fill("nJet"+lvl, nGJet_ , _w ); 
  fill("nBJet"+lvl, nGBJet_ , _w ); //nGBJet_

  fill("decHLT"+lvl, passHLT(), _w );

  int vHLT = testHLT("HLT_Ele27_WP80")*1 + (testHLT("HLT_IsoMu24") || testHLT("HLT_IsoMu24_eta2p1") )*2;
  int k =_vc->getUI("ls");
  if( (_ids=="Mu" && vHLT==1 && lvl=="Presel") || (_ids=="Ele" && vHLT==2 && lvl=="Presel") )
    cout<<_vc->getUI("event")<<"   "<<k<<"    "<<_vc->getUI("run")<<"   "<<anConf.getDataset(_ids)->findProcess(_ie)<<endl;
  //   cout<<_vc->getUI("event")<<"   "<<k<<"    "<<_vc->getUI("run")<<endl;

  fill("testHLT"+lvl, vHLT, _w );

  fill("run"+lvl, _vc->getUI("run"), _w );

}




void  
LQTopAnalysis::loadPUWeights() {
  
  TFile * file = new TFile("/home/mmarionn/Documents/CMS/dev/LUNE/data/rootFiles/puWeightsLQ.root","READ"); //puWeightsS10
  puweights_ = (TH1F*)file->Get("pileup");

}

float 
LQTopAnalysis::puWeight(float trueNint) {
  return puweights_->GetBinContent( puweights_->GetXaxis()->FindBin( trueNint)  );
}


bool
LQTopAnalysis::testHLT(string hlt) {
  
  for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
    if( triggerDecision(ih, hlt) )  //HLT_Ele27_WP80  //HLT_IsoMu24
      return true;
  }
  return false;
}

bool 
LQTopAnalysis::passHLT() {

  for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
    if(  triggerDecision(ih, "HLT_IsoMu24") )  //HLT_Ele27_WP80  //HLT_IsoMu24
    //triggerDecision(ih, "HLT_Ele27_WP80") ||
      return true;
  }
  return false;
}

bool
LQTopAnalysis::triggerDecision(size_t ihlt, string ref) {

  // if(_vc->getUI("run")==193621 && _vc->getUI("event")==27324441)
  //   cout<<_vc->getS("HLTInsideDatasetTriggerNames",ihlt)<<"   "<<_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt)<<" ==>   "<<_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt)<<endl;

  if(!_au->simpleCut<size_t>(_vc->getS("HLTInsideDatasetTriggerNames",ihlt).find(ref),-1,"!=") ) return false;
  if(!_au->simpleCut<int>(_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt),1,"=")) return false;
  if(!_au->simpleCut(_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt) ) ) return false;
  return true;
}


void
LQTopAnalysis::storeBTagInfos(float pt, float eta, float disc, int flav) {
  
  float eff = bTagDB_->getBTagEff(pt,eta,disc,flav);
  float sf = bTagDB_->getBTagSF(pt,eta,flav);

  JetInfo jetinfo(eff,sf);
  vector<JetInfo> jetInfoForAllOPs;
  jetInfoForAllOPs.push_back(jetinfo);
  jetInfos_.push_back(jetInfoForAllOPs);
}

float
LQTopAnalysis::getBTagWeight() {
  return bTagSFTool_->weight(jetInfos_);
}


int
LQTopAnalysis::findZDecayChannel() {

  size_t nGen=_vc->getSize("GenParticlePt"); 
 
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),30,">") ) continue;
    if(!_au->simpleCut<double>(fabs(_vc->getD("GenParticleEta",ig)),2.1,"<") ) continue;
   
    if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") ) ) continue; 

    if(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),23,"=")) 
      return fabs(_vc->getI("GenParticlePdgId",ig));
  }

  return -1;
}


vector<int>
LQTopAnalysis::findTTDecayChannel() {

  size_t nGen=_vc->getSize("GenParticlePt"); 
 
  vector<int> vals;

  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),10,">") ) continue;
   
    if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") ) ) continue; 

    if(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),24,"=")) 
      vals.push_back(fabs(_vc->getI("GenParticlePdgId",ig)));
  }

  return vals;
}



float
LQTopAnalysis::getPtWeight(float pt) {
  
  if(pt>25*20) return 1.;

  float w[20]={
       1.,
	 1.531,
	 0.916266,
	 0.55286,
	 0.48693,
    0.194363,
    0.341003,
    0.140223,
    0.297049,
    0.492066,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
	 1.};

  return w[ (int)(pt/25) ];
  
}




float
LQTopAnalysis::getPtWeightE(float pt) {
  
  float w[52]={
    0,
    0,
    0,
    0,
    0.91784,
    0.967081,
    1.03851,
    1.05251,
    0.93962,
    1.03914,
    1.10977,
    1.15356,
    1.12054,
    0.993283,
    1.07542,
    1.02049,
    1.2113,
    0.97865,
    1.19056,
    0.834884,
    1.43732,
    0.686431,
    0.908996,
    1.33255,
    0.568368,
    0.982145,
    2.36354,
    0.910808,
    0.213205,
    0.268851,
    0,
    0.848574,
    0.00239896,
    1.30862,
    0.471323,
    0,
    0.00361314,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0};

  return w[ (int)(pt/10) ];
  
}

void
LQTopAnalysis::endAnalysis() {

  cout<<"effe = "<<effe/N<<" +- "<<sqrt(deffe_2)/N<<endl;
  cout<<"effeNH = "<<effeNH/N<<" +- "<<sqrt(deffe_2)/N<<endl;
  cout<<"effm = "<<effm/N<<" +- "<<sqrt(deffm_2)/N<<endl;
  cout<<"effmHLT = "<<effmHLT/N<<" +- "<<sqrt(deffmHLT_2)/N<<endl;



}
