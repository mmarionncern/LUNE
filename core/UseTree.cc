#include "core/UseTree.hh"

#include <boost/timer.hpp>
#include <boost/progress.hpp>

#include "TStopwatch.h"

ClassImp(UseTree)

UseTree::UseTree():
_hm(0), _vc(0), _dbm(0)
{
  _init=true;
 
  _vc=new VarClass();
  _hm=new HistoManager();
  _au = new AnaUtils();

  _dbm=new DataBaseManager();
  _vc->_su->init(_dbm); //database for systematic uncertainties
  

  _recompute=true;
  _treeAnalysis=true;

  _nDC=0;

  _fullSkim=true;
  _skim=false;

  _testNMax=(size_t)-1;

}

UseTree::~UseTree() {
  reset();
  delete _hm;
  delete _vc;
  delete _dbm;
}

void
UseTree::configure() {
  _dsnames = anConf.getDSNames();
 
  _au->setDDCuts(anConf.getDDCuts() );
  
  _hm->configAnalysis(_dsnames, anConf.getDDCuts().size(), 
		      anConf.getDDDSStatus(), anConf.getCSData() );

  //  removeDDMarkers();

  for(size_t ids=0;ids<_dsnames.size();ids++) {
    dp.configure(_dsnames[ids], 
		 anConf.getDataset(_dsnames[ids])->getColor(),
		 anConf.getDataset(_dsnames[ids])->isGhost() );
    //Default, weight=1 for anyone
    dp.setWeight( _dsnames[ids], 1. );
  }

}

void
UseTree::configureAnalysis(bool skim, bool invCut, vector<string> cutNM1Var ) {
  _skim = skim;
  
  _au->setSkipCut(cutNM1Var,invCut);
}

void
UseTree::launchTest(size_t nMax) {
  _testNMax = nMax;
  anConf.setNMax(_testNMax);
}

void
UseTree::analyze() {
}
 
void
UseTree::endAnalysis() {
}
 
void 
UseTree::modifyWeight() {
}

void
UseTree::globalAnalyze() {

  TStopwatch stw;
  float timeCPU=0;
  float timeWall=0;
  int nE=0;

  bookHistograms();
 
  //Loop over the datasets ===================
  for(size_t ids=0;ids<_dsnames.size();ids++) {
  
    _ids = _dsnames[ids];
    _inds = ids;
    
    Dataset* ds=anConf.getDataset( _ids );
    string tmpDs= _ids;
    if( ds->isGhost() ) _ids+="GHO";
    
    //cout<<_ids<<" !!! is data driven "<<ds->isDataDriven()<<"   "<<ds->isFromCS()<<endl;
    //cout<<" !! is ghost ? "<< ds->isGhost()<<endl;

    _isData = ds->isDataset("data") || ds->isDataset("Data");
    
    _nevts = ds->getNEvents();
    if( _testNMax!=(size_t)-1) _nevts=_testNMax;

    _vc->reset();
    _hm->initStatus();
    
    if( ds->isFromCS() && (_nevts==0 || _testNMax!=(size_t)-1) ) continue;
    if(ds->isDataDriven() ) continue;
    
    if(_skim && _nevts==0) {
      _storeTuple=true;
      finalizeSkimming();
    }
  
    //protection against empty trees
    if(_nevts==0) continue;
    cout<<" Starting processing dataset : "<<ds->getName()<<endl;
    
    boost::progress_display show_progress( _nevts );
    for(_ie=-1;_ie<_nevts;_ie++) { //-1 for the initialization
      ++show_progress;
      
      stw.Start();
      
      if(_ie==-1) {
	_au->changeInitStatus(true);

      }
      if(_ie==0) { //first scan done, now intitialize stuff
	_vc->finalizeInit();
	_vc->buildTree( ds->getTree() , _skim&&_fullSkim );
	_hm->startFilling();
	_au->changeInitStatus(false);
      }
      
      if(_skim) {
	finalizeSkimming();
	initSkimming();
      }
      
      ds->getTree()->GetEntry(_ie);
      
      _w = anConf.getWeight( tmpDs, _ie );
      _wBack = _w;
      
      if(_isData )
	{
	  //FIXME
	  if(!checkDoubleCount()) continue;
	}
      
      modifyWeight();
      
      _isDDBkg=0;

      //integrated systematic uncertainties
      if(_isData)
	analyze();
      else { //simulation
	_unc = "";//reinitialization
	_au->setUncSrc("", SystUtils::kNone );
	
	analyze(); //central analysis
	//cout<<" ==> "<<_unc<<endl;
	//and systematic uncertainties after

	for(size_t iu=0;iu<_uncSrcs.size();iu++) {
	  _w = _wBack;
	  if(iu==0) _vc->nextEvent();
	  else _vc->sameEvent();
	  //cout<<" starting : "<<iu<<"   "<<_uncSrcs[iu]<<"  "<<_uncSrcs.size()<<endl;
	  _unc = _uncSrcs[iu];
	  _uDir = _uncDirs[iu];
	  _au->setUncSrc(_unc, _uDir );
	  applySystVar( _vc->_su->getSystInfos(_unc, _uDir) );
	  analyze();
	  _vc->backPortAllVars();
	  //reinitVars( _vc->_su->getSystInfos(_unc, _uDir).modVar );
	}
      }
      
      timeWall+=stw.RealTime();
      timeCPU+=stw.CpuTime();
      nE++;

    }//events

    //skimming for last dataset
    if(_skim && ids==_dsnames.size()-1 ) {
      //cout<<" gloubi "<<_storeTuple<<endl;
      finalizeSkimming();
    }
  }//datasets	
  
  cout<<" End processing datasets "<<endl;
  
  cout<<" +++ Execution frequency +++ "<<endl;
  cout<<"   Real time = "<<timeWall/nE<<" s/evt "<<" -> "<<nE/timeWall<<" Hz"<<endl;
  cout<<"   CPU time = "<<timeCPU/nE<<" s/evt "<<" -> "<<nE/timeCPU<<" Hz"<<endl<<endl;

  cout<<" Filling data-driven backgrounds "<<endl;
  _hm->fillDDbkg();
  cout<<" Filling backgrounds from control samples "<<endl;
  _hm->fillCSbkg();

  //end processing, last configurable analysis actions
  endAnalysis();

  
}

