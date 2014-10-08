#include "src/METAnalysis.hh"

using namespace std;

ClassImp(METAnalysis)

METAnalysis::METAnalysis():
UseTree() {

  //loadPUWeights();

  //name in the ntuple, name in the analysis, postfix if needed
  //addMET("pat_patType1CorrectedPFMet","pfT1Phi","Smeared");
  //addMET("pat_patType1CorrectedPFMet","pfT1Phi","NoSmear");
  
  //addMET("genMET","pfT1Phi","");

  // addMET("pat_patPFMet","pfPhi","Smeared");

  // // addMET("pat_patPFMetMVA","mva","Smeared");
  // addMET("pat_patPFMetMVA","mvaPhi","Smeared");
  
  // // addMET("pat_patPFMetNoPileUp","noPU","Smeared");
  // addMET("pat_patPFMetNoPileUp","noPUPhi","Smeared");
  
  // addMET("pat_patPFMetMVA","mvaUnityPhi","UnitySmeared");

  //addMET("Calo_caloType1CorrectedMet","caloPhi","");

  //only one call of the function to avoid confusion
  
  // addUnc("JetEnDown", "JES", "Do");
  // addUnc("JetEnUp", "JES", "Up");
  // // addUnc("JetResDown", "JER", "Do");
  // // addUnc("JetResUp", "JER", "Up");
  // // addUnc("MuonEnDown", "MES", "Do"); 
  // // addUnc("MuonEnUp", "MES", "Up"); 
  // addUnc("UnclusteredEnDown", "UES", "Do");
  // addUnc("UnclusteredEnUp", "UES", "Up");

  vector<string> uncs;
  for(_itUM=_uncMap.begin();_itUM!=_uncMap.end();_itUM++)
    uncs.push_back( _itUM->second.first+_itUM->second.second );

  _metTools =new METFunctions("PF",uncs);

  
  double binsQt[18]={0,5,10,15,20,25,30,40,60,85,100,120,145,165,200,250,300,400};
  vector<double> binsSEt;
  vector<double> binsQT;
  //double binNVtx[40];

  for(int i=0;i<30;i++) {
    binsSEt.push_back(i*100);

    if(i<18)
      binsQT.push_back(binsQt[i]);
    //binsNVtx[i] = i;
  }
  

  _metTools->configureBinning(binsQT, binsSEt);//, binsNVtx);

  _dbm->loadDb("ZeeW","ZEMPTW.root","weights");
  _dbm->loadDb("qTGZW","gamZW.db");

  _dbm->loadDb("puDR1","weightsPURD1.root","pileup");
  _dbm->loadDb("puS10","puWeightsLQ.root","pileup");

  _dbm->loadDb("puWUncUp","puWeightsUnc.root","up");
  _dbm->loadDb("puWUncDo","puWeightsUnc.root","down");

  _dbm->loadDb("elLeak","elLeakage.db");

  _fillPlots=true;

}

METAnalysis::~METAnalysis() {

}


void
METAnalysis::resoStudy(string metType) {
  _fillPlots=false;

  //clear mets and fill again
  _mets.clear();

  string name="pat_patType1CorrectedPFMet";
  if(metType=="noPU")
    name = "pat_patPFMetNoPileUp";
  if(metType=="raw")
    name = "pat_patMETsPF";
  if(metType=="mva")
    name = "pat_patPFMetMVA";
  if(metType=="gen")
    name = "genMET";
  if(metType=="calo") {
    name = "Calo_caloType1CorrectedMet";
    addMET(name,metType+"Phi","");
  }  
  else
    //addMET(name,metType+"Phi","EleSmeared");
    addMET(name,metType+"Phi","Smeared");
    //addMET(name,metType,"");

  if(metType=="mvaU") {
    _mets.clear();
    addMET("pat_patPFMetMVA",metType+"UnityPhi","UnitySmeared");
  }

  setFlavor(metType);


}


void
METAnalysis::bookHistograms() {

  if(_fillPlots) {
    for(size_t im=0;im<_mets.size();im++) 
      prepareMETHistos(_mets[im][0]);

    //Z
    _hm->addVariable("qT",400,0,400,"q_{T} [GeV]");
    _hm->addVariable("mass",1600,0,400,"M_{#mu#mu} [GeV]");
    _hm->addVariable( "phi",128,0,TMath::Pi()*2,"#phi(Z) [rad]" );

    _hm->addVariable("ptl1",400,0,200,"p_{T}(#mu_{1}) [GeV]");
    _hm->addVariable("ptl2",400,0,200,"p_{T}(#mu_{2}) [GeV]");
    _hm->addVariable("etal1",120,-3,3,"#eta(#mu_{1}) ");
    _hm->addVariable("etal2",120,-3,3,"#eta(#mu_{2}) ");
    _hm->addVariable("phil1",128,0,TMath::Pi()*2,"#phi(#mu_{1}) [rad]");
    _hm->addVariable("phil2",128,0,TMath::Pi()*2,"#phi(#mu_{2}) [rad]");

    _hm->addVariable("dphill",128,0,TMath::Pi()*2,"#Delta#phi(l,l) [rad]");

    _hm->addVariable("dptl1",400,-10,10,"#Delta p_{T}(#mu_{1}) [GeV]");
    _hm->addVariable("dptl2",400,-10,10,"#Delta p_{T}(#mu_{2}) [GeV]");

    _hm->addVariable( "vjDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]" );
    _hm->addVariable( "vljDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]" );
    _hm->addVariable( "vvljDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]" );

    _hm->addVariable("njet",21,-0.5,20.5,"Jet multiplicity");

    _hm->addVariable("vjpt",400,0,400,"#sum E_{T} [GeV]");
    _hm->addVariable("vljpt",400,0,400,"#sum E_{T} [GeV]");
    _hm->addVariable("vvljpt",400,0,400,"#sum E_{T} [GeV]");

    _hm->addVariable( "metvsvj",128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]","#Delta#phi(#slash{E}_{T},vj) [rad]" );
    _hm->addVariable( "metvsvlj",128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]","#Delta#phi(#slash{E}_{T},vlj) [rad]" );
    _hm->addVariable( "metvsvvlj",128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]","#Delta#phi(#slash{E}_{T},vvlj) [rad]" );
    _hm->addVariable( "metvsunc",128,0,TMath::Pi(),128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},Z) [rad]","#Delta#phi(#slash{E}_{T},unc) [rad]" );
  

    //Vertex
    _hm->addVariable("NVertex",50,0,50,"number of vertices");
 


  //occupancy variables
  _hm->addVariable( "l1PhDEta", 100,-0.5,0.5, "#Delta#eta(l1, ph)");
  _hm->addVariable( "l2PhDEta", 100,-0.5,0.5, "#Delta#eta(l2, ph)");
  _hm->addVariable( "l1ChDEta", 100,-0.5,0.5, "#Delta#eta(l1, ch)");
  _hm->addVariable( "l2ChDEta", 100,-0.5,0.5, "#Delta#eta(l2, ch)");
  _hm->addVariable( "l1NhDEta", 100,-0.5,0.5, "#Delta#eta(l1, nh)");
  _hm->addVariable( "l2NhDEta", 100,-0.5,0.5, "#Delta#eta(l2, nh)");

  _hm->addVariable( "l1PhDPhi", 100,-0.5,0.5, "#Delta#phi(l1, ph)");
  _hm->addVariable( "l2PhDPhi", 100,-0.5,0.5, "#Delta#phi(l2, ph)");
  _hm->addVariable( "l1ChDPhi", 100,-0.5,0.5, "#Delta#phi(l1, ch)");
  _hm->addVariable( "l2ChDPhi", 100,-0.5,0.5, "#Delta#phi(l2, ch)");
  _hm->addVariable( "l1NhDPhi", 100,-0.5,0.5, "#Delta#phi(l1, nh)");
  _hm->addVariable( "l2NhDPhi", 100,-0.5,0.5, "#Delta#phi(l2, nh)");

  _hm->addVariable( "l1Ph2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l1, ph)","#Delta#phi(l1, ph)" );
  _hm->addVariable( "l2Ph2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l2, ph)","#Delta#phi(l2, ph)" );
  _hm->addVariable( "l1Ch2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l1, ch)","#Delta#phi(l1, ch)" );
  _hm->addVariable( "l2Ch2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l2, ch)","#Delta#phi(l2, ch)" );
  _hm->addVariable( "l1Nh2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l1, nh)","#Delta#phi(l1, nh)" );
  _hm->addVariable( "l2Nh2D",100,-0.5,0.5,100,-0.5,0.5,"#Delta#eta(l2, nh)","#Delta#phi(l2, nh)" );
  
  _hm->addVariable( "l1NPh", 50,0,50, "N_{ph} (l1)");
  _hm->addVariable( "l2NPh", 50,0,50, "N_{ph} (l2)");
  _hm->addVariable( "l1NCh", 50,0,50, "N_{ch} (l1)");
  _hm->addVariable( "l2NCh", 50,0,50, "N_{ch} (l2)");
  _hm->addVariable( "l1NNh", 50,0,50, "N_{nh} (l1)");
  _hm->addVariable( "l2NNh", 50,0,50, "N_{nh} (l2)");

  _hm->addVariable( "l1PhPt", 500,0,50, "p_{T}(ph) (l1)");
  _hm->addVariable( "l2PhPt", 400,0,200, "p_{T}(ph) (l2)",true);
  _hm->addVariable( "l1NhPt", 400,0,200, "p_{T}(nh) (l1)",true);
  _hm->addVariable( "l2NhPt", 400,0,200, "p_{T}(nh) (l2)",true);
  _hm->addVariable( "l1ChPt", 500,0,50, "p_{T}(ch) (l1)");
  _hm->addVariable( "l2ChPt", 500,0,50, "p_{T}(ch) (l2)");

  _hm->addVariable( "footPrintEB", 400,0,200, "p_{T} (l)",true);
  _hm->addVariable( "footPrintEE", 400,0,200, "p_{T} (l)",true);

  _hm->addVariable( "l1NhDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(nh) (l1)","#Delta#eta(l1, nh)" );
  _hm->addVariable( "l1NhDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(nh) (l1)","#Delta#phi(l1, nh)" );
  _hm->addVariable( "l2NhDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(nh) (l2)","#Delta#eta(l2, nh)" );
  _hm->addVariable( "l2NhDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(nh) (l2)","#Delta#phi(l2, nh)" );

  _hm->addVariable( "l1ChDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(ch) (l1)","#Delta#eta(l1, ch)" );
  _hm->addVariable( "l1ChDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(ch) (l1)","#Delta#phi(l1, ch)" );
  _hm->addVariable( "l2ChDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(ch) (l2)","#Delta#eta(l2, ch)" );
  _hm->addVariable( "l2ChDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(ch) (l2)","#Delta#phi(l2, ch)" );

  _hm->addVariable( "l1PhDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(ph) (l1)","#Delta#eta(l1, ph)" );
  _hm->addVariable( "l1PhDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(ph) (l1)","#Delta#phi(l1, ph)" );
  _hm->addVariable( "l2PhDEtaPt",100,0,20,100,-0.5,0.5,"p_{T}(ph) (l2)","#Delta#eta(l2, ph)" );
  _hm->addVariable( "l2PhDPhiPt",100,0,20,100,-0.5,0.5,"p_{T}(ph) (l2)","#Delta#phi(l2, ph)" );

  }
 
}


