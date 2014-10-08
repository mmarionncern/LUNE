#include "src/LQFastAnalysis.hh"

using namespace std;

ClassImp(LQFastAnalysis)

LQFastAnalysis::LQFastAnalysis():
UseTree() {

  loadPUWeights();

  levels_.push_back( "Presel" );
  levels_.push_back( "Veto" );
  levels_.push_back( "MultJet" );
  levels_.push_back( "BTag" );
  levels_.push_back( "Adv" );
  levels_.push_back( "Final" );

  //special
  levels_.push_back( "Iso" );
  levels_.push_back( "AIso" );
  levels_.push_back( "WIso" );
  levels_.push_back( "WAIso" );

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
  
  mcFRTauRes_=0;
  mcFR_=0;
  dFR_=0;

  osCharge_=true;
  lqdSel_=false;

  tauES_=false;
  tauER_=false;
  jetES_=false;
  jetER_=false;
  var_=0;
}

LQFastAnalysis::~LQFastAnalysis() {

}

void
LQFastAnalysis::modifySkimming() {
}

void
LQFastAnalysis::advOptions(bool osCharge, bool lqdSel, string unc) {
  osCharge_ = osCharge;
  lqdSel_ = lqdSel;

  if(unc.find("TES")!=(size_t)-1 ) {
    tauES_=true;
  }
  if(unc.find("TER")!=(size_t)-1 ) {
    tauER_=true;
  }
  if(unc.find("JES")!=(size_t)-1 ) {
    jetES_=true;
  }
  if(unc.find("JER")!=(size_t)-1 ) {
    jetER_=true;
  }
  
  if(unc.find("Do")!=(size_t)-1 ) {
    var_=-1;
  }
  if(unc.find("Up")!=(size_t)-1 ) {
    var_=1;
  }

}


void
LQFastAnalysis::bookHistograms() {

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

    // _hm->addVariable("muPt"+levels_[ilvl],500,0,500,"p_{T}(#mu) [GeV] ");
    // _hm->addVariable("muEta"+levels_[ilvl],120,-3,3,"#eta(#mu)  ");
    // _hm->addVariable("muPhi"+levels_[ilvl],256,-TMath::Pi(),TMath::Pi(),"#phi(#mu) [rad] ");

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
   

    _hm->addVariable("etauMass"+levels_[ilvl],1000,0,1000,"M_{e#tau} [GeV] ");
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
    //_hm->addVariable("STbjet"+levels_[ilvl],1000,0,2000,"S_{T} [geV] ");
    _hm->addVariable("STbjet"+levels_[ilvl],30,0,1200,"S_{T} [geV] ");



    _hm->addVariable("STVsMass"+levels_[ilvl],100,0,2000,100,0,1000,"S_{T} [GeV] ","M(#tau,b-j2) [GeV] ");



    _hm->addVariable("dalitzMass"+levels_[ilvl],100,0,1000,100,0,1000,"M(#tau,b) [GeV] ","M(e,j) [GeV] ");
    _hm->addVariable("dalitzBMass"+levels_[ilvl],100,0,1000,100,0,1000,"M(#tau,b) [GeV] ","M(e,b) [GeV] ");

    //multiplicity
    _hm->addVariable("nVertex"+levels_[ilvl],50,0,50,"vertex multiplicity "); 
    _hm->addVariable("nEl"+levels_[ilvl],10,0,10,"electron multiplicity "); 
    _hm->addVariable("nTau"+levels_[ilvl],10,0,10,"tau multiplicity "); 
    _hm->addVariable("nJet"+levels_[ilvl],20,0,20,"jet multiplicity "); 
    _hm->addVariable("nBJet"+levels_[ilvl],10,0,10,"b-jet multiplicity "); 

    //miscellaneous
    _hm->addVariable("decHLT"+levels_[ilvl],2,-0.5,1.5,"decision HLT "); 

    _hm->addVariable("bjetSource"+levels_[ilvl],25,-0.5,24.5,"b jet source ");
    _hm->addVariable("ljetSource"+levels_[ilvl],25,-0.5,24.5,"l jet source ");
    _hm->addVariable("ljetTauSource"+levels_[ilvl],25,-0.5,24.5,25,-0.5,24.5,"l jet source ","tau source");
 
  }

  _hm->addVariable("STbjetFR",1000,0,2000,"S_{T} [GeV] ");

  _hm->addVariable("STbjet",25,100,1100,"S_{T} ");
  _hm->addVariable("STbjetLB",120,0,1200,"S_{T} ");
  _hm->addVariable("STbjetF",50,0,2000,"S_{T} ");

  if(!lqdSel_) {
    int nBins = 13;
    vector<float> bins;
    bins.push_back(140);
    bins.push_back(210);
    bins.push_back(280);
    bins.push_back(350);
    bins.push_back(420);
    bins.push_back(480);
    bins.push_back(540);
    bins.push_back(620);
    bins.push_back(700);
    bins.push_back(780);
    bins.push_back(870);
    bins.push_back(980);
    bins.push_back(1100);
    bins.push_back(1200);
    _hm->addVariable("STbjetVar",nBins,bins,"S_{T} ");
  }
  else {
    int nBins = 12;
    vector<float> bins;
    bins.push_back(200);
    bins.push_back(280);
    bins.push_back(350);
    bins.push_back(420);
    bins.push_back(480);
    bins.push_back(540);
    bins.push_back(610);
    bins.push_back(680);
    bins.push_back(770);
    bins.push_back(860);
    bins.push_back(960);
    bins.push_back(1100);
    bins.push_back(1200);
    _hm->addVariable("STbjetVar",nBins,bins,"S_{T} ");
  }  

  _hm->addVariable("NeuralNet",100,0,1.00,"NN output");
  _hm->addVariable("elPdgId",101,-50.5,50.5,"el pdgId");
  _hm->addVariable("tauPdgId",101,-50.5,50.5,"tau pdgId");
  _hm->addVariable("bPdgId",21,0,21,"bPdgId"); 
  //  _hm->addVariable("finalMass",1000,0,1000,"M_{#tauj} [geV] "); 
  _hm->addVariable("finalMass",50,0,1000,"M_{#tauj} [geV] "); 

}