void
UseTree::bookHistograms() {

}



bool
UseTree::checkDoubleCount() {
  //cout<<" --> "<<_vc->tryType("run","UI")<<"  "<<_vc->tryType("run","I")<<endl;
  int run=_vc->tryType("run","UI")?_vc->getUI("run"):_vc->getI("run");
  int event=_vc->tryType("event","UI")?_vc->getUI("event"):_vc->getI("event");
  
  if(_ie==-1) return true;

  bool doubleCount=false;
  std::pair<int,int> tmp(run,event);
  _itEvt = _events.find( tmp );
  if(_itEvt != _events.end() ) {
    doubleCount=true;
    //abort(); ?? FIXME -> no abort by default
  }
  int nT = 1;
  if(doubleCount)// || !anConf.passRunFilter(run) )
    { 
      if(_itEvt->second.second==1) 
	_nDC++;
      else
	cout<<" ==> multiple counting "<< _ids<<"  "<<run<<"  "<<event<<"  "<< anConf.getDataset(_ids)->findProcess(_ie)<<endl;
      nT = _itEvt->second.second +1;
      //cout<<" ==> double counting "<< _ids<<"  "<<run<<"  "<<event<<"  "<< anConf.getDataset(_ids)->findProcess(_ie)<<endl;
      return false; }
  
 
  string t1("");//,t2("");
  std::pair<string,int> tmp2( t1, nT );
	  
  _events[ tmp ] = tmp2;
  _evtsInFile.push_back(event);
   
  return true;
}



