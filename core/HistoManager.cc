#include "HistoManager.hh"
#include <assert.h>

#include "utils/AnaUtils.hh"
#include "utils/HistoUtils.hh"

using namespace std;

ClassImp(HistoManager)

HistoManager::HistoManager() {
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TH1::SetDefaultSumw2(true);

  _initsequence=true;
}

HistoManager::~HistoManager() {
}

 //initilaization

void
HistoManager::configAnalysis(vector<string> datasets, int nddBkg, vector<int> ddStatus, vector<pair<string, float> > csData)  {
  _dsNames = datasets;
  _ddStatus = ddStatus;
  _csData = csData;
  _nds = datasets.size(); 
  _nDDBkg=nddBkg;
  _varDDBkg.resize(_nDDBkg);
}


 //Histogram access and booking

void 
HistoManager::addVariable(string var, int nBin, float min, float max, string Xleg, bool prof, string type) {

  //Protection against overdeclaration
 
  _itVar = _variables.find(var);
  if(_itVar!=_variables.end()) {
    cout<<" Take care : "<< var <<" already declared "<<endl;
    return;
  }

  vector<float> bins(2,-1);
  bins[0] = min; bins[1] = max;
  hObs tmp;
  tmp = preparehObs(var, nBin, bins, Xleg,"", type,prof);
  _variables[ var ]= tmp;
  
  for(size_t ib=0;ib<_nDDBkg;ib++) {
    _varDDBkg[ib][ var ] = AnaUtils::cloneHObs( &tmp );
  }

}


void 
HistoManager::addVariable(string var, int nBin, vector<float> bins, string Xleg,
			  bool prof, string type) {
 
  //cout<<" !! "<<_nds<<"   "<<var<<endl;

  //Protection against overdeclaration
  _itVar = _variables.find(var);
  if(_itVar!=_variables.end()) {
    cout<<" Take care : "<< var <<" already declared "<<endl;
    return;
  }

  hObs tmp;
  tmp = preparehObs( var, nBin, bins, Xleg,"", type, prof);
  _variables[ var ]= tmp;
 
  for(size_t ib=0;ib<_nDDBkg;ib++) {
    _varDDBkg[ib][ var ] = AnaUtils::cloneHObs( &tmp );
  }

}

void
HistoManager::addVariable(string var, int nBinX, float minX, float maxX, int nBinY, float minY,
			  float maxY, string Xleg, string Yleg, bool prof, string type) {

  //Protection against overdeclaration
  _itVar = _variables.find(var);
  if(_itVar!=_variables.end()) {
    cout<<" Take care : "<< var <<" already declared "<<endl;
    return;
  }

  vector<float> binsX(2,-1);
  vector<float> binsY(2,-1);

  binsX[0]=minX; binsX[1]=maxX;
  binsY[0]=minY; binsY[1]=maxY;

  hObs tmp;
  tmp = preparehObs(var, nBinX, binsX, Xleg,Yleg, type, prof, nBinY, binsY);
  _variables[ var ]= tmp;

 for(size_t ib=0;ib<_nDDBkg;ib++) {
   _varDDBkg[ib][ var ] = AnaUtils::cloneHObs( &tmp );
  }

}

void
HistoManager::addVariable(string var, int nBinX, vector<float> binsX, int nBinY,
			  vector<float> binsY, string Xleg, string Yleg,
			  bool prof, string type) {

  //Protection against overdeclaration
  _itVar = _variables.find(var);
  if(_itVar!=_variables.end()) {
    cout<<" Take care : "<< var <<" already declared "<<endl;
    return;
  }
  hObs tmp;
  tmp = preparehObs(var, nBinX, binsX, Xleg, Yleg, type, prof, nBinY, binsY);
  _variables[ var ]= tmp;

  for(size_t ib=0;ib<_nDDBkg;ib++) {
    _varDDBkg[ib][ var ] = AnaUtils::cloneHObs( &tmp );
  }

}


