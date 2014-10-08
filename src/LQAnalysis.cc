#include "src/LQAnalysis.hh"

using namespace std;

ClassImp(LQAnalysis)

LQAnalysis::LQAnalysis():
UseTree() {

  loadPUWeights();

  levels_.push_back( "Presel" );
  levels_.push_back( "Charge" );
  levels_.push_back( "MultJet" );
  levels_.push_back( "BTag" );
  levels_.push_back( "Adv" );
  levels_.push_back( "Final" );

  _dbm->loadDb("elID","elIDMedium.db");
  _dbm->loadDb("elTrig","TrigEff_MediumID_Ele27WP80_2D.root","h_eff");
  _dbm->loadDb("muID","muIDTight.db");
  _dbm->loadDb("muIso","muIsoTight.db");

  _dbm->loadDb("tauFR","tauFRData.db");
  _dbm->loadDb("tauFRMC","tauFRMC.db");

  //_dbm->loadDb("tauIDIso","Zprime_highPt.root","DQMData/RecoTauV/hpsPFTauProducerZTT_hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits/LooseCombinedIsolationDBSumPtCorr3HitsEffpt");
  _dbm->loadDb("tauIDIso","tauID.root","tauID");

  _dbm->loadDb("elTauD","Zprime_highPt.root","DQMData/RecoTauV/hpsPFTauProducerZTT_hpsPFTauDiscriminationByMVA3LooseElectronRejection/MVA3LooseElectronRejectionEffpt");
  _dbm->loadDb("muTauD","Zprime_highPt.root","DQMData/RecoTauV/hpsPFTauProducerZTT_hpsPFTauDiscriminationByLooseMuonRejection2/LooseMuonRejection2Effpt");

  bTagSFTool_ = new BTagWeight(1);
  bTagDB_ = new BTagDB("CSVL","mean");
  
  //Create Neural Net
  NN = CreateNN();
 
  efft =0.;
  N=0;
 
  nFTStd_=0;
  nFTMM_=0;
  nFTKK_=0;

}

LQAnalysis::~LQAnalysis() {

}

void
LQAnalysis::modifySkimming() {

  addSkimBranch("pt_lep", &ptL_);
  addSkimBranch("pt_tau",&ptT_);
  addSkimBranch("mtb",&mTb_);
  addSkimBranch("meb",&mTj_);
  addSkimBranch("met",&TmassL_);
  addSkimBranch("tmassT",&TmassJ_);
  addSkimBranch("dphiLM",&dPhiLM_);
  addSkimBranch("STbjet",&STbjet_);
  addSkimBranch("weight",&w_);
  // addSkimBranch();
  // addSkimBranch();
  // addSkimBranch();
  // addSkimBranch();
  // addSkimBranch();
  // addSkimBranch();
  // addSkimBranch();


}


