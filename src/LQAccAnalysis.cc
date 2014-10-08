#include "src/LQAccAnalysis.hh"

using namespace std;

ClassImp(LQAccAnalysis)

LQAccAnalysis::LQAccAnalysis():
UseTree() {

  decayChan[1] = "ee";
  decayChan[2] = "mm";;
  decayChan[3] = "tete";;
  decayChan[4] = "tmtm";;
  decayChan[5] = "thth";;
  decayChan[10] = "em";;
  decayChan[11] = "ete";;
  decayChan[12] = "etm";;
  decayChan[13] = "eth";;
  decayChan[21] = "mte";;
  decayChan[22] = "mtm";;
  decayChan[23] = "mth";;
  decayChan[50] = "tetm";;
  decayChan[51] = "teth";;
  decayChan[52] = "tmth";;
  
}

LQAccAnalysis::~LQAccAnalysis() {

}

void
LQAccAnalysis::modifySkimming() {
}


void
LQAccAnalysis::bookHistograms() {

  for(map<int,string>::const_iterator it=decayChan.begin();
      it!=decayChan.end();it++) {

    _hm->addVariable(it->second+"Acc",1,0.5,1.50,it->second+"Acc");
    _hm->addVariable(it->second+"NoAcc",1,0.5,1.50,it->second+"Acc");
  }

  _hm->addVariable("decay",60,0.5,60,"decay channel");
 

}

void
LQAccAnalysis::modifyWeight() {
}


void
LQAccAnalysis::analyze() {
  
  vector<genPart> daus = findDaughters();

  int dT=-1;
  bool isAcc= isInAcceptance(daus, dT);
  
  fill("decay",dT,_w);
  
  map<int,string>::const_iterator it=decayChan.find(dT);
  if(it!=decayChan.end()) {
    if(isAcc)
      fill( it->second+"Acc", 1, _w );
    else
      fill( it->second+"NoAcc", 1, _w );
  }  

}


vector<genPart>
LQAccAnalysis::findDaughters() {

  size_t nGen=_vc->getSize("GenParticlePt"); 
 
  vector<genPart> daus;

  //vector<genPart> neutrinos;

  for(size_t ig=0;ig<nGen;ig++) {
      
    // if(!_au->simpleCut<double>(_vc->getD("GenParticlePt",ig),10,">") ) continue;
   
    //if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),11,"=") ) ) continue; 
    //if( !(_au->simpleCut<int>(fabs(_vc->getI("GenParticlePdgId",ig)),15,"!=") ) ) continue; 
    if( !(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),14,"!=") ) ) continue; //nu_mu
    if( !(_au->simpleCut<int>(fabs(_vc->getI("genPdgId",ig)),12,"!=") ) ) continue; //nu_e

    if( _au->simpleCut<int>(fabs(_vc->getI("genPdgId", _vc->getI("genMother",ig))),15,"=")  ||
	_au->simpleCut<int>(fabs(_vc->getI("genPdgId", _vc->getI("genMother",ig))),24,"=")
       ) { 
      genPart p;
      p.pdgId = fabs( _vc->getI("genPdgId",ig) );
      p.idx = ig;
      p.mothIdx = _vc->getI("genMother",ig);
      p.mothId = fabs(_vc->getI("genPdgId", _vc->getI("genMother",ig)));
      p.p4.SetPtEtaPhiM(_vc->getD("genPt",ig),
			_vc->getD("genEta",ig),
			_vc->getD("genPhi",ig),
			1.7 );

      //cout<<p.pdgId<<"  "<<_vc->getI("GenParticleStatus",ig)<<"  "<<p.mothId<<endl;

      if( _vc->getI("GenParticleStatus",ig)!=2 && p.pdgId == 15) continue;

      daus.push_back(  p );
    }
  }
  
  return daus;
}