hObs 
HistoManager::preparehObs(string var, int nbinsX, vector<float> bins, string Xleg,
			  string Yleg, string type, bool prof, 
			  int nbinsY, vector<float> binsY ) {
  
  bool twoDim = (nbinsY!=-1);
  
  hObs otmp;
    
  otmp.nBX = nbinsX;
  otmp.nBY = nbinsY;
  otmp.binsX = bins;
  otmp.binsY = binsY;

  otmp.titleX=Xleg;
  otmp.titleY=Yleg;
  otmp.name=var;
  otmp.type=type;
  otmp.htype=(prof?(twoDim?"2DP":"1DP"):(twoDim?"2D":"1D"));
  
  for(size_t id=0;id<_nds;id++) {

    string nameH = var+"_"+_dsNames[id];

    otmp.hs.push_back(NULL);
    
    if(!prof) { //normal histos
      if(!twoDim) {
	if(bins.size()==2) { //no var Bin
	  TH1F* htmp=new TH1F(nameH.c_str(), var.c_str(), nbinsX, bins[0], bins[1]);
	  otmp.hs[id]=(htmp);
	}
	else {
	  float tmpB[ bins.size() ];
	  for(size_t ib=0;ib<bins.size();ib++) {tmpB[ib]=bins[ib];}
	  TH1F* htmp=new TH1F(nameH.c_str(), var.c_str(), nbinsX, tmpB);
	  otmp.hs[id]=(htmp);
	}
      }
      else { //two dim histos
	if(bins.size()==2) { //no var Bin
	  TH2F* htmp=new TH2F(nameH.c_str(), var.c_str(), nbinsX, bins[0], bins[1], nbinsY, binsY[0], binsY[1] );
	  otmp.hs[id]=(htmp);
	}
	else {
	 
	  float* tmpBX= new float[ bins.size() ];
	  for(size_t ib=0;ib<bins.size();ib++) tmpBX[ib]=bins[ib];
	  float* tmpBY= new float[ binsY.size() ];
	  for(size_t ib=0;ib<binsY.size();ib++) tmpBY[ib]=binsY[ib];

	  TH2F* htmp=new TH2F(nameH.c_str(), var.c_str(), nbinsX, tmpBX, nbinsY, tmpBY);
	  otmp.hs[id]=(htmp);

	  delete tmpBX;
	  delete tmpBY;
	}
      }
    } // now profile
    else {
      if(!twoDim) {
	if(bins.size()==2) { //no var Bin
	  TProfile* htmp=new TProfile(nameH.c_str(), var.c_str(), nbinsX, bins[0], bins[1]);
	  otmp.hs[id]=(htmp);
	}
	else {
	  float tmpB[ bins.size() ];
	  for(size_t ib=0;ib<bins.size();ib++) tmpB[ib]=bins[ib];
	  TProfile* htmp=new TProfile(nameH.c_str(), var.c_str(), nbinsX, tmpB);
	  otmp.hs[id]=(htmp);
	}
      }
      else { //two dim histos
	if(bins.size()==2) { //no var Bin
	  TProfile2D* htmp=new TProfile2D(nameH.c_str(), var.c_str(), nbinsX, bins[0], bins[1], nbinsY, binsY[0], binsY[1] );
	  otmp.hs[id]=(htmp);
	}
	else {
	  double* tmpBX= new double[ bins.size() ];
	  for(size_t ib=0;ib<bins.size();ib++) tmpBX[ib]=bins[ib];
	  double* tmpBY= new double[ binsY.size() ];
	  for(size_t ib=0;ib<binsY.size();ib++) tmpBY[ib]=binsY[ib];
	  TProfile2D* htmp=new TProfile2D(nameH.c_str(), var.c_str(), nbinsX, tmpBX, nbinsY, tmpBY);
	  otmp.hs[id]=(htmp);

	  delete tmpBX;
	  delete tmpBY;
	}
      }//2D histos
    } //profile
    
  }// number ds
  
  return otmp;

}

TH1* 
HistoManager::getHisto(string obs, int ds) {
  
  _cItVar=_variables.find(obs);
  if(_cItVar!=_variables.end() ) {

    if(_cItVar->second.type != "u" )
      _hname = _cItVar->second.name;

    return _cItVar->second.hs[ds];
  }
  else
    return NULL;
}