void
LQAnalysis::bookHistograms() {

  // _hm->addVariable("eMETTMassOS",1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
  // _hm->addVariable("eMETTMassSS",1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");

  for(size_t ilvl=0;ilvl<levels_.size();ilvl++) {


    //leptons
    _hm->addVariable("elPt"+levels_[ilvl],500,0,500,"p_{T}(e) [GeV] ");
    _hm->addVariable("elEta"+levels_[ilvl],120,-3,3,"#eta(e)  ");
    _hm->addVariable("elPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e) [rad] ");
    _hm->addVariable("elCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e} ");

    _hm->addVariable("elIsEl"+levels_[ilvl],2,-0.5,1.5,"e from e ");
    _hm->addVariable("elIsTau"+levels_[ilvl],2,-0.5,1.5,"e from #tau ");
    _hm->addVariable("elSource"+levels_[ilvl],11,0,11,"e source ");
  
    _hm->addVariable("tauPt"+levels_[ilvl],500,0,500,"p_{T}(#tau) [GeV] ");
    _hm->addVariable("tauEta"+levels_[ilvl],120,-3,3,"#eta(#tau)  ");
    _hm->addVariable("tauPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau) [rad] ");
    _hm->addVariable("tauCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{#tau} ");

    _hm->addVariable("tauIsEl"+levels_[ilvl],2,-0.5,1.5,"#tau from e ");
    _hm->addVariable("tauIsTau"+levels_[ilvl],2,-0.5,1.5,"#tau from #tau ");
    _hm->addVariable("tauSource"+levels_[ilvl],11,0,11,"#tau source ");
  
    _hm->addVariable("PtVsPt"+levels_[ilvl],500,0,500,500,0,500,
		     "p_{T}(e) [GeV] ","p_{T}(#tau) [GeV] ");

    _hm->addVariable("muPt"+levels_[ilvl],500,0,500,"p_{T}(#mu) [GeV] ");
    _hm->addVariable("muEta"+levels_[ilvl],120,-3,3,"#eta(#mu)  ");
    _hm->addVariable("muPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#mu) [rad] ");

    //jets
    _hm->addVariable("jet1Pt"+levels_[ilvl],500,0,500,"p_{T}(j1) [GeV] ");
    _hm->addVariable("jet1Eta"+levels_[ilvl],120,-3,3,"#eta(j1)  ");
    _hm->addVariable("jet1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(j1) [rad] ");
  
    _hm->addVariable("jet2Pt"+levels_[ilvl],500,0,500,"p_{T}(j2) [GeV] ");
    _hm->addVariable("jet2Eta"+levels_[ilvl],120,-3,3,"#eta(j2)  ");
    _hm->addVariable("jet2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(j2) [rad] ");

    //jets
    _hm->addVariable("bjet1Pt"+levels_[ilvl],500,0,500,"p_{T}(b-j1) [GeV] ");
    _hm->addVariable("bjet1Eta"+levels_[ilvl],120,-3,3,"#eta(b-j1)  ");
    _hm->addVariable("bjet1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(b-j1) [rad] ");
  
    _hm->addVariable("bjet2Pt"+levels_[ilvl],500,0,500,"p_{T}(b-j2) [GeV] ");
    _hm->addVariable("bjet2Eta"+levels_[ilvl],120,-3,3,"#eta(b-j2)  ");
    _hm->addVariable("bjet2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(b-j2) [rad] ");
  
    //met
    _hm->addVariable("metPt"+levels_[ilvl],500,0,500,"#slash{E}_{T} [GeV] ");
    _hm->addVariable("metPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#slash{E}_{T}) [rad] ");

    //e+tau variables
    _hm->addVariable("etauCharge"+levels_[ilvl],3,-1.5,1.5,"Q_{e}#times Q_{#tau} ");
    _hm->addVariable("etauGenCharge"+levels_[ilvl],3,-1.5,1.5," gen Q_{e}#times Q_{#tau} ");
   

    _hm->addVariable("etauMass"+levels_[ilvl],1000,0,1000,"M(e,#tau) [GeV] ");
    _hm->addVariable("etauPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#tau) [GeV] ");
    _hm->addVariable("etauY"+levels_[ilvl],120,-3,3,"Y(e,#tau) ");
    _hm->addVariable("etauPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,#tau) [rad] ");
    _hm->addVariable("etauDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,#tau) [rad] ");

    //e+jet1
    _hm->addVariable("ej1Mass"+levels_[ilvl],1000,0,1000,"M(e,j1) [GeV] ");
    _hm->addVariable("ej1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,j1) [GeV] ");
    // _hm->addVariable("ej1Y"+levels_[ilvl],120,-3,3,"Y(e,j1) ");
    // _hm->addVariable("ej1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,j1) [rad] ");

    //e+jet2
    _hm->addVariable("ej2Mass"+levels_[ilvl],1000,0,1000,"M(e,j2) [GeV] ");
    _hm->addVariable("ej2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,j2) [GeV] ");
    // _hm->addVariable("ej2Y"+levels_[ilvl],120,-3,3,"Y(e,j2) ");
    // _hm->addVariable("ej2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,j2) [rad] ");

    //e+bjet1
    _hm->addVariable("ebj1Mass"+levels_[ilvl],1000,0,1000,"M(e,b-j1) [GeV] ");
    _hm->addVariable("ebj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,b-j1) [GeV] ");
    // _hm->addVariable("ebj1Y"+levels_[ilvl],120,-3,3,"Y(e,b-j1) ");
    // _hm->addVariable("ebj1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,b-j1) [rad] ");

    //e+bjet2
    _hm->addVariable("ebj2Mass"+levels_[ilvl],1000,0,1000,"M(e,b-j2) [GeV] ");
    _hm->addVariable("ebj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(e,b-j2) [GeV] ");
    // _hm->addVariable("ebj2Y"+levels_[ilvl],120,-3,3,"Y(e,b-j2) ");
    // _hm->addVariable("ebj2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(e,b-j2) [rad] ");


    //tau+jet1
    _hm->addVariable("tauj1Mass"+levels_[ilvl],1000,0,1000,"M(#tau,j1) [GeV] ");
    _hm->addVariable("tauj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,j1) [GeV] ");
    // _hm->addVariable("tauj1Y"+levels_[ilvl],120,-3,3,"Y(#tau,j1) ");
    // _hm->addVariable("tauj1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau,j1) [rad] ");
  
    //tau+jet2
    _hm->addVariable("tauj2Mass"+levels_[ilvl],1000,0,1000,"M(#tau,j2) [GeV] ");
    _hm->addVariable("tauj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,j2) [GeV] ");
    // _hm->addVariable("tauj2Y"+levels_[ilvl],120,-3,3,"Y(#tau,j2) ");
    // _hm->addVariable("tauj2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau,j2) [rad] ");
  

    //tau+bjet1
    _hm->addVariable("taubj1Mass"+levels_[ilvl],1000,0,1000,"M(#tau,b-j1) [GeV] ");
    _hm->addVariable("taubj1Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,b-j1) [GeV] ");
    // _hm->addVariable("taubj1Y"+levels_[ilvl],120,-3,3,"Y(#tau,b-j1) ");
    // _hm->addVariable("taubj1Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau,b-j1) [rad] ");
  
    //tau+bjet2
    _hm->addVariable("taubj2Mass"+levels_[ilvl],1000,0,1000,"M(#tau,b-j2) [GeV] ");
    _hm->addVariable("taubj2Pt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,b-j2) [GeV] ");
    // _hm->addVariable("taubj2Y"+levels_[ilvl],120,-3,3,"Y(#tau,b-j2) ");
    // _hm->addVariable("taubj2Phi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#tau,b-j2) [rad] ");
  
    _hm->addVariable("triTMassE"+levels_[ilvl],1000,0,1000,"M(e,bj1,#slash{E}_{T}) [GeV] ");

    //e+met
    _hm->addVariable("eMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(e,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("eMETDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(e,#slash{E}_{T}) [rad] ");

    //tau+met
    _hm->addVariable("tauMETTMass"+levels_[ilvl],1000,0,1000,"M_{T}(#tau,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("tauMETPt"+levels_[ilvl],1000,0,1000,"p_{T}(#tau,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("tauMETDPhi"+levels_[ilvl],256,0,TMath::Pi(),"#Delta#phi(#tau,#slash{E}_{T}) [rad] ");

    //jet+met
    _hm->addVariable("j1METTMass"+levels_[ilvl],1000,0,1000,"M_{T}(j1,#slash{E}_{T}) [GeV] ");
    _hm->addVariable("j1METPt"+levels_[ilvl],1000,0,1000,"p_{T}(j1,#slash{E}_{T}) [GeV] ");
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

    _hm->addVariable("STVsMass"+levels_[ilvl],100,0,2000,100,0,1000,"S_{T} [GeV] ","M(#tau,b-j2) [GeV] ");



    _hm->addVariable("dalitzMass"+levels_[ilvl],100,0,1000,100,0,1000,"M(#tau,b) [GeV] ","M(e,j) [GeV] ");
    _hm->addVariable("dalitzBMass"+levels_[ilvl],100,0,1000,100,0,1000,"M(#tau,b) [GeV] ","M(e,b) [GeV] ");

    //multiplicity
    _hm->addVariable("nVertex"+levels_[ilvl],50,0,50,"vertex multiplicity "); 
    _hm->addVariable("nEl"+levels_[ilvl],10,0,10,"electron multiplicity "); 
    _hm->addVariable("nTau"+levels_[ilvl],10,0,10,"tau multiplicity "); 
    _hm->addVariable("nJet"+levels_[ilvl],10,0,10,"jet multiplicity "); 
    _hm->addVariable("nBJet"+levels_[ilvl],10,0,10,"b-jet multiplicity "); 

    //miscellaneous
    _hm->addVariable("decHLT"+levels_[ilvl],2,-0.5,1.5,"decision HLT "); 

    _hm->addVariable("bjetSource"+levels_[ilvl],25,-0.5,24.5,"b jet source ");
    _hm->addVariable("ljetSource"+levels_[ilvl],25,-0.5,24.5,"l jet source ");
    _hm->addVariable("ljetTauSource"+levels_[ilvl],25,-0.5,24.5,25,-0.5,24.5,"l jet source ","tau source");
 
  }

  _hm->addVariable("NeuralNet",100,0,1.00,"NN output");
  _hm->addVariable("elPdgId",101,-50.5,50.5,"el pdgId");
  _hm->addVariable("tauPdgId",101,-50.5,50.5,"tau pdgId");
  _hm->addVariable("bPdgId",21,0,21,"bPdgId"); 
  _hm->addVariable("finalMass",1000,0,1000,"M_{1} [GeV] "); 

}

void
LQAnalysis::modifyWeight() {

  if(!_isData) {
    float w=puWeight(_vc->getF("trueNPU") );
    _w *= w;
  }
  
}


void
LQAnalysis::analyze() {
  
  //if( !_au->simpleCut(_vc->getUI("event")==46898804) ) return;
  //if(!_au->simpleCut(_ie==4598) ) return; 

  //splitting Z, ttbar =================================
  // if(_ids.find("Z(")!=(size_t)-1) {
  //   int ltype = findZDecayChannel();
  //   if( _ids == "Z(ll)+jets" ) {
  //     if(!_au->simpleCut<int>(ltype,15,"!=")) return;
  //   }
  //   else {
  //     if(!_au->simpleCut<int>(ltype,15,"=")) return;
  //   }
  // }

  // if(_ids.find("Z(")!=(size_t)-1) {
  //   int ltype = findZDecayChannel();
  //   if( _ids == "Z(ll)+jets" ) {
  //     if(!_au->simpleCut<int>(ltype,15,"!=")) return;
  //   }
  //   else {
  //     if(!_au->simpleCut<int>(ltype,15,"=")) return;
  //   }
  // }

 
  //=============================================


  _au->makeCut(true,_ids,"beginning",_w);

  nGoodVtx_=0;
  for(size_t ivtx = 0; ivtx < _vc->getSize("VertexRho") ; ++ivtx) {
    //double chi2 = _vc->getD("VertexChi2",ivtx);
    double ndf  = _vc->getD("VertexNDF",ivtx);
    double z    = _vc->getD("VertexZ",ivtx);
    double rho  = _vc->getD("VertexRho",ivtx);
    
  
    // cout<<ivtx<<"   "<<_vc->getB("VertexIsFake",ivtx)<<"  "<<ndf<<"  "<<fabs(z)<<"   "<<fabs(rho)<<"  "<<(_vc->getB("VertexIsFake",ivtx) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2)<<endl;
    if (!(_vc->getB("VertexIsFake",ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2 ) {
      ++nGoodVtx_; 
      vtx_[ ivtx ] = -10000;
      //cout<<" youpui "<<endl;
      // if(_vc->getI("ElectronVtxIndex",elIdx_) == ivtx) Z= z;
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
  
  elIdx_=-1;
  tauIdx_=-1;
  muIdx_=-1;

  elL_.SetPtEtaPhiM(0.00001,0,0,0.0005);
  muL_.SetPtEtaPhiM(0.00001,0,0,0.106);
  muSL_.SetPtEtaPhiM(0.00001,0,0,0.106);
  tauL_.SetPtEtaPhiM(0.00001,0,0,1.78);
  jetL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetSL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBL_.SetPtEtaPhiE(0.00001,0,0,0.);
  jetBSL_.SetPtEtaPhiE(0.00001,0,0,0.);

  jIdx_.resize(0);
  jetPt_.resize(0);
  
  float pttmp=0,pttmp2=0;


  //========== Gen matching
  //cout<<"================ new event "<<endl;
  // size_t mIdx=-1;
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     // if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),13,"=")// ||
  //     // 	   //_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=")
  //     // 	   ) ) continue; 
  // if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 23, "=") || 
  //      _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 15, "=") 
  //      )) continue;
  //   mIdx = _vc->getI("GenParticleMotherIndex",ig);
  //========== Gen matching
  for(size_t im=0;im<nMu;im++) {

    //gen matching
    // if(_au->simpleCut(!_isData) ) {
    //   if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("MuonEta",im) ,_vc->getD("GenParticleEta",ig), _vc->getD("MuonPhi",im), _vc->getD("GenParticlePhi",ig)), 0.3 ,"<")) continue; 
    // }
     
    if( !_au->simpleCut(muonID(im)) ) continue;
    //if(!mif(_vc->getD("MuonPt",im)>50) ) continue;
    if(!_au->simpleCut( vtx_.find( _vc->getI("MuonVtxIndex",im) )!=vtx_.end() ) ) continue;

    if( _au->simpleCut<double>(_vc->getD("MuonPt",im),pttmp,">") ) {
      muL_.SetPtEtaPhiM( _vc->getD("MuonPt",im), 
			 _vc->getD("MuonEta",im),
			 _vc->getD("MuonPhi",im), 0.106 );
      muIdx_ = im;
      pttmp = muL_.Pt();
    }
    nGMu_++;
      
  }//for imu

  //}

  pttmp=0;
  for(size_t im=0;im<nMu;im++) {
    if( _au->simpleCut(muonID(im) && muIdx_ != im ) ) {
      if( _au->simpleCut<double>(_vc->getD("MuonPt",im),pttmp,">") ) {
  	muSL_.SetPtEtaPhiM( _vc->getD("MuonPt",im), 
			    _vc->getD("MuonEta",im),
			    _vc->getD("MuonPhi",im), 0.106 );
  	muSLIdx_ = im;
  	pttmp = muSL_.Pt();
      }
    }
  }

  
  //========== Gen matching
  //cout<<"================ new event "<<endl;
  // size_t mIdx=-1;
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=")// ||
  // 	   //_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=")
  // 	   ) ) continue; 
  //   if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 24, "=") || 
  // 	  _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 15, "=") 
  // 	 )) continue;
  //   mIdx = _vc->getI("GenParticleMotherIndex",ig);
  //========== Gen matching
  pttmp=0;
  float Z=-100;
  for(size_t ie=0;ie<nEl;ie++) {
      
    //gen matching
    // if(_au->simpleCut(!_isData) ) {
    // 	if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("ElectronEta",ie) ,_vc->getD("GenParticleEta",ig), _vc->getD("ElectronPhi",ie), _vc->getD("GenParticlePhi",ig)), 0.3 ,"<")) continue; 
    //  }
    // cout<<"==> "<<ie<<"   "<<_vc->getD("ElectronPt",ie)<<"  "<<elecID(ie)<<"   "<<_vc->getI("ElectronVtxIndex",ie)<<endl;
    if(! _au->simpleCut(elecID(ie)) ) continue;
    
    if(!_au->simpleCut( vtx_.find( _vc->getI("ElectronVtxIndex",ie) )!=vtx_.end() ) ) continue;
    
    if(_au->simpleCut<double>(_vc->getD("ElectronPt",ie),pttmp,">") ) {
      elL_.SetPtEtaPhiM( _vc->getD("ElectronPt",ie), 
			 _vc->getD("ElectronEta",ie),
			 _vc->getD("ElectronPhi",ie), 0.0005 );
      elIdx_ = ie;
      pttmp = elL_.Pt();
    }    

    nGEl_++;
	
    
  }//for iel

  //}//gen matching
  //cout<<_ie<<"    "<<nGEl_<<endl;
  vtx_.clear();
 


  for(size_t ie=0;ie<nEl;ie++) {
    // cout<<"1    "<<_vc->getI("ElectronCharge",elIdx_)*_vc->getI("ElectronCharge",ie)
    //   	<<"    "<<elLooseID(ie)<<"    "<<_vc->getD("ElectronPt",ie)<<"    "<<( elIdx_!=ie)<<endl;

    //if( _au->simpleCut(elLooseID(ie) ) ) {
    if( _au->simpleCut(elecID(ie) ) ) {
      // if( elIdx_!=ie)
      {
	secLepOS_=true;
	break;
      }
    }
  }
    
  for(size_t im=0;im<nMu;im++) {
    // cout<<"0    "<<_vc->getI("ElectronCharge",elIdx_)*_vc->getI("MuonCharge",im)
    //    	<<"    "<<muonID(im)<<"    "<<_vc->getD("ElectronPt",im)<<"    "<<( elIdx_!=im)<<endl;
    //if( _au->simpleCut(muonID(im) ) ) {
    if( _au->simpleCut(muonLooseID(im) ) ) {
      if( muIdx_!=im)
	{
	  secLepOS_=true; break;
	}
    }
  }
  
  //cout<<_ie<<"   "<<secLepOS_<<endl;
  

  //if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
  
  if(!_au->makeCut<int>( nGEl_, 0 , ">" ,_ids,"el multiplicity",_w) ) return;

  //if(!_au->makeCut<int>( nGMu_, 1 , ">" ,_ids,"mu multiplicity",_w) ) return;
  
  //bool passLM=(nGMu_>0 && _ids=="ttbarMu") || (nGEl_>0 && _ids=="ttbarEl") ;

  //if(!_au->makeCut( passLM, _ids ,"lep multiplicity",_w) ) return;

  // if(_isData) {
  // if(_isData) {
  if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
  // }
  // else {
  //   _w *=_dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
  //   if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  // }


  //if(!_au->simpleCut( mIdx!=(size_t)-1 ) ) return;

  pttmp=0;
  //float genpt,geneta;
  bool hasTauIso=false;
  // //========== Gen matching
  // size_t nGen=_vc->getSize("GenParticlePt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if( !_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=")) continue; 
    //if(!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",_vc->getI("GenParticleMotherIndex",ig))), 24, "=") )) continue;
    //========== Gen matching
    vector<TLorentzVector> aitaus;
   
    for(size_t it=0;it<nTau;it++) {

      //gen matching
      // if(_au->simpleCut(!_isData) )
      // 	if(!_au->simpleCut<float>(KineUtils::dR( _vc->getD("HPSTauEta",it) ,_vc->getD("GenParticleEta",ig), _vc->getD("HPSTauPhi",it), _vc->getD("GenParticlePhi",ig)), 0.2 ,"<")) continue; 
  
      
      tmpJet_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
			    _vc->getD("HPSTauEta",it),
			    _vc->getD("HPSTauPhi",it), 1.7 );
    
      float dRel = muL_.DeltaR( tmpJet_ );
      //float dRel = elL_.DeltaR( tmpJet_ );

      //cout<<" glou 1"<<endl;
      // cout<<_vc->getD("HPSTauPt",it)<<"   "<<_vc->getI("HPSTauVtxIndex",it)<<"   "<<_vc->getD("HPSTauVertexZ",it)<<"   "<<_vc->getI("ElectronVtxIndex",elIdx_)<<"   "<<Z
      //  	<<"   "<<dRel<<"   "<<_vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",it)<<"   "<<_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", it)
      //  	<<"   "<<_vc->getD("HPSTauEta", it)<<"   "<<_vc->getD("HPSTauagainstElectronLooseMVA3", it)<<"   "<<_vc->getD("HPSTauagainstMuonLoose2", it) <<endl;

      if( !_au->simpleCut(tauID(it)) ) continue;
    
      //cout<<_ie<<"   "<<nTau<<"   "<<_vc->getD("HPSTauPt",it)<<endl;
      
      // float dRel = KineUtils::dR( _vc->getD("ElectronEta",elIdx_),  _vc->getD("HPSTauEta",it),
      // 				_vc->getD("ElectronPhi",elIdx_), _vc->getD("HPSTauPhi",it) );
    
      if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
  
      if(!_au->simpleCut(_vc->getI("HPSTauVtxIndex",it)==_vc->getI("ElectronVtxIndex",elIdx_) ) ) continue;
      //if(!_au->simpleCut(_vc->getI("HPSTauVtxIndex",it)==_vc->getI("MuonVtxIndex",muIdx_) ) ) continue;
    

      //if(!_au->simpleCut<double>( fabs( _vc->getD("HPSTauVertex",it)-Z ),0.2,"<")) continue;
      
      //vtx_[ _vc->getI("ElectronVtxIndex",elIdx_) ]
      //  cout<<_vc->getI("ElectronCharge",elIdx_)<<"  "<<_vc->getI("HPSTauCharge",it)<<"   "<<endl;
      //if(!_au->simpleCut<int>(_vc->getI("MuonCharge",muIdx_)*_vc->getI("HPSTauCharge",it),-1,"=") ) continue;
      //if(!_au->simpleCut<int>(_vc->getI("MuonCharge",muIdx_)*_vc->getI("MuonCharge",muSLIdx_),-1,"=") ) continue;
      if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",it),-1,"=") ) continue;


      if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", it),1.,"=")) {
	hasTauIso=true;
	continue;
      }
      
      //hasTauIso=true;
      if( _au->simpleCut<double>(_vc->getD("HPSTauPt",it),pttmp,">") ) {
	tauL_ = tmpJet_;
	tauIdx_ = it;
	    
	pttmp = tauL_.Pt();

	//genpt = _vc->getD("GenParticlePt",ig);
	//geneta = _vc->getD("GenParticleEta",ig);
      }
      nGTau_++;
      aitaus.push_back( tmpJet_ );
      // cout<<" ===>  "<<nGTau_<<endl;
      // cout<<_vc->getUI("event")<<"\t"<<_vc->getI("ElectronVtxIndex",elIdx_)<<"\t"<<fabs(_vc->getD("HPSTauVertexZ",it)-Z)<<endl;
	
    }//for itau
    
    //}//gen matching

    //cout<<" NGTau : "<<nGTau_<<endl;

    pttmp=0;
  for(size_t it=0;it<nTau;it++) {
    if( _au->simpleCut(tauID(it) && tauIdx_ != it ) ) {
      float dRel = muL_.DeltaR( tmpJet_ );
      if(_au->simpleCut<double>(dRel,0.5,">") ) {
	if( _au->simpleCut<double>(_vc->getD("HPSTauPt",it),pttmp,">") ) {
	  tauSL_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
			       _vc->getD("HPSTauEta",it),
			       _vc->getD("HPSTauPhi",it), 1.7 );
	  tauSLIdx_ = it;
	  pttmp = tauSL_.Pt();
	}
      }
    }
  }


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

    //float dRel = tmpJet_.DeltaR( elL_ );
    float dRmu = tmpJet_.DeltaR( muL_ );
  
    // if(tmpJet_.Pt()>30 && fabs(tmpJet_.Eta())<3)
    //   cout<<tmpJet_.Pt()<<"   "<<tmpJet_.Eta()<<"   "<<dRel<<"   "<<dRmu<<endl;

    if(!_au->simpleCut<double>( _vc->getD("PFJetPt",ij), 30 , ">") ) continue;
    if(!_au->simpleCut<double>( fabs(_vc->getD("PFJetEta",ij)), 2.4, "<") ) continue;
    //if(_au->simpleCut(_ids=="ttbarEl") )
    //if(!_au->simpleCut<float>(dRel, 0.5, ">") ) continue;
    // if(_au->simpleCut(_ids=="ttbarMu") )
    if(!_au->simpleCut<float>(dRmu, 0.5, ">") ) continue;

    // //standard iso code
    //    float dRtau = tmpJet_.DeltaR( tauL_ );
    float dRtau = tmpJet_.DeltaR( muSL_ );
    if(!_au->simpleCut<float>(dRtau, 0.5, ">") ) continue;
      
    // bool matched=false;
    // for(size_t it=0;it<aitaus.size();it++) {
    //   float dRtau = tmpJet_.DeltaR( aitaus[it] );
    //   if(!_au->simpleCut<float>(dRtau, 0.5, ">") ) {matched=true; break;}
    // }
    // if(!_au->simpleCut( !matched ) ) continue;


    if(!_au->simpleCut<int>(_vc->getI("PFJetPassLooseID",ij), 1, "=") ) continue;

    nGJet_++;
    jetPt_.push_back( tmpJet_.Pt() );
    jIdx_.push_back(ij);
    //cout<<ie_<<"   "<<tmpJet_.Pt()<<"   "<<tmpJet_.Eta()<<"   "<<dRel<<endl;
    
    storeBTagInfos( tmpJet_.Pt(), tmpJet_.Eta(), 
		    _vc->getD("PFJetCombinedSecondaryVertexBTag",ij),
		    _vc->getI("PFJetPartonFlavour",ij) );
    

    if( tmpJet_.Pt() > pttmp ) {
      jetL_ = tmpJet_;
      pttmp = jetL_.Pt();
      jetLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmp2 && tmpJet_.Pt() < pttmp && jetLIdx_!=ij  ) {
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
  
  // if(_ids.find("W+")!=(size_t)-1) {
  //   if(_vc->getI("ElectronCharge",elIdx_)==_vc->getI("HPSTauCharge",tauIdx_) )
  //     _w*=1.099;
  //   else
  //     _w*=0.939;
  // }

  // if(!_isData)
  //   _w *=_dbm->getDBValue ("elID", fabs(elL_.Eta()), elL_.Pt() );

   // if(!_isData)
   //   _w *=_dbm->getDBValue ("muID", fabs(elL_.Eta()), elL_.Pt() )*_dbm->getDBValue ("muIso", fabs(elL_.Eta()), elL_.Pt() );
  //FIXME mumu selection for SF jet mult

  // }
  // else {
  //   _w *=_dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
  //   if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  // }
  
  //FIXME 0

  if(!_au->makeCut<int>( nGTau_, 0 , ">" ,_ids,"tau multiplicity",_w) ) return;
  
  if(!_au->makeCut( !hasTauIso,_ids,"tau isolation",_w) ) return;  
   
 
 

  //ttbar cuts
  met_.SetMagPhi( _vc->getD("PFMETPatType1"), _vc->getD("PFMETPhiPatType1") );
  if(!_au->makeCut<float>( (muL_+muSL_).M() , 20 , ">" ,_ids,"DY",_w) ) return;
  if(!_au->makeCut<float>( (muL_+muSL_).M(), 70 , "[!]" ,_ids,"mass",_w,120) ) return;
  //if(!_au->makeCut<float>( met_.Mod(), 50 , ">" ,_ids,"MET",_w) ) return;

  
  fillPlots( "Presel");
  
  if(!_au->makeCut<int>( nGJet_, 2 , ">=" ,_ids,"jet multiplicity",_w) ) return;
  
  fillPlots( "MultJet");
  
 
  // btagging
  if(!_isData)
    _w *=getBTagWeight(); 
  if(!_au->makeCut<int>( nGBJet_, 1 , ">=" ,_ids,"b-tagging",_w) ) return;
  


 
 
  
  met_.SetMagPhi( _vc->getD("PFMETPatType1"), _vc->getD("PFMETPhiPatType1") );

  // bool isSameVtx=false;
  // for(size_t ivtx = 0; ivtx < _vc->getSize("VertexRho") ; ++ivtx) {
  //   //double chi2 = _vc->getD("VertexChi2",ivtx);
  //   double ndf  = _vc->getD("VertexNDF",ivtx);
  //   double z    = _vc->getD("VertexZ",ivtx);
  //   double rho  = _vc->getD("VertexRho",ivtx);

  //   bool goodVtx = !(_vc->getB("VertexIsFake",ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2;
	
  //   if(_au->simpleCut(goodVtx) ) {
  //     bool isGVTau = (int)ivtx==_vc->getI("HPSTauVtxIndex",tauIdx_);
  //     bool isGVEl = _vc->getI("ElectronVtxIndex",elIdx_)==(int)ivtx;
  //     //bool isGVMu = _vc->getI("MuonVtxIndex",muIdx_)==ivtx;
  //     //if(_ids=="ttbarEl")
  //     isSameVtx=isGVTau && isGVEl;
  //     // else
  //     // 	isSameVtx=isGVTau && isGVMu;

  //     if(isSameVtx) break;
  //   }
  // }
  
  //if(!_au->makeCut(isSameVtx, _ids,"Vtx matching", _w ) ) return;

  //if(_au->simpleCut(_ids=="ttbarEl"))
  //if(!_au->makeCut<int>( _vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",tauIdx_), -1 , "=" ,_ids,"opposite charge",_w) ) return;
  
  // if(_au->simpleCut(_ids=="ttbarMu"))
  //   if(!_au->makeCut<int>( _vc->getI("MuonCharge",muIdx_)*_vc->getI("HPSTauCharge",tauIdx_), -1 , "=" ,_ids,"opposite charge",_w) ) return;

  //FIXME -> !secLepOS_
  //if(!_au->makeCut( !secLepOS_ ,_ids,"veto add lepton ",_w) ) return;
 

  //efft += _dbm->getDBValue("tauIDIso",genpt,geneta );
  //  cout<<_dbm->getDBValue("tauIDIso",genpt,geneta )<<endl;
  // * _dbm->getDBValue("elTauD"  ,tauL_.Pt() )
  // * _dbm->getDBValue("muTauD"  ,tauL_.Pt() );
  // N++;

  fillPlots( "Charge");

  nFTStd_ += frStd( tauL_.Pt() );
  nFTMM_  += frMM( tauL_.Pt(), tauSL_.Pt() );
  nFTKK_  += frKK( tauL_.Pt(), tauSL_.Pt() );
  
 
  //2 >=
 
  fillPlots( "BTag");


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
  // 	 }
     
  //   }
  //   fill("bPdgId",pdgId,1.);
  // }


  //additional variables when sikimming is done
  
 
  //STbjet_ = elL_.Pt() + tauL_.Pt() + jetBL_.Pt() + ((jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt()) ;
  STbjet_ = muL_.Pt() + muSL_.Pt() + jetBL_.Pt();
  bool isB=false;
  if(_ie!=-1) {
    for(size_t ij=0;ij<4;ij++) {
      
      if(jIdx_[ij]!=jetBLIdx_) {
	STbjet_ += jetPt_[ij];
      }
      else
	isB=true;
    }
    
    if(isB) {
      STbjet_ += jetPt_[4];
    }
  }

  w_ = _w;

  //if(_skim)
  
  float M11,M12;
  //if(_ids=="ttbarEl")  {
  // M11 = (elL_ +jetBL_).M();
  // M12 = (elL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  // }
  // else {
  M11 = (muL_ +jetBL_).M();
  M12 = (muL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  // }
  // float M21 = (tauL_+jetBL_).M();
  // float M22 = (tauL_+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
  // float M11 = (muL_ +jetBL_).M();
  // float M12 = (muL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  // float M21 = (tauL_+jetBL_).M();
  // float M22 = (tauL_+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
  // float M11 = (muL_ +jetBL_).M();
  // float M12 = (muL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  float M21 = (muSL_+jetBL_).M();
  float M22 = (muSL_+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
  float m1=0;//,m2=0;
  if (fabs(M11-M22) < fabs(M12-M21)) {
    m1 = M22;
    //m1 = M11;
  }else{
    m1 = M21;
    //m1 = M12;
  }
  
  fill("finalMass",m1,_w);


  
  //if(!_au->makeCut<float>( tauL_.Pt() , 50 , ">" ,_ids,"tau pT cut",_w) ) return;
  if(!_au->makeCut<float>( muL_.Pt() , 50 , ">" ,_ids,"tau pT cut",_w) ) return;
  
  fillPlots( "Adv");

  
  // cout<<jetLIdx_<<" !! "<<jetBLIdx_<<" ___> "<<jetL_.Pt()<<"  "<<jetSL_.Pt()<<endl;
  
  //   cout<<_ie<<" ===> "<<elL_.Pt()<<"    "<<tauL_.Pt()
  // 	<<"    "<<jetBL_.Pt()<<"    "
  // 	<<((jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt())<<"   "<<(elL_ +jetBL_).M()
  // 	<<"   "<< (tauL_+jetBL_).M()<<endl;
  
     


  if(!_au->makeCut<float>( m1, 250, ">", _ids,"tauB Mass",_w) ) return;  

  fillPlots( "Final");


}


bool LQAnalysis::tauID(size_t idx) {


  if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", idx)), 2.3, "<")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", idx), 30, ">")) return false;
  //if(!_au->simpleCut<double>(_vc->getD("HPSTauDecayModeFindingDiscr",idx),1,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstElectronLooseMVA3", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstMuonLoose2", idx),1.,"=")) return false;
  // if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", idx),1.,"!=")) return false; // != means inverted isolation
  // if(!_au->simpleCut<double>(_vc->getD("HPSTauMediumCombinedIsolationDeltaBetaCorrDiscr", idx),0.,"!=")) return false;
  return true;
}

bool LQAnalysis::muonID(size_t idx) {

  if(!_au->simpleCut<double>( _vc->getD("MuonPt",idx), 30 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonEta",idx)), 2.1 , "<" ) ) return false;
  
  if(!_au->simpleCut<bool>( _vc->getI("MuonPassID",idx), true, "=" ) ) return false;
  if(!_au->simpleCut<bool>( _vc->getI("MuonIsPF",idx), true, "=" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonStationMatches",idx), 0, ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonBestTrackVtxDistXY",idx)), 0.2, "<" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonBestTrackVtxDistZ",idx)), 0.5, "<" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonPixelHits",idx), 0, ">" ) ) return false;
  if(!_au->simpleCut<int>( _vc->getI("MuonTrackLayersWithMeasurement",idx), 5, ">" ) ) return false;

  float iso = _vc->getD("MuonPFIsoR04ChargedHadron",idx) + 
    max( _vc->getD("MuonPFIsoR04NeutralHadron",idx) + _vc->getD("MuonPFIsoR04Photon",idx) - 0.5*_vc->getD("MuonPFIsoR04PU",idx) ,0.);
  iso /= _vc->getD("MuonPt",idx);

  if( !_au->simpleCut<float>(iso, 0.12, "<") ) return false;

  return true;

}

bool LQAnalysis::muonLooseID(size_t idx) {

  if(!_au->simpleCut<double>( _vc->getD("MuonPt",idx), 20 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("MuonEta",idx)), 2.4 , "<" ) ) return false;
  
  if(!_au->simpleCut<bool>( _vc->getI("MuonIsGlobal",idx), true, "=" ) ) return false;
  if(!_au->simpleCut<bool>( _vc->getI("MuonIsPF",idx), true, "=" ) ) return false;
  //if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",elIdx_)*_vc->getI("MuonCharge",idx),-1,"=") ) return false;

  float iso = _vc->getD("MuonPFIsoR04ChargedHadron",idx) + 
    max( _vc->getD("MuonPFIsoR04NeutralHadron",idx) + _vc->getD("MuonPFIsoR04Photon",idx) - 0.5*_vc->getD("MuonPFIsoR04PU",idx) ,0.);
  iso /= _vc->getD("MuonPt",idx);

  if( !_au->simpleCut<float>(iso, 0.3, "<") ) return false;

  return true;

}



bool LQAnalysis::elecID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 30 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronEta",idx)), 2.5 , "<" ) ) return false;
  if(!_au->simpleCut<double>(fabs(_vc->getD("ElectronEta",idx)), 1.4442,"]![",1.56 ) ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDMedium",idx)==1023);//1023->iso, 895=antiiso
}


bool LQAnalysis::elLooseID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 20 , ">" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronEta",idx)), 2.5 , "<" ) ) return false;
  if(!_au->simpleCut<double>(fabs(_vc->getD("ElectronEta",idx)), 1.4442,"]![",1.56 ) ) return false;
  if(!_au->simpleCut<int>(_vc->getI("ElectronCharge",idx)*_vc->getI("ElectronCharge",elIdx_),-1,"=") ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDLoose",idx)==1023);
}


void LQAnalysis::fillPlots(string lvl) {

  etau_  = elL_ + tauL_;
  ej1_  = elL_ + jetL_;
  ej2_  = elL_ + jetSL_;
  tauj1_ = tauL_ + jetL_;
  tauj2_ = tauL_ + jetSL_;

  ebj1_  = elL_ + jetBL_;
  ebj2_  = elL_ + jetBSL_;
  taubj1_ = tauL_ + jetBL_;
  taubj2_ = tauL_ + jetBSL_;
  
  float etauY = 0.5*log( (etau_.E()+etau_.Pz())/(etau_.E()-etau_.Pz()) );

  float tmassE  = sqrt(2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))));
  float tmassTau = sqrt(2*tauL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),tauL_.Phi() )))); 
  float tmassJ1 = sqrt(2*jetL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetL_.Phi() ))));
  float tmassJ2 = sqrt(2*jetSL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetSL_.Phi() ))));
  //float tmassBJ1 = sqrt(2*jetBL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetL_.Phi() ))));
  float tmassBJ2 = sqrt(2*jetBSL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),jetSL_.Phi() ))));



  float ptEMET  = (met_ + elL_.Vect().XYvector()).Mod() ;
  float ptTauMET = (met_ + tauL_.Vect().XYvector()).Mod() ;
  float ptJ1MET = (met_ + jetL_.Vect().XYvector()).Mod() ;
  float ptJ2MET = (met_ + jetSL_.Vect().XYvector()).Mod() ;
  float ptBJ1MET = (met_ + jetBL_.Vect().XYvector()).Mod() ;
  float ptBJ2MET = (met_ + jetBSL_.Vect().XYvector()).Mod() ;

  float dphiEMET   = fabs( met_.DeltaPhi( elL_.Vect().XYvector() ) );
  float dphiTauMET = fabs( met_.DeltaPhi( tauL_.Vect().XYvector() ) );
  float dphiJ1MET  = fabs( met_.DeltaPhi( jetL_.Vect().XYvector() ) );
  float dphiJ2MET  = fabs( met_.DeltaPhi( jetSL_.Vect().XYvector() ) );
  float dphiBJ1MET = fabs( met_.DeltaPhi( jetBL_.Vect().XYvector() ) );
  float dphiBJ2MET = fabs( met_.DeltaPhi( jetBSL_.Vect().XYvector() ) );

  float mTEMETb = sqrt( 2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))) 
			+ 2*elL_.Pt()*jetBL_.Pt()*(1-cos(KineUtils::dPhi(jetBL_.Phi(),elL_.Phi() )))
			+ 2*met_.Mod()*jetBL_.Pt()*(1-cos(KineUtils::dPhi(jetBL_.Phi(),met_.Phi() ))) );

  //======
  fill("elPt"+lvl, elL_.Pt() , _w );
  fill("elEta"+lvl, elL_.Eta() , _w );
  fill("elPhi"+lvl, elL_.Phi() , _w );
  //fill("elCharge"+lvl, _vc->getI("ElectronCharge",elIdx_) , _w );
  
  fill("elIsTau"+lvl, isGenParticle( &elL_, 15) , _w );
  fill("elIsEl"+lvl, isGenParticle( &elL_, 11) , _w );
  
  fill("muPt"+lvl, muL_.Pt() , _w );
  fill("muEta"+lvl, muL_.Eta() , _w );
  fill("muPhi"+lvl, muL_.Phi() , _w );

  fill("tauPt"+lvl, tauL_.Pt() , _w );
  fill("tauEta"+lvl, tauL_.Eta() , _w );
  fill("tauPhi"+lvl, tauL_.Phi() , _w ); 
  //fill("tauCharge"+lvl, _vc->getI("HPSTauCharge",tauIdx_) , _w );

  fill("tauIsTau"+lvl, isGenParticle( &tauL_, 15) , _w );
  fill("tauIsEl"+lvl, isGenParticle( &tauL_, 11) , _w );
  //fill("tauSource"+lvl, genParticle( fabs(matchGenParticle( &tauL_) ) ) , _w );

  fill("PtVsPt"+lvl, elL_.Pt(), tauL_.Pt() , _w );

 
  int cp=-1;
  // if(signbit( matchGenParticle( &elL_) ) == signbit( matchGenParticle( &tauL_) ) )
  //   cp = 1;

  fill("etauGenCharge"+lvl, cp, _w );

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
  
  //fill("etauCharge"+lvl, _vc->getI("ElectronCharge",elIdx_)*_vc->getI("HPSTauCharge",tauIdx_) , _w ); 
  fill("etauMass"+lvl, etau_.M() , _w ); 
  fill("etauPt"+lvl, etau_.Pt() , _w ); 
  fill("etauY"+lvl, etauY , _w ); 
  fill("etauPhi"+lvl, etau_.Phi() , _w ); 
  fill("etauDPhi"+lvl, fabs(tauL_.Vect().XYvector().DeltaPhi( elL_.Vect().XYvector() ) ) , _w ); 
  
  fill("ej1Mass"+lvl, ej1_.M() , _w ); 
  fill("ej1Pt"+lvl, ej1_.Pt() , _w ); 
  
  fill("ej2Mass"+lvl, ej2_.M() , _w ); 
  fill("ej2Pt"+lvl, ej2_.M() , _w ); 

  fill("ebj1Mass"+lvl, ebj1_.M() , _w ); 
  fill("ebj1Pt"+lvl, ebj1_.Pt() , _w ); 
  
  fill("ebj2Mass"+lvl, ebj2_.M() , _w ); 
  fill("ebj2Pt"+lvl, ebj2_.M() , _w ); 
  
  fill("tauj1Mass"+lvl, tauj1_.M() , _w ); 
  fill("tauj1Pt"+lvl, tauj1_.Pt() , _w ); 

  fill("tauj2Mass"+lvl, tauj2_.M() , _w ); 
  fill("tauj2Pt"+lvl, tauj2_.Pt() , _w ); 

  fill("taubj1Mass"+lvl, taubj1_.M() , _w ); 
  fill("taubj1Pt"+lvl, taubj1_.Pt() , _w ); 

  fill("taubj2Mass"+lvl, taubj2_.M() , _w ); 
  fill("taubj2Pt"+lvl, taubj2_.Pt() , _w ); 

  fill("eMETTMass"+lvl, tmassE , _w ); 
  fill("eMETPt"+lvl, ptEMET , _w ); 
  fill("eMETDPhi"+lvl, dphiEMET , _w ); 

  fill("tauMETTMass"+lvl, tmassTau , _w ); 
  fill("tauMETPt"+lvl, ptTauMET , _w ); 
  fill("tauMETDPhi"+lvl, dphiTauMET , _w ) ;

  fill("j1METTMass"+lvl, tmassJ1 , _w ); 
  fill("j1METPt"+lvl, ptJ1MET , _w ); 
  fill("j1METDPhi"+lvl, dphiJ1MET , _w ); 

  fill("j2METTMass"+lvl, tmassJ2 , _w ); 
  fill("j2METPt"+lvl, ptJ2MET , _w ); 
  fill("j2METDPhi"+lvl, dphiJ2MET , _w ); 
  
  // if(lvl=="BTag")
  //   fill("bj1METTMass"+lvl, mT2() , _w ); 
  fill("bj1METPt"+lvl, ptBJ1MET , _w ); 
  fill("bj1METDPhi"+lvl, dphiBJ1MET , _w ); 

  fill("bj2METTMass"+lvl, tmassBJ2 , _w ); 
  fill("bj2METPt"+lvl, ptBJ2MET , _w ); 
  fill("bj2METDPhi"+lvl, dphiBJ2MET , _w ); 
  
  fill("triTMassE"+lvl, mTEMETb, _w );

  fill("STjet"+lvl, elL_.Pt() + tauL_.Pt() + jetL_.Pt() + jetSL_.Pt(), _w );
  fill("STbjet"+lvl, STbjet_, _w );

  fill("STVsMass"+lvl,STbjet_,mTb_,_w);

  fill("nVertex"+lvl, nGoodVtx_ , _w ); 
  fill("nEl"+lvl, nGEl_ , _w ); 
  fill("nTau"+lvl, nGTau_ , _w ); 
  fill("nJet"+lvl, nGJet_ , _w ); 
  fill("nBJet"+lvl, nGBJet_ , _w ); 

  fill("decHLT"+lvl, passHLT(), _w );

  fill("dalitzMass"+lvl, taubj1_.M() , ej1_.M() , _w );
  fill("dalitzBMass"+lvl, taubj1_.M() , ebj1_.M() , _w );

  fill("bjetSource"+lvl,  fabs(matchGenParticle( &jetBL_) ) , _w );
  fill("ljetSource"+lvl,  fabs(matchGenParticle( (jetLIdx_!=jetBLIdx_)?&jetL_:&jetSL_) ) , _w );
  if( fabs(matchGenParticle( &elL_) ) == 15 )
    fill("ljetTauSource"+lvl,  fabs(matchGenParticle( (jetLIdx_!=jetBLIdx_)?&jetL_:&jetSL_) ), fabs(matchGenParticle( &tauL_) ) , _w );
}