void
LQFastAnalysis::modifyWeight() {

  if(!_isData) {
    float w=puWeight(_vc->getF("trueNPU") );
    _w *= w;
  }
  
}


void
LQFastAnalysis::analyze() {
  
  if(_ie==0) {
    mcFRTauRes_=0;
    mcFR_=0;
    dFR_=0;
  }

  //if(!_au->simpleCut(_vc->getI("event")==983595640) ) return; 

  //splitting events with fake/real tau =================================
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
  set<int> vtxs;
  for(size_t ivtx = 0; ivtx < _vc->getSize("vtxRho") ; ++ivtx) {
    double ndf  = _vc->getF("vtxNdf",ivtx);
    double z    = _vc->getF("vtxZ",ivtx);
    double rho  = _vc->getF("vtxRho",ivtx);
	
    if (!(_vc->getB("vtxIsFake",ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2 ) {
      ++nGoodVtx_;
      vtxs.insert( ivtx );
    }
  }			
 
  if(!_au->makeCut<int>(nGoodVtx_,1,">=",_ids,"has good vtx",_w)) return;
 
  //========================= Leptons
  //

  size_t nLep  =_vc->getSize("lepPt");
  size_t nTau = _vc->getSize("tauPt");
  
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

  jIdx_.resize(0);
  jetPt_.resize(0);

  float pttmp=0,pttmp2=0;
  for(size_t im=0;im<nLep;im++) {
    if( _au->simpleCut( _vc->getI("lepPdgId",im) ==13 ) ) {
      if(_au->simpleCut(  (_vc->getI("lepIdLvl",im)&2)==2 ) ) { //0

	if(!_au->simpleCut( vtxs.find( _vc->getUL("lepVtx",im) )!=vtxs.end() ) ) continue;

	if( _au->simpleCut<double>(_vc->getF("lepPt",im),pttmp,">") ) {
	  muL_.SetPtEtaPhiM( _vc->getF("lepPt",im), 
			     _vc->getF("lepEta",im),
			     _vc->getF("lepPhi",im), 0.106 );
	  muIdx_ = im;
	  pttmp = muL_.Pt();
	}
	nGMu_++;
	  
	//if(muIdx_!=im) { //Sec lep Os
	//}
      }
    }
  }//for imu
  
  pttmp=0;
  // for(size_t im=0;im<nLep;im++) {
  //   if( _au->simpleCut( _vc->getI("lepPdgId",im) ==13 ) && muIdx_ != im ) {
  //     if(_au->simpleCut(  _vc->getI("lepIdLvl",im)!=0 ) ) {
  // 	if( _au->simpleCut<double>(_vc->getF("lepPt",im),pttmp,">") ) {
  // 	  muSL_.SetPtEtaPhiM( _vc->getF("lepPt",im), 
  // 			      _vc->getF("lepEta",im),
  // 			      _vc->getF("lepPhi",im), 0.106 );
  // 	  muSLIdx_ = im;
  // 	  pttmp = muSL_.Pt();
  // 	}
  //     }
  //   }
  // }


  //========== Gen matching
  //cout<<"================ new event "<<endl;
  // size_t mIdx=-1;
  // size_t nGen=_vc->getSize("genPt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  //   if(_au->simpleCut(!_isData) )
  //     if(!(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),11,"=")// ||
  // 	   //_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),15,"=")
  // 	   ) ) continue; 
  //   if(!(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",_vc->getUL("genMother",ig))), 24, "=") || 
  // 	  _au->simpleCut<int>(fabs(_vc->getI("genPdgId",_vc->getUL("genMother",ig))), 15, "=") 
  // 	 )) continue;
  //   mIdx = _vc->getUL("genMother",ig);
  //========== Gen matching
  pttmp=0;
  for(size_t ie=0;ie<nLep;ie++) {
    if( !_au->simpleCut( _vc->getI("lepPdgId",ie) ==11 ) ) continue;
      
    tmpJet_.SetPtEtaPhiM( _vc->getF("lepPt",ie), 
			 _vc->getF("lepEta",ie),
			 _vc->getF("lepPhi",ie), 0.0005 );

    //gen matching
    // bool isMCLepTau=isGenParticle( &tmpJet_, 11 ); 
    // if(!_au->simpleCut( isMCLepTau) ) continue; 
    
    if( !_au->simpleCut(  (_vc->getI("lepIdLvl",ie)&2)==2 ) ) continue;
    if(!_au->simpleCut( vtxs.find( _vc->getUL("lepVtx",ie) )!=vtxs.end() ) ) continue;
    
    if( _au->simpleCut<double>(_vc->getF("lepPt",ie),pttmp,">") ) {
      elL_ = tmpJet_;
      elIdx_ = ie;
      pttmp = elL_.Pt();
    }
    nGEl_++;
	
  }//for iel

  //}//gen matching
  
  for(size_t ie=0;ie<nLep;ie++) {
   
    
     //OF first
    if(_au->simpleCut(_vc->getI("lepPdgId",ie)!=_vc->getI("lepPdgId",elIdx_) ) ) {
      if(!_au->simpleCut( (_vc->getI("lepIdLvl",ie)&2)!=0) ) continue;
      if( elIdx_!=ie) {
	secLepOS_=true;
	break;
      }
    }
    else { //SF
      if(!_au->simpleCut(_vc->getI("lepCharge",elIdx_)*_vc->getI("lepCharge",ie)== -1 ) ) continue;
      if(!_au->simpleCut( _vc->getI("lepIdLvl",ie)&1 ) ) continue;
      if(!_au->simpleCut(_vc->getF("lepPt",ie)>20) ) continue;
      if( elIdx_!=ie) {
	secLepOS_=true;
	break;
      }
    }
    
  }    
  
  pttmp=0;
  // //========== Gen matching
  // size_t nGen=_vc->getSize("genPt"); 
  // if(_isData) nGen=1;
  // for(size_t ig=0;ig<nGen;ig++) {
  // if(_au->simpleCut(!_isData) ) {
  //   if( !_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),15,"=")) continue; 
  //   if( !_au->simpleCut<int>(fabs(_vc->getI("genPdgId",_vc->getUL("genMother",ig))), 24, "=") ) continue;
  // }
 
  //========== Gen matching
  bool hasTauIso=false;
  vector<TLorentzVector> aitaus;
  for(size_t it=0;it<nTau;it++) {

    //cout<<_ie<<"   "<<nTau<<"   "<<_vc->getF("tauPt",it)<<endl;

    tmpJet_.SetPtEtaPhiM( _vc->getF("tauPt",it), 
			  _vc->getF("tauEta",it),
			  _vc->getF("tauPhi",it), 1.7 );

    if(mif(tauES_ && !_isData)) tesUnc(tmpJet_,var_);
    if(mif(tauER_ && !_isData)) terUnc(tmpJet_,it,var_);
    
    if( !_au->simpleCut( tmpJet_.Pt()>30 ) ) continue;
    //gen matching
    // if(_au->simpleCut(!_isData) )
    // 	if(!_au->simpleCut<float>(KineUtils::dR( _vc->getF("tauEta",it) ,_vc->getF("genEta",ig), _vc->getF("tauPhi",it), _vc->getF("genPhi",ig)), 0.2 ,"<")) continue; 
    
    //float dRel = muL_.DeltaR( tmpJet_ );
    //if( !_au->simpleCut( !_vc->getB("tauIso",it) ) ) continue;
   
    float dRel = elL_.DeltaR( tmpJet_ );
    
    if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
    
    if(!_au->simpleCut(_vc->getUL("tauVtx",it)==_vc->getUL("lepVtx",elIdx_) ) ) continue;
    if(!_au->simpleCut<int>(_vc->getI("lepCharge",elIdx_)*_vc->getI("tauCharge",it),(osCharge_?-1:1),"=") ) continue;

    //do not use tau if not fake for the fake tau shape 
    if(_au->simpleCut(_ids.find("ftSH")!=(size_t)-1) )
      {
	bool isMCTau=isGenParticle( &tmpJet_, 15 );
	if(!_au->simpleCut( !isMCTau ) ) continue; 
      }

    //only use real taus for the V+jet background 
    if(_au->simpleCut(_ids=="Z+jets") )
      {
	bool isMCTau=isGenParticle( &tmpJet_, 15 );
	if(!_au->simpleCut( isMCTau ) ) continue; 
      }

    if( _au->simpleCut(_ids.substr(0,8)=="t#bar{t}") ) {
      bool isMCTau=isGenParticle( &tmpJet_, 15 );
      if(!_au->simpleCut( isMCTau ) ) continue; 
    } 

    //count only events with a fake tau from leptons in ttbar lep-tau bkg
    if(_au->simpleCut(_ids=="topleptau") ) {
      bool isMCLepTau=isGenParticle( &tmpJet_, 13 ) 
	|| isGenParticle( &tmpJet_, 11 );
	 
      if(!_au->simpleCut( isMCLepTau) ) continue; 
    }	 
    if(_vc->getB("tauIso",it) )
      hasTauIso=true;
 
    if( _au->simpleCut<double>(tmpJet_.Pt(),pttmp,">") ) {
      tauL_ = tmpJet_;
      tauIdx_ = it;
	    
      pttmp = tauL_.Pt();

    }
    nGTau_++;
    aitaus.push_back( tmpJet_ );
	
  }//for itau
  // }//gen matching
  //cout<<" NGTau : "<<nGTau_<<endl;
  
  //
  // ==============================

  //FIXME
  met_.SetMagPhi( _vc->getF("metPt"), _vc->getF("metPhi") );

  //metXYCorrection();

  // if(!_au->simpleCut( _vc->getF("metPt")< 50 ) ) return;
  // float tmass  = sqrt(2*tauL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),tauL_.Phi() ))));
  // if(!_au->simpleCut( tmass< 100 ) ) return;
  //====
  

  //FIXME mumu selection for SF jet mult

  if(!_au->makeCut<int>( nGEl_, 0 , ">" ,_ids,"el multiplicity",_w) ) return;

  if(!_isData)
    _w *=_dbm->getDBValue ("elID", fabs(elL_.Eta()), elL_.Pt() );


  // if(!_au->makeCut<int>( nGMu_, 0 , ">" ,_ids,"mu multiplicity",_w) ) return;
  
  // if(!_isData)
  //   _w *=_dbm->getDBValue ("muID", fabs(elL_.Eta()), elL_.Pt() )*_dbm->getDBValue ("muIso", fabs(elL_.Eta()), elL_.Pt() );

  if(_isData) {
    if(!_au->makeCut(passHLT(), _ids, "HLT decision", _w ) ) return;
  }
  else {
    _w *=_dbm->getDBValue ("elTrig", elL_.Pt(), fabs(elL_.Eta()) );
    if(!_au->makeCut(true, _ids, "HLT decision", _w ) ) return;
  }

  //FIXME 0
  if(!_au->makeCut<int>( nGTau_, 0 , ">" ,_ids,"tau multiplicity",_w) ) return;
  
  //FIXME, let's try iso cut here
  //_isDDBkg =  1-hasTauIso;

  if(!_au->makeCut( hasTauIso,_ids,"tau isolation",_w) ) return;

  //ok, we have an isoalted tau, let find which one is isolated
  if(_au->simpleCut( hasTauIso ) ) {
    pttmp=0;
    for(size_t it=0;it<nTau;it++) {
      tmpJet_.SetPtEtaPhiM( _vc->getF("tauPt",it), 
			    _vc->getF("tauEta",it),
			    _vc->getF("tauPhi",it), 1.7 );

      if(mif(tauES_ && !_isData)) tesUnc(tmpJet_,var_);
      if(mif(tauER_ && !_isData)) terUnc(tmpJet_,it,var_);
      
      if(_au->simpleCut(_ids=="Z+jets") )
	{
	  bool isMCTau=isGenParticle( &tmpJet_, 15 );
	  if(!_au->simpleCut( isMCTau ) ) continue; 
	}

      if( _au->simpleCut(_ids.substr(0,8)=="t#bar{t}") ) {
       	bool isMCTau=isGenParticle( &tmpJet_, 15 );
       	if(!_au->simpleCut( isMCTau ) ) continue; 
      } 


      float dRel = elL_.DeltaR( tmpJet_ );
    

      //cout<<_vc->getF("tauPt",it)<<"  "<<dRel<<"  "<<(_vc->getUL("tauVtx",it)==_vc->getUL("lepVtx",elIdx_))<<"   "<<(_vc->getI("lepCharge",elIdx_)*_vc->getI("tauCharge",it),(osCharge_?-1:1))<<"   "<<_vc->getB("tauIso",it)<<"   "<<it<<endl;

      if( !_au->simpleCut( tmpJet_.Pt()>30 ) ) continue;
      if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
      if( !_au->simpleCut( _vc->getB("tauIso",it) ) ) continue;
      if(!_au->simpleCut(_vc->getUL("tauVtx",it)==_vc->getUL("lepVtx",elIdx_) ) ) continue;
      if(!_au->simpleCut<int>(_vc->getI("lepCharge",elIdx_)*_vc->getI("tauCharge",it),(osCharge_?-1:1),"=") ) continue; 

      //cout<<"leading : "<<tmpJet_.Pt()<<"   "<<_vc->getB("tauIso",it)<<"  "<<it<<endl;
    
      if( _au->simpleCut<double>(tmpJet_.Pt(),pttmp,">") ) {

	tauL_ = tmpJet_;
	tauIdx_ = it;
	pttmp = tauL_.Pt();
      }
    } //ntau 
  } //iso tau
  
  fillPlots( "Presel");
  
  fillPlots( "Veto");


  //========================== Jets
  //
  size_t nJets  =_vc->getSize("jetPt");
  nGJet_=0;
  nGBJet_=0;
  pttmp=0;
  pttmp2=0;
  float pttmpb=0;
  float pttmpb2=0;
  
  jetInfos_.clear();
  for(size_t ij=0;ij<nJets;ij++){ 
    
    tmpJet_.SetPtEtaPhiE( _vc->getF("jetPt",ij),
			  _vc->getF("jetEta",ij),
			  _vc->getF("jetPhi",ij),
			  _vc->getF("jetE",ij) );

    if(mif(jetES_ && !_isData)) jesUnc(tmpJet_,ij,var_);
    if(mif(jetER_ && !_isData)) jerUnc(tmpJet_,ij,var_);

    if(!_au->simpleCut<double>( tmpJet_.Pt(), 30 , ">") ) continue;
    if(!_au->simpleCut<double>( fabs(tmpJet_.Eta()), 2.4, "<") ) continue;

   

    float dRel = tmpJet_.DeltaR( elL_ );
    if(!_au->simpleCut<float>(dRel, 0.5, ">") ) continue;  
    
    float dRtau = tmpJet_.DeltaR( tauL_ );
    if(!_au->simpleCut<float>(dRtau, 0.5, ">") ) continue;
    
    // bool matched=false;
    // for(size_t it=0;it<aitaus.size();it++) {
    //   float dRtau = tmpJet_.DeltaR( aitaus[it] );
    //   if(!_au->simpleCut<float>(dRtau, 0.5, ">") ) {matched=true; break;}
    //  }
    // if(!_au->simpleCut( !matched ) ) continue;
  
    if(!_au->simpleCut<int>(_vc->getB("jetId",ij), 1, "=") ) continue;

    nGJet_++;
    
    storeBTagInfos( tmpJet_.Pt(), tmpJet_.Eta(), 
		    _vc->getF("jetBTag",ij),
		    _vc->getI("jetFlav",ij) );
    

    if( tmpJet_.Pt() > pttmp ) {
      jetL_ = tmpJet_;
      pttmp = jetL_.Pt();
      jetLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmp2 && tmpJet_.Pt() < pttmp&& jetLIdx_!=ij ) {
      jetSL_ = tmpJet_;
      pttmp2 = jetSL_.Pt();
      jetSLIdx_ = ij;
    }
    
    jetPt_.push_back( tmpJet_.Pt() );
    jIdx_.push_back( ij );
    
    //cout<<" btag? "<<tmpJet_.Pt()<<"   "<<_vc->getF("jetBTag",ij)<<endl;

    if(!_au->simpleCut<double>(_vc->getF("jetBTag",ij), 0.244, ">") ) continue; //0.679 = medium, 0.244=loose
    //cout<<" -> btag! "<<endl;

    nGBJet_++;
    
    if( tmpJet_.Pt() > pttmpb ) {
      jetBL_ = tmpJet_;
      pttmpb = jetBL_.Pt();
      jetBLIdx_ = ij;
    }
    if( tmpJet_.Pt() > pttmpb2 && tmpJet_.Pt() < pttmpb&& jetBLIdx_!=ij ) {
      jetBSL_ = tmpJet_;
      pttmpb2 = jetBSL_.Pt();
      jetBSLIdx_ = ij;
    }
    
  }// ijet
  

  //2 >=
  if(!_au->makeCut<int>( nGJet_, (lqdSel_?5:2) , ">=" ,_ids,"jet multiplicity",_w) ) return;
  //if(!_au->makeCut<int>( nGJet_, 3 , "[]" ,_ids,"jet multiplicity",_w,4) ) return;
 
  
  fillPlots( "MultJet");

  
  // btagging
  if(!_isData)
    _w *=getBTagWeight(); 
  if(!_au->makeCut<int>( nGBJet_, 1 , ">=" ,_ids,"b-tagging",_w) ) return;
    
  
  if(!_au->makeCut( !secLepOS_ ,_ids,"veto add lepton ",_w) ) return;
  
  
  
  //Temporary W
  //if(_au->simpleCut( met_.Mod() > 50 ) )  {
    // float tmassE  = sqrt(2*elL_.Pt()*met_.Mod()*(1-cos(KineUtils::dPhi(met_.Phi(),elL_.Phi() ))));
    // if(_au->simpleCut( tmassE > 70 ) ) {
    //   if(_au->simpleCut( hasTauIso) )
    // 	fillPlots( "WIso");
    //   else
    // 	fillPlots( "WAIso");
    // }
    //}
  //


  fillPlots( "BTag");

  float M11,M12;
  M11 = (elL_ +jetBL_).M();
  M12 = (elL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();
  // M11 = (elL_+jetL_).M();
  // M12 = (elL_+ jetSL_).M();

  float M21 = (tauL_+jetBL_).M();
  float M22 = (tauL_+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
  // float M21 = (tauL_+jetL_).M();
  // float M22 = (tauL_+ jetSL_).M();
  float m1=0;
  if (fabs(M11-M22) < fabs(M12-M21)) {
    m1 = M22;
  }else{
    m1 = M21;
  }
  
  // if(_au->simpleCut( met_.Mod() > 50 ) )  {
  //   if(_au->simpleCut( hasTauIso) )
  //     fillPlots( "Iso");
  //   else
  //     fillPlots( "AIso");
  // }
  //fill("finalMass",m1,_w);
  
  //cout<<_vc->getI("run")<<"   "<<_vc->getI("event")<<"   "<<tauL_.Pt()<<"   "<<elL_.Pt()<<endl;

  if(!_au->makeCut<float>( tauL_.Pt() , 50 , ">" ,_ids,"tau pT cut",_w) ) return;
  //if(!_au->makeCut( hasTauIso,_ids,"tau isolation",_w) ) return; 
  fillPlots( "Adv");
  fill("finalMass",m1,_w);
  //fill("finalMass",m1,_w);
  if(_au->simpleCut(!lqdSel_) )
    if(!_au->makeCut<float>( m1, 250, ">", _ids,"tauB Mass",_w) ) return;  

  if(!_isData) {
    //cout<<_w<<"    "<<  fr()<<"   "<<frRealTau()<<"    "<<mcFR_<<"   "<<mcFRTauRes_<<endl;
    mcFRTauRes_ += _w*frRealTau();
    mcFR_ += _w*fr();
  
  }
  else {
    dFR_ += fr();
    //cout<<dFR_<<endl;
  }

  fillPlots( "Final");
  fill("STbjet", STbjet_, _w );
  fill("STbjetF", STbjet_, _w );
  fill("STbjetLB", STbjet_, _w );
  fill("STbjetVar", STbjet_, _w );
  //fillFRShape(aitaus);
}


void LQFastAnalysis::fillPlots(string lvl) {

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
  fill("elCharge"+lvl, _vc->getI("lepCharge",elIdx_) , _w );
  
  // fill("elIsTau"+lvl, isGenParticle( &elL_, 15) , _w );
  // fill("elIsEl"+lvl, isGenParticle( &elL_, 11) , _w );
  

  fill("tauPt"+lvl, tauL_.Pt() , _w );
  fill("tauEta"+lvl, tauL_.Eta() , _w );
  fill("tauPhi"+lvl, tauL_.Phi() , _w ); 
  fill("tauCharge"+lvl, _vc->getI("tauCharge",tauIdx_) , _w );

  // fill("tauIsTau"+lvl, isGenParticle( &tauL_, 15) , _w );
  // fill("tauIsEl"+lvl, isGenParticle( &tauL_, 11) , _w );
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
  //if(lvl!="BTag") return;
  fill("STjet"+lvl, elL_.Pt() + tauL_.Pt() + jetL_.Pt() + jetSL_.Pt(), _w );
  if(!lqdSel_)
    STbjet_ = elL_.Pt() + tauL_.Pt() + jetBL_.Pt() + ((jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt() ) ;
  else {
    //cout<<" standard : "<<elL_.Pt() + tauL_.Pt() + jetBL_.Pt() + ((jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt() )<<endl;
    STbjet_ = elL_.Pt() + tauL_.Pt() + jetBL_.Pt();
    //cout<<" modified : "<<elL_.Pt()<<"  "<<tauL_.Pt()<<"  "<<jetBL_.Pt()<<" / "<<STbjet_<<endl;
    bool isB=false;
    if(_ie!=-1) {
      for(size_t ij=0;ij<4;ij++) {
	//cout<<" test: "<<jetPt_[ij]<<"   "<<isB<<endl;
	if(jIdx_[ij]!=jetBLIdx_) {
	  //cout<<jetPt_[ij]<<endl;
	  STbjet_ += jetPt_[ij];
	}
	else
	  isB=true;
      }
      
      if(isB) {
	STbjet_ += jetPt_[4];
	//cout<<" plus "<<jetPt_[4]<<endl;
      }
      //cout<<" ==> "<<STbjet_<<endl;
    }

    //temporary no bjet
    // STbjet_ = elL_.Pt() + tauL_.Pt();
    //   if(_ie!=-1) {
    // 	for(size_t ij=0;ij<5;ij++) {
    // 	  STbjet_ += jetPt_[ij];
    // 	}
    //   }
  }
 
  fill("STbjet"+lvl, STbjet_, _w );

  //fill("STVsMass"+lvl,STbjet_,mTb_,_w);

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
LQFastAnalysis::loadPUWeights() {
  
  TFile * file = new TFile("/home/mmarionn/Documents/CMS/LUNE/data/rootFiles/puWeightsLQ.root","READ"); //puWeightsS10
  puweights_ = (TH1F*)file->Get("pileup");

}

float 
LQFastAnalysis::puWeight(float trueNint) {
  return puweights_->GetBinContent( puweights_->GetXaxis()->FindBin( trueNint)  );
}





bool 
LQFastAnalysis::passHLT() {

  bool pass=false;
  
  for(size_t ih=0;ih<_vc->getSize("HLTName");ih++) {
    pass = triggerDecision(ih, "HLT_Ele27_WP80");// || triggerDecision(ih, "HLT_IsoMu24");
    //pass = triggerDecision(ih, "HLT_IsoMu24");
    if(pass) return true;
  }
  return false;
}

bool
LQFastAnalysis::triggerDecision(size_t ihlt, string ref) {

  if(!_au->simpleCut<size_t>(_vc->getS("HLTName",ihlt).find(ref),-1,"!=") ) return false;
  if(!_au->simpleCut<int>( (int)_vc->getF("HLTPres", ihlt),1,"=")) return false;
  if(!_au->simpleCut(_vc->getB("HLTDec",ihlt) ) ) return false;
  return true;
}

int
LQFastAnalysis::findZDecayChannel() {

  size_t nGen=_vc->getSize("genPt"); 
 
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getF("genPt",ig),10,">") ) continue;
   
    if( !(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),15,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),13,"=") ||
	  _au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),11,"=") ) ) continue; 

    if(_au->simpleCut<int>(fabs(_vc->getI("genPdgId", _vc->getUL("genMother",ig))),23,"=")) 
      return fabs(_vc->getI("genPdgId",ig));
  }

  return -1;
}