void
UseTree::doAnalysis() {

  //reconfigure the histo manager 
  //and the analysis -> permits to change the colors and 
  // eventually the removal of one process on the fly
  configure();

  //tree analysis
  if(_treeAnalysis) {
    if(_recompute) {
      //reseting everything
      //if(!_init)
      //reset();
      
      //redo the analysis
      globalAnalyze();
      
      _recompute=false;
      _init=false;
    }
  }
  else {
    ah.access();
    setHistograms();
  }
  
  //See if a fit is needed for the normalization
  //ugly....
  string fitVar=dp.getFitVar();
  if(fitVar!="") {
    const hObs* wobs = _hm->getHObs( fitVar );
    if( wobs!=NULL)
      dp.initWeights(wobs); 
    else
      cout<<" Error, no observable of name : "<<fitVar<<" for fitting "<<endl;
  }
  
  //Find the observables and draw them
  //could be done in a better way...
  vector<string> obs_ = dp.getObservables();
  vector<const hObs*> Obs_;
  vector<vector<systM> > systs_;
  
  for(size_t io=0;io<obs_.size();io++) {
    //cout<<" !!!!! "<< obs_[io]<<"  "<<_hm->getHObs( obs_[io] )<<endl;
    _currentObs=obs_[io];

    if( _hm->getHObs( obs_[io] )==NULL) {
      cout<<" Error, no observable of name : "<<obs_[io]<<endl;
      continue;
    }

    Obs_.push_back( _hm->getHObs( obs_[io] ) );
    systs_.push_back( _hm->getSystObs( obs_[io] ) );
  }  
  
  if(Obs_.size()!=0) {
    dp.setSystematicsUnc( systs_ );
    dp.plotDistributions( Obs_ );
  }

}

void
UseTree::reset() {

  _hm->reset();
  _vc->reset();
  
  dp.reset();
  
  _au->reset();
  anConf.reset();

}


void
UseTree::drawRatio(string o1, string o2 ) {

  vector<const hObs*> Obs_;
  // vector<vector<systM> > systs_;
  
  Obs_.push_back( _hm->getHObs( o1 ) );
  Obs_.push_back( _hm->getHObs( o2 ) );

  if( Obs_[0]==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
 if( Obs_[1]==NULL) {
    cout<<" Error, no observable of name : "<<o2<<endl;
    return;
  }

  // systs_.push_back( _hm->getSystObs( o1 ) );
  // systs_.push_back( _hm->getSystObs( o2 ) );
  
  //dp.setSystematicsUnc( systs_ );
  dp.ratioObservables( Obs_ );

}

void
UseTree::drawResiduals(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );
  systs_.push_back( _hm->getSystObs( o1 ) );
  dp.setSystematicsUnc( systs_ );
  dp.residualData( Obs_ );
}

void
UseTree::drawSignificance(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.showSignificance( Obs_ );
}


void
UseTree::drawCumulativePlots(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawCumulativeHistos( Obs_ );
}

void
UseTree::drawEfficiencies(string o1) {
  const hObs* Obs_;
  
  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawEfficiency( Obs_ );
}


void
UseTree::drawROCcurves(string o1) {
  const hObs* Obs_;
  
  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawROCCurves( Obs_ );
}


void
UseTree::multiROCcurves() {
  vector<const hObs*> Obs_;
  vector<string> obs_ = dp.getObservables();
  
  for(size_t io=0;io<obs_.size();io++) {
    if( _hm->getHObs( obs_[io] )==NULL) {
      cout<<" Error, no observable of name : "<<obs_[io]<<endl;
      continue;
    }
    
    Obs_.push_back( _hm->getHObs( obs_[io] ) );
  }  
  
  dp.compaROCCurves( Obs_ );
}


void
UseTree::saveHistosLQ(string o1) {
  //const hObs* Obs_;
  //Obs_ =  _hm->getHObs( o1 );

  // if( Obs_==NULL) {
  //   cout<<" Error, no observable of name : "<<o1<<endl;
  //   return;
  // }
  
  dp.saveHistosSpecLQ(o1);

}

void
UseTree::saveHistos(string o1) {
  const hObs* Obs_;
  Obs_ =  _hm->getHObs( o1 );

   if( Obs_==NULL) {
     cout<<" Error, no observable of name : "<<o1<<endl;
     return;
   }
   
   dp.saveHistos(o1, Obs_);

}


void
UseTree::saveDataMCRatio(string fname,string hname) {
  dp.saveDMCRWeight(fname,hname);
}


void
UseTree::fill(string var, float valx, float weight) {
  //

  if(_unc=="")
    _hm->fill(var, _inds, valx,(_isDDBkg==0)?weight:(weight* modWeightDDB()) , _isDDBkg );
  else {
    if(_uDir==SystUtils::kUp)
      fillUnc(var,_unc,valx,weight,"Up");
    if(_uDir==SystUtils::kDown)
      fillUnc(var,_unc,valx,weight,"Do");
  }    
}