void  
LQAnalysis::loadPUWeights() {
  
  TFile * file = new TFile("/home/mmarionn/Documents/CMS/dev/LUNE/data/rootFiles/puWeightsLQ.root","READ"); //puWeightsS10
  puweights_ = (TH1F*)file->Get("pileup");

}

float 
LQAnalysis::puWeight(float trueNint) {
  return puweights_->GetBinContent( puweights_->GetXaxis()->FindBin( trueNint)  );
}





bool 
LQAnalysis::passHLT() {

  bool pass=false;
  
  for(size_t ih=0;ih<_vc->getSize("HLTInsideDatasetTriggerNames");ih++) {
    pass =  triggerDecision(ih, "HLT_IsoMu24"); //triggerDecision(ih, "HLT_Ele27_WP80_v");// ||triggerDecision(ih, "HLT_IsoMu24"); //
    
    //pass = triggerDecision(ih, "HLT_IsoMu24");
    if(pass){ return true;}
  }
  return false;
}

bool
LQAnalysis::triggerDecision(size_t ihlt, string ref) {
  if(!_au->simpleCut<size_t>(_vc->getS("HLTInsideDatasetTriggerNames",ihlt).find(ref),-1,"!=") ) return false;
  if(!_au->simpleCut<int>(_vc->getI("HLTInsideDatasetTriggerPrescales", ihlt),1,"=")) return false;
  if(!_au->simpleCut(_vc->getB("HLTInsideDatasetTriggerDecisions",ihlt) ) ) return false;
  return true;
}