bool
LQFastAnalysis::isGenParticle(TLorentzVector* v4, int pdgId) {

  size_t nGen=_vc->getSize("genPt"); 
  
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getF("genPt",ig),10,">") ) continue;
    
    if( !(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),pdgId,"=") ) ) continue; 
    
    float dR = KineUtils::dR(_vc->getF("genEta",ig), v4->Eta() ,_vc->getF("genPhi",ig), v4->Phi() );
    //if(pdgId==15)cout<<_vc->getF("genEta",ig)<<"  "<<_vc->getF("genPhi",ig)<<"   "<<_vc->getF("genPt",ig)<<" <> "<<v4->Pt()<<"   "<<(_vc->getI("genPdgId",ig))<<endl; 
    if(dR<0.2) {return true;}

  }

  return false;
}


int
LQFastAnalysis::matchGenParticle(TLorentzVector* v4) {

  size_t nGen=_vc->getSize("genPt"); 
  float tmp=100;
  int pdgId=-1;
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getF("genPt",ig),v4->Pt()/3.,">") ) continue;
    
    float dR = KineUtils::dR(_vc->getF("genEta",ig), v4->Eta() ,_vc->getF("genPhi",ig), v4->Phi() );
    if(dR<tmp && fabs(_vc->getI("genPdgId",ig))!=42) {
      tmp =dR;
      pdgId = _vc->getI("genPdgId",ig);
    }

  }
  if(tmp>0.5) return -1;
  return pdgId;
}


