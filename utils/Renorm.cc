#include "utils/Renorm.hh"


Renorm::Renorm() {
}


Renorm::~Renorm() {
}

map<string,bool>
Renorm::parse(string str) {

  size_t p0=str.find("::") +1;

  map<string,bool> cmap;
  size_t pt=p0;
  string name;
  bool dec;

  while( pt != (size_t)-1 ) {

    size_t pi=str.find(":",pt);
    pt = str.find(":",pi+1);
  
    if( pt == (size_t)-1) {
      name= str.substr(pi+1,str.size()-pi-1-2);
      dec = atoi( (str.substr(str.size()-1,1)).c_str() );
    }
    else {
      name= str.substr(pi+1,pt-pi-1-2);
      dec = atoi( (str.substr(pt-1,1)).c_str() );
    }
    cmap[ name ] = dec;
  }
  
  return cmap;
}

void
Renorm::initialize(vector<TH1*> hmcs, TH1* hdata, string str) {
  
  _hMCs = hmcs;
  _hData = hdata;
  
  _conf = parse(str);
}


void
Renorm::doFit() {

  //X axis ======================
  RooRealVar x("x","x", _hData->GetXaxis()->GetXmin(), _hData->GetXaxis()->GetXmax() );
  
  //data ========================
  RooDataHist data("data","data",x, _hData );

  //simulation ==================
  vector<RooHistPdf*> mcShapes;
  vector<RooDataHist*> mcShapesDH;
  vector<RooRealVar*> mcYieldsI;
  vector<RooRealVar*> mcYieldsF;
  vector<string> dss;
  for(size_t im=0;im<_hMCs.size();im++) {
    string name = ((string)_hMCs[im]->GetName());
  
    RooDataHist* tmpDH=new RooDataHist( ("pdf_"+name).c_str(), ("pdf_"+name).c_str(), x,_hMCs[im]);
    mcShapesDH.push_back(tmpDH);

    RooHistPdf* tmp=new RooHistPdf( ("pdf_"+name).c_str(), ("pdf_"+name).c_str(), x, *mcShapesDH.back() );
    mcShapes.push_back(tmp);

    RooRealVar* tmp2=new RooRealVar( ("yield_"+name).c_str(), ("yield_"+name).c_str(), _hMCs[im]->Integral(0,1000000) );
    mcYieldsI.push_back(tmp2);

    RooRealVar* tmp3=new RooRealVar( ("yield_"+name).c_str(), ("yield_"+name).c_str(),  
				     _hMCs[im]->Integral(0,1000000), _hMCs[im]->Integral(0,1000000)/10., _hMCs[im]->Integral(0,1000000)*4 );
    mcYieldsF.push_back(tmp3);
  }

  //build the global MC pdf =====
  RooArgList listPdfs("listPdf");
  RooArgList listYields("listW");
  
  for(size_t im=0;im<_hMCs.size();im++) {
    string nh=(string)(_hMCs[im]->GetName());
    
    listPdfs.add( *mcShapes[im] );
    bool noMatch=true;
    for(map<string,bool>::const_iterator itSB=_conf.begin();
	itSB!=_conf.end();itSB++) {

      if( nh.find(itSB->first )!=(size_t)-1) {//find string
	dss.push_back( itSB->first );
	noMatch=false;

	if( itSB->second ) { //receive a weight
	    listYields.add( *mcYieldsF[im] );
	  }
	  else { // is fixed
	    listYields.add( *mcYieldsI[im] );
	  }
      } 
    } //matching string
    
    if(noMatch) { //found no string, but is here -> typo or other
      cout<<" Missing specification during fit for "<<nh<<", fixed by default "<<endl;
      listYields.add( *mcYieldsI[im] );
    }

  }//loop on hmcs
  
  RooAddPdf totalPdf("totalPdf","totalPdf",listPdfs,listYields);

  //fit ===========================
  RooFitResult* res = totalPdf.fitTo(data,RooFit::SumW2Error(kFALSE) );


  //factors =======================
  for(size_t im=0;im<_hMCs.size();im++) {
    _weights[ dss[im] ] = mcYieldsF[im]->getVal()/mcYieldsI[im]->getVal();
    //cout<<dss[im]<<"   "<<_weights[ dss[im] ]<<endl;
  }

  
  // cleaning =====================
  for(size_t im=0;im<_hMCs.size();im++) {
    delete mcShapes[im];
    delete mcYieldsI[im];
    delete mcYieldsF[im];
  }
  mcShapes.clear();
  mcYieldsI.clear();
  mcYieldsF.clear();

}


map<string,float>
Renorm::getWeights() {
  
  doFit();
  
  return _weights;
}