int
LQAnalysis::findZDecayChannel() {

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


bool
LQAnalysis::isGenParticle(TLorentzVector* v4, int pdgId) {

  size_t nGen=_vc->getSize("GenParticlePt"); 
  
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),10,">") ) continue;
    
    if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),pdgId,"=") ) ) continue; 

    float dR = KineUtils::dR(_vc->getD("GenParticleEta",ig), v4->Eta() ,_vc->getD("GenParticlePhi",ig), v4->Phi() );
    if(dR<0.2) return true;

  }

  return false;
}


int
LQAnalysis::matchGenParticle(TLorentzVector* v4) {

  size_t nGen=_vc->getSize("GenParticlePt"); 
  float tmp=100;
  int pdgId=-1;
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),v4->Pt()/3.,">") ) continue;
    
    float dR = KineUtils::dR(_vc->getD("GenParticleEta",ig), v4->Eta() ,_vc->getD("GenParticlePhi",ig), v4->Phi() );
    if(dR<tmp && fabs(_vc->getI("GenParticlePdgId",ig))!=42) {
      tmp =dR;
      pdgId = _vc->getI("GenParticlePdgId",ig);
    }

  }
  if(tmp>0.5) return -1;
  return pdgId;
}


int
LQAnalysis::findIdxGenParticle(TLorentzVector* v4, string l) {
  cout<<" !!====== "<<l<<endl;
  size_t nGen=_vc->getSize("GenParticlePt"); 
  float tmp=100;
  int idx=-1;
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),v4->Pt()/3.,">") ) continue;
    
    float dR = KineUtils::dR(_vc->getD("GenParticleEta",ig), v4->Eta() ,_vc->getD("GenParticlePhi",ig), v4->Phi() );
    if(dR<tmp && fabs(_vc->getI("GenParticlePdgId",ig))!=42) {
      tmp =dR;
      idx = ig;
      cout<<" \t -> "<<tmp<<"   "<<ig<<"   "<<_vc->getI("GenParticlePdgId",ig)<<endl;
    }

  }
  if(tmp>0.5) return -1;
  return idx;
}