void
METAnalysis::prepareMETHistos(string name) {
  
  _hm->addVariable( name+"MET",400,0,400,"#slash{E}_{T} [GeV]" );
  _hm->addVariable( name+"Phi",128,0,TMath::Pi()*2,"#phi(#slash{E}_{T}) [rad]" );
  _hm->addVariable( name+"DPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},q_{T}) [rad]" );
  _hm->addVariable( name+"X",400,-200,200,"#slash{E}_{T,X} [GeV]" );
  _hm->addVariable( name+"Y",400,-200,200,"#slash{E}_{T,Y} [GeV]" );

  _hm->addVariable( name+"DPhil1",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},l1) [rad]" );
  _hm->addVariable( name+"DPhil2",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},l2) [rad]" );
  
  _hm->addVariable( name+"vjDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},vj) [rad]" );
  _hm->addVariable( name+"vljDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},vlj) [rad]" );
  _hm->addVariable( name+"vvljDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},vvlj) [rad]" );

  _hm->addVariable( name+"METpara",800,-400,400,"#slash{E}_{T,||} [GeV]" );
  _hm->addVariable( name+"METperp",800,-400,400,"#slash{E}_{T,#perp}   [GeV]" );

  _hm->addVariable( name+"SumEt",1000,0,2000,"#sum E_{T} [GeV]" );
 
  _hm->addVariable( name+"DMET",400,0,400,"(#slash{E}_{T} - #slash{E}_{T,gen}) [GeV]" );
  _hm->addVariable( name+"UvsQt",400,0,400,"qT",true );
  // _hm->addVariable( "jetvsQt",400,0,400,"qT",true );
  // _hm->addVariable( "uncPt",400,0,400,"unc pT" );
  // _hm->addVariable( "uncDPhi",128,0,3.1415,"#Delta#phi(q_{T},unc) [rad] " );

  // _hm->addVariable( "uncPhPt",400,0,400,"unc pT" );
  // _hm->addVariable( "uncPhDPhi",128,0,3.1415,"#Delta#phi(q_{T},unc) [rad] " );
  // _hm->addVariable( "uncChPt",400,0,400,"unc pT" );
  // _hm->addVariable( "uncChDPhi",128,0,3.1415,"#Delta#phi(q_{T},unc) [rad] " );
  // _hm->addVariable( "uncNhPt",400,0,400,"unc pT" );
  // _hm->addVariable( "uncNhDPhi",128,0,3.1415,"#Delta#phi(q_{T},unc) [rad] " );

  // _hm->addVariable( "uncMETDPhi",128,0,3.1415,"#Delta#phi(#slash{E}_{T},unc) [rad] " );

  _hm->addVariable( name+"Upara",800,-600,200,"u_{||} [GeV]" );
  _hm->addVariable( name+"Uperp",800,-400,400,"u_{#perp}   [GeV]" );
  _hm->addVariable( name+"redUpara",800,-400,400,"u_{||}+q_{T} [GeV]" );
  
  _hm->addVariable( name+"UDPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{u}_{T},q_{T}) [rad]" );
 _hm->addVariable( name+"UDPhil1",128,0,TMath::Pi(),"#Delta#phi(#slash{u}_{T},l1) [rad]" );
 _hm->addVariable( name+"UDPhil2",128,0,TMath::Pi(),"#Delta#phi(#slash{u}_{T},l2) [rad]" );

  _hm->addVariable( name+"TMass1",400,0,400,"M_{T}(l1) [GeV]" );
  _hm->addVariable( name+"TMass2",400,0,400,"M_{T}(l2) [GeV]" );

  _hm->addVariable( name+"j1DPhi",128,0,TMath::Pi(),"#Delta#phi(#slash{E}_{T},jet) [rad]" );

  _hm->addVariable( name+"MT2",400,0,400,"M_{T2} [GeV]" );

}


void
METAnalysis::modifyWeight() {

  if(!_isData && _ids!="Mu") {
    if(_ids=="Z(ll)+jets") 
      _w *= _dbm->getDBValue("puDR1", _vc->getF("trueNint") );
    else
      _w *= _dbm->getDBValue("puS10", _vc->getF("trueNint") );
  }

   // if(_ids=="Mu") {
   //   _w *= 1./196544.000;
   // }
   // else
   //   _w *= 1./137201.00;
    //_w *= _puweights->GetBinContent( _puweights->GetXaxis()->FindBin( _vc->getF("trueNint"))  );
 
}


