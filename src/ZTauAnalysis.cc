#include "src/ZTauAnalysis.hh"

#include "utils/KineUtils.hh"

using namespace std;

ClassImp(ZTauAnalysis)

ZTauAnalysis::ZTauAnalysis():
UseTree() {
  
  _dbm->loadDb("muIDMC","muIDTightMCEff.db");
  _dbm->loadDb("muIsoMC","muIsoTightMCEff.db");
  _dbm->loadDb("elIDMC","elIDMediumMCEff.db");
  effmID = 0.;
  effmIso = 0.;
  effm = 0.;
  N=0;
}

ZTauAnalysis::~ZTauAnalysis() {

}

void
ZTauAnalysis::modifySkimming() {
}


void
ZTauAnalysis::bookHistograms() {

  _hm->addVariable("PtAcc",100,0,200,"p_{T} [GeV] ");
  _hm->addVariable("PtAccMatch",100,0,200,"p_{T} [GeV] ");
  _hm->addVariable("PtAccID",100,0,200,"p_{T} [GeV] ");

  _hm->addVariable("EtaAcc",100,-2.5,2.5,"#eta ");
  _hm->addVariable("EtaAccMatch",100,-2.5,2.5,"#eta ");
  _hm->addVariable("EtaAccID",100,-2.5,2.5,"#eta ");
  _hm->addVariable("PtVsEtaAcc",100,0,200,100,-2.5,2.5,"p_{T} [GeV] ","#eta ");
  _hm->addVariable("PtVsEtaAccMatch",100,0,200,100,-2.5,2.5,"p_{T} [GeV] ","#eta ");
  _hm->addVariable("PtVsEtaAccID",100,0,200,100,-2.5,2.5,"p_{T} [GeV] ","#eta ");

  _hm->addVariable("dPt",200,-1,1,"#Delta(p_{T})/p_{T} ");
  _hm->addVariable("nTau",3,-0.5,2.5,"# #tau_{h}  ");

}

void
ZTauAnalysis::modifyWeight() {
}


