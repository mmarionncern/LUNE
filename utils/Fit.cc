#include "utils/Fit.hh"


Fit::Fit() {
}


Fit::~Fit() {
}

void::adaptString(string str) {








}


map<string,bool>
Fit::parse(string str) {
  
  fitM fitMap;
  size_t pt=0;
  string name,fit;
  bool dec;

  while( pt != (size_t)-1 ) {

    size_t pi=str.find(":",pt) +1;
    if(pt==0) pi=0;
    pt = str.find(":",pi+1);
  
    if( pt == (size_t)-1) {
      name= str.substr(pi+1,str.size()-pi-1-2);
      dec = atoi( (str.substr(str.size()-1,1)).c_str() );
    }
    else { //not the end
      
      //ds name
      size_t pen=str.find("-",pi); 
      name = str.subtr(pi,pen-pi);

      //ok, now the pdf... 
      
      //fit Type
      size_t pfn=str.find("[",pen); 
      fit =  str.subtr(pen+1,pfn-pen-1);

      //input values
      
      

      name= str.substr(pi+1,pt-pi-1-2);
      dec = atoi( (str.substr(pt-1,1)).c_str() );
    }
    cmap[ name ] = dec;
  }
  
  return cmap;
}

pair<string,vector<vector<float> > >
Fit::parseStdPdf(string str) {
  
  //fit Type
  size_t pfn=str.find("[",0); 
  string fit=str.substr(0,pfn);

  //input values
  size_t m=pfn+1,n=pfn+1,pt=pfn+1;
  size_t tn,tm;
  vector<vector<float> > vals(1,vector<float>(0,0));

  while( pt != (size_t)-1 ) {
    
    tm=str.find(",",m);
    tn=str.find("/",n);

    if(tn<tm) { //means new param
      float val=atof( (str.substr(m,tn-m)).c_str() );
      pt= tn;
      m=tn+1;
      vals.back().push_back(val);
      
      vector<float> tmp;
      vals.push_back( tmp );
      n=tn+1;
    }
    else { //means still the same param
      float val;

      if( tm==(size_t)-1)
	val =atof( (str.substr(m,tm-1-m)).c_str() );
      else
	val =atof( (str.substr(m,tm-m)).c_str() );
    
      pt= tm;
      m=tm+1;
      vals.back().push_back(val);
    }
  }

  pair<string,vector<vector<float> > > p(fit,vals);

  return p;
}

vector<string>
Fit::parseCpxPdf(string str) {

  size_t pt=0;

  vector<string> pdfs;
  vector<string> ops;

  while(true) {

    size_t pp=str.find("+",pt);
    size_t pm=str.find("*",pt);
    size_t pc=str.find("@",pt);

    size_t pp=str.find("+",pt);
    size_t pp=str.find("+",pt);
    size_t pm=str.find("*",pt);
    size_t pc=str.find("@",pt);
    
    size_t k=min(pp,min(pm,pc) );
    
    pdfs.push_back( str.substr(pt,k-pt) );
    pt = k+1;
    if(k == (size_t)-1) break;
  }

}



void
Fit::initialize(vector<TH1*> hmcs, TH1* hdata, string str) {
  
  _hMCs = hmcs;
  _hData = hdata;
  
  _conf = parse(str);
}


void
Fit::doFit() {

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
Fit::getWeights() {
  
  doFit();
  
  return _weights;
}