void
METAnalysis::analyze() {

  //nVtx selection, I don't store statistic informations
  // a simpel if may not work properly since the program 
  //loop one time before reading the tree to identify which
  // variables are needed
  
  if(!_au->simpleCut<int>( _vc->getI("nVtx"),1,">=")) return;
  //if(!_au->simpleCut<int>( _vc->getI("nVtx"),20,"<")) return;

  //if(!mif(_vc->getI("njet")==1) ) return;

  //pdgId
  bool pdgId1 = fabs(_vc->getI("pdgId_m1"))==13;
  bool pdgId2 = fabs(_vc->getI("pdgId_m2"))==13;
  if(!_au->makeCut(pdgId1 && pdgId2, _ids, "lepton pdgId sel",_w)) return;
   
  //leptons
  bool pt1 = _vc->getF("pt_m1")>20;
  bool pt2 = _vc->getF("pt_m2")>20;

  //first selection, keep the statistic information for later.
  if(!_au->makeCut(pt1 && pt2, _ids, "lepton pt sel",_w)) return;

  // //leptons
  bool eta1 = fabs(_vc->getF("eta_m1"))>1.5;
  bool eta2 = fabs(_vc->getF("eta_m2"))>1.5;
  //if(!_au->makeCut( !(eta1 && eta2), _ids, "lepton eta sel",_w)) return;

  _l1.SetPtEtaPhiM(_vc->getF("pt_m1"), _vc->getF("eta_m1"), _vc->getF("phi_m1"), 0);
  _l2.SetPtEtaPhiM(_vc->getF("pt_m2"), _vc->getF("eta_m2"), _vc->getF("phi_m2"), 0);
  // if(_ids=="Mu") {
  //   // _l1.SetPtEtaPhiM(_vc->getF("pt_m1"), _vc->getF("eta_m1"), _vc->getF("phi_m1"), 0);
  //   // _l2.SetPtEtaPhiM(_vc->getF("pt_m2"), _vc->getF("eta_m2"), _vc->getF("phi_m2"), 0);
  //   if(!mif(fabs(_vc->getI("pdgId_m1"))==13)) return;
  // }
  // else {
  //   // _l1.SetPtEtaPhiM(_vc->getF("pt_cm1"), _vc->getF("eta_cm1"), _vc->getF("phi_cm1"), 0);
  //   // _l2.SetPtEtaPhiM(_vc->getF("pt_cm2"), _vc->getF("eta_cm2"), _vc->getF("phi_cm2"), 0);
  //   if(!mif(fabs(_vc->getI("pdgId_m1"))==11)) return;
  //   //if(!mif(_vc->getF("fppt_m1")!=0 && _vc->getF("fppt_m1")!=0 ) ) return;
  // }
  //if(_ids=="Mu") {
  if(!_isData) {
    _l1 *= (fabs(_vc->getF("eta_m1"))>1.3)?0.998:0.999;//1.003/1.001
    _l2 *= (fabs(_vc->getF("eta_m2"))>1.3)?0.998:0.999;
      
    _l1 *= rnd.Gaus(1,(fabs(_vc->getF("eta_m1"))<1.3)?0.025:0.025); 
    _l2 *= rnd.Gaus(1,(fabs(_vc->getF("eta_m2"))<1.3)?0.025:0.025);
    
  }
  //   //_w *= _dbm->getDBValue("ZeeW", (_l1+_l2).Pt() );
    
  // }
  // else {
  //   _l1 *= (fabs(_vc->getF("eta_m1"))>1.5)?0.987:0.998;
  //   _l2 *= (fabs(_vc->getF("eta_m2"))>1.5)?0.987:0.998;
  // }

    //if(!_au->makeCut<float>( (_l1+_l2).Pt(), 10, "<", _ids, "qT sel", _w)) return;

  //another selection, still keeping the stat information
  if(!_au->makeCut<float>( _vc->getF("massZ"), 60, "[]", _ids, "mass sel", _w, 120)) return; 

  //if(!_au->makeCut<int>( _vc->getI("nbtag"), 2, ">=", _ids, "bjet", _w)) return; 



  //now go through the module used to compute the resolution and response
  for(size_t im=0;im<_mets.size();im++)
    fillModuleMET( _mets[im] );

  //if(!_au->makeCut<float>( _metpara, 10, "[]", _ids, "large unc ", _w, 70 ) ) return;

  if(_fillPlots) {
    fill("qT", (_l1+_l2).Pt(), _w );
    fill("mass", (_l1+_l2).M(), _w );
    fill("ptl1", _vc->getF("pt_m1"), _w );
    fill("ptl2", _vc->getF("pt_m2"), _w );
    fill("etal1", _vc->getF("eta_m1"), _w );
    fill("etal2", _vc->getF("eta_m2"), _w );
    fill("phil1", _vc->getF("phi_m1"), _w );
    fill("phil2", _vc->getF("phi_m2"), _w );
    fill("njet", _vc->getI("njet"), _w );
    fill("NVertex", _vc->getI("nVtx"), _w );
    if(!_isData)
      fillUnc("NVertex", "XSect", _vc->getI("nVtx"), _w,
	      _w*_dbm->getDBValue("puWUncUp", _vc->getF("trueNint") ),
	      _w*_dbm->getDBValue("puWUncDo", _vc->getF("trueNint") ) );
    
    fill("dphill", fabs( KineUtils::dPhi(_vc->getF("phi_m1"),_vc->getF("phi_m2"))) , _w );

    fillUnc("mass","MES", (_l1*0.998+_l2*0.998).M(), _w, "Do" );
    fillUnc("mass","MES", (_l1*1.002+_l2*1.002).M(), _w, "Up" );

    fillUnc("qT","MES", (_l1*0.998+_l2*0.998).Pt(), _w, "Do" );
    fillUnc("qT","MES", (_l1*0.998+_l2*0.998).Pt(), _w, "Up" );
    
    // fill("dptl1", (_vc->getF("pt_cm1")-_vc->getF("pt_m1")), _w );
    // fill("dptl2", (_vc->getF("pt_cm2")-_vc->getF("pt_m2")), _w );

    fill("phi",  TVector2::Phi_0_2pi((_l1+_l2).Phi()), _w );
      
    // TVector2 vj,vlj,vvlj;
    // vj.SetMagPhi(1, _vc->getF("vj_phi") );
    // vlj.SetMagPhi(1, _vc->getF("vlj_phi") );
    // vvlj.SetMagPhi(1, _vc->getF("vvlj_phi") );
    

    fill("vjpt", _vc->getF("vj_pt"), _w );
    fill("vljpt", _vc->getF("vlj_pt"), _w );
    fill("vvljpt", _vc->getF("vvlj_pt"), _w );
    
      // fill("vjDPhi", fabs(vj.DeltaPhi( (_l1+_l2).Vect().XYvector() )), _w );
    // fill("vljDPhi", fabs(vlj.DeltaPhi( (_l1+_l2).Vect().XYvector() )), _w );
    // fill("vvljDPhi", fabs(vvlj.DeltaPhi( (_l1+_l2).Vect().XYvector() )), _w );

    // TVector2 fp1 = footPrintEl("1",_l1);
    // TVector2 fp2 = footPrintEl("2",_l2);
    // if(fp1.Mod()<0)
    //   cout<<fp1.Mod()<<"  "<<fp2.Mod()<<endl;
    
    // if( fabs(_l1.Eta())<1.5)
    //   fill("footPrintEB",  _l1.Pt(), fp1.Mod(), 1 ); 
    // else
    //   fill("footPrintEE",  _l1.Pt(), fp1.Mod(), 1 ); 
    // if( fabs(_l2.Eta())<1.5)
    //   fill("footPrintEB",  _l2.Pt(), fp2.Mod(), 1 );
    // else    
    //   fill("footPrintEE",  _l2.Pt(), fp2.Mod(), 1 );
    //fill("footPrint", _upara, _w );
    //fill("footPrint", (_ids=="Mu")?_vc->getF("pt_m2"):_vc->getF("fppt_m2"), _w );

    //occupancyVars();
  }


}


void METAnalysis::fillPlots(string met) {

  fill( met+"MET", _met.Mod() , _w );  
  fill( met+"Phi", _met.Phi() , _w );  
  fill( met+"DPhi", fabs(_met.DeltaPhi(_qT)) , _w );  

  fill( met+"DPhil1", fabs(_met.DeltaPhi(_l1.Vect().XYvector())) , _w );  
  fill( met+"DPhil2", fabs(_met.DeltaPhi(_l2.Vect().XYvector())) , _w );  
  fill( met+"UvsQt",  _qT.Mod() ,_upara+_qT.Mod() , _w );
 
  // TVector2 vj,vlj,vvlj,unc(0,0);
  // TVector2 uncPh(0,0), uncCh,uncNh;
  // vj.SetMagPhi(_vc->getF("vj_pt"), _vc->getF("vj_phi") );
  // vlj.SetMagPhi(_vc->getF("vlj_pt"), _vc->getF("vlj_phi") );
  // vvlj.SetMagPhi(_vc->getF("vvlj_pt"), _vc->getF("vvlj_phi") );
  // unc.SetMagPhi(_vc->getF("unc_pt"), _vc->getF("unc_phi") );
  // uncPh.SetMagPhi(_vc->getF("ph_pt"), _vc->getF("ph_phi") );
  // uncNh.SetMagPhi(_vc->getF("nh_pt"), _vc->getF("nh_phi") );
  // uncCh.SetMagPhi(_vc->getF("ch_pt"), _vc->getF("ch_phi") );

  //unc -= _met+_qT+vj+vlj+vvlj;

  //fill( "jetvsQt",  _qT.Mod() ,  KineUtils::dPhi( _l1.Phi() ,(_met-_qT-vj-vlj-vvlj).Phi()) , _w );
  //TVector2 d(0,0);
  // fill( "uncPt", unc.Mod(), _w );
  // fill( "uncDPhi", fabs( KineUtils::dPhi(_qT.Phi(),unc.Phi())), _w );
  // fill( "uncMETDPhi", fabs( KineUtils::dPhi(_met.Phi(),unc.Phi())), _w );
  

  // fill( "uncPhPt", uncPh.Mod(), _w );
  // fill( "uncPhDPhi", fabs( KineUtils::dPhi(_qT.Phi(),uncPh.Phi())), _w );
  // fill( "uncChPt", uncCh.Mod(), _w );
  // fill( "uncChDPhi", fabs( KineUtils::dPhi(_qT.Phi(),uncCh.Phi())), _w );
  // fill( "uncNhPt", uncNh.Mod(), _w );
  // fill( "uncNhDPhi", fabs( KineUtils::dPhi(_qT.Phi(),uncNh.Phi())), _w );

  // fill(met+"vjDPhi", fabs(_qT.DeltaPhi(vj)), _w );
  // fill(met+"vljDPhi", fabs(_qT.DeltaPhi(vlj)), _w );
  // fill(met+"vvljDPhi", fabs(_qT.DeltaPhi(vvlj)), _w );

  // fill( "metvsvj", fabs(_met.DeltaPhi(_qT)), fabs(_qT.DeltaPhi(vj)), _w );
  // fill( "metvsvlj", fabs(_met.DeltaPhi(_qT)), fabs(_qT.DeltaPhi(vlj)), _w );
  // fill( "metvsvvlj", fabs(_met.DeltaPhi(_qT)), fabs(_qT.DeltaPhi(vvlj)), _w );
  // fill( "metvsunc", fabs(_met.DeltaPhi(_qT)), fabs(KineUtils::dPhi(_met.Phi(), (_met-_qT-vj-vlj-vvlj).Phi() ) ) , _w );

  fill( met+"X", _met.X() , _w );  
  fill( met+"Y", _met.Y() , _w ); 
  fill( met+"SumEt", _sumEt , _w ); 

  fill( met+"METpara", _metpara , _w );
  fill( met+"METperp", _metperp , _w );

  fill( met+"Upara", _upara , _w );
  fill( met+"Uperp", _uperp , _w );
  fill( met+"redUpara", _upara+_qT.Mod() , _w ); 
 
  fill( met+"UDPhi", _udphi , _w );

  float MT1=sqrt( 2* _vc->getF("pt_m1")*_met.Mod()*(1-cos(KineUtils::dPhi( _vc->getF("phi_m1"),_met.Phi()))));
  float MT2=sqrt( 2* _vc->getF("pt_m2")*_met.Mod()*(1-cos(KineUtils::dPhi( _vc->getF("phi_m2"),_met.Phi()))));

  fill( met+"TMass1", MT1 , _w );  
  fill( met+"TMass2", MT2 , _w );  
  
  // if(mif(_vc->getI("njet")>0) )
  //   fill(met+"j1DPhi", fabs(KineUtils::dPhi(_met.Phi(), _vc->getF("jetPhis",0) ) ), _w );


}