void
ZTauAnalysis::analyze() {
  
  size_t nGen=_vc->getSize("GenParticlePt"); 
 
  tau1_.SetPtEtaPhiE(0.00001,0,0,0);
  tau2_.SetPtEtaPhiE(0.00001,0,0,0);
  tmpTau_.SetPtEtaPhiM(0.00001,0,0,0);
  
  vector<genPart> daus;

  //cout<<" ************ event : "<<_ie<<endl;

  for(size_t ig=0;ig<nGen;ig++) {
      
    //if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),10,">") ) continue;
   
    // if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"=") ) ||
    // 	!(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),16,"=") ) ) continue; 

    if( !( (_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),24,"=") && fabs(_vc->getI("GenParticlePdgId",ig))==15 ) ||
    	   (_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),15,"=") ) ) ) continue; 

    genPart p;
    p.pdgId = fabs( _vc->getI("GenParticlePdgId",ig) );
    p.idx = ig;
    p.mothIdx = _vc->getI("GenParticleMotherIndex",ig);
    p.mothId = fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig)));
    p.p4.SetPtEtaPhiE(_vc->getD("GenParticlePt",ig),
    		      _vc->getD("GenParticleEta",ig),
    		      _vc->getD("GenParticlePhi",ig),
    		      _vc->getD("GenParticleEnergy",ig) );

  
    if( _vc->getI("GenParticleStatus",ig)!=2 && p.pdgId == 15) continue;

    //cout<<"! "<<p.pdgId<<"   "<<p.idx<<"   "<<p.mothIdx<<"   "<<p.mothId<<"   "<<_vc->getI("GenParticleStatus",ig)<<"    "<<p.p4.Pt()<<endl;
    daus.push_back(  p );

     
  }

  int tauH=0;
  bool acc1=false;
  bool acc2 =false;
  isInAcc(daus, acc1, acc2, tauH);

  fill("nTau",tauH, 1);

  //if(!_au->simpleCut<int>(tauH,1,"=") ) return;

  // if(_au->makeCut(acc1, _ids, "in acceptance", 1.)) {
     
  //   size_t nTau = _vc->getSize("HPSTauPt");
  //   bool match=false;
  //   //cout<<" size "<<nTau<<endl;
  //   for(size_t it=0;it<nTau;it++) {
  //     tmpTau_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
  // 			    _vc->getD("HPSTauEta",it),
  // 			    _vc->getD("HPSTauPhi",it), 1.7 );
      
  //     //cout<<" !!-> "<<tmpTau_.Pt()<<"   "<<tmpTau_.Eta()<<"   "<<tmpTau_.Phi()<<endl;

  //     // if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", it)), 2.3, "<")) continue;
  //     // if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", it), 20, ">")) continue;

  //     float dR1 = tmpTau_.DeltaR( tau1_ );
  //     if( _au->simpleCut<double>( dR1, 0.2, "<" ) ) {
  // 	fill("dPt", (tau1_.Pt()-tmpTau_.Pt())/tau1_.Pt(), 1 );
  // 	//cout<<tau1_.Pt()<<"   "<<tmpTau_.Pt()<<endl;
  // 	if(_au->makeCut(true, _ids, "match", _w) ) {
  // 	  fill("PtAccMatch", tmpTau_.Pt());
  // 	  fill("EtaAccMatch", tmpTau_.Eta());
  // 	  fill("PtVsEtaAccMatch", tmpTau_.Pt(), tmpTau_.Eta());
  // 	}
  // 	match=true;
  // 	if(_au->makeCut( tauID(it) , _ids, "ID", _w)) {
  //  	  fill("PtAccID", tmpTau_.Pt());
  //  	  fill("EtaAccID", tmpTau_.Eta());
  //  	  fill("PtVsEtaAccID", tmpTau_.Pt(), tmpTau_.Eta());
  //  	}
  //     }
  //   }

  //   //if(!match) cout<<" -> "<<tau1_.Pt()<<"   "<<tau1_.Eta()<<"   "<<tau1_.Phi()<<endl;

  // }

    for(size_t ig=0;ig<nGen;ig++) {
     
      if( !_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)), 11, "=")) continue;
      if( !( _au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId", _vc->getI("GenParticleMotherIndex",ig))),24,"=") ) ) continue; 
      
      if( !_au->simpleCut( _vc->getD("GenParticlePt", ig)>30)) continue;
      
      if( !_au->simpleCut( fabs(_vc->getD("GenParticleEta", ig))<2.1)) continue;
      _au->makeCut(true, _ids, "mu in acc", 1.); 
      
      size_t nMu = _vc->getSize("ElectronPt");
      for(size_t im=0;im<nMu;im++) {
	
	float dR=KineUtils::dR(_vc->getD("ElectronEta",im),
			       _vc->getD("GenParticleEta",ig),
			       _vc->getD("ElectronPhi",im),
			       _vc->getD("GenParticlePhi",ig) );
	
	if(!_au->simpleCut(dR<0.2)) continue;
	// if(!_au->simpleCut( fabs(_vc->getD("MuonEta",im))<0.9)) continue;
	// if(!_au->simpleCut( _vc->getD("MuonPt",im)<35 && _vc->getD("MuonPt",im)>30 )) continue;
	
	_au->makeCut(true, _ids, "mu matched", 1.); 
	{
	  // effm += _dbm->getDBValue ("muIDMC", fabs(_vc->getD("MuonEta",im)), _vc->getD("MuonPt",im) )*_dbm->getDBValue ("muIsoMC", fabs(_vc->getD("MuonEta",im)), _vc->getD("MuonPt",im) );
	  // effmID +=  _dbm->getDBValue ("muIDMC", fabs(_vc->getD("MuonEta",im)), _vc->getD("MuonPt",im) );
	  effmIso += _dbm->getDBValue ("elIDMC", fabs(_vc->getD("ElectronEta",im)), _vc->getD("ElectronPt",im) );
	  N++;
	}

	_au->makeCut(elecID(im), _ids, "mu ID", 1.); 
	
      }
     
  }





  return;
  


  if( _au->simpleCut(acc1) )
  {
    _au->makeCut(true, _ids, "in acceptance", 1.); 
    fill("PtAcc", tau1_.Pt());
    fill("EtaAcc", tau1_.Eta());
    fill("PtVsEtaAcc", tau1_.Pt(), tau1_.Eta());

    size_t nTau = _vc->getSize("HPSTauPt");
    for(size_t it=0;it<nTau;it++) {
      tmpTau_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
  			    _vc->getD("HPSTauEta",it),
  			    _vc->getD("HPSTauPhi",it), 1.7 );
      
      if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", it)), 2.3, "<")) continue;
      if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", it), 30, ">")) continue;
      //if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", it),1.,"=")) continue;
      float dR1 = tmpTau_.DeltaR( tau1_ );
      
      if( _au->simpleCut<double>( dR1, 0.2, "<" ) ) {
  	fill("dPt", (tau1_.Pt()-tmpTau_.Pt())/tau1_.Pt(), 1 );
  	if(_au->makeCut(true, _ids, "match", _w) ) {
  	  fill("PtAccMatch", tau1_.Pt());
  	  fill("EtaAccMatch", tau1_.Eta());
  	  fill("PtVsEtaAccMatch", tau1_.Pt(), tau1_.Eta());
  	}
  	if(_au->makeCut( tauID(it) , _ids, "ID", _w)) {
  	  fill("PtAccID", tau1_.Pt());
  	  fill("EtaAccID", tau1_.Eta());
  	  fill("PtVsEtaAccID", tau1_.Pt(), tau1_.Eta());
  	}
  	break;	
      }
    }
  }
    
  if(!_au->simpleCut<int>(tauH,2,"<") ) return;
  
  if( _au->simpleCut(acc2) ) {
    _au->makeCut(acc2, _ids, "in acceptance", 1.); 
    fill("PtAcc", tau2_.Pt());
    fill("EtaAcc", tau2_.Eta());
    fill("PtVsEtaAcc", tau2_.Pt(), tau2_.Eta());
    //cout<<" new tau **********************"<<endl;
    //cout<<tau2_.Pt()<<"   "<<tau2_.Eta()<<"   "<<tau2_.Phi()<<endl;

    size_t nTau = _vc->getSize("HPSTauPt");
    //cout<<" ===== Ntau : "<<nTau<<endl;
    for(size_t it=0;it<nTau;it++) {
      tmpTau_.SetPtEtaPhiM( _vc->getD("HPSTauPt",it), 
  			    _vc->getD("HPSTauEta",it),
  			    _vc->getD("HPSTauPhi",it), 1.7 );
      

      float dR2 = tmpTau_.DeltaR( tau2_ );
      //cout<<" -> "<<tmpTau_.Pt()<<"   "<<tmpTau_.Eta()<<"   "<<tmpTau_.Phi()<<" => "<<dR2<<endl;

      if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", it)), 2.3, "<")) continue;
      if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", it), 30, ">")) continue;
      // if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", it),1.,"=")) continue;
    
      
      if (_au->simpleCut<double>( dR2, 0.2, "<" )&& _au->simpleCut(acc2) ) {
	
  	fill("dPt", (tau2_.Pt()-tmpTau_.Pt())/tau2_.Pt(), 1 );
	
  	if(_au->makeCut(true, _ids, "match", _w) ) {
  	  fill("PtAccMatch", tau2_.Pt());
  	  fill("EtaAccMatch", tau2_.Eta());
  	  fill("PtVsEtaAccMatch", tau2_.Pt(), tau2_.Eta(), 1);
  	}
  	if(_au->makeCut( tauID(it) , _ids, "ID", _w)) {
  	  fill("PtAccID", tau2_.Pt());
  	  fill("EtaAccID", tau2_.Eta());
  	  fill("PtVsEtaAccID", tau2_.Pt(), tau2_.Eta(), 1);
  	}
  	break;
      }
    }
  }


}