int
LQFastAnalysis::findIdxGenParticle(TLorentzVector* v4, string l) {
  //cout<<" !!====== "<<l<<endl;
  size_t nGen=_vc->getSize("genPt"); 
  float tmp=100;
  int idx=-1;
  for(size_t ig=0;ig<nGen;ig++) {
      
    if(!_au->simpleCut<double>(_vc->getF("genPt",ig),v4->Pt()/3.,">") ) continue;
    
    float dR = KineUtils::dR(_vc->getF("genEta",ig), v4->Eta() ,_vc->getF("genPhi",ig), v4->Phi() );
    if(dR<tmp && fabs(_vc->getI("genPdgId",ig))!=42) {
      tmp =dR;
      idx = ig;
      cout<<" \t -> "<<tmp<<"   "<<ig<<"   "<<_vc->getI("genPdgId",ig)<<endl;
    }

  }
  if(tmp>0.5) return -1;
  return idx;
}


int 
LQFastAnalysis::genParticle(int pdgId) {
  
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
LQFastAnalysis::modWeightDDB() {
  if(_isDDBkg==1) {
    return fr();
  }
  else return 1.;

}


void
LQFastAnalysis::storeBTagInfos(float pt, float eta, float disc, int flav) {
  
  float eff = bTagDB_->getBTagEff(pt,eta,disc,flav);
  float sf = bTagDB_->getBTagSF(pt,eta,flav);

  JetInfo jetinfo(eff,sf);
  vector<JetInfo> jetInfoForAllOPs;
  jetInfoForAllOPs.push_back(jetinfo);
  jetInfos_.push_back(jetInfoForAllOPs);
}

float
LQFastAnalysis::getBTagWeight() {
  return bTagSFTool_->weight(jetInfos_);
}

void
LQFastAnalysis::endAnalysis() {

  cout<<" evaluation level fake from data : "<<dFR_<<endl;
  cout<<" evaluation level fake from MC : "<<mcFR_<<endl;
  cout<<" evaluation fake residual MC : "<<mcFRTauRes_<<endl;

}


float
LQFastAnalysis::fr() {

  size_t ntau= _vc->getSize("tauPt");

  float p=1;

  for(size_t it=0;it<ntau;it++) {
    if( !_au->simpleCut( _vc->getF("tauPt",it)>50 ) ) continue;

    tmpJet_.SetPtEtaPhiM( _vc->getF("tauPt",it), 
			  _vc->getF("tauEta",it),
			  _vc->getF("tauPhi",it), 1.7 );
    
    float dRel = elL_.DeltaR( tmpJet_ );
    if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
    if(!_au->simpleCut(_vc->getUL("tauVtx",it)==_vc->getUL("lepVtx",elIdx_) ) ) continue;
    if(!_au->simpleCut<int>(_vc->getI("lepCharge",elIdx_)*_vc->getI("tauCharge",it),-1,"=") ) continue;
    
    
    float fr=_dbm->getDBValue("tauFR", min(_vc->getF("tauPt",it),(float)199.) );
    //cout<<min(_vc->getF("tauPt",it),(float)199.)<<"   "<<fr<<endl;
    if(!_isData)
      fr=_dbm->getDBValue("tauFRMC", min(_vc->getF("tauPt",it),(float)199.) );
    p *= 1-fr;
  }
  //cout<<" == "<<p<<"   "<<(1-p)/p<<endl;
  return (1-p)/p;
}


float
LQFastAnalysis::frRealTau() {

  size_t ntau= _vc->getSize("tauPt");

  float pu=1;
  float pd=1;

  for(size_t it=0;it<ntau;it++) {
    if( !_au->simpleCut( _vc->getF("tauPt",it)>50 ) ) continue;
    tmpJet_.SetPtEtaPhiM( _vc->getF("tauPt",it), 
			  _vc->getF("tauEta",it),
			  _vc->getF("tauPhi",it), 1.7 );
  
    float dRel = elL_.DeltaR( tmpJet_ );
    if(!_au->simpleCut<double>(dRel,0.5,">") ) continue;
    if(!_au->simpleCut(_vc->getUL("tauVtx",it)==_vc->getUL("lepVtx",elIdx_) ) ) continue;
    if(!_au->simpleCut<int>(_vc->getI("lepCharge",elIdx_)*_vc->getI("tauCharge",it),-1,"=") ) continue;

    float fr=_dbm->getDBValue("tauFR", min(_vc->getF("tauPt",it),(float)199.) );
    pd *= 1-fr;

    bool isMCTau=isGenParticle( &tmpJet_, 15 );
    //cout<<" ===> "<<pd<<"   "<<isMCTau<<"   "<<fr<<"   "<<pu<<endl;
    if(isMCTau) pu *=1-fr;
    
  }

  return (1-pu)/pd;
}

void
LQFastAnalysis::fillFRShape(vector<TLorentzVector> aitaus) {

  float M11,M12;
  M11 = (elL_ +jetBL_).M();
  M12 = (elL_ + ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_)  ).M();


  float p0=1;

  for(size_t i=0;i<aitaus.size();i++) {
    if(aitaus[i].Pt() < 50 ) continue;
    float fr=_dbm->getDBValue("tauFR", min( aitaus[i].Pt(),(Double_t)199.) );
    p0 *=(1-fr);
  }

  for(size_t i=0;i<aitaus.size();i++) {

    if(aitaus[i].Pt() < 50 ) continue;
    
    float fr=_dbm->getDBValue("tauFR", min( aitaus[i].Pt(),(Double_t)199.) );
    float p=1;
    for(size_t j=0;j<i;j++) {
      if(aitaus[j].Pt() < 50 ) continue;
      float fr2=_dbm->getDBValue("tauFR", min( aitaus[j].Pt(),(Double_t)199.) );

      p*=(1-fr2);
      
    }

    // float w = fr*p/p0;
    float ST = elL_.Pt() + aitaus[i].Pt() + jetBL_.Pt() + ((jetLIdx_!=jetBLIdx_)?jetL_.Pt():jetSL_.Pt() ) ;

    float tM21 = (aitaus[i]+jetBL_).M();
    float tM22 = (aitaus[i]+ ((jetLIdx_!=jetBLIdx_)?jetL_:jetSL_) ).M();
    float tm1=0;
    if (fabs(M11-tM22) < fabs(M12-tM21)) {
      tm1 = tM22;
    }else{
      tm1 = tM21;
    }
    if(_au->simpleCut( tm1 > 250 ) )
      fill("STbjetFR", ST, _w/aitaus.size() );
  }
  
}