void
METAnalysis::fillMT2(string met) {
  
  //additional cuts
  float dphi =  fabs(KineUtils::dPhi( _vc->getF("phi_j1"),_vc->getF("phi_j2") ) ); 
  if(!_au->simpleCut(dphi > 2.2) ) return;
 

  _j1.SetPtEtaPhiM(_vc->getF("pt_j1"),_vc->getF("eta_j1"),_vc->getF("phi_j1"), 4.67);
  _j2.SetPtEtaPhiM(_vc->getF("pt_j2"),_vc->getF("eta_j2"),_vc->getF("phi_j2"), 4.67);

  if( _j1.Pt()<45 ) return;
  if( _j2.Pt()<45 ) return;
  if( _met.Mod() < 20) return;
  if( _j1.Pt() + _j2.Pt() + _l1.Pt() + _l2.Pt() < 130) return;
  

  TLorentzVector mVa_1 = _l1 + _j1;
  TLorentzVector mVb_1 = _l2 + _j2;
  
  TLorentzVector mVa_2 = _l1 + _j2;
  TLorentzVector mVb_2 = _l2 + _j1;
  
  float mt2_1 = _mT2Fcts.mT2(mVa_1, mVb_1, _met);
  float mt2_2 = _mT2Fcts.mT2(mVa_2, mVb_2, _met);
  
  float mt2 = min( mt2_1, mt2_2 );
  
  fill( met+"MT2", mt2 , _w );  

}

void METAnalysis::fillUncPlots(string met, string unc, string dir ) {

  fillUnc( met+"MET", unc, _metUnc.Mod() , _w, dir );  
  fillUnc( met+"Phi", unc, _metUnc.Phi() , _w, dir );  
  fillUnc( met+"DPhi", unc, fabs(_metUnc.DeltaPhi(_qT)) , _w, dir );  

  fillUnc( met+"X", unc, _metUnc.X() , _w, dir );  
  fillUnc( met+"Y", unc, _metUnc.Y() , _w, dir ); 
  fillUnc( met+"SumEt", unc, _sumEtUnc , _w, dir ); 

  fillUnc( met+"Upara", unc, _uupara , _w, dir );
  fillUnc( met+"Uperp", unc, _uuperp , _w, dir );
  fillUnc( met+"redUpara", unc, _uupara+_qT.Mod() , _w, dir ); 

  float MT1=sqrt( 2* _vc->getF("pt_m1")*_metUnc.Mod()*(1-cos(KineUtils::dPhi( _vc->getF("phi_m1"),_metUnc.Phi()))));
  float MT2=sqrt( 2* _vc->getF("pt_m2")*_metUnc.Mod()*(1-cos(KineUtils::dPhi( _vc->getF("phi_m2"),_metUnc.Phi()))));

  fillUnc( met+"TMass1", unc, MT1 , _w, dir );  
  fillUnc( met+"TMass2", unc, MT2 , _w, dir );  


}