bool ZTauAnalysis::tauID(size_t idx) {
  // if(!_au->simpleCut<double>(fabs(_vc->getD("HPSTauEta", idx)), 2.3, "<")) return false;
  // if(!_au->simpleCut<double>(_vc->getD("HPSTauPt", idx), 30, ">")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauDecayModeFindingDiscr",idx),1,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstElectronLooseMVA3", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTauagainstMuonLoose2", idx),1.,"=")) return false;
  if(!_au->simpleCut<double>(_vc->getD("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", idx),1.,"=")) return false; // != means inverted isolation
  //  if(!_au->simpleCut<double>(_vc->getD("HPSTauCombinedIsolationDeltaBetaCorrDiscr", idx),0.,"!=")) return false;
  return true;
}


void
ZTauAnalysis::isInAcc( vector<genPart> vp , bool& acc1, bool& acc2, int& tauH ) {

  bool acc[2]={0,0};
  int tauLidx=-1;
  //int tauH=0;
  for(size_t i=0;i<vp.size();i++) {

     if( vp[i].pdgId == 15 && vp[i].mothId == 15) {

      bool hasL=false;
      for(size_t j=0;j<vp.size();j++) {
	if( ( vp[j].pdgId == 11 || vp[j].pdgId == 13 ) && vp[i].idx==vp[j].mothIdx)
	  {hasL=true; tauLidx=vp[i].idx; break;}
      }
      if(!hasL)
	tauH++;
    }
  }
  
  //cout<<" number of tau H "<<tauH<<endl;

  if(tauH==0) {
     acc1 = acc[0];
     acc2 = acc[1];
    return;
  }

  if(tauH==1) {
    TLorentzVector n1,t1,t2;int mt1=-1, mn1=-1;//,mt2;
    for(size_t i=0;i<vp.size();i++) {
      if(vp[i].pdgId == 16 && tauLidx!=vp[i].mothIdx && vp[i].mothId == 15) { n1=vp[i].p4; mn1=vp[i].mothIdx; }
      if(vp[i].pdgId == 15 && mt1==-1) { t1=vp[i].p4; mt1=vp[i].idx;}
      if(vp[i].pdgId == 15 && mt1!=-1) { t2=vp[i].p4; /*mt2=vp[i].idx;*/} 
    }
    
    if(mt1==mn1) {
      acc[0] = isTauInAcc( (t1-n1) );
      TVector3 v=t1.Vect() - n1.Vect();
      //cout<<t1.Pt()<<"   "<<n1.Pt()<<"   "<<(t1-n1).Pt()<<"   "<<v.Pt()<<endl;
      // cout<<" --> "<<t1.Px()<<"  "<<t1.Py()<<" / "<<n1.Px()<<"   "<<n1.Py()<<endl;
      tau1_ = t1-n1;
    }
    else {
      acc[0] = isTauInAcc( (t2-n1) );
      TVector3 v=t2.Vect() - n1.Vect();
      //cout<<t2.Pt()<<"   "<<n1.Pt()<<"   "<<(t2-n1).Pt()<<"   "<<v.Pt()<<endl;
      //cout<<" --> "<<t2.Px()<<"  "<<t2.Py()<<" / "<<n1.Px()<<"   "<<n1.Py()<<endl;
      tau1_ = t2-n1;
    }

  
    tau2_.SetPtEtaPhiM(0.0001,0,0,0);
  }
  if( tauH==2) {
    TLorentzVector n1,t1,n2,t2;
    int mn1=-1,mt1=-1;//,mn2,mt2;
    for(size_t i=0;i<vp.size();i++) {
      if(vp[i].pdgId == 16 && vp[i].mothId == 15 ) { 
	if(mn1==-1) { n1=vp[i].p4; mn1=vp[i].mothIdx;}
	else  { n2=vp[i].p4; /*mn2=vp[i].mothIdx;*/}
      }
      if(vp[i].pdgId == 15) {
	if(mt1==-1) { t1=vp[i].p4; mt1=vp[i].idx;}
	else { t2=vp[i].p4; /*mt2=vp[i].idx;*/}
      }
    }//for

    //cout<<mn1<<"   "<<mt1<<"   "<<mn2<<"   "<<mt2<<endl;
    // n1.Print();
    // t1.Print();
    // n2.Print();
    // t2.Print();

    if(mn1==mt1) {
      acc[0] = isTauInAcc( (t1-n1) ); 
      acc[1] = isTauInAcc( (t2-n2) ); 
      tau1_ = t1-n1;
      tau2_ = t2-n2;
    }
    else {
      acc[0] = isTauInAcc( (t1-n2) ); 
      acc[1] = isTauInAcc( (t2-n1) ); 
      tau1_ = t1-n2;
      tau2_ = t2-n1;
    }
    
  }

  //cout<<"is in acc : "<<acc[0]<<"   "<<acc[1]<<endl;

  acc1 = acc[0];
  acc2 = acc[1];

}


