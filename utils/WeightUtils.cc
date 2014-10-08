#include "utils/WeightUtils.hh"

using namespace std;

ClassImp(WeightUtils)

WeightUtils::WeightUtils() {

}


WeightUtils::~WeightUtils() {

}


void 
WeightUtils::reset() {

  _wHistos.clear();
  _wDB.clear();
  _wVar.clear();
  _wVar2D.clear();

}

void 
WeightUtils::addWeight( string wname, TH1* wh ) {

  _itWH=_wHistos.find(wname);
  if(_itWH!=_wHistos.end()) {
    cout<<" Error, weight histogram "<<wname<<" already loaded" <<endl;
    return;
  }
    
  _wHistos[ wname ]=wh;
}


void 
WeightUtils::addWeightH(string wname, string loc, string hname ) {

  TFile* file=TFile::Open(loc.c_str() );
  if(file==NULL) { cout<<" Error, no such file "<<loc<<endl; abort();}

  TH1* htmp=(TH1*)file->Get( hname.c_str() );
  addWeight(wname, htmp);

}



void 
WeightUtils::addWeightDB(string wname, string loc, string fname ) {



}


float WeightUtils::findWeight(string wname, float valx, float valy) {

  _itWH = _wHistos.find(wname);
  if(_itWH!=_wHistos.end()) {
    return _itWH->second->GetBinContent( _itWH->second->GetXaxis()->FindBin( valx ));
  }
  else {
    _itWDB = _wDB.find(wname);
    if(_itWDB!=_wDB.end()) {
      return 1.;
    }
    else
      {
	cout<<" No such weight! "<<wname<<endl; 
	return 1;
      }
  }
}