bool
LQAccAnalysis::isInAcceptance(vector<genPart> vp, int& dT) {

  //bool hasWEl=false;
  // bool hasWMu=false;
  //bool hasTEl=false;
  //bool hasTMu=false;
  //bool hasTHad=false;
  bool hasWJ=false;

  int muW=0;
  int elW=0;
  int muT=0;
  int elT=0;
  int tauH=0;

  int tauLidx=-1;

  for(size_t i=0;i<vp.size();i++) {
    if( vp[i].pdgId == 11 && vp[i].mothId == 24) elW++;
    if( vp[i].pdgId == 13 && vp[i].mothId == 24) muW++;
    if( vp[i].pdgId <5  && vp[i].mothId == 24) hasWJ=true;
    if( vp[i].pdgId == 11 && vp[i].mothId == 15) elT++;
    if( vp[i].pdgId == 13 && vp[i].mothId == 15) muT++;
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

  if(hasWJ) {dT=-1; return 0;}  
  //cout<<_ie<<" !!! "<<vp.size()<<" == "<<elW<<"  "<<muW<<"  "<<elT<<"  "<<muT<<"  "<<tauH<<endl;

  if(elW==2) dT=1;
  if(muW==2) dT=2;
  if(elT==2) dT=3;
  if(muT==2) dT=4;
  if(tauH==2) dT=5;
  if(elW==1 && muW==1) dT=10;
  if(elW==1 && elT==1) dT=11;
  if(elW==1 && muT==1) dT=12;
  if(elW==1 && tauH==1) dT=13;
  if(muW==1 && elT==1) dT=21;
  if(muW==1 && muT==1) dT=22;
  if(muW==1 && tauH==1) dT=23;
  if(elT==1 && muT==1) dT=50;
  if(elT==1 && tauH==1) dT=51;
  if(muT==1 && tauH==1) dT=52;


  bool acc[2]={false,false}; 
  bool f=true;

  for(size_t i=0;i<vp.size();i++) {
    if( (dT>=10 && dT<14) || dT==21
	|| dT==3 || dT==50 || dT==51 || dT==1) {
      if(vp[i].pdgId == 11) {
	if(f) {acc[0] = isElAcc( vp[i].p4 ); f=false; }
	else  {acc[1] = isElAcc( vp[i].p4 ); }
      }
    }
    if(  dT==10 ||
	dT==12 || (dT>20 && dT<24) || dT==2 || dT==4 || dT==50 || dT==52) {
      if(vp[i].pdgId == 13) {
	if(f) {acc[0] = isMuAcc( vp[i].p4 ); f=false; }
	else  {acc[1] = isMuAcc( vp[i].p4 ); }
      }
    }
  }
  
  // if(dT==5)
  //   for(size_t i=0;i<vp.size();i++)
  //     cout<<i<<"\t"<<vp[i].pdgId<<"   "<<vp[i].mothId<<"   // "<<vp[i].idx<<"  "<<vp[i].mothIdx<<"   "<<vp[i].p4.Pt()<<"   "<<vp[i].p4.Eta()<<"     "<<acc[0]<<"    "<<acc[1]<<endl;


  if(  dT==51 || dT==52 || dT==13 || dT==23 ) {
    TLorentzVector n1,t1,t2;int mt1=-1,mn1=-1; //,mt2;
    for(size_t i=0;i<vp.size();i++) {
      if(vp[i].pdgId == 16 && tauLidx!=vp[i].mothIdx && vp[i].mothId == 15) { n1=vp[i].p4; mn1=vp[i].mothIdx; }
      //if( (vp[i].pdgId == 13 || vp[i].pdgId == 11) && tauLidx!=vp[i].mothIdx) { n1=vp[i].p4; mn1=vp[i].mothIdx; }
      if(vp[i].pdgId == 15 && mt1==-1) { t1=vp[i].p4; mt1=vp[i].idx;}
      if(vp[i].pdgId == 15 && mt1!=-1) { t2=vp[i].p4; /*mt2=vp[i].idx;*/} 
    }

    if(mt1==mn1) {
      //cout<<f<<" found "<<mt1<<"   "<<mn1<<"   "<<(t1-n1).Pt()<<"    "<<(t1-n1).Eta()<<endl;
      if(f) {acc[0] = isHTauAcc( (t1-n1) ); f=false; }
      else  {acc[1] = isHTauAcc( (t1-n1) ); }
    }
    else {
      if(f) {acc[0] = isHTauAcc( (t2-n1) ); f=false; }
      else  {acc[1] = isHTauAcc( (t2-n1) ); }
    }
  }
  
  if( dT==5 ) {
    TLorentzVector n1,t1,n2,t2;
    int mn1=-1,mn2,mt1=-1;//,mt2;
    for(size_t i=0;i<vp.size();i++) {
      if(vp[i].pdgId == 16  && vp[i].mothId == 15) { 
	if(mn1==-1) { n1=vp[i].p4; mn1=vp[i].mothIdx;}
	else  { n2=vp[i].p4; mn2=vp[i].mothIdx;}
      }
      if(vp[i].pdgId == 15 && vp[i].mothId == 15) {
	if(mt1==-1) { t1=vp[i].p4; mt1=vp[i].idx;}
	else { t2=vp[i].p4; /*mt2=vp[i].idx;*/}
      }
    }//for
    if(mn1==mt1) {
      //cout<<f<<" found "<<mt1<<"   "<<mn1<<"   "<<(t1-n1).Pt()<<"    "<<(t1-n1).Eta()<<endl;
      acc[0] = isHTauAcc( (t1-n1) ); 
      acc[1] = isHTauAcc( (t2-n2) ); 
    }
    else {
      //cout<<f<<" found "<<mt1<<"   "<<mn1<<"   "<<mn2<<"   "<<(t1-n2).Pt()<<"    "<<(t1-n2).Eta()<<endl;
      acc[0] = isHTauAcc( (t1-n2) ); 
      acc[1] = isHTauAcc( (t2-n1) ); 
    }
  }
  return (acc[0]&&acc[1]);
  
}

bool 
LQAccAnalysis::isElAcc(TLorentzVector p) {

  if(p.Pt()<30) return false;
  if( fabs(p.Eta()) >2.5 || (fabs(p.Eta())<1.56 && fabs(p.Eta())>1.4442 ) ) return false;
  return true;
}

bool 
LQAccAnalysis::isMuAcc(TLorentzVector p) {

  if(p.Pt()<30) return false;
  if( fabs(p.Eta()) >2.1 ) return false;
  return true;
}

bool 
LQAccAnalysis::isHTauAcc(TLorentzVector p) {

  if(p.Pt()<30) return false;
  if( fabs(p.Eta()) >2.3 ) return false;
  return true;
}


void
LQAccAnalysis::endAnalysis() {

  for(map<int,string>::const_iterator it=decayChan.begin();
      it!=decayChan.end();it++) {

    float tA =  _hm->getHisto( it->second+"Acc", 0 )->Integral();
    float tNA =  _hm->getHisto( it->second+"NoAcc", 0 )->Integral();
    
    float acc= tA/(tA+tNA);

    cout<<it->second<<" : "<<tA<<" / "<<tNA<<" / "<<tA+tNA
	<<" = "<<acc<<" +- "<<StatUtils::BinomError(tA+tNA,acc)<<endl;



  }


}