void METAnalysis::fillModuleMET(vector<string> metName) {

 
  string name=metName[1]+metName[2];
  string postfix=metName[2];
  //you may remove that line in you don't have change of postfix
  if(_isData) replaceString(name, "Smeared","NoSmear");
  if(_isData) replaceString(postfix, "Smeared","NoSmear");
   
  // if(_isData) replaceString(name, "NoSmear","EleNoSmear");
  // if(_isData) replaceString(postfix, "NoSmear","EleNoSmear");

  //first, compute useful elements
  _qT.SetMagPhi( (_l1+_l2).Pt(), (_l1+_l2).Phi() );
  _met.SetMagPhi( _vc->getF(name+"__pt"), _vc->getF(name+"__phi") );
  //_met.SetMagPhi( _vc->getF(name+"Pt"), _vc->getF(name+"Phi") );
  _sumEt =0;// _vc->getF(name+"__sumEt") - _l1.Pt() - _l2.Pt();

  float wqt=(_qT.Mod()>100)?_w:0;
  wqt *= _dbm->getDBValue("qTGZW",_qT.Mod() );
  
  //phi correction
  if( (metName[0].find("Phi")!=(size_t)-1) ) {
    _met = phiCorrection(_met, _vc->getI("nVtx"), metName[0]);
    
  }

  _metx = _met.X();
  _mety = _met.Y(); 

  _metpara = (float)(( _qT*_met)/_qT.Mod());;
  TVector2 m = _met.Rotate( TMath::Pi()/2);
  _metperp = (float)(( _qT*m)/_qT.Mod());;

  TVector2 u(0,0);
  u -= (_met +_qT); 
  // if(_ids!="Mu") {
  //   TVector2 qp1(0,0);
  //   TVector2 qp2(0,0);
    
  //   qp1.SetMagPhi( _vc->getF("fppt_m1"), _vc->getF("fpphi_m1") );
  //   qp2.SetMagPhi( _vc->getF("fppt_m2"), _vc->getF("fpphi_m2") );
  //   u.SetMagPhi(0,0);
  //   u -= (_met + qp1+ qp2);
    
  // }
 
  // TVector2 vj,vlj,vvlj;
  // vj.SetMagPhi(_vc->getF("vj_pt"), _vc->getF("vj_phi") );
  // vlj.SetMagPhi(_vc->getF("vlj_pt"), _vc->getF("vlj_phi") );
  // vvlj.SetMagPhi(_vc->getF("vvlj_pt"), _vc->getF("vvlj_phi") );
  // u = vj+vlj+vvlj;

  // //FIXME : electron leakage test
 //  if(_isData) {
 //    // u -= _l1.Vect().XYvector()*_dbm->getDBValue("elLeak", fabs(_l1.Eta()), _l1.Pt() ) 
 //    //   + _l2.Vect().XYvector()*_dbm->getDBValue("elLeak", fabs(_l2.Eta()), _l2.Pt() );
  
 //    //(fabs(_l1.Eta())<1.5)?(0.21233+_l1.Pt()*0.01157):(0.3545+_l1.Pt()*0.00537);
 //    //(fabs(_l2.Eta())<1.5)?(0.21233+_l2.Pt()*0.01157):(0.3545+_l2.Pt()*0.00537);

 //    float c1= getRecoilCor(_l1.Pt(), _l1.Eta(), true, true);
 //    float c2= getRecoilCor(_l2.Pt(), _l2.Eta(), true, true);

 //    TVector2 fp1; fp1.SetMagPhi(c1, _l1.Phi());
 //    TVector2 fp2; fp2.SetMagPhi(c2, _l2.Phi());//= footPrintEl("2",_l2);

 //    // //cout<<c1<<"   "<<c2<<endl;
 //    // fp1 = footPrintEl("1",_l1);
 //    // fp2= footPrintEl("2",_l2);
 //    u -= fp1 + fp2;

 //    // fill("footPrint", _l1.Pt(), fp1, _w ); 
 //    // fill("footPrint", _l2.Pt(), fp2, _w ); 

 //  }
 //  else {
 //    // u -= _l1.Vect().XYvector()*_dbm->getDBValue("elLeak", fabs(_l1.Eta()), _l1.Pt() ) 
 //    //   + _l2.Vect().XYvector()*_dbm->getDBValue("elLeak", fabs(_l2.Eta()), _l2.Pt() );
 // //    TVector2 t,t2;
 // // // //    // // t.SetMagPhi(0.07+0.3,_l1.Phi() );
 // // // //    //    // t2.SetMagPhi(0.07+0.3,_l2.Phi() );

 // //    float c1 = getRecoilCor(_l1.Pt(), _l1.Eta(), true, false);
 // //     float c2 = getRecoilCor(_l2.Pt(), _l2.Eta(), true, false);
 // // // // //    //cout<<c1<<"   "<<c2<<endl;
 // //      t.SetMagPhi(c1,_l1.Phi() );
 // //      t2.SetMagPhi(c2,_l2.Phi() );

 // //      u -= t + t2;

 //      // TVector2 qp1(0,0);
 //      // TVector2 qp2(0,0);
    
 //      // qp1.SetMagPhi( _vc->getF("fppt_m1"), _vc->getF("fpphi_m1") );
 //      // qp2.SetMagPhi( _vc->getF("fppt_m2"), _vc->getF("fpphi_m2") );
 //      // TVector2 v; v.SetMagPhi(0,0);
 //      // v -= (_met + qp1+ qp2);
 //      // cout<<(_l1.Pt()+c1)-_vc->getF("fppt_m1")<<" / "<<(_l2.Pt()+c2)-_vc->getF("fppt_m2")<<" :: "
 //      // 	  << _vc->getF("fppt_m1")<<" / "<<_vc->getF("fppt_m2")
 //      // 	  <<" :: "<<u.Mod()<<" / "<<v.Mod()<<endl;
 //      //_upara = _l1.Pt()+c1-_vc->getF("fppt_m1") ;
 //  }

//   u *= getRespW(_qT.Mod() );
  
  _upara =(float)(( _qT*u)/_qT.Mod());
  _udphi = fabs(u.DeltaPhi( _qT ));
  u = u.Rotate( TMath::Pi()/2);
  _uperp = (float)(( _qT*u)/_qT.Mod());

  // fill( metName[0]+"UDPhil1", fabs(u.DeltaPhi(_l1.Vect().XYvector())) , _w );  
  // fill( metName[0]+"UDPhil2", fabs(u.DeltaPhi(_l2.Vect().XYvector())) , _w );  
  //cout<<_qT.Mod()<<"   "<<_qT.Phi()<<"   "<<_met.Mod()<<"   "<<_met.Phi()<<" --> "<<_upara<<endl;

  //cout<<_upara<<"   "<<_uperp<<endl;

  //if(!_au->makeCut<float>( _upara, 20, "[]", _ids, "large unc ", _w, 25 ) ) return;

  if(_fillPlots) {
    fillPlots( metName[0] );
  
    //fillMT2( metName[0] );
  }

  //uncertainties ===============
  if(mif(!_isData)) {
    
    _upa.clear();
    _upe.clear();
    _urpa.clear();
    _umx.clear();
    _umy.clear();

    TVector2 qT(0,0);

    for(_itUM=_uncMap.begin();_itUM!=_uncMap.end();_itUM++) {
      //cout<<metName[0]<<"   "<<metName[1]+_itUM->first+postfix<<endl;
      _metUnc.SetMagPhi(_vc->getF(metName[1]+_itUM->first+postfix+"__pt"), _vc->getF(metName[1]+_itUM->first+postfix+"__phi"));
      _sumEtUnc = _vc->getF(metName[1]+_itUM->first+postfix+"__sumEt") - _l1.Pt() - _l2.Pt();
      
      //phi correction
      if( (metName[0].find("Phi")!=(size_t)-1) ) {
	_metUnc = phiCorrection(_metUnc, _vc->getI("nVtx"), metName[0]);
      }
      

      //QT correction for MES
      if( _itUM->second.first+_itUM->second.second == "MESDo" ) {
	float pT = ( _l1*(1-((_l1.Pt()>100)?0.05:0.002)) +
		     _l2*(1-((_l2.Pt()>100)?0.05:0.002)) ).Pt();
	qT.SetMagPhi( pT, (_l1+_l2).Phi() );
      }
      else if( _itUM->second.first+_itUM->second.second == "MESUp" ) {
	float pT = ( _l1*(1+((_l1.Pt()>100)?0.05:0.002)) +
		     _l2*(1+((_l2.Pt()>100)?0.05:0.002)) ).Pt();
	qT.SetMagPhi( pT, (_l1+_l2).Phi() );
      }
      else
	qT= _qT;

      TVector2 uu(0,0);
      uu -= (_metUnc + qT ); 
      
      _uupara = (float)(( qT*uu)/qT.Mod());
      uu = uu.Rotate( TMath::Pi()/2);
      _uuperp = (float)(( qT*uu)/qT.Mod());
      
      _umetx = _metUnc.X();
      _umety = _metUnc.Y(); 

      _upa[ _itUM->second.first+_itUM->second.second ] = _uupara;
      _upe[ _itUM->second.first+_itUM->second.second ] = _uuperp;
      _urpa[ _itUM->second.first+_itUM->second.second ] = _uupara + qT.Mod();
      
      _umx[ _itUM->second.first+_itUM->second.second ] = _umetx;
      _umy[ _itUM->second.first+_itUM->second.second ] = _umety;

      //cout<<_qT.Mod()<<"  "<<_l1.Pt()<<"  "<<_l2.Pt()<<"   "<<_upara<<"   "<<_itUM->second.first+_itUM->second.second<<"   "<<_uupara<<endl;

      if(_fillPlots)
	fillUncPlots(metName[0], _itUM->second.first, _itUM->second.second);
    }
  }

  //==============================
  if(!_fillPlots)
    _metTools->fillRFTree( _qT.Mod(), _upara, _uperp, _upara+_qT.Mod(), 
			   _vc->getI("nVtx"), _sumEt, _metx, _mety,
			   _upa,_upe,_urpa, _umx, _umy, _w, wqt, _isData );
  
  
}


void  
METAnalysis::loadPUWeights() {
  string envPath;
  envPath = (string)(getenv ("LUNE"));
  //TFile * file = new TFile( (envPath+"/data/rootFiles/puWeightsLQ.root").c_str(),"READ"); //puWeightsS10
  TFile * file = new TFile( (envPath+"/data/rootFiles/weightsPURD1.root").c_str(),"READ"); //puWeightsS7
  _puweights = (TH1F*)file->Get("pileup");

}

void
METAnalysis::addMET(string tn, string name, string postfix) {
  vector<string> p(3,"");
  p[0] = name;
  p[1] = tn;
  p[2] = postfix;
  _mets.push_back( p );
}



void
METAnalysis::addUnc(string tn, string name, string dir) {
  pair<string,string> p(name,dir);
  _uncMap[ tn ]=p;
}


void
METAnalysis::replaceString(string& str, string a, string b) {

  size_t p1=str.find(a);
  size_t n=a.size();

  if(p1==(size_t)-1) return;
  str.replace(p1,n,b);
}

void
METAnalysis::computeResponse(bool unc, bool fit) {

  _metTools->computeResponse(unc, fit);

}


void
METAnalysis::computeResolution(std::string comp, bool fNV, bool fSEt, bool fit, bool qTCut, bool unc) {

  _metTools->computeResolution(comp,fNV,fSEt,fit,qTCut,unc);
  
}

void
METAnalysis::setFlavor(string flav) {

  _metTools->setFlavor(flav);
  
}

void
METAnalysis::checkFit(bool resp, string comp, float val, bool isData, bool fNV, bool fSEt, bool fit, bool qTCut) {
  _metTools->checkPoint(resp, comp,val, isData, fNV, fSEt, fit, qTCut);
}

// TVector2 
// METAnalysis::phiCorrection(TVector2 met, int Nvtx, string type) {

//   float corX,corY;
  
//   // bool ids=_isData;
//   // if(!ids) _isData=true;

//   //why is it inverted?

//   if(!_isData) { //MC per definition
  
//     if(type.find("mva")!=(size_t)-1) {
//       corX = -0.235531 - 0.0216895*Nvtx; 
//       corY = 0.0628718 - 0.0274277*Nvtx;
//     }
//     else if(type.find("noPU")!=(size_t)-1){
//       corX = 0.188526 - 0.00344137*Nvtx; 
//       corY = 0.0992166 - 0.0885794*Nvtx;
//     }
//     else if(type.find("calo")!=(size_t)-1){
//       corX =  0*Nvtx; 
//       corY =  0*Nvtx;
//     }
//     else {
//       corX = 0.162861 - 0.0238517*Nvtx; 
//       corY = 0.360860 - 0.130335*Nvtx;
//     }
//   }
//   else {
   