void
UseTree::fill(string var, float valx, float valy, float weight) {
  //(_isDDBkg==0)?weight:(weight* modWeightDDB())
  _hm->fill(var, _inds, valx, valy,(_isDDBkg==0)?weight:(weight* modWeightDDB()) , _isDDBkg );
}

void
UseTree::fillUnc(string var, string unc, float val, float weight, string dir) {
  if(!_isData)
    _hm->fill( var, unc, val, weight,dir);
}

void
UseTree::fillUnc(string var, string unc, float val, float weight,
		 float wup, float wdo) {
  _hm->fill(var, _inds, val,(_isDDBkg==0)?weight:(weight* modWeightDDB()) , _isDDBkg );
  if(!_isData && _isDDBkg==0 ) {
    _hm->fill( var, unc, val, wup,"Up");
    _hm->fill( var, unc, val, wdo,"Do");
  }
}

void
UseTree::setHistograms() {

  TH1* htmp(0); 
  for(size_t ih=0;ih<ah.getNHistos();ih++) {
    htmp = ah.getHistogram(ih);
    
    _inds = anConf.findChan( htmp->GetName() );
    
    bool isUnc=((string)htmp->GetName()).find("Unc")!=(size_t)-1;
    if(isUnc) _hm->copyHisto( ah.getVar(), -10, htmp, ah.getXtitle() );
    if(_inds==-1) continue;
    _hm->copyHisto( ah.getVar(), _inds, htmp, ah.getXtitle() );
    
  }
  delete htmp;

}

void
UseTree::modifySkimming() {

}

void
UseTree::initSkimming() {
  
  _storeTuple=false;
  if(_ie==-1) return;

  string tmpDs = anConf.getSample(_ids,_ie);
  if(_wDs!=tmpDs) {
    _oFile = new TFile( ("Skimming/"+tmpDs+"_skim.root").c_str(),"RECREATE");
    anConf.getDataset( _ids )->getTree()->LoadTree(0);
    if(_fullSkim) {
      _skimTree = (TTree*)anConf.getDataset( _ids )->getTree()->CloneTree(0);
      _hnSkim =new TH1I( anConf.getHName().c_str(), anConf.getHName().c_str(), 1, 0, 1);
      _hnSkim->SetBinContent(1,anConf.getDataset( _ids )->getNProcEvent(_ie) );
    }
    else {
      TString name = anConf.getDataset( _ids )->getTree()->GetName();
      _skimTree = new TTree( name, name );
    }
    _skimTree->SetDirectory( _oFile );

    modifySkimming();
    _wDs = tmpDs;
  }

  if(_ie+1<_nevts)
    tmpDs = anConf.getSample(_ids,_ie+1);
  if(_wDs!=tmpDs || _ie+1==_nevts) {
    _storeTuple=true;

  }
  
}

void
UseTree::finalizeSkimming() {

   if(!_storeTuple) return;

   _oFile->cd();
   _skimTree->Write();
   if(_hnSkim)
     _hnSkim->Write();
   _oFile->Write();
   _oFile->Close();
}


bool
UseTree::isInitStatus() {
  return _recompute;
}

void
UseTree::unlockStatus() {
  reset();
  _recompute=true;
  
}


void
UseTree::savePlot(string path, string advname) {
  _hm->savePlots(path, dp.getCanvas(), advname);
    
}

void
UseTree::producePlots(string path) {
  
  vector<string> obss = _hm->getObservables();

  for(size_t is=0;is<obss.size();is++) {
    refresh();
    dp.setObservables( obss[is] );
    doAnalysis();
    savePlot(path);
  }

}


void
UseTree::getStatistics() {
  _au->getNumbers();
}

void
UseTree::getYield(string ds, string lvl) {
  _au->getYieldSysts(ds, lvl);
}

void
UseTree::drawStatistics() {
  vector<pair<string,vector<vector<float> > > > valsStat = _au->retrieveNumbers();
  dp.drawStatistics( valsStat );
}


void
UseTree::drawDetailSyst(bool cumul) {
  vector<vector<systM> > systs_;
  systs_.push_back( _hm->getSystObs( _currentObs ) );
  dp.setSystematicsUnc( systs_ );
  dp.drawDetailSystematics( cumul );
}

void
UseTree::getIntegral(float x1, float x2, float y1, float y2) {
  dp.getIntegral(x1, x2, y1, y2);
}