const hObs*
HistoManager::getHObs(string obs) {
  
  _cItVar=_variables.find(obs);
  if(_cItVar!=_variables.end() ) {
    
    if(_cItVar->second.type != "u" )
      _hname = _cItVar->second.name;

    return &(_cItVar->second);
  }
  else
    return NULL;
}

vector<string>
HistoManager::getObservables() {

  vector<string> names;
  for(_cItVar=_variables.begin();_cItVar!=_variables.end();_cItVar++) {
    if(_cItVar->second.type != "u" )
      names.push_back( _cItVar->first );
  }

    return names;
}


vector<systM>
HistoManager::getSystObs(string obs) {

  vector<systM> systs;
  
  systs.push_back( findSysts(obs,"") );
  systs.push_back( findSysts(obs,"Up") );
  systs.push_back( findSysts(obs,"Do") );

  return systs;

}


void
HistoManager::copyHisto(string var, int ds, TH1* htmp, string xtitle ) {
  
  //uncertainties
  string uVar=var+htmp->GetName();
  bool isUnc= ds==-10;
  if(isUnc) {var=uVar; ds=0;}
  
  _itVar= _variables.find(var);

  // cout<<var<<" // "<<uVar<<" // "<<isUnc<<endl;

  if(_itVar==_variables.end() ) {
    //get binning
    vector<float> binsX = HistoUtils::getXbinning(htmp);
    //cout<<binsX.size()<<endl;
    addVariable(var, binsX.size()-1, binsX, xtitle , false , isUnc?"u":"" );
    _itVar=_variables.find(var);
    //profile not yet supported
  //   cout<<" h1 nBin : "<<htmp->GetXaxis()->GetNbins()<<" Xmin : "
  // 	<<htmp->GetXaxis()->GetXmin()<<" Xmax: "<<htmp->GetXaxis()->GetXmax()
  // 	<<"   "<<htmp->GetBinContent(13)<<"   "<<htmp->Integral(-1,10000)<<endl;
  // cout<<" h2 nBin : "<<_itVar->second.hs[ds]->GetXaxis()->GetNbins()<<" Xmin : "
  //     <<_itVar->second.hs[ds]->GetXaxis()->GetXmin()<<" Xmax: "<<_itVar->second.hs[ds]->GetXaxis()->GetXmax()<<"  "
  //     <<_itVar->second.hs[ds]->GetBinContent(13)<<endl;
    //cout<<var<<"  "<<htmp->Integral()<<"   "<<htmp->Integral(-1,100000)<<endl;
    _itVar->second.hs[ds]->Add( htmp );
    // cout<<var<<"  "<<_itVar->second.hs[ds]->Integral()
    // 	<<"   "<<_itVar->second.hs[ds]->Integral(-1,100000)<<endl;
    //cout<<" gloubi1 "<<endl;
    // for(int ib=0;ib<htmp->GetNbinsX()+2;ib++) {
    // //   // cout<<_itVar->second.hs[ds]->GetBinLowEdge(ib)
    // //   // 	  <<"   "<<ib<<"   "<<htmp->GetBinContent(ib)<<endl;
    //   _itVar->second.hs[ds]->SetBinContent(ib, htmp->GetBinContent(ib) );
    // }
  }
  else {
    //cout<<" gloubo "<<endl;
    _itVar->second.hs[ds]->Add( htmp );
    //cout<<" gloubo1 "<<endl;
    // cout<<" check of number of bins and bin edges "<<endl;
    // cout<<_itVar->second.hs[ds]->GetNbinsX()<<" / "<<htmp->GetNbinsX()<<" / "<<_itVar->second.hs[ds]->GetXaxis()->GetBinLowEdge(10)<<" / "<<htmp->GetXaxis()->GetBinLowEdge(10)<<endl;
  //   for(int ib=0;ib<htmp->GetNbinsX()+2;ib++)
  //     _itVar->second.hs[ds]->SetBinContent(ib, htmp->GetBinContent(ib)+_itVar->second.hs[ds]->GetBinContent(ib) );
  }
  
}