//    if(type.find("mva")!=(size_t)-1) {
//       corX = 0.0920426 + 0.0524015*Nvtx; 
//       corY = 0.107972 - 0.00567229*Nvtx;
//     }
//     else if(type.find("noPU")!=(size_t)-1){
//       corX = 0.0991346 + 0.119915*Nvtx; 
//       corY = 0.331467 - 0.0596704*Nvtx;
//     }
//     else if(type.find("calo")!=(size_t)-1){
//       corX = 0*Nvtx; 
//       corY = 0*Nvtx;
//     }
//     else {
//       corX = 0.0483642 + 0.248870*Nvtx; 
//       corY = -0.150135 - 0.0827917*Nvtx;
//     }

//   }

//   //if(!ids) _isData=false;

//   TVector2 corMET(0,0);
//   corMET.Set( met.X() - corX, met.Y() -corY );


//   return corMET;

// }



//new corrections
TVector2 
METAnalysis::phiCorrection(TVector2 met, int Nvtx, string type) {

  float corX,corY;
  
  // bool ids=_isData;
  // if(!ids) _isData=true;

  //why is it inverted?

  if(!_isData) { //MC per definition
  
    if(type.find("mvaUnity")!=(size_t)-1) {
      corX = -0.137292 - 0.0209885*Nvtx; 
      corY = 0.0630793 - 0.0500763*Nvtx;
    }
    else if(type.find("mva")!=(size_t)-1) {
      corX = -0.162116 - 0.0124185*Nvtx; 
      corY = 0.0139111 - 0.0302930*Nvtx;
    }
    else if(type.find("noPU")!=(size_t)-1){
      corX = 0.0976287 + 0.00963174*Nvtx; 
      corY = -0.0565974 - 0.106818*Nvtx;
    }
    else if(type.find("calo")!=(size_t)-1){
      corX =  0*Nvtx; 
      corY =  0*Nvtx;
    }
    else { //pf
      corX = 0.0982473 + 0.0137847*Nvtx;
      corY = 0.180283 - 0.134661*Nvtx;
    }
  }
  else {
   

    if(type.find("mvaUnity")!=(size_t)-1) {
      corX = 0.146641 + 0.0648981*Nvtx; 
      corY = -0.0923696 - 0.0186981*Nvtx;
    }
    else if(type.find("mva")!=(size_t)-1) {
      corX = 0.137644 + 0.0502850*Nvtx; 
      corY = -0.0936122 - 0.00695024*Nvtx;
    }
    else if(type.find("noPU")!=(size_t)-1){
      corX = 0.0414566 + 0.136941*Nvtx; 
      corY = -0.277215 - 0.0651703*Nvtx;
    }
    else if(type.find("calo")!=(size_t)-1){
      corX = 0*Nvtx; 
      corY = 0*Nvtx;
    }
    else { //pf
      corX = 0.0798741 + 0.239297*Nvtx; 
      corY = -0.137049 - 0.0802186*Nvtx;
    }

  }

  //if(!ids) _isData=false;

  TVector2 corMET(0,0);
  corMET.Set( met.X() - corX, met.Y() -corY );


  return corMET;

}





float
METAnalysis::getRespW(double qt) {

  double binsQt[18]={0,5,10,15,20,25,30,40,60,85,100,120,145,165,200,250,300,400};
  vector<double> binsQT;

  for(int i=0;i<18;i++) {
    binsQT.push_back(binsQt[i]);
  }
  int b = StatUtils::findBin(qt, binsQT);

  if(b>16) return 1;

  float w[17]={1.05105,
	       1.03485,
	       1.02626,
	       1.02579,
	       1.03074,
	       1.02471,
	       1.02276,
	       1.02141,
	       1.02495,
	       1.02713,
	       1.0235,
	       1.01233,
	       1.02079,
	       1.01762,
	       1.01831,
	       1.01743,
	       1.01229};


  return w[b];

}