int 
LQAnalysis::genParticle(int pdgId) {
  
  if(pdgId==11)
    return 0;
  else if(pdgId==13)
    return 1;
  else if(pdgId==15)
    return 2;
  else if(pdgId==12 ||pdgId==14 ||pdgId==16)
    return 3;
  else if(pdgId<=3)
    return 4;
  else if(pdgId>3 && pdgId<=6)
    return 5;
  else if(pdgId==211 || pdgId==111)
    return 6;
  else if(pdgId==22)
    return 7;
  else if(pdgId==23)
    return 8;
  else if(pdgId==24)
    return 9;
  else
    return 10;
  
}


float
LQAnalysis::modWeightDDB() {
  if(_isDDBkg==1) {

    float fr= _dbm->getDBValue ("tauFR", tauL_.Pt() );
    
    return fr/(1-fr);

    // // if(_ids.find("W")!=(size_t)-1)
    
    // // else    
    //   return 1.07; //0.9386
  }
  else return 1.;

}


void
LQAnalysis::storeBTagInfos(float pt, float eta, float disc, int flav) {
  
  float eff = bTagDB_->getBTagEff(pt,eta,disc,flav);
  float sf = bTagDB_->getBTagSF(pt,eta,flav);

  JetInfo jetinfo(eff,sf);
  vector<JetInfo> jetInfoForAllOPs;
  jetInfoForAllOPs.push_back(jetinfo);
  jetInfos_.push_back(jetInfoForAllOPs);
}