bool
ZTauAnalysis::isTauInAcc(TLorentzVector tau) {
 
  //cout<<" --> "<<tau.Pt()<<"  "<<tau.Eta()<<endl;

  if(tau.Pt()<30) return false;
  if( fabs(tau.Eta()) >2.3 ) return false;
  return true;

}


bool 
ZTauAnalysis::muonID(size_t idx) {

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

  //if( !_au->simpleCut<float>(iso, 0.12, "<") ) return false;

  return true;

}


bool 
ZTauAnalysis::elecID(size_t idx) {
  
  if(!_au->simpleCut<double>( _vc->getD("ElectronPt",idx), 30 , ">=" ) ) return false;
  if(!_au->simpleCut<double>( fabs(_vc->getD("ElectronEta",idx)), 2.5 , "<" ) ) return false;
  if(!_au->simpleCut<double>(fabs(_vc->getD("ElectronEta",idx)), 1.4442,"]![",1.56 ) ) return false;
  
  //return true;
  return (_vc->getI("ElectronPassEGammaIDMedium",idx)==1023);//1023->iso, 895=antiiso
}


void
ZTauAnalysis::endAnalysis() {

  cout<<"mu ID "<<effmID/N<<endl;
  cout<<"mu Iso "<<effmIso/N<<endl;
  cout<<"mu ID-Iso "<<(effmID*effmIso)/(N*N)<<endl;
  cout<<"mu C "<<effm/N<<endl;

}