void
METAnalysis::occupancyVars() {


  //photons ================================================================
  size_t nPh1=_vc->getSize("oph1_pt");
  size_t nPh2=_vc->getSize("oph2_pt");

  int nph1=0,nph2=0;
  
  vector<float> phpt1;
  vector<float> pheta1;
  vector<float> phphi1;
  vector<float> chpt1;
  vector<float> cheta1;
  vector<float> chphi1;
  vector<float> nhpt1;
  vector<float> nheta1;
  vector<float> nhphi1;

  vector<float> phpt2;
  vector<float> pheta2;
  vector<float> phphi2;
  vector<float> chpt2;
  vector<float> cheta2;
  vector<float> chphi2;
  vector<float> nhpt2;
  vector<float> nheta2;
  vector<float> nhphi2;

  float sumPt=0, sumPtO=0;

  for(size_t ip=0;ip<nPh1;ip++) {
    
    //if(!genCleaning("oph1",ip,"og1") ) continue;    
    if(!occCleaning("oph1",ip,_l1) ) continue;
    

    fill( "l1PhDEta", _l1.Eta()-_vc->getF("oph1_eta",ip), _w*_vc->getF("oph1_pt",ip) );
    fill( "l1PhDPhi", KineUtils::dPhi(_l1.Phi(), _vc->getF("oph1_phi",ip) ), _w*_vc->getF("oph1_pt",ip) );
    fill( "l1Ph2D", _l1.Eta()-_vc->getF("oph1_eta",ip), 
	  KineUtils::dPhi(_l1.Phi(), _vc->getF("oph1_phi",ip) ), _w );
   

    fill( "l1PhDEtaPt", _vc->getF("oph1_pt",ip), _l1.Eta()-_vc->getF("oph1_eta",ip) , _w );
    fill( "l1PhDPhiPt", _vc->getF("oph1_pt",ip), KineUtils::dPhi(_l1.Phi(), _vc->getF("oph1_phi",ip) ) , _w );

    if(KineUtils::dR(_l1.Eta(), _vc->getF("oph1_eta",ip), _l1.Phi(), _vc->getF("oph1_phi",ip) )>0.4 && 
       KineUtils::dR(_l1.Eta(), _vc->getF("oph1_eta",ip), _l1.Phi(), _vc->getF("oph1_phi",ip) )<0.7 && nPh1!=0 )
      sumPtO += _vc->getF("oph1_pt",ip);
    if(KineUtils::dR(_l1.Eta(), _vc->getF("oph1_eta",ip), _l1.Phi(), _vc->getF("oph1_phi",ip) )<0.15)
      sumPt += _vc->getF("oph1_pt",ip);

    phpt1.push_back( _vc->getF("oph1_pt",ip) );
    pheta1.push_back( _vc->getF("oph1_eta",ip) );
    phphi1.push_back( _vc->getF("oph1_phi",ip) );

    nph1++;
  }
  
  //if(sumPt!=0)
  fill( "l1PhPt", sumPt/*+sumPtO*0.4*0.4/(0.7*0.7-0.4*0.4)*/, _w );
  fill( "l2PhPt", _l1.Pt(), sumPt/*+sumPtO*0.4*0.4/(0.7*0.7-0.4*0.4)*/, _w );

  sumPt=0;
  sumPtO=0;

  for(size_t ip=0;ip<nPh2;ip++) {

    //if(!genCleaning("oph2",ip,"og2") ) continue;    
    if(!occCleaning("oph2",ip,_l2) ) continue;
  

    fill( "l1PhDEta", _l2.Eta()-_vc->getF("oph2_eta",ip), _w*_vc->getF("oph2_pt",ip) );
    fill( "l1PhDPhi", KineUtils::dPhi(_l2.Phi(), _vc->getF("oph2_phi",ip) ), _w*_vc->getF("oph2_pt",ip) );
    fill( "l1Ph2D", _l2.Eta()-_vc->getF("oph2_eta",ip), 
	  KineUtils::dPhi(_l2.Phi(), _vc->getF("oph2_phi",ip) ), _w );
    //fill( "l1PhPt", _vc->getF("oph2_pt",ip), _w );

    fill( "l1PhDEtaPt", _vc->getF("oph2_pt",ip), _l2.Eta()-_vc->getF("oph2_eta",ip) , _w );
    fill( "l1PhDPhiPt", _vc->getF("oph2_pt",ip), KineUtils::dPhi(_l2.Phi(), _vc->getF("oph2_phi",ip) ) , _w );

    phpt2.push_back( _vc->getF("oph2_pt",ip) );
    pheta2.push_back( _vc->getF("oph2_eta",ip) );
    phphi2.push_back( _vc->getF("oph2_phi",ip) );

    if(KineUtils::dR(_l2.Eta(), _vc->getF("oph2_eta",ip), _l2.Phi(), _vc->getF("oph2_phi",ip) )>0.4 && 
       KineUtils::dR(_l2.Eta(), _vc->getF("oph2_eta",ip), _l2.Phi(), _vc->getF("oph2_phi",ip) )<0.7 && nPh2!=0 )
      sumPtO += _vc->getF("oph2_pt",ip);
    if(KineUtils::dR(_l2.Eta(), _vc->getF("oph2_eta",ip), _l2.Phi(), _vc->getF("oph2_phi",ip) )<0.15)
      sumPt += _vc->getF("oph2_pt",ip);

    nph2++;
  }

  //if(sumPt!=0)
  fill( "l1PhPt", sumPt/*+sumPtO*0.4*0.4/(0.7*0.7-0.4*0.4)*/, _w );
  fill( "l2PhPt", _l2.Pt(), sumPt/*+sumPtO*0.4*0.4/(0.7*0.7-0.4*0.4)*/, _w );

  //fill( "l1NPh", nPh1, _w );
  //fill( "l2NPh", nPh2, _w );
  fill( "l1NPh", nph1, _w );
  fill( "l1NPh", nph2, _w );
  

  //charged ================================================================
  size_t nCh1=_vc->getSize("och1_pt");
  size_t nCh2=_vc->getSize("och2_pt");

  int nch1=0,nch2=0;
 
  for(size_t ip=0;ip<nCh1;ip++) {
    
    if(!occCleaning("och1",ip,_l1) ) continue;
    
    fill( "l1ChDEta", _l1.Eta()-_vc->getF("och1_eta",ip), _w );
    fill( "l1ChDPhi", KineUtils::dPhi(_l1.Phi(), _vc->getF("och1_phi",ip) ), _w );
    fill( "l1Ch2D", _l1.Eta()-_vc->getF("och1_eta",ip), 
	  KineUtils::dPhi(_l1.Phi(), _vc->getF("och1_phi",ip) ), _w );
    fill( "l1ChPt", _vc->getF("och1_pt",ip), _w );

    // if(_vc->getF("och1_pt",ip)>20) 
    //   cout<<KineUtils::dR(_l1.Eta(), _vc->getF("och1_eta",ip), _l1.Phi(), _vc->getF("och1_phi",ip) )<<"  "<<fabs(_l1.Eta()-_vc->getF("och1_eta",ip))
    // 	  <<"   "<<KineUtils::dPhi(_l1.Phi(), _vc->getF("och1_phi",ip) )
    // 	  <<"  -> "<<fabs(_vc->getF("och1_pt",ip)-_l1.Pt())/_l1.Pt()
    // 	  <<" // "<<_l1.Pt()<<"   "<<_vc->getF("och1_pt",ip)<<endl;
  
    fill( "l1ChDEtaPt", _vc->getF("och1_pt",ip), _l1.Eta()-_vc->getF("och1_eta",ip) , _w );
    fill( "l1ChDPhiPt", _vc->getF("och1_pt",ip), KineUtils::dPhi(_l1.Phi(), _vc->getF("och1_phi",ip) ) , _w );

    chpt1.push_back( _vc->getF("och1_pt",ip) );
    cheta1.push_back( _vc->getF("och1_eta",ip) );
    chphi1.push_back( _vc->getF("och1_phi",ip) );

      nch1++;
  }
  
  for(size_t ip=0;ip<nCh2;ip++) {
    
    if(!occCleaning("och2",ip,_l2) ) continue;
    
    fill( "l1ChDEta", _l2.Eta()-_vc->getF("och2_eta",ip), _w );
    fill( "l1ChDPhi", KineUtils::dPhi(_l2.Phi(), _vc->getF("och2_phi",ip) ), _w );
    fill( "l1Ch2D", _l2.Eta()-_vc->getF("och2_eta",ip), 
	  KineUtils::dPhi(_l2.Phi(), _vc->getF("och2_phi",ip) ), _w );
    fill( "l1ChPt", _vc->getF("och2_pt",ip), _w );

    fill( "l1ChDEtaPt", _vc->getF("och2_pt",ip), _l2.Eta()-_vc->getF("och2_eta",ip) , _w );
    fill( "l1ChDPhiPt", _vc->getF("och2_pt",ip), KineUtils::dPhi(_l2.Phi(), _vc->getF("och2_phi",ip) ) , _w );

    chpt2.push_back( _vc->getF("och2_pt",ip) );
    cheta2.push_back( _vc->getF("och2_eta",ip) );
    chphi2.push_back( _vc->getF("och2_phi",ip) );


    nch2++;
  }

  // fill( "l1NCh", nCh1, _w );
  // fill( "l2NCh", nCh2, _w );
  fill( "l1NCh", nch1, _w );
  fill( "l1NCh", nch2, _w );



  //neutral ================================================================
  size_t nNh1=_vc->getSize("onh1_pt");
  size_t nNh2=_vc->getSize("onh2_pt");

  int nnh1=0,nnh2=0;
  sumPt=0; sumPtO=0;

  for(size_t ip=0;ip<nNh1;ip++) {

    //if(!genCleaning("onh1",ip,"og1") ) continue;  
    if(!occCleaning("onh1",ip,_l1) ) continue;
   

    fill( "l1NhDEta", _l1.Eta()-_vc->getF("onh1_eta",ip), _w*_vc->getF("onh1_pt",ip) );
    fill( "l1NhDPhi", KineUtils::dPhi(_l1.Phi(), _vc->getF("onh1_phi",ip) ), _w*_vc->getF("onh1_pt",ip) );
    fill( "l1Nh2D", _l1.Eta()-_vc->getF("onh1_eta",ip), 
	  KineUtils::dPhi(_l1.Phi(), _vc->getF("onh1_phi",ip) ), _w );
    
    if(KineUtils::dR(_l1.Eta(), _vc->getF("onh1_eta",ip), _l1.Phi(), _vc->getF("onh1_phi",ip) )>0.15 && 
       KineUtils::dR(_l1.Eta(), _vc->getF("onh1_eta",ip), _l1.Phi(), _vc->getF("onh1_phi",ip) )<0.3 && nNh1!=0 )
      sumPtO += _vc->getF("onh1_pt",ip);
    if(KineUtils::dR(_l1.Eta(), _vc->getF("onh1_eta",ip), _l1.Phi(), _vc->getF("onh1_phi",ip) )<0.15)
      sumPt += _vc->getF("onh1_pt",ip);

    fill( "l1NhDEtaPt", _vc->getF("onh1_pt",ip), _l1.Eta()-_vc->getF("onh1_eta",ip) , _w );
    fill( "l1NhDPhiPt", _vc->getF("onh1_pt",ip), KineUtils::dPhi(_l1.Phi(), _vc->getF("onh1_phi",ip) ) , _w );


    if(!mif(fabs(KineUtils::dPhi(_l1.Phi(),_vc->getF("onh1_phi",ip) ))<0.15 && fabs(_l1.Eta()- _vc->getF("onh1_eta",ip))<0.15 ) ) continue;

    nhpt1.push_back( _vc->getF("onh1_pt",ip) );
    nheta1.push_back( _vc->getF("onh1_eta",ip) );
    nhphi1.push_back( _vc->getF("onh1_phi",ip) );

    nnh1++;
  }
  //if(sumPt!=0)
  fill( "l1NhPt", _l1.Pt(), -sumPt+sumPtO*0.15*0.15/(0.4*0.4-0.15*0.15), _w );
  fill( "l2NhPt", _l1.Pt(), sumPt, _w );

  sumPt=0; sumPtO=0;

  for(size_t ip=0;ip<nNh2;ip++) {
    
    //if(!genCleaning("onh2",ip,"og2") ) continue;  
    if(!occCleaning("onh2",ip,_l2) ) continue;
    
    fill( "l1NhDEta", _l2.Eta()-_vc->getF("onh2_eta",ip), _w*_vc->getF("onh2_pt",ip) );
    fill( "l1NhDPhi", KineUtils::dPhi(_l2.Phi(), _vc->getF("onh2_phi",ip) ), _w*_vc->getF("onh2_pt",ip) );
    fill( "l1Nh2D", _l2.Eta()-_vc->getF("onh2_eta",ip), 
	  KineUtils::dPhi(_l2.Phi(), _vc->getF("onh2_phi",ip) ), _w );
    if(KineUtils::dR(_l2.Eta(), _vc->getF("onh2_eta",ip), _l2.Phi(), _vc->getF("onh2_phi",ip) )>0.15 && 
       KineUtils::dR(_l2.Eta(), _vc->getF("onh2_eta",ip), _l2.Phi(), _vc->getF("onh2_phi",ip) )<0.4 && nNh2!=0)
      sumPtO += _vc->getF("onh2_pt",ip);
    if(KineUtils::dR(_l2.Eta(), _vc->getF("onh2_eta",ip), _l2.Phi(), _vc->getF("onh2_phi",ip) )<0.15)
      sumPt += _vc->getF("onh2_pt",ip);
    
    fill( "l1NhDEtaPt", _vc->getF("onh2_pt",ip), _l2.Eta()-_vc->getF("onh2_eta",ip) , _w );
    fill( "l1NhDPhiPt", _vc->getF("onh2_pt",ip), KineUtils::dPhi(_l2.Phi(), _vc->getF("onh2_phi",ip) ) , _w );

    if(!mif(fabs(KineUtils::dPhi(_l2.Phi(),_vc->getF("onh2_phi",ip) ))<0.15 && fabs(_l2.Eta()- _vc->getF("onh2_eta",ip))<0.15 ) ) continue;

    nhpt2.push_back( _vc->getF("onh2_pt",ip) );
    nheta2.push_back( _vc->getF("onh2_eta",ip) );
    nhphi2.push_back( _vc->getF("onh2_phi",ip) );

    nnh2++;
  }
  //if(sumPt!=0)
  fill( "l1NhPt", _l2.Pt(),-sumPt+sumPtO*0.15*0.15/(0.4*0.4-0.15*0.15), _w );
  fill( "l2NhPt", _l2.Pt(), sumPt, _w );

  // fill( "l1NNh", nNh1, _w );
  // fill( "l2NNh", nNh2, _w );
  fill( "l1NNh", nnh1, _w );
  fill( "l1NNh", nnh2, _w );


  //======================== display
  int ch1=_vc->getI("ch_m1") ;
  int ch2=  _vc->getI("ch_m2");
  
  



  if(_ie!=-1) {
    if(nnh1!=0) // || nPh1!=0 || nCh1!=0 )
      ld.fill(_l1.Eta(), _l1.Phi(),ch1 ,
	      phpt1, pheta1, phphi1,
	      chpt1, cheta1, chphi1,
	      nhpt1, nheta1, nhphi1);
  
    if(nnh2!=0) // || nPh2!=0 || nCh2!=0)
      ld.fill(_l2.Eta(), _l2.Phi(),ch2,
	      phpt2, pheta2, phphi2,
	      chpt2, cheta2, chphi2,
	      nhpt2, nheta2, nhphi2);
  }
  
}