void 
HistoManager::fill(string var, int ds, float val, float weight, int isDDBkg) {
 
  //cout<<var<<"  "<<ds<<"  "<<val<<"   "<<weight<<endl;
  if(_initsequence) return;

  if(isDDBkg==0 || _nDDBkg==0) {
    _itVar= _variables.find(var);
  }
  else {
    int ib=0;
    for(int i=0;i<10;i++) if(isDDBkg>>i == 0) {ib=i-1;break;}
    _itVar=_varDDBkg[ib].find(var);
  }
  if(_itVar==_variables.end() ) {
    cout<<" Error, no such variable declared : "<<var<<endl;
    return;
  }
  else {
    //if(var=="finalMass")
      //cout<<val<<"   "<<weight<<endl;
    _itVar->second.hs[ds]->Fill(val,weight);
    //if(var=="finalMass")
      //cout<< _itVar->second.hs[ds]->GetName()<<"   "<<_itVar->second.hs[ds]->Integral()<<"   "<< _itVar->second.hs[ds]<<endl;
  }
}

void 
HistoManager::fill(string var, int ds, float valx, float valy, float weight, int isDDBkg) {

  if(_initsequence) return;

  if(isDDBkg==0 || _nDDBkg==0) {
    _itVar= _variables.find(var);
  }
  else {
    int ib=0;
    for(int i=0;i<10;i++) if(isDDBkg>>i == 0) {ib=i-1;return;}
    _itVar=_varDDBkg[ib].find(var);
  }
  // _itVar=_variables.find(var);
  if(_itVar==_variables.end() ) {
    cout<<" Error, no such variable declared : "<<var<<endl;
    return;
  }
  else {
    if(_itVar->second.htype.find("P")!=(size_t)-1) {
      dynamic_cast<TProfile*>(_itVar->second.hs[ds])->Fill(valx,valy,weight);
      // cout<<var<<"   "<<valy<<"   "<<_itVar->second.hs[ds]->GetBinContent(_itVar->second.hs[ds]->GetXaxis()->FindBin(valx) )<<endl; 
    }
    else
      dynamic_cast<TH2*>(_itVar->second.hs[ds])->Fill(valx,valy,weight); //to be checked...
  }
}


void 
HistoManager::fill(string var, string type, float value, float weight,string dir) {
  
  if(_initsequence) return;

  _cItVar=_variables.find(var);
  if(_cItVar==_variables.end() ) {
    cout<<" Error, no such variable declared : "<<var<<endl;
    return;
  }
  else {
    
    string nameH=var+"Unc"+type+dir;
    
    _itVar=_variables.find(nameH);
    if(_itVar==_variables.end() ) {
      
      if(_cItVar->second.binsX.size()==2) {
	addVariable(nameH, _cItVar->second.nBX, _cItVar->second.binsX[0], 
		    _cItVar->second.binsX[1], _cItVar->second.titleX, 
		    (_cItVar->second.htype.find("P")!=(size_t)-1), "u" );
      }
      else {
	addVariable(nameH, _cItVar->second.nBX, _cItVar->second.binsX, 
		    _cItVar->second.titleX, (_cItVar->second.htype.find("P")!=(size_t)-1), "u" );
      }
      //and point to the good object
      _itVar=_variables.find(nameH);
    }
    
    _itVar->second.hs[0]->Fill(value, weight);
  }
  
}


void
HistoManager::savePlots(string path,const TCanvas* c, string advname) {

  string extension[4]={"pdf","eps","png","root"};

  string Name=_hname;

  if(advname!="")
    Name=advname;

  string name1 = "plots/" + path + "/"+extension[0]+"/" + Name + "."+extension[0];
  string name2 = "plots/" + path + "/"+extension[1]+"/" + Name + "."+extension[1];
  string name3 = "plots/" + path + "/"+extension[2]+"/" + Name + "."+extension[2];
  string name4 = "plots/" + path + "/"+extension[3]+"/" + Name + "."+extension[3];
 
  FILE *test_;
  for(int i=0;i<4;i++) {
    TString dirname_ = "plots/" + path +"/"+extension[i];
    if(i==1)
      cout<<" Dirname "<<dirname_<<endl;
    test_=fopen( dirname_.Data(), "r" );
    if( test_==0 )
      {
	TString command_ = TString("mkdir -p ") + dirname_; 
	system( command_.Data());
      }
    else
      fclose( test_ );
  }
  
  c->SaveAs(name1.c_str() );
  c->SaveAs(name2.c_str() );
  c->SaveAs(name3.c_str() );
  c->SaveAs(name4.c_str() );

}


