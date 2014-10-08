#include "core/Dataset.hh"

using namespace std;

ClassImp(Dataset)

Dataset::Dataset():
_chain(0)
{
  _isDataDriven=false;
  _isGhost=false;
  _skiptree=false;

  _testNMax=(size_t)-1;
}

Dataset::Dataset(string name, int color):
_chain(0)
{
  _name=name;
  _color=color;
  _isDataDriven=false;
  _isGhost=false;
  _skiptree=false;

  _testNMax=(size_t)-1;
}

Dataset::~Dataset() {
  delete _chain;
}

void
Dataset::setNMax(size_t nmax) {
  _testNMax=nmax;
}

void
Dataset::isHistoAnalysis(bool skiptree) {
  _skiptree=skiptree;
}

void Dataset::addSample(string sname, string path, string dir, string treeName, string hname, float xSect, float kFact, float lumi, float eqLumi) {
  
  //protection against double loading
  for(size_t is=0;is<_samples.size();is++) {
    if(_samples[is].getName()==sname) {
      //cout<<" Debug "<<__FILE__<<"  "<<__LINE__<<endl;
      return;
    }
  }
  
  //Chain initialisation =========
  if(!_skiptree)
    if(_chain==NULL)
      _chain = new TChain(treeName.c_str());
  //======

  //is Data driven?
  if(sname.find("DD")!=(size_t)-1 ) {
    _isDataDriven=true;
 
    Sample s(sname, 0, xSect, kFact, eqLumi);
     _samples.push_back(s);
    _weights.push_back(1.);
 
    return;
  }

  //is from Control Sample?
  _isFromCS=0;
  if(sname.find("CS")!=(size_t)-1 ) {
    _isFromCS=1;
    if(sname.find("CSS")!=(size_t)-1)
      _isFromCS=2;
    if(sname.find("CSC")!=(size_t)-1)
      _isFromCS=3;
    if(sname.find("CSN")!=(size_t)-1)
      _isFromCS=4;
    
    if(sname.find("OCS")!=(size_t)-1 )
      _isFromCS+=10;

    Sample s(sname, 0, xSect, kFact, eqLumi);
    _samples.push_back(s);
    _weights.push_back(1.);
 
    return;
  }
  
  //is ghost?
  //cout<<sname<<" ghost? "<<(sname.find("ghost")!=(size_t)-1 )<<endl;
  if(sname.find("ghost")!=(size_t)-1 ) {
    _isGhost =true;
    //cout<<" !!! ghost!!! "<<endl;
    size_t p0=sname.find(" ");
    sname=sname.substr(p0+1,sname.size()-p0-1) ;
  }



  //tree analysis 
 
  if(!_skiptree) {
 
    TFile* datafile(NULL);
  
    string NameF = path+"/data/"+dir+"/"+sname+".root"; 
    if(path.find(":")!=(size_t)-1) NameF=dir+"/"+sname+".root";

    datafile = TFile::Open(NameF.c_str());
    if(datafile==NULL) { 
      cout<<" No such file "<<sname<<endl; return;
    }

 
    //Looking for the tree if not data-driven
    int nEvent = 0;

    TTree* tmptree = (TTree*)datafile->Get( treeName.c_str() );
  
    if(tmptree != NULL ) {
      // cout <<"Tree  --> "<<NameF+"/"+treeName
      // 	 <<"   "<<tmptree->GetEntries()<<"   "<<_chain->GetEntries()<<endl;
      _chain->Add( (NameF+"/"+treeName).c_str() ); 

      nEvent = tmptree->GetEntries();	 
    }
    delete tmptree;
 
    //event database

    bool empty=_events.size()==0;
    int m=empty?0:_events.back().second;
    int M=empty?nEvent:_events.back().second+nEvent;
    pair<int,int> p(m,M);
    _events.push_back(p);
  
    //sample database
    int nProcEvt =0;
 
    //if(_name.find("data")==(size_t)-1 && _name.find("Data")==(size_t)-1 )
    nProcEvt = getNProcEvents(path, dir, sname, hname);
    // else
    //   nProcEvt=0;

    float weight = computeWeight(nProcEvt,xSect,kFact,lumi,eqLumi);

    // if(_testNMax!=(size_t)-1) {
    //   weight *=  min(nEvent,(int)_testNMax)/(float)nEvent;
    // }
    Sample s(sname, nEvent, xSect, kFact, eqLumi);
    _samples.push_back(s);
    _weights.push_back(weight);

    cout<<" Adding "<<sname+".root"<<"  to "<<_name
	<<"   :  nEvt "<<_chain->GetEntries()<<" ("<<nProcEvt
	<<" gen) "<<" / w= "<<weight<<endl;
  }
  else {
    Sample s(sname, 0, 0, 0, 0);
    _samples.push_back(s);
    _weights.push_back(1);
  }

}


int
Dataset::getNProcEvents(string path, string dir, string sname, string hname) {
  string NameF = path+"/data/"+dir+"/"+sname+".root";
  
  if(dir.find(":")!=(size_t)-1) NameF=path+"/"+sname+".root";	 
  TFile* file = TFile::Open( NameF.c_str() );
  TH1* htmp = (TH1*)file->Get(hname.c_str());
  
   int nProc=0;
  if(htmp) {
    nProc = htmp->Integral(0,1001);
    delete htmp;
  }
  else nProc = 0;

  file->Close();

  return nProc;
}

int
Dataset::getNProcEvent(int evt) {
  
  for(size_t iv=0;iv<_events.size();iv++) {
    if(evt>=_events[iv].first && evt<_events[iv].second) {
      return _nprocs[iv];
    }
  }
  return 0;

}

float Dataset::computeWeight(float nEvts, float xSect, float kFact, float lumi, float& eqLumi) {
  
  float weight=1.;

  if(eqLumi==-1) { //means that Xsection used for reweigthing
    eqLumi = nEvts/xSect;
  }
  
  weight =(1./eqLumi)*lumi*kFact;
  return weight;
}


string 
Dataset::findProcess(int evt ) {

  for(size_t iv=0;iv<_events.size();iv++) {
    if(evt>=_events[iv].first && evt<_events[iv].second) {
      return _samples[iv].getName();
    }
  }
  return "";
}


float
Dataset::findWeight(int evt ) {
  
  for(size_t iv=0;iv<_events.size();iv++) {
    if(evt>=_events[iv].first && evt<_events[iv].second) {
      return _weights[iv];
    }
  }
  return 0;
}


vector<string>
Dataset::getSamples() {
  vector<string> snames;
  for(size_t is=0;is<_samples.size();is++) {
    snames.push_back( _samples[is].getName() );
  }
  return snames;
}