float
LQAnalysis::getBTagWeight() {
  return bTagSFTool_->weight(jetInfos_);
}




IClassifierReader* 
LQAnalysis::CreateNN() {

  //Définition des variables (comme pour le training)
  vector<string> inputVars;

  inputVars.push_back("mtb");
  inputVars.push_back("meb");
  inputVars.push_back("met");
  inputVars.push_back("tmassT");
  inputVars.push_back("dphiLM");
  inputVars.push_back("STbjet");
  // inputVars.push_back("pt_lep");
  // inputVars.push_back("pt_tau");

  IClassifierReader* NN = new NeuralNet( inputVars );

  return NN;

}


float 
LQAnalysis::mT2() {


  float sx = 5.;
  float sy = 5.;

  TLorentzVector sljt = taubj2_;//(jetLIdx_!=jetBLIdx_)?tauj1_:tauj2_;
  TLorentzVector slje = ebj2_;//(jetLIdx_!=jetBLIdx_)?ej1_:ej2_;

  float mT2_1=10000000;
  float mT2_2=10000000;
  float mT2=1000000;

  TVector2 nu1(0,0);
  TVector2 nu2(0,0);

  TVector2 nu1_s(0,0);
  TVector2 nu2_s(0,0);

  //cout<<" begin event ================================ "<<endl;

  for(int i=1;i<100;i++) {
    for(int j=1;j<100;j++) {
      
      float px1 = met_.Px() -i*sx;
      float py1 = met_.Py() -j*sy;

      float px2 = met_.Px() - px1;
      float py2 = met_.Py() - py1;

      nu1.Set(px1,py1);
      nu2.Set(px2,py2);
      
      // nu1.Print();
      // nu2.Print();

      { //pair 1
	float m1 = taubj1_.M();
	float m2 = slje.M();
	
	float mt1 = sqrt( pow(m1,2) + 2*(taubj1_.Et()*nu1.Mod() - taubj1_.Vect().XYvector()*nu1) );
	float mt2 = sqrt( pow(m2,2) + 2*(slje.Et()*nu2.Mod() - slje.Vect().XYvector()*nu1) );
	
	//cout<<" pair 1 : "<<mt1<<" / "<<mt2<<endl;

	mT2_1 = max(mt1,mt2);

      }
      { //pair 2
	float m1 = sljt.M();
	float m2 = ebj1_.M();
	
	float mt1 = sqrt( pow(m1,2) + 2*(sljt.Et()*nu1.Mod() - sljt.Vect().XYvector()*nu1) );
	float mt2 = sqrt( pow(m2,2) + 2*(ebj1_.Et()*nu2.Mod() - ebj1_.Vect().XYvector()*nu1) );


	//cout<<" pair 2 : "<<mt1<<" / "<<mt2<<endl;
	mT2_2 = max(mt1,mt2);
      }
     
      //cout<<" minimization : "<<mT2<<" / "<<min(mT2_1,mT2_2)<<endl;
      if(mT2> min(mT2_1,mT2_2) ) {
	mT2 = min(mT2_1,mT2_2);
	nu1_s = nu1;
	nu2_s = nu2;
	//cout<<" minimized! "<<endl;
      }

      //mT2 = min(mT2, min(mT2_1,mT2_2) );
    }
  }
  
  //cout<<" MET "<<endl;
  // met_.Print();
  // nu1_s.Print();
  // nu2_s.Print();
  //cout<<" mT2 : "<<mT2<<endl;

  return mT2;

}