void
HistoManager::reset() {

  for(_itVar=_variables.begin();_itVar!=_variables.end();_itVar++) {
    for(size_t ih=0;ih<_itVar->second.hs.size();ih++)
      delete _itVar->second.hs[ ih ];
    
    _itVar->second.hs.clear();
  }
  _variables.clear();
  _dsNames.clear();
  _initsequence=true;

  _nds=0;
  _hname="";

}

//systematic uncertainties


systM
HistoManager::findSysts(string var,string type) { //for uncertainties

  systM vars;
  //cout<<type<<"   "<<var<<endl;
  vector<string> names;
  for(_itVar=_variables.begin();_itVar!=_variables.end();_itVar++) {

    string name = (*_itVar).first;

    if( (name).find(var+"Unc")==(size_t)-1) continue;
      
    if(type=="Up" && name.substr( name.size()-2,2 ) =="Up") {
      // (name).find("Up")!=(size_t)-1) {
      //  cout<<" up! -> "<<name<<endl;
      names.push_back(name);
    }
    if(type=="Do" && name.substr( name.size()-2,2 ) =="Do") {
      //!=(size_t)-1) {
      //cout<<" do! -> "<<name<<endl;
      names.push_back(name);
    }
    if(type=="" && name.substr( name.size()-2,2 ) !="Up"
       && name.substr( name.size()-2,2 ) !="Do"
       ) {
      //==(size_t)-1 
      // &&  (name).find("Do")==(size_t)-1 ) {
      //cout<<" !!! -> "<<name<<endl;
      names.push_back(name);
    }
  }//loop
  


  //No idea of why it is needed to make that in two time...
  for(size_t in=0;in<names.size();in++) {
    // cout<<names[in]<<endl;
    vars[ names[in] ] = (TH1*)getHisto( names[in] ,0)->Clone();
  }
  
  return vars;
}



//background determination ====


void
HistoManager::fillDDbkg() {

  bool test=false;

  for(_itVar=_variables.begin();_itVar!=_variables.end();_itVar++) { //for each observable

    for(size_t ib=0;ib<_nDDBkg;ib++) { //for each background
      
      _cItVar=_varDDBkg[ib].find( _itVar->first );
      //cout<<(_cItVar==_varDDBkg.end())<<endl;
      if(test)
	cout<<_cItVar->second.hs.size()<<"  "<<_nds<<"   "<<_cItVar->second.hs[_nds-1]->Integral()<<"   "<<_cItVar->second.hs[_nds-1]->GetName()<<endl;
      //data -simu ===================
      TH1* tmp = (TH1*)_cItVar->second.hs[_nds-1]->Clone(); //data
      tmp->SetName( (((string)_cItVar->second.hs[_nds-1]->GetName())+"_DD").c_str() );
 
      //MC subtraction, should not be the case all the time!!
      // for(int ids=0;ids<(_nds-1);ids++) {
      // 	if(test)
      // 	  cout<<_cItVar->second.hs.size()<<"  "<<ids<<"   "<<"   "<<_cItVar->second.hs[ids]->Integral()<<"   "<<_cItVar->second.hs[ids]->GetName()<<"   "<<_ddStatus[ids]<<endl;

      // 	if(_ddStatus[ids]!=1) {
      // 	  if(test)
      // 	    cout<<" adding "<<_cItVar->second.hs[ids]->GetName()<<endl;

      // 	  tmp->Add(  (TH1*)_cItVar->second.hs[ids], -1); //mc
      // 	}

      // } //nds-1

      for(int i=0;i<tmp->GetNbinsX()+2;i++) {
     	if(tmp->GetBinContent(i)<0) tmp->SetBinContent(i,0);
      }

      size_t n=0;
      for(size_t i=0;i<_nds-1;i++) {
	if(_ddStatus[i]==1) {
	  if(test)
	    cout<<i<<"   "<<n<<endl;
	  if(ib==n) _itVar->second.hs[i] = (TH1*)tmp->Clone();
	  n++;
	}
      }
      
    }//nBkg

    test=false;
  }//variables

}




