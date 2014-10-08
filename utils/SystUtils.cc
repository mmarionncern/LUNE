#include "utils/SystUtils.hh"

using namespace std;

ClassImp(SystUtils)

SystUtils::SystUtils() {
}

SystUtils::~SystUtils() {
}

string SystUtils::uDir[SystUtils::kNDirs]={
  "Up",
  "Do",
  ""
};


vector<string>
SystUtils::parseDb(string db) {

  vector<string> vars;

  size_t ib=0;
  size_t ie=0;
  while(ie!=(size_t)-1) {
    ie = db.find(":",ib);
    vars.push_back( db.substr(ib,ie) );
    //cout<<vars.back()<<"  "<<ib<<"  "<<ie<<" / "<<(size_t)-1<<endl;
    ib = ie+1;
  }

  return vars;
}

void
SystUtils::addSystSource(string name, int dir, string type, vector<string> modVar, 
			 float val) {

  vector<string> pvars;// = parseDb(vars);
 
  
  SystST tmp;
  tmp.type = type;
  tmp.val = val;
  tmp.db = name;
  tmp.dir = dir;
  tmp.vars = pvars;
  tmp.modVar = modVar;

  string sName=name;
  if(dir==kUp) sName+="Up";
  if(dir==kDown) sName+="Do";
  
  _uncSrcs[ sName ] = tmp;
}

void
SystUtils::addSystSource(string name, int dir, string type, vector<string> modVar,
			 string db, string hname) {

  vector<string> dbVars = parseDb(db);
  
  string file = dbVars[0];
 
  SystST tmp;
  tmp.type = type;
  tmp.val = -100;
  tmp.db = name;  
  tmp.dir = dir;
  dbVars.erase( dbVars.begin() );
  tmp.vars = dbVars;
  tmp.modVar = modVar;

 
  string sName=name;
  if(dir==kUp) sName+="Up";
  if(dir==kDown) sName+="Do";

  
  if(hname!="" && hname!="e" && hname!="E")
    _dbm->loadDb(db, file , hname);


  _uncSrcs[ sName ] = tmp;
}


SystST
SystUtils::getSystInfos(string name, int dir) {
  
  map<string,SystST>::const_iterator it;
  
  if(dir==kUp) name+="Up";
  if(dir==kDown) name+="Do";

  it = _uncSrcs.find(name);
  if(it==_uncSrcs.end()) {
    cout<<" Error, no "<<name<<" systematic uncertainty detected, emergency abort "<<endl;
    abort();
  }

  return it->second;
}

void
SystUtils::systOpW(string name, int dir, string type, float& w,
		   string db, vector<float> vals) {

  float dw=0;
  if(dir==kUp) dw = _dbm->getDBErrH(db,vals[0],vals[1],vals[2],vals[3],vals[4], 
				    vals[5], vals[6], vals[7], vals[8], vals[9]);
  if(dir==kDown) dw = _dbm->getDBErrL(db,vals[0],vals[1],vals[2],vals[3],vals[4], 
				      vals[5], vals[6], vals[7], vals[8], vals[9]);
  
  systOp<float>(name, dir, type, w, dw);
  //cout<<" :=> "<<w<<endl;

}

void
SystUtils::systOpW(string name, int dir, string type, float& w, float x) {
  systOp<float>(name, dir, type, w, x);
}