void
LQFastAnalysis::metXYCorrection() {
  float corX,corY;

  if(!_isData) {
    corX = 0.162861 - 0.0238517*nGoodVtx_; 
    corY = 0.360860 - 0.130335*nGoodVtx_;
  }
  else {
    corX = 0.0483642 + 0.248870*nGoodVtx_; 
    corY = -0.150135 - 0.0827917*nGoodVtx_;
  }

  TVector2 corMET(0,0);
  corMET.Set( met_.X() - corX, met_.Y() -corY );
  
  met_ = corMET;

}


void
LQFastAnalysis::jesUnc(TLorentzVector& jet, size_t idx, int var) {
  
 
  if(var==-1) { //down variation
    jet *= (1-_vc->getF("jetUnc",idx) );
  }
  if(var==1) { //up variation
    jet *= (1+_vc->getF("jetUnc",idx) );
  }
  
}


void
LQFastAnalysis::jerUnc(TLorentzVector& jet, size_t idx, int var) {
  
  if( _vc->getF("jetPtGen",idx)==-100) return;
 
  float c = 1.;
  float eta = (_ie!=-1)?fabs(jet.Eta()):0.;
  if (eta <= 0.5)
    c = 1.052;
  else if (eta >0.5 && eta <=1.1)
    c = 1.057;
  else if (eta >1.1 && eta <=1.7)
    c = 1.096;
  else if (eta >1.7 && eta <=2.3)
    c = 1.134;
  else 
    c = 1.288;

  float f=0;
  if(var==-1) { //down variation
    if(jet.Pt()!=0)
      f= max(_vc->getF("jetPtGen",idx) - (1.-(c-1.))*(jet.Pt()-_vc->getF("jetPtGen",idx)),0.)/jet.Pt();
    if(f!=0)
      jet*= f;
    else
      jet.SetPtEtaPhiM(0.0001,0,0,0);
  }
  if(var==1) { //up variation
    if(jet.Pt()!=0)
      f= max(_vc->getF("jetPtGen",idx) + c*(jet.Pt()-_vc->getF("jetPtGen",idx)),0.)/jet.Pt();
    if(f!=0)
      jet*= f;
    else
      jet.SetPtEtaPhiM(0.0001,0,0,0);
  } 
  
}