bool 
METAnalysis::occCleaning(string type, size_t i, TLorentzVector l) {
  
  if(!mif( fabs(_vc->getF(type+"_pId",i) ) != 13 && fabs(_vc->getF(type+"_pId",i) ) != 11 && !(KineUtils::dR(l.Eta(), _vc->getF(type+"_eta",i), l.Phi(), _vc->getF(type+"_phi",i) )<0.05 && fabs(_vc->getF(type+"_pt",i)-l.Pt())/l.Pt()<0.4) )) return false;
  if(KineUtils::dR(l.Eta(), _vc->getF(type+"_eta",i), l.Phi(), _vc->getF(type+"_phi",i) )<0.05 && fabs(_vc->getF(type+"_pt",i)-l.Pt())/l.Pt()<0.4 ) return false;
  if(KineUtils::dR(l.Eta(), _vc->getF(type+"_eta",i), l.Phi(), _vc->getF(type+"_phi",i) )<0.005) return false;
  //if(KineUtils::dR(l.Eta(), _vc->getF(type+"_eta",i), l.Phi(), _vc->getF(type+"_phi",i) )>0.4) return false;

  // cout<<(type=="onh1")<<"  "<<(type=="onh2")
  //     <<"  "<<(fabs(KineUtils::dPhi(l.Phi(),_vc->getF(type+"_phi",i) ))<0.15)
  //     <<"  "<<(fabs(l.Eta()- _vc->getF(type+"_eta",i))<0.15)<<endl;

  // cout<<" ---> "<<( (type=="onh1" || type=="onh2") && fabs(KineUtils::dPhi(l.Phi(),_vc->getF(type+"_phi",i) ))<0.15 && fabs(l.Eta()- _vc->getF(type+"_eta",i))<0.15 )<<endl;

  return true;
}


bool 
METAnalysis::genCleaning(string type, size_t i, string gType) {
  
  int nGen = _vc->getSize(gType+"_pt");
  bool pdg=false;
  float dR=0;

  for(int ig=0;ig<nGen;ig++) {

    pdg = fabs(_vc->getF(type+"_pId",i))==_vc->getF(gType+"_pId",i);
    dR= KineUtils::dR(_vc->getF(type+"_eta",i), _vc->getF(gType+"_eta",ig) , _vc->getF(type+"_phi",i), _vc->getF(gType+"_phi",i) );

    
    if( pdg && dR < 0.02 )
      return false;

    if(!pdg && dR<0.02) {
      if( ( fabs(_vc->getF(gType+"_pId",i)) == 111 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 211 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 130 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 310 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 321 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 411 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 421 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 2122 ||
	    fabs(_vc->getF(gType+"_pId",i)) == 2112 ) && 
	  ( fabs(_vc->getF(type+"_pId",i)) == 130 ||
	    fabs(_vc->getF(type+"_pId",i)) == 211 ) )
	return false;

    }
    
  }
  
  return true;
}


TVector2
METAnalysis::footPrintEl(string l, TLorentzVector tl) {

  float sumPt=0;
  //photons ================================================================
  size_t nPh=_vc->getSize("oph"+l+"_pt");
  for(size_t ip=0;ip<nPh;ip++) {
    if(!occCleaning("oph"+l,ip,tl) ) continue;
    
    if(KineUtils::dR(tl.Eta(), _vc->getF("oph"+l+"_eta",ip), tl.Phi(), _vc->getF("oph"+l+"_phi",ip) )<0.15 || 
       (fabs(KineUtils::dPhi(tl.Phi(), _vc->getF("oph"+l+"_phi",ip) ))<0.3 && fabs(tl.Eta()-_vc->getF("oph"+l+"_eta",ip))<0.05 ) ) 
      sumPt += _vc->getF("oph"+l+"_pt",ip);
  }

  //neutral ================================================================
  size_t nNh=_vc->getSize("onh"+l+"_pt");
  for(size_t ip=0;ip<nNh;ip++) {
    if(!occCleaning("onh"+l,ip,tl) ) continue;
    
    if(KineUtils::dR(tl.Eta(), _vc->getF("onh"+l+"_eta",ip), tl.Phi(), _vc->getF("onh"+l+"_phi",ip) )<0.15) 
      sumPt += _vc->getF("onh"+l+"_pt",ip);
  }

  //charged ================================================================
  size_t nCh=_vc->getSize("och"+l+"_pt");
  for(size_t ip=0;ip<nCh;ip++) {
    if(!occCleaning("och"+l,ip,tl) ) continue;
    
    if(KineUtils::dR(tl.Eta(), _vc->getF("och"+l+"_eta",ip), tl.Phi(), _vc->getF("och"+l+"_phi",ip) )<0.03) 
      sumPt += _vc->getF("och"+l+"_pt",ip);
  }

  TVector2 t;t.SetMagPhi( sumPt, tl.Phi() );

  return t;
}

float 
METAnalysis::getRecoilCor(float pt, float eta, bool el, bool data) {

  float c=0;

  if(el) {
    if(!data) {
      if(fabs(eta)<1.5) c=0.5090+min(pt,(float)130.)*0.0075;
      else c=0.2969+min(pt,(float)130.)*0.0135;
    }
    else {
      if(fabs(eta)<1.5) c=0.4633+min(pt,(float)130.)*0.0055;
      else c=0.3951+min(pt,(float)140.)*0.0125;
    }
      // if(fabs(eta)<1.5) c = 0.184+pt*0.012;
    // else c = 0.3202+pt*0.0063;
  }
  else { //muons
    if(fabs(eta)<1.5) {
      c = 0.1937; //photons
      c+= 0.0282+pt*0.0002; //hadrons
    }
    else {
      c = 0.283*(1-exp(-0.042*pt - 0.278) ); //photons
      c+= 0.03035+pt*0.00005; //hadrons
    }
  }
  
  return c;
}





// TVector2
// METAnalysis::neutralMu(string l, TLorentzVector tl) {
  
//   TVector2 addNeutral(0,0);

//   //neutral ================================================================
//   size_t nNh=_vc->getSize("onh"+l+"_pt");
//   for(size_t ip=0;ip<nNh;ip++) {
//     if(!occCleaning("onh"+l,ip,tl) ) continue;
    
//     if(KineUtils::dR(tl.Eta(), _vc->getF("onh"+l+"_eta",ip), tl.Phi(), _vc->getF("onh"+l+"_phi",ip) )<0.08) {
//       TVector2 tmp; tmp.SetMagPhi( _vc->getF("onh"+l+"_pt",ip), _vc->getF("onh"+l+"_phi",ip) );
//       addNeutral += tmp; 
//     }
//   }
  
//   if(addNeutral.Mod()<2) addNeutral.SetMagPhi(0,0);

// }
