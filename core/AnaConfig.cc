#include "core/AnaConfig.hh"

using namespace std;


AnaConfig::AnaConfig() {

  _dir="";
  _path=string(getenv ("LUNE"));

  _useXSect=true;
  _noData=false;
 
  _skiptree=false;

  _lumi=0;
  _runFilter=0;
  _runNum=0;

  _testNMax=(size_t)-1;
  
}

AnaConfig::~AnaConfig() {
  reset();
}


void AnaConfig::reset() {

  _dir="";
  _path=string(getenv ("LUNE"));

  for(_itDs=_datasets.begin();_itDs!=_datasets.end();_itDs++)
    delete _itDs->second;
  
  _datasets.clear();
  _numDS.clear();

  _lumi=0;
  _runFilter=0;
  _runNum=0;

  _useXSect=true;
  _noData=false;
  _skiptree=false;
  
}

void AnaConfig::setNMax(size_t testNMax) {
  _testNMax = testNMax;
}

float AnaConfig::getWeight(int channel, int evt) {

  if(evt==-1) return 0; //shield against initalization step
  
  string chan = findDS(channel);
  return getWeight(chan,evt);
}


float AnaConfig::getWeight(string chan, int evt) {

  if(evt==-1) return 0; //shield against initalization step
  return _datasets[chan]->findWeight(evt);
}


string AnaConfig::getSample(int channel, int evt) {

  if(evt==-1) return ""; //shield against initalization step
  
  string chan = findDS(channel);
  return getSample(chan, evt);

  cout<<" ********************** Warning Dataset "<<endl;
  return "";
}

string AnaConfig::getSample(string chan, int evt) {

  if(evt==-1) return ""; //shield against initalization step
  
  _itDs=_datasets.find(chan);
  if(_itDs!=_datasets.end()) {
    return _itDs->second->findProcess(evt);
  }
  
  cout<<" ********************** Warning dataset : "<<chan<<endl;
  return "";
}

Dataset*
AnaConfig::getDataset(string name) {
  return  _datasets[ name ];
}

string 
AnaConfig::findDS(int channel) {

  _itNDS=_numDS.find(channel);
  if(_itNDS==_numDS.end()) {
    cout<<" Warning, no dataset for the channel "<<channel<<endl;
    return "";
  }
  
  return _itNDS->second;
}

int
AnaConfig::findChan(string ds) {
  for(_itNDS=_numDS.begin();_itNDS!=_numDS.end();_itNDS++) {
  
    vector<string> samples = _datasets[ _itNDS->second ]->getSamples();
    for(size_t is=0;is<samples.size();is++) {
      if(ds == samples[is]) return _itNDS->first;
    }
  }

  cout<<" Warning, no channel for the dataset "<<ds<<endl;
  return -1;
}

void AnaConfig::configureLumi(map<string,float> LumisXS, map<string,float> Kfac, float l,bool useXS) {
  
  _xSecLumis = LumisXS;
  _kFactors = Kfac;
  _lumi =l;
  _useXSect = useXS;
  
}

void 
AnaConfig::configureNames(string dir, string treeName, string hName) {
  _dir = dir;
  _treeName = treeName;
  _hname = hName;
}


void AnaConfig::configureData(bool runfilter, int runnum,bool MCOnly ) {
  _runFilter=runfilter;
  _runNum=runnum;

  _noData = MCOnly;
}

void
AnaConfig::isHistoAnalysis() {
  _skiptree=true;
}