void
LQFastAnalysis::tesUnc(TLorentzVector& tau, int var) {

  if(var==-1) { //down variation
    tau *= 1 - 0.03;
  }
  if(var==1) { //up variation
    tau *= 1 + 0.03;
  }
}



void
LQFastAnalysis::terUnc(TLorentzVector& tau, size_t idx, int var) {
  
  float c = 0.1;
  float ptS=1;

  if( _vc->getF("tauPtGen",idx)==-1000) return;
  //if( _vc->getF("tauPtJet",idx)==-100) return;

  //cout<<tau.Pt()<<"  "<<_vc->getF("tauPtGen",idx)<<"  "<<_vc->getF("tauPtJet",idx)<<endl;

  // if(var==-1) { //down variation
  //   ptS = max( _vc->getF("tauPtGen",idx) - c*(_vc->getF("tauPtJet",idx)-_vc->getF("tauPtGen",idx) ), (float)0. );
  //   cout<<ptS<<" -> "<<ptS/ _vc->getF("tauPtJet",idx)<<endl;
  //   tau*= ptS/ _vc->getF("tauPtJet",idx);
  // }
  // if(var==1) { //up variation
  //   ptS = max( _vc->getF("tauPtGen",idx) + c*(_vc->getF("tauPtJet",idx)-_vc->getF("tauPtGen",idx)), (float)0. );
  //   tau*= ptS/_vc->getF("tauPtJet",idx);
  // }
  
  if(var==-1) { //down variation
    ptS = max( _vc->getF("tauPtGen",idx) + (1-c)*(tau.Pt()-_vc->getF("tauPtGen",idx) ), 0. );
    if(ptS!=0)
      tau*= ptS/ tau.Pt();
    else
      tau.SetPtEtaPhiM(0.0001,0,0,0);
  }
  if(var==1) { //up variation
    ptS = max( _vc->getF("tauPtGen",idx) + (1+c)*(tau.Pt()-_vc->getF("tauPtGen",idx)), 0. );
    if(ptS!=0)
      tau*= ptS/tau.Pt();
    else
      tau.SetPtEtaPhiM(0.0001,0,0,0);
  }
 
}