void
UseTree::refresh() {
  dp.reset();
}



float
UseTree::modWeightDDB() {
  return 1.;
}


bool
UseTree::mif(bool b) {
  
  return _au->simpleCut(b);
}


void
UseTree::applySystVar(SystST s) {

  //reinitializing variables
  
  //protection against weight variation
  if(_uType[ _unc ]) return;

  if(s.val!=-100) {
    //fixed variation
    for(size_t iv=0;iv<s.modVar.size();iv++) //loop over variables
      _vc->applySystVar(_unc, _uDir, s.modVar[iv], s.val, s.type);
  }
  else {
    //db variation
    for(size_t iv=0;iv<s.modVar.size();iv++) //loop over variables
      _vc->applySystVar(_unc, _uDir, s.modVar[iv], s.vars, s.db, s.type);
  }

}

// void
// UseTree::reinitVars( vector<string> mvars ) {
//   for(size_t iv=0;iv<mvars.size();iv++)
//     _vc->reinitVars( mvars[iv] );
// }

float
UseTree::getDBVal(string db, float v1, float v2, float v3, float v4, float v5,
		  float v6, float v7, float v8, float v9, float v10) {
  
  if(_unc=="")
    return _dbm->getDBValue(db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
  else
    return applySystDBVar( _vc->_su->getSystInfos(_unc, _uDir), db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);

}

float
UseTree::applySystDBVar(SystST s, string db, float v1, float v2, float v3, float v4,
		       float v5,float v6,float v7,float v8,float v9, float v10 ) {

  float tmpw= _dbm->getDBValue(db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );

  //not the good variation
  if(db!=s.db) {
    return tmpw;
  }
  
  //variation on event weight
  if(s.val!=-100) {
    //fixed variation
    //cout<<tmpw<<endl;
    _vc->_su->systOpW(_unc, _uDir, s.type , tmpw, s.val);
    //cout<<tmpw<<endl;
  }
  else {
    //db variation
    vector<float> v(10,-100000);
    v[0]=v1; v[1]=v2; v[2]=v3; v[3]=v4; v[4]=v5;
    v[5]=v6; v[6]=v7; v[7]=v8; v[8]=v9; v[9]=v10;
    _vc->_su->systOpW(_unc, _uDir, s.type , tmpw, s.db, v);
  }
  
  return tmpw;
}

void
UseTree::addWSystSource(string name, int dir, string type, float val) {
  vector<string> t;
  addSystSource(name, dir, type, t, val, true);
}

void
UseTree::addWSystSource(string name, int dir, string type, string db, string hname) {
  vector<string> t;
  addSystSource(name, dir, type, t, db, hname, true);
}

void
UseTree::addSystSource(string name, int dir, string type, vector<string> modVar,
			     float val, bool wUnc) {

  _uType[ name ] = wUnc;

  //check the direction
  if(dir!=SystUtils::kNone) {
    _uncSrcs.push_back(name);
    _uncDirs.push_back(dir);
    _vc->_su->addSystSource(name, dir, type, modVar, val);
  }
  else {
    //up variation
    _uncSrcs.push_back(name);
    _uncDirs.push_back(SystUtils::kUp);
    _vc->_su->addSystSource(name, SystUtils::kUp, type, modVar, val);

    //down variation
    _uncSrcs.push_back(name);
    _uncDirs.push_back(SystUtils::kDown);
    _vc->_su->addSystSource(name, SystUtils::kDown, type, modVar, val);
  }

}

void
UseTree::addSystSource(string name, int dir, string type, vector<string> modVar,
		       string db, string hname, bool wUnc) {
  
  _uType[ name ] = wUnc;

  //check the direction
  if(dir!=SystUtils::kNone) {
    _uncSrcs.push_back(name);
    _uncDirs.push_back(dir);
    _vc->_su->addSystSource(name, dir, type, modVar, db, hname);
  }
  else {
    //up variation
    _uncSrcs.push_back(name);
    _uncDirs.push_back(SystUtils::kUp);
    _vc->_su->addSystSource(name, SystUtils::kUp, type, modVar, db, hname);
    
    //down variation
    _uncSrcs.push_back(name);
    _uncDirs.push_back(SystUtils::kDown);
    _vc->_su->addSystSource(name, SystUtils::kDown, type, modVar, db, hname);
  }

}