void
AnaConfig::addSample( string str, string sname, int col) {
  
  if( str.find("DD")!=(size_t)-1 ) {
    _ddCuts.push_back(str.substr(3, str.size()-3));
    //sname+="DD";
  }

  if( str.find("CS")!=(size_t)-1 ){
    //parse the line
    size_t p0=str.find(" ");
    size_t p1=str.find(" ",p0+1);
    
    float norm;
    string name;
    if(p0!=(size_t)-1 && p1!=(size_t)-1) {
      norm=atof( (str.substr(p0,p1-p0)).c_str() );
      name=str.substr(p1+1,str.size()-p1-1);
    }
    else {
      norm=0;
      name=str.substr(p0+1,str.size()-p0-1);
    }
    //cout<<" norm "<<norm<<"  "<<name<<endl;
    _csData.push_back( pair<string, float>(name,norm) );
  }
  
  // if( str.find("ghost")!=(size_t)-1 ){
  //   //parse the line
  //   //size_t p0=str.find(" ");
  //   //  str=str.substr(p0+1,str.size()-p0-1) ;
  // }
  
  _itDs=_datasets.find(sname);
  
  if(_itDs==_datasets.end() ) {
    Dataset* ds=new Dataset(sname,col);
    _datasets[ sname ] = ds;
    _numDS[ _numDS.size() ] = sname;
    _datasets[ sname ]->setNMax(_testNMax);
    _datasets[ sname ]->isHistoAnalysis(_skiptree);
  }
  
  // for(_itDs=_datasets.begin();_itDs!=_datasets.end();_itDs++)
  //   cout<<_itDs->first<<"   "<<_itDs->second->getName()<<endl;
  // for(_itNDS=_numDS.begin();_itNDS!=_numDS.end();_itNDS++)
  //   cout<<_itNDS->first<<"   "<<_itNDS->second<<endl;
  
 

  if(sname=="data" || sname=="Data" || 
     str.find("DD")!=(size_t)-1 ||
     str.find("CS")!=(size_t)-1 ) {
    
    _datasets[ sname ]->addSample(str, _path, _dir, _treeName,
				  _hname,0., 1., 1., 1.);
    
    return;
  }
  
  //histogram analysis
  if(!_skiptree) {

    //find xSect/kFact/eqLumi
    float xSect=1.,kFact=1.,eqLumi=1.;
    //for(_itXS=_xSecLumis.begin();_itXS!=_xSecLumis.end();_itXS++)
    string tmpStr=str;
    if(str.find("ghost")!=(size_t)-1) {
      size_t p0=str.find(" ");
      tmpStr=str.substr(p0+1,str.size()-p0-1) ;
    }

    _itXS=_xSecLumis.find(tmpStr);
    if(_itXS==_xSecLumis.end()) {
      xSect =1.; eqLumi=1.;
      cout<<" Be careful, no specified "
	  <<(string)((_useXSect)?" cross section":" equivalent luminosity")
	  <<" for "<<str<<" , 1 by default *****======== "<<endl;
    
      //FIXME, set weight to 1 by compensating with the lumi
      xSect =1./_lumi; eqLumi=_lumi;
    }
    else {
      if(_useXSect || _hname=="")
	{eqLumi=-1; xSect =_itXS->second ;}
      else  
	{eqLumi=_itXS->second; xSect =-1;}
    }
  
    _itKF=_kFactors.find(tmpStr);
    if(_itKF!=_kFactors.end() ) {
      kFact = _itKF->second;
    }

    _datasets[ sname ]->addSample(str, _path, _dir, _treeName,
				  _hname, xSect, kFact, _lumi,
				  eqLumi);
  }
  else {
    cout<<sname<<" --> "<<str<<endl;
    _datasets[ sname ]->addSample(str, "", "", "", "", 0, 0, 0, 0);
  }
  //cout<<" ya? "<< _datasets[ sname ]<<"  "<<sname<<"  "<<str<<endl;
 
}


bool
AnaConfig::passRunFilter(int run) {
  if(!_runFilter) return true;
  if(run > _runNum ) return false;
  return true;
}


vector<string > 
AnaConfig::getDSNames() {
 
  vector<string> names;

  // for(_itDs=_datasets.begin();_itDs!=_datasets.end();_itDs++) {
  //   cout<<_itDs->first<<"   "<<_itDs->second->getName()<<endl;
  //   names.push_back( _itDs->second->getName() );
  // }
  for(_itNDS=_numDS.begin();_itNDS!=_numDS.end();_itNDS++) {
    _itDs = _datasets.find( _itNDS->second );
    names.push_back( _itDs->second->getName() );
  }
  
  return names;
}

vector<int>
AnaConfig::getDDDSStatus() {
  vector<int> status;

  for(_itNDS=_numDS.begin();_itNDS!=_numDS.end();_itNDS++) {
    _itDs = _datasets.find( _itNDS->second );
    status.push_back( _itDs->second->isDataDriven() + _itDs->second->csCode()*2 );
  }

  return status;
}

vector<pair<string, float> >
AnaConfig::getCSData() {
  return _csData;
}

bool
AnaConfig::isNoDataAna() {
  
  bool mcOnly;

  _itDs = _datasets.find("data");
  map<string, Dataset* >::iterator _itDs2;
  _itDs2 = _datasets.find("data");
  if( _itDs==_datasets.end() && _itDs2==_datasets.end() )
    mcOnly = true;
  else {
    mcOnly = _noData;
  }
  
  return mcOnly;
}


string
AnaConfig::getHName() {


  if(_hname.find("/")==(size_t)-1)
    {return _hname;}
  else {
    size_t p = _hname.find("/");

    return _hname.substr(p+1, _hname.size()-p-1);

  }

}