void
HistoManager::fillCSbkg() {

  bool test=false;

  for(_itVar=_variables.begin();_itVar!=_variables.end();_itVar++) { //for each observable
    int ib=0;
    for(size_t i=0;i<_nds;i++) { //for each background
   
      if(_ddStatus[i]<2) continue;

      string bckName=_csData[ib].first;
      float norm=_csData[ib].second;
      
      TFile* iFile(0);
      TH1* tmp(0);
      
      if(_ddStatus[i]<8) {
	iFile=TFile::Open( ("data/rootFiles/Shapes/"+_itVar->first+bckName+".root").c_str(),"read");
	if(iFile)
	  tmp=(TH1*)iFile->Get( (_itVar->first+"_"+bckName).c_str() );
      }      
      
      if(_ddStatus[i]<=8) {
     
	if(_ddStatus[i]==2) { //both shape and normalization
	  if(tmp) {
	    tmp->Scale( norm/tmp->Integral(0,100000) );
	    //_itVar->second.hs[i] = (TH1*)tmp->Clone();
	    HistoUtils::manualClone(tmp, _itVar->second.hs[i] );
	  }
	
	  delete tmp;
	  delete iFile;
	
	}
	else if(_ddStatus[i]==4) { // shape only
	  norm = _itVar->second.hs[i]->Integral(0,100000);
	  if(tmp) {
	    tmp->Scale( norm/tmp->Integral(0,100000) );
	    //_itVar->second.hs[i] = (TH1*)tmp->Clone();
	    HistoUtils::manualClone(tmp, _itVar->second.hs[i] );
	  }
	  // else
	  //   cout<<" ****** Error, no such shape!! "<<endl;
	}
	else if(_ddStatus[i]==8) { // norm only
	  _itVar->second.hs[i]->Scale( norm/_itVar->second.hs[i]->Integral(0,100000) );
	}
	else if(_ddStatus[i]==6) { // shape with std normalization
	  if(tmp) {
	    //_itVar->second.hs[i] = (TH1*)tmp->Clone();
	    HistoUtils::manualClone(tmp, _itVar->second.hs[i] );
	  }
	}

      } //from shapes or external sources
      else {
	int ic=-1;
	for(size_t ik=0;ik<_dsNames.size();ik++) {
	  if(bckName==_dsNames[ik] ) ic= ik;
	}
	
	if(ic==-1) continue;
	tmp = (TH1*)_itVar->second.hs[ ic ];

	if(_ddStatus[i]==22) { //both shape and normalization
	  if(norm>=0) {
	    tmp->Scale( norm/tmp->Integral(0,100000) );
	  }
	  else {//special case, from control region
	    int num=-1*( (int)norm%10);
	    ostringstream os; os<<num;
	    string nameCR="CR"+os.str();
	    for(size_t ik=0;ik<_dsNames.size();ik++) {
	      if(nameCR==_dsNames[ik] ) 
		norm = ((TH1*)_itVar->second.hs[ ic ])->Integral();
	    }
	    //	    cout<<" Normalization = "<<norm<<endl;
	  }
	  
	  HistoUtils::manualClone(tmp, _itVar->second.hs[i] );
	}
	else if(_ddStatus[i]==24) { // shape only
	  norm = _itVar->second.hs[i]->Integral(0,100000);
	  tmp->Scale( norm/tmp->Integral(0,100000) );
	  
	  HistoUtils::manualClone(tmp, _itVar->second.hs[i] );
	}
	else if(_ddStatus[i]==28) { // norm only
	  _itVar->second.hs[i]->Scale( norm/_itVar->second.hs[i]->Integral(0,100000) );
	}
      }
    
      ib++;
    } //nBkgs
  }//variables
  
}