void
LQAnalysis::endAnalysis() {

  cout<<" efft "<<efft<<"   "<<N<<"   "<<efft/N<<endl;


  cout<<" Fake tau background measurement "<<endl;
  cout<<" Standard : "<<nFTStd_<<endl;
  cout<<" Matthieu : "<<nFTMM_<<endl;
  cout<<" Keti     : "<<nFTKK_<<endl;


}


bool 
LQAnalysis::isElAcc(float pt, float eta) {

  if(pt<30) return false;
  if( fabs(eta) >2.5 || (fabs(eta)<1.56 && fabs(eta)>1.4442 ) ) return false;
  return true;
}

float
LQAnalysis::frMM(float pt1, float pt2) {

  float fr1=_dbm->getDBValue("tauFRMC", pt1);
  float fr2=_dbm->getDBValue("tauFRMC", pt2);

  float fr = fr1 + (1-fr1)*fr2;

  return fr/(1-fr);
}


float
LQAnalysis::frKK(float pt1, float pt2) {

  float fr1=_dbm->getDBValue("tauFRMC", pt1);
  float fr2=_dbm->getDBValue("tauFRMC", pt2);

  return fr1/(1-fr1) + (1- fr1/(1-fr1))*fr2/(1-fr2);
}


float
LQAnalysis::frStd(float pt1) {

  float fr=_dbm->getDBValue("tauFRMC", pt1);
 
  return fr/(1-fr);
}


