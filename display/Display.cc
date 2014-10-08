#include "display/Display.hh"

#include "utils/HistoUtils.hh"

ClassImp(Display)

Display::Display():
_c(0),_leg(0),_empty(0),_hMC(0),_hData(0),_gData(0)
{
  
  TGaxis::SetMaxDigits(5);
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  _rmLabel=false;

  _lockData=false;

  _datNorm=1.;

  _uncDet =false;
  _mcSyst=false;

  _is1D=true;
  _is2D=false;
  _isProf=false;

  _userYScale=false;

  _dOnly=true;

  _cumulative=false;

  _fitStr="";
  _fitNorm = new Renorm();

}

Display::~Display() {


}

void
Display::reset() {
  _gWeights.clear();
  _colors.clear();
  _names.clear();
  _ghosts.clear();
  _iNs.clear();

  _normOpts.clear();
  _vars.clear();
  
  _lockData=false;
  _rmLabel=false;
  
  _datNorm=1.;

  delete _empty;
  delete _hMC;
  delete _hData;
  delete _gData;

  for(size_t ii=0;ii<_hClones.size();ii++)
    delete _hClones[ii];
  _hClones.clear();

  for(size_t ii=0;ii<_hClonesNoStack.size();ii++)
    delete _hClonesNoStack[ii];
  _hClonesNoStack.clear();

  for(size_t ii=0;ii<_hSigClones.size();ii++)
    delete _hSigClones[ii];
  _hSigClones.clear();
  
  for(size_t ii=0;ii<_mcUncert.size();ii++)
    delete _mcUncert[ii];
  _mcUncert.clear();

  for(size_t ix=0;ix<_pads.size();ix++)
    for(size_t iy=0;iy<_pads[ix].size();iy++) {
      delete _pads[ix][iy];
    }  

  _pads.clear();

  delete _leg;
  delete _c;

  _empty=NULL;
  _hMC=NULL;
  _hData=NULL;
  _gData=NULL;
  _c=NULL;
  _leg=NULL;
  
  _hCoords.clear();

  _uncDet =false;
  
  _is1D=true;
  _is2D=false;
  _isProf=false;

  _xmin = _xCoordSave[0];
  _xmax = _xCoordSave[1];
  _gBin =  _binningSave;

}

void
Display::softReset() {
  delete _empty;
  delete _hMC;
  delete _hData;
  delete _gData;

  for(size_t ii=0;ii<_hClones.size();ii++)
    delete _hClones[ii];
  _hClones.clear();

  for(size_t ii=0;ii<_hClonesNoStack.size();ii++)
    delete _hClonesNoStack[ii];
  _hClonesNoStack.clear();

  for(size_t ii=0;ii<_hSigClones.size();ii++)
    delete _hSigClones[ii];
  _hSigClones.clear();
  
  for(size_t ii=0;ii<_mcUncert.size();ii++)
    delete _mcUncert[ii];
  _mcUncert.clear();

  for(size_t ix=0;ix<_pads.size();ix++)
    for(size_t iy=0;iy<_pads[ix].size();iy++) {
      delete _pads[ix][iy];
    }  

  _pads.clear();

  delete _leg;
  delete _c;

  _empty=NULL;
  _hMC=NULL;
  _hData=NULL;
  _gData=NULL;
  _c=NULL;
  _leg=NULL;
  
  _hCoords.clear();

}

void
Display::refreshHistos() {
 
  for(size_t ii=0;ii<_hClones.size();ii++)
    delete _hClones[ii];
  _hClones.clear();
  
  for(size_t ii=0;ii<_hClonesNoStack.size();ii++) {
    delete _hClonesNoStack[ii];
  }
  _hClonesNoStack.clear();
  
  for(size_t ii=0;ii<_hSigClones.size();ii++)
    delete _hSigClones[ii];
  _hSigClones.clear();
  
  for(size_t ii=0;ii<_mcUncert.size();ii++)
    delete _mcUncert[ii];
  _mcUncert.clear();

  delete _leg;
  _leg=NULL;

}

void
Display::setFitStr(string str) {
  _fitStr=str;
}

void 
Display::setNormalization(string str) {

  size_t b=(size_t)-1;
  
  if(str.find("dnorm")!=b || str.find("DNorm")!=b || 
     str.find("dNorm")!=b || str.find("Dnorm")!=b )
    _normOpts.insert("dnorm");
  else if( str.find("norm")!=b || str.find("Norm")!=b )
    _normOpts.insert("norm");
  else if( str.find("dif")!=b || str.find("Dif")!=b )
    _normOpts.insert("dif");
  else if( str.find("uni")!=b || str.find("Uni")!=b )
    _normOpts.insert("uni");
  else if( str.find("fit")!=b || str.find("Fit")!=b )
    _normOpts.insert("fit");
  // if(str.find("w")!=b) {
  //   size_t pos = str.find("w");
  //   size_t pos = str.find("w");
    


  // }
  
  // if(str.find("c")!=b) {
    
    
  
  // }
  

}


void 
Display::setObservables(string v1, string v2, string v3,
			string v4, string v5, string v6) {

  _vars.push_back(v1);
  if(v2!="") _vars.push_back(v2);
  if(v3!="") _vars.push_back(v3);
  if(v4!="") _vars.push_back(v4);
  if(v5!="") _vars.push_back(v5);
  if(v6!="") _vars.push_back(v6);

  _nvars=_vars.size();
 
}

// void
// Display::getSystUnc( TGraphAsymmErrors* mcUnc) {
//   _mcUncert = mcUnc;
// }

vector<vector<TPad*> > 
Display::preparePads() {
  
  //  float m=1.3;

  size_t n_ = _nvars;
  float m=1.3-(n_-1)*.1; //other
  //float m=1.4-(n_-1)*.1; //LQ
  
  vector<TPad*> padhigh_;
  vector<vector<TPad*> > pad_;
  Double_t e_ = 35*m; //15
  Double_t t_ = 30*m; //30
  Double_t b_ = 70*m; //80
  Double_t g_ = 85*m; //87
  //Double_t d_ = 35*m; //30
  Double_t d_ = e_; //30
  Double_t h_ = 375*m; //400
  Double_t w_ = 375*m; //350
  Double_t ww_ = g_ + w_ + e_ ;
  Double_t W_ = g_ + n_*w_ + (n_-1)*(g_-e_) + n_*d_;
  Double_t H_ = t_ + h_ + b_ ;
  
  _wpad = ww_;
  _hpad = H_;

  _c=new TCanvas("variables","variables",100,100,W_,H_);
  Double_t xlow_= 0;
  Double_t ylow_=0.;
  Double_t xup_=0;
  Double_t yup_=1.;

  for(int unsigned i=0;i<n_;i++) {

    TString padname_("pad_");
    padname_+=i;
    xup_ = xlow_ + ww_ /W_;
    TPad* padtmp = new TPad( padname_, padname_, 
			     xlow_, ylow_, xup_, yup_,
			     kWhite,0,0);
    //xlow_ += (w_ + 2*e_)/W_;
    xlow_ += (w_ + g_)/W_;
    padtmp->SetLeftMargin(  g_/ww_ );
    padtmp->SetRightMargin( e_/ww_ );
    padtmp->SetTopMargin(  t_/H_ );
    padtmp->SetBottomMargin( b_/H_ );
    padtmp->SetFillColor(0);
    padtmp->SetTickx(1);
    padtmp->SetTicky(1);
    padtmp->SetFrameFillStyle(0);
    padtmp->SetFrameLineWidth(2);
    padtmp->SetFrameBorderMode(0);
    
    //    vector<TPad*> vpadtmp;
    padhigh_.push_back(padtmp);

  }

  pad_.push_back(padhigh_);

  return pad_;
}


vector<vector<TPad*> > 
Display::preparePadsWithRatio() {


  float m=1.3;

  size_t n_ = _nvars;

  vector<TPad*> padhigh_;
  vector<TPad*> padlow_;
  Double_t e_ = 15*m; //15
  Double_t k_ = 7*m; //15
  Double_t t_ = 30*m; //30
  Double_t b_ = 50*m; //80
  Double_t g_ = 87*m; //87
  Double_t d_ = 30*m; //30
  Double_t h_ = 350*m; //400
  Double_t w_ = 350*m; //350
  Double_t hl_ = 70*m;
  
  Double_t ww_ = g_ + w_ + e_ ;
  Double_t W_ = g_ + n_*w_ + 2*(n_-1)*e_ + d_;
  Double_t H_ = t_ + h_ + b_ + hl_ +2*k_ ;
    
  
  _wpad = ww_;
  _hpad = t_ + h_ +k_ ;

  _c=new TCanvas("variables","variables",100,100,W_,H_);
  //  c2=new TCanvas("c2","Test",300,300,1.2*W_,1.2*H_);
  Double_t xlow_= 0;
  Double_t ylow_=0.;
  Double_t xup_=0;
  Double_t yup_=1.;
  for(int unsigned i=0;i<n_;i++) {
      
    TString padname_("padhigh_");
    padname_+=i;
    xup_ = xlow_ + ww_ /W_;
    yup_ = 1.; 
    ylow_ = (hl_ + b_ + k_ ) /H_;
    
    TPad* padtmp = new TPad( padname_, padname_, 
			     xlow_, ylow_, xup_, yup_,
			     kWhite,0,0);
    xlow_ += (w_ + 2*e_)/W_;
    padtmp->SetLeftMargin(  g_/ww_ );
    padtmp->SetRightMargin( e_/ww_ );
    padtmp->SetTopMargin(  t_/H_ );
    padtmp->SetBottomMargin( k_/H_ );
    padtmp->SetFillColor(0);
    padtmp->SetTickx(1);
    padtmp->SetTicky(1);
    padtmp->SetFrameFillStyle(0);
    padtmp->SetFrameLineWidth(_wline);
    padtmp->SetFrameBorderMode(0);
      
    padhigh_.push_back(padtmp);
  }
  
  xlow_= 0; //reset
  _c->cd(); //reset
  for(int unsigned i=0;i<n_;i++) {

    TString padname_("padlow_");
    padname_+=i;
    xup_ = xlow_ + ww_ /W_;
    yup_ = (hl_ + b_ + k_ ) /H_; 
    ylow_ = 0;

    TPad* padtmp  = new TPad( padname_, padname_, 
			      xlow_, ylow_, xup_, yup_,
			      kWhite,0,0);
    xlow_ += (w_ + 2*e_)/W_;
    padtmp->SetLeftMargin(  g_/ww_ );
    padtmp->SetRightMargin( e_/ww_ );
    padtmp->SetTopMargin( k_/H_ );
    padtmp->SetBottomMargin( b_ /(hl_ + b_ + k_ ) );
    padtmp->SetFillColor(0);
    padtmp->SetTickx(1);
    padtmp->SetTicky(1);
    padtmp->SetFrameFillStyle(0);
    padtmp->SetFrameLineWidth(_wline);
    padtmp->SetFrameBorderMode(0);

    padlow_.push_back(padtmp);
  } 
  
  vector<vector<TPad*> > pad_;
  pad_.push_back(padhigh_);
  pad_.push_back(padlow_);
  
  return pad_;
}


void
Display::preparePadsForSystDetail() {
  
  _cSyst = new TCanvas("systematics", "systematics",1250,380);
  _cSyst->Range(0,0,1,1);
  _cSyst->SetLeftMargin(0.10);
  _cSyst->SetRightMargin(0.05);
  _cSyst->SetTopMargin(0.10);
  _cSyst->SetBottomMargin(0.2);
  _cSyst->cd();
  _padleft = new TPad( "pleft","pleft", 
		       0,0,945/1250. ,1,
		       kWhite,0,0);
  
  _padleft->SetLeftMargin( 0.093 );
  _padleft->SetRightMargin( 0.03 );
  _padleft->SetTopMargin( 0.04 );
  _padleft->SetBottomMargin( 0.21 );
  _padleft->SetFillColor(0);
  _padleft->SetTickx(1);
  _padleft->SetTicky(1);
  _padleft->SetFrameFillStyle(0);
  _padleft->SetFrameLineWidth(_wline);
  _padleft->SetFrameBorderMode(0);
  
  _cSyst->cd(); //reset
 
  _padright  = new TPad( "pright","pright", 
			 955/1250., 0, 1, 1,
			 kWhite,0,0);
  _padright->SetLeftMargin( 0.1 );
  _padright->SetRightMargin( 0.1 );
  _padright->SetTopMargin( 0.1 );
  _padright->SetBottomMargin( 0.1 );
  _padright->SetFillColor(0);
  _padright->SetTickx(1);
  _padright->SetTicky(1);
  _padright->SetFrameFillStyle(0);
  _padright->SetFrameLineWidth(_wline);
  _padright->SetFrameBorderMode(0);
  
}




void 
Display::plotDistributions(vector<const hObs*> theObs) {

  //first clean objects
  for(size_t ix=0;ix<_pads.size();ix++)
    for(size_t iy=0;iy<_pads[ix].size();iy++) {
      delete _pads[ix][iy];
    }
  
  _pads.clear();

  delete _leg;
  delete _c;
  //====================================
  //and initialize weights
  
  //====================================
  
  if( _normOpts.find("norm")!=_normOpts.end() ||
      theObs[0]->htype.find("2D")!=(size_t)-1 ) {
    _showRatio=false;
    //_addSyst=false;
  }
  if(_mcOnly || _dOnly) _showRatio=false;
  if(_dOnly) { _showRatio=false; _addSyst=false;}

  if(!_showRatio) _pads = preparePads();
  else _pads = preparePadsWithRatio();
 
  _c->Draw();

  for(int io=(int)_nvars-1;io>=0;io--) {
    
    if(io!=0) _rmLabel=false;
    refreshHistos();
    plotDistribution( theObs[io], io );

  }
  
 
  _c->Update();

}


void Display::plotDistribution(const hObs* theobs, int iobs) {

  _c->cd();
  _pads[0][iobs]->Draw();
  _pads[0][iobs]->cd();

  _csystM = _systMUnc[iobs];

  drawDistribution(theobs);

  _pads[0][iobs]->RedrawAxis();

  if(_logYScale)
    _pads[0][iobs]->SetLogy(1);
  if(_showGrid) {
    _pads[0][iobs]->SetGridx(1);
    _pads[0][iobs]->SetGridy(1);
  }

  _pads[0][iobs]->Update();

  //Lumi and legend
 
  cmsPrel();
 
  _pads[0][iobs]->Update();
 
  if(theobs->htype.find("1D")!=(size_t)-1) { //fixme -> legend adjustment not supported for 2D plots
    adjustLegend(iobs);
    if(_leg) {
      TLegend* tmpleg=(TLegend*)_leg->Clone();
      tmpleg->Draw("same");
    } 
  }

  _pads[0][iobs]->Update();

  if(!_showRatio || 
     theobs->htype.find("1D")==(size_t)-1 ||
     theobs->type=="u" || !_hMC ) return;

  _c->cd();
  _pads[1][iobs]->Draw();
  _pads[1][iobs]->cd();

  drawDataMCRatio();

  _pads[1][iobs]->RedrawAxis();
  _pads[1][iobs]->Update();

}  


void
Display::configure(string dsname, int col, bool isGhost) {
  // if(dsname.find("DD")!=(size_t)-1)
  //   _names.push_back( dsname.substr(0,dsname.size()-2) );
  // else
  
  _ghosts[ dsname ] = isGhost;
  //cout<<" ghost? "<<isGhost<<"   "<<dsname<<endl;
  if(!isGhost) {
    //cout<<dsname<<"   "<<_ghosts.size()-1<<"   "<<col<<endl;
    _names.push_back(dsname);
    _colors[ dsname ] = col;
    _iNs.push_back( _ghosts.size()-1 );
  }  

  //Check existence of data
  if(dsname=="data" || dsname=="Data") checkData();
  else isNoData();
}

void
Display::setWeight(string dsname, float w) {
  _gWeights[ dsname ] = w;
  _saveWeights[ dsname ] = w;
}

string
Display::getFitVar() {
  size_t p=_fitStr.find("::");
  string nvar=_fitStr.substr(0,p);
  return nvar;
}

void
Display::initWeights(const hObs* theobs) {

  if(_normOpts.find("fit")!=_normOpts.end() ) {
    prepareHistograms(theobs);
    
    _fitNorm->initialize(_hClonesNoStack, _hData, _fitStr);
    map<string,float> ws=_fitNorm->getWeights();
    for(_itW=ws.begin();_itW!=ws.end();_itW++)
      _gWeights[ _itW->first ] *= _itW->second;
  }
  else
    for(_itW=_saveWeights.begin();_itW!=_saveWeights.end();_itW++)
      _gWeights[ _itW->first ] = _itW->second;

}

void
Display::drawDistribution(const hObs* theobs) {

  //prepare the histograms
  prepareHistograms(theobs);
  
  //Drawing now.... (+legend) ============================
  if(_is1D)
    _empty->DrawCopy();
  // else
  //   _empty->DrawCopy();
  
  //signals
  vector<size_t> sigs;
  string opt;
  if(_is1D) opt="same hist";
  if(_is2D) opt="same box";//box
  if(_isProf) opt="same";
  if( !(_isProf && _is1D) ) {
    
    bool f=true;
    for(size_t i=0;i<_nhmc;i++) {
      string nh = (string)( _hClones[i]->GetName());
      if( !_sSignal && nh.find("sig")!=(size_t)-1) 
	sigs.push_back(i);
      else {

	if(_is1D) {
	  _hClones[i]->DrawCopy( opt.c_str() );
	}
	else {
	  if(f) {
	    _hClones[i]->DrawCopy( "box" ); //"box"
	    f=false;
	  }
	  else {
	    _hClones[i]->DrawCopy( opt.c_str() );
	  }
	}
      }
    }
  }
  // systematic uncertainties now ============
  if(_addSyst && !_is2D && _hMC) {
    computeSystematics(_isProf,true);
    
    if(_addSyst) {
      TGraphAsymmErrors* mcUnc = (TGraphAsymmErrors*) _mcUncert.back()->Clone();
      
      mcUnc->SetMarkerSize(0); 
      mcUnc->SetMarkerStyle(1);
      mcUnc->SetMarkerColor(1);
      mcUnc->SetFillStyle(3001);
      mcUnc->SetFillColor(kGray+1);

      TGraphAsymmErrors* tmpUnc=(TGraphAsymmErrors*)mcUnc->Clone();
      tmpUnc->SetName("uncertainties");
      tmpUnc->SetTitle("uncertainties");
      tmpUnc->Draw("p E2");
     
    }

  }
  // the data ==============================================
  // cout<<theobs->titleX<<"   "<<_gData<<endl;
  if(_is1D && !_isProf) {
    if(!_mcOnly && _gData!=NULL)
      _gData->Draw("p");
    if(_hMC!=0 && _normOpts.find("norm")==_normOpts.end() )
      _hMC->DrawCopy( opt.c_str() );
  }
  else if(_is1D && _isProf) {
    if(!_mcOnly) {
      _hData->SetMarkerStyle(20);
      _hData->Draw( opt.c_str() );
      //    _gData->Draw("same");
    }
    if(_hMC!=0 && _normOpts.find("norm")==_normOpts.end()) {
      _hMC->SetMarkerStyle(24);
      _hMC->DrawCopy( opt.c_str() );
    }
  }
  else if(_is2D && !_mcOnly)
    _hData->DrawCopy( opt.c_str() );
  // ===========================================
  

 
  //and finally signals if some exists ===========
  for(size_t is=0;is<sigs.size();is++)
    _hClones[ sigs[is] ]->DrawCopy( opt.c_str() );

  //===============================================

  //Information printing
  // if(!_is2D) {
  //   if(!_mcOnly && !_dOnly && _normOpts.find("norm")==_normOpts.end()) {
  //     cout<<" Integral : data -> "<< _hData->Integral(0,10000)<<" // MC-> "<<_hMC->Integral(0,10000)<<endl;
  //   }
  //   else if(_mcOnly && _hMC) cout<<" Integral : MC -> "<<_hMC->Integral(0,100000)<<endl;
  //   else if(_dOnly) cout<<" Integral : data -> "<<_hData->Integral(0,100000)<<endl;
 
  //   if(!_dOnly) {
  //     float tmp=0;
  //     for(size_t ih=0;ih<_nhmc;ih++) {
  // 	cout<<"\t"<<_names[ih]<<" : "<<_hClones[_nhmc-ih-1]->Integral(0,10000000)-tmp<<endl;
  // 	tmp = _hClones[_nhmc-ih-1]->Integral(0,10000000);
  //     }
  //   }
  // }
  printInteg(0,1000000,0,1000000);

}




void
Display::changeGeVToTeV(TH1*& h,string xtitle, string ytitle, float xmin, float xmax, float ymin, float ymax, string hType, bool& xAxTeV, bool& yAxTeV) {

  bool isXTitle=xtitle.find("GeV")!=(size_t)-1;
  bool isYTitle=ytitle.find("GeV")!=(size_t)-1;

  //bool is1D=hType.find("1D")!=(size_t)-1;

  bool changeX = isXTitle && (fabs(xmax)>=1000 || fabs(xmin)>=1000 );
  bool changeY = isYTitle && (fabs(ymax)>=1000 || fabs(ymin)>=1000 );

  if(!changeX && _is1D) return; //1D protection
  if(!changeY && !changeX) return; //2D protection
  
  if(changeX) xAxTeV=true;
  if(changeY) yAxTeV=true;

  TH1* tmp=NULL;
  TH2* tmp2=NULL;
  //delete h;

  float* xBins=NULL;
  float* yBins=NULL;
 
  //Retrieve binning and modify it =======================
  if(_is1D) {
    
    tmp =(TH1*)(h->Clone());
    xBins=new float[ tmp->GetNbinsX()+1 ];
    xBins[0] = tmp->GetXaxis()->GetBinLowEdge(1)/(changeX?1000.:1.);
    for(int ix=0;ix<tmp->GetNbinsX();ix++)
      xBins[ix+1] = tmp->GetXaxis()->GetBinUpEdge(ix+1)/(changeX?1000.:1.);
  }
  
  if(_is2D) {
    tmp2 =(TH2*)h->Clone();
    xBins=new float[ tmp2->GetNbinsX()+1 ];
    yBins=new float[ tmp2->GetNbinsY()+1 ];
    xBins[0] = tmp2->GetXaxis()->GetBinLowEdge(1)/(changeX?1000.:1.);
    for(int ix=0;ix<tmp2->GetNbinsX();ix++)
      xBins[ix+1] = tmp2->GetXaxis()->GetBinUpEdge(ix+1)/(changeX?1000.:1.);
    yBins[0] = tmp2->GetYaxis()->GetBinLowEdge(1)/(changeY?1000.:1.);
    for(int iy=0;iy<tmp2->GetNbinsY();iy++)
      yBins[iy+1] = tmp2->GetYaxis()->GetBinUpEdge(iy+1)/(changeY?1000.:1.);
  }


  //=========================================================

  if(_is1D) {
    h =new TH1F(tmp->GetName(), tmp->GetTitle(),
		tmp->GetNbinsX(), xBins );
      
    //color/style/title
    HistoUtils::copyStyle(tmp,h);

    for(int ib=0;ib<tmp->GetNbinsX()+2;ib++) {
      h->SetBinContent(ib, tmp->GetBinContent(ib) );
      h->SetBinError(ib, tmp->GetBinError(ib) );
    }
    delete tmp;
  }
  else {
    TH2F* htmp =new TH2F(tmp2->GetName(), tmp2->GetTitle(),
			 tmp2->GetNbinsX(), xBins, 
			 tmp2->GetNbinsY(), yBins );
    
    //color/style/title
    HistoUtils::copyStyle((TH1*)tmp2,(TH1*&)htmp);

    for(int ix=0;ix<tmp2->GetNbinsX()+2;ix++) {
      for(int iy=0;iy<tmp2->GetNbinsY()+2;iy++) {
	htmp->SetBinContent(ix,iy, tmp2->GetBinContent(ix,iy) );
	htmp->SetBinError(ix,iy,  tmp2->GetBinError(ix,iy) );
      }
    }
    
    h = (TH1*)htmp->Clone();
    delete htmp;
    delete tmp2;
  }
  
}
  
void
Display::prepareHistograms(const hObs* theobs) {

  // //bypass for ghost datasets =====
  // for(size_t ih=0;ih<_nhmc;ih++) {
  //   cout<<ih<<"   "<<_names[_nhmc-ih-1]<<"   "<<_nhmc<<endl;
  //   cout<<(_ghosts.find( _names[_nhmc-ih-1] ) )->second<<endl;
  //   if( (_ghosts.find( _names[_nhmc-ih-1] ) )->second ) continue;
  //   //===============================
  // }

  // _nh=theobs->hs.size();
  // _nhmc=_mcOnly?_nh:_nh-1;
  // _ihd=_mcOnly?-1:_nhmc;

  _nh=_iNs.size();
  _nhmc=_mcOnly?_nh:_nh-1;
  _ihd=_mcOnly?-1:theobs->hs.size()-1;
  
  //first, remove ghost datasets ===  
  vector<TH1*> hTmps(_nhmc,0); 
  //size_t sTmp=_ghosts.size();
  // for(int i=0;i<3;i++)
  //   cout<<theobs->hs[i]->GetName()<<"   "<<3-i-1<<endl;
  // cout<<"ih    it    gs-ih-1  "<<endl;
  for(size_t it=0;it<_nhmc;it++) {
    size_t ih = (size_t)_iNs[_nhmc-it-1];
    //cout<<ih<<"   "<<it<<"   "<<_ghosts.size()-ih-1<<"    ";
    hTmps[it] = (TH1*)theobs->hs[ih]->Clone();
    //cout<<hTmps[it]->GetName()<<"  "<<hTmps[it]->Integral()<<"   "<<_ihd<<endl;//<<"    "<<_names[_nhmc-it-1];
    //cout<<ih<<"   "<<hTmps[ih]->GetName()<<"  "<<hTmps[ih]->Integral()<<endl;
  }
  //================================
    
  _is1D=theobs->htype.find("1D")!=(size_t)-1;
  _is2D=theobs->htype.find("2D")!=(size_t)-1;
  _isProf=theobs->htype.find("P")!=(size_t)-1;
  
  //Histo Cloning and coloring ===========
  //Stacking and weighting ==============
  for(size_t ii=0;ii<_hClones.size();ii++)
    delete _hClones[ii];
  _hClones.clear();
  
  for(size_t ii=0;ii<_hClonesNoStack.size();ii++)
    delete _hClonesNoStack[ii];
  _hClonesNoStack.clear();

  for(size_t ii=0;ii<_hSigClones.size();ii++)
    delete _hSigClones[ii];
  _hSigClones.clear();

  delete _empty;
  delete _hMC;
  delete _hData;
  delete _gData;
  
  _hClones.resize(_nhmc);
  _hClonesNoStack.resize(_nhmc);
  //_hSigClones.resize(_nhsig);
  _hMC= NULL;
  _hData = NULL;
  _gData = NULL;
  // = _colors.begin();
  // = _gWeights.begin();
  
  size_t nsig=0;
  float iMC=0.; float iData=0.;
  for(size_t ih=0;ih<_nhmc;ih++) {
    
    _itCol = _colors.find( _names[_nhmc-ih-1] );
    _itW = _gWeights.find( _names[_nhmc-ih-1] );
    cout<<_names[_nhmc-ih-1] <<" ->"<<_itW->second<<endl;
    _hClones[ih] = (TH1*)hTmps[ih]->Clone();// (TH1*)theobs->hs[_nhmc-ih-1]->Clone();
    _hClonesNoStack[ih] = (TH1*)hTmps[ih]->Clone();

    //FIXME, test weighint fake rate
    // if(_names[_nhmc-ih-1]=="fake #tau") {
    //   TFile ofile("/home/mmarionn/Documents/CMS/dev/LUNE/database/FRRatio.root","read");
    //   TH1F* rat=(TH1F*)ofile.Get("FRSHRatio");
    //   _hClones[ih]->Multiply(rat);
    //   _hClonesNoStack[ih]->Multiply(rat);
    // }

    // if(_is2D)
    //   _empty =  (TH1*)theobs->hs[_nhmc-ih-1]->Clone();
    string nh="";
    if(_normOpts.find("norm")==_normOpts.end() || _is2D) {
      iMC += _hClones[ih]->Integral(0,1000000);

      nh = (string)( _hClones[ih]->GetName());
      if(nh.find("sig")!=(size_t)-1 && !_sSignal) nsig++;
      
      if( nh.find("sig")==(size_t)-1 || 
	  (_sSignal && nh.find("sig")!=(size_t)-1 ) ) {
     
	for(size_t ij=ih+1;ij<(_noStack?min(ih+1,_nhmc):_nhmc);ij++) {
	  
	  string nh2 = (string)( hTmps[ih]->GetName()); //theobs->hs[_nhmc-ij-1]->GetName());
	  if( nh2.find("sig")==(size_t)-1 )
	    _hClones[ih]->Add( (TH1*)hTmps[ij]->Clone(), _itW->second ); //(TH1*)theobs->hs[_nhmc-ij-1]->Clone()
	  else if( _sSignal )
	    _hClones[ih]->Add( (TH1*)hTmps[ij]->Clone(), _itW->second ); //(TH1*)theobs->hs[_nhmc-ij-1]->Clone()
	}  
      }  
    
    }
    else { //FIXME
      if(_hClones[ih]->Integral(0,10000000)!=0.)
       	_hClones[ih]->Scale( 1./ _hClones[ih]->Integral(0,10000000) );
      iMC += _hClones[ih]->Integral(0,1000000);
    }

   
    _hClones[ih]->SetLineColor( _itCol->second );
    _hClones[ih]->SetFillColor( _itCol->second );
    _hClones[ih]->SetMarkerColor( _itCol->second );
    _hClones[ih]->SetLineWidth( _wLine );
    _hClones[ih]->SetMarkerStyle( 0 );
    _hClones[ih]->GetYaxis()->SetLabelSize(0);
    _hClones[ih]->GetYaxis()->SetTitleSize(0);
    _hClones[ih]->GetYaxis()->SetLabelOffset(1000);
    _hClones[ih]->GetYaxis()->SetTitleOffset(1000);
    _hClones[ih]->GetXaxis()->SetLabelOffset(1000);
    _hClones[ih]->GetXaxis()->SetTitleOffset(1000);
    _hClones[ih]->GetYaxis()->SetNdivisions(0,0,0);
    _hClones[ih]->GetYaxis()->SetNdivisions(0,0,0);
    
    if( !_sSignal && nh.find("sig")!=(size_t)-1) {
      _hClones[ih]->SetFillStyle(0);
      _hClones[ih]->SetLineWidth(2); 
    }

    if(_normOpts.find("norm")!=_normOpts.end()) {
      _hClones[ih]->SetFillStyle(0);
      _hClones[ih]->SetLineWidth(2); 
    }

    if(ih<_nhmc-1) {
      _itCol++;_itW++;
    }
  }
  if(!_mcOnly) {
    _hData = (TH1*)theobs->hs[_ihd]->Clone();
    iData = _hData->Integral(0,1000000);
  }
 
  //normalization to data =====================
  if(_normOpts.find("dnorm")!=_normOpts.end()) {
    for(size_t ih=0;ih<_nhmc;ih++) {
      _hClones[ih]->Scale( iData/iMC );
    }
  }
  //normalization to 1 =====================
  if(_normOpts.find("uni")!=_normOpts.end()) {
    for(size_t ih=0;ih<_nhmc;ih++) {
      _hClones[ih]->Scale( 1./iMC );
    }
  }
  
  //Global Rebinning ======================

  if(_gBin==0) { //automatic binning
    _gBin = HistoUtils::autoBinning(_xmin, _xmax, theobs->nBX, 
				    theobs->binsX[0], theobs->binsX.back() );
    _gBckBin=1;
  }

  //FIXME MM
  // for(size_t ih=0;ih<_nhmc;ih++) {
  //   cout<<" fixme, line 960 Display.cc "<<endl;
  //   HistoUtils::manualCompleteRebin(_hClones[ih], 15,0,15);
  //   HistoUtils::manualCompleteRebin(_hClonesNoStack[ih],15,0,15);
  // }
  // HistoUtils::manualCompleteRebin(_hData, 15,0,15);
  //===========================

  for(size_t ih=0;ih<_nhmc;ih++) {
    if(_is1D) {
      _hClones[ih]->Rebin(_gBin);
      _hClonesNoStack[ih]->Rebin(_gBin);
    }
    if(_is2D) {
      ((TH2*)_hClones[ih])->Rebin2D(_gBin,_gBin);
      ((TH2*)_hClonesNoStack[ih])->Rebin2D(_gBin,_gBin);
    }
  }
  if(!_mcOnly) {
    if(_is1D)
      _hData->Rebin(_gBin);
    else
      ((TH2*)_hData)->Rebin2D(_gBin,_gBin);
  }
  //===============================

  //Background rebinning for 1D histograms
  if(_is1D) {
    for(size_t ih=1;ih<_hClones.size();ih++) {
      if(_isProf) {
	HistoUtils::rebin1DHisto(_hClones[ih], _gBckBin );
	HistoUtils::rebin1DHisto(_hClonesNoStack[ih], _gBckBin );
      }
      else {
	HistoUtils::manualRebin(_hClones[ih], _gBckBin );
	HistoUtils::manualRebin(_hClonesNoStack[ih], _gBckBin );
      }
    } //histos
  } //1D condition
    //==========================================

    //Overflow/Underflow bins, only for 1D histos==
  if(_is1D) {
    if(_overflow) {
      if(!_mcOnly)
	HistoUtils::addOverflowBin(_hData, _xmax);
      for(size_t ih=0;ih<_hClones.size();ih++) {
	HistoUtils::addOverflowBin(_hClones[ih], _xmax);
	HistoUtils::addOverflowBin(_hClonesNoStack[ih], _xmax);
      }
    }
    if(_underflow) {
      if(!_mcOnly)
	HistoUtils::addUnderflowBin(_hData, _xmin);
      for(size_t ih=0;ih<_hClones.size();ih++) {
	HistoUtils::addUnderflowBin(_hClones[ih], _xmin);
	HistoUtils::addUnderflowBin(_hClonesNoStack[ih], _xmin);
      }
    }
  }
  //=============================================
  
  // differential normalization ==============
  if(_normOpts.find("dif")!=_normOpts.end()) {
    for(size_t ih=0;ih<_nhmc;ih++) {
      HistoUtils::changeToDiffHisto( _hClones[ih] );
    }
    //  HistoUtils::changeToDiffHisto( _hData );
  }
  // ====================================


  //Passing from GeV to Tev =====================
  bool xAxTeV=false;
  bool yAxTeV=false;
  
  if(_xmin > (_mcOnly?_hClones[0]->GetXaxis()->GetXmax():_hData->GetXaxis()->GetXmax()) ) {
    _xmin = _mcOnly?_hClones[0]->GetXaxis()->GetXmin():_hData->GetXaxis()->GetXmin();
  }
  
  _xmin = max( (double)_xmin, _mcOnly?_hClones[0]->GetXaxis()->GetXmin():_hData->GetXaxis()->GetXmin() );
  _xmax = min( (double)_xmax, _mcOnly?_hClones[0]->GetXaxis()->GetXmax():_hData->GetXaxis()->GetXmax() );
  
  //if(_is1D)
  {
    if(!_mcOnly)
      changeGeVToTeV( _hData, theobs->titleX, theobs->titleY,
		      _xmin, _xmax, _ymin, _ymax, theobs->htype,
		      xAxTeV, yAxTeV );
 
    for(size_t ih=0;ih<_hClones.size();ih++) {
      changeGeVToTeV( _hClones[ih], theobs->titleX, theobs->titleY,
		      _xmin, _xmax, _ymin, _ymax, theobs->htype,
		      xAxTeV, yAxTeV );
    }
  }
  //=============================================

  //data conversion =============================
  _gData= NULL;
  TH1* hData=NULL;
  if(_is1D && !_mcOnly) {
    hData=(TH1*)_hData->Clone();
    if(_hData->GetEntries()!=0) {
      if(_normOpts.find("norm")!=_normOpts.end()) 
	_datNorm= 1./_hData->Integral(0,10000000);
      
      if(_normOpts.find("dif")!=_normOpts.end() ) 
	HistoUtils::changeToDiffHisto(hData);
      
      bool isDif=_normOpts.find("dif")!=_normOpts.end();
      _gData = HistoUtils::convertHistoToGraph( _hData, _datNorm, isDif );
      _gData->SetMarkerStyle(20);
    }
  }
  //===========================================

  //cumulate if needed =========================
  if(_cumulative) {
    for(size_t ih=0;ih<_nhmc;ih++) {
      TH1* htmp=(TH1*)_hClones[ih]->Clone();
      htmp->Reset("icem");
      int nb=htmp->GetNbinsX();
      for(int ib=0;ib<nb+2;ib++) {
	for(int ic=0;ic<=ib;ic++)
	  htmp->AddBinContent(ic, _hClones[ih]->GetBinContent(ib) );
      }
      _hClones[ih] = (TH1*)htmp->Clone();
    
      delete htmp;
    }
    //Data
    if(!_mcOnly) {
      TH1* htmp=(TH1*)_hData->Clone();
      htmp->Reset("icem");
      int nb=htmp->GetNbinsX();
      for(int ib=0;ib<nb+2;ib++) {
	for(int ic=0;ic<=ib;ic++) {
	  htmp->AddBinContent(ic, _hData->GetBinContent(ib) );
	}
	
      }
      _hData = (TH1*)htmp->Clone();
      delete htmp;
    }
    
  }
  //============================================
  
  //cout<<_hClones.size()<<"   "<<nsig<<endl;  
  //MC total ===================================
  if( _is1D && !_dOnly) {
    if(_hClones.size()!=nsig)
      _hMC = (TH1*)_hClones[nsig]->Clone();
    _hMC->SetLineWidth(2);
    _hMC->SetLineColor(kBlack);
    _hMC->SetFillStyle(0);
    _hMC->SetName("simulation");
  }
  // ===========================================

  //Ok, now the empty histo ====================
  
  if(_is1D) {
    _empty=NULL;
    if(_hMC)
      _empty=(TH1*)_hMC->Clone();
    else if(_hData)
      _empty=(TH1*)_hData->Clone();
  }
  else {
    //   cout<<_hClones[0]<<endl;
    //   //TH1* tmp=(TH1*)_hClones[0]))->Clone();
    _empty = (TH1*)((TH1*)(_hClones[0]))->Clone();//tmp;
  }
  
  _empty->Reset("ICEM");
  _empty->SetName("h_tmp");
  _empty->SetTitle("h_tmp");
  
  //X axis
  _xmin = xAxTeV?(_xmin/1000):_xmin;
  _xmax = xAxTeV?(_xmax/1000):_xmax;

  _empty->GetXaxis()->SetRangeUser( _xmin, _xmax -0.0001 );

  if(_is1D && !_isProf) {
    if(_overflow) {
      _empty->GetXaxis()->SetRangeUser(_xmin,_xmax-_empty->GetBinWidth(1)/1.5 -0.0001);// + empty->GetBinWidth(1) -0.00001);
      if(_underflow) {
	_empty->GetXaxis()->SetRangeUser(_xmin,_xmax-0.0001);
      }
    }
    if(_underflow) {
      _empty->GetXaxis()->SetRangeUser(_xmin ,_xmax-0.0001);
    }
  }
  
  _empty->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  
  _xtitle=theobs->titleX;
  _empty->GetXaxis()->SetTitle( xAxTeV?(_xtitle.replace( _xtitle.find("GeV") , 3, "TeV" ) + " ").c_str():(_xtitle+" ").c_str() );
  
  
  if(_is2D) {
    _ytitle = theobs->titleY;
    _empty->GetYaxis()->SetTitle( yAxTeV?(_ytitle.replace( _ytitle.find("GeV") , 3, "TeV" ) + " ").c_str():(_ytitle+" ").c_str() );
  }


  if(_hMC)
    _hMC->SetTitle( _xtitle.c_str() );

  //Y axis ===================================
  
  string addbin="";
  if(!_isProf && !_is2D) {
    float bin;
    if(_hMC!=0)
      bin = _hMC->GetXaxis()->GetBinWidth(1);
    else
      bin = _hData->GetXaxis()->GetBinWidth(1);
    ostringstream os;
    os<<bin;
    
    size_t p1= _xtitle.find("[");
    size_t p2= _xtitle.find("]");
    if(p1!=(size_t)-1 && p2!=(size_t)-1) {
      string pBin= (os.str().size()>4)?((os.str()).substr(0,4)):os.str();

      addbin=" / "+pBin+" ";
      addbin += _xtitle.substr(p1+1,p2-p1-1)+" ";
      
      if(_xtitle.substr(p1+1,p2-p1-1)=="TeV" && bin<0.1) {
	ostringstream os2;
	os2<<bin*1000;
	addbin=" / "+(os2.str()).substr(0,3)+" GeV ";
	if(_normOpts.find("dif")!=_normOpts.end())
	  addbin = " / GeV";
      }
    }
    else {
      addbin="  ";
    }

  }
  //====================================================
  if(_is1D) {
    double mcM= (!_hMC)?0.:HistoUtils::getHistoYhighWithError(_hMC,_xmin,_xmax,"G");
    double dM= _mcOnly?0.:HistoUtils::getHistoYhighWithError(hData,_xmin,_xmax,"p");
    float ymax=max((_hMC!=0)?mcM:1., dM );
    ymax *=_logYScale?15:1.5;
    if(_normOpts.find("norm")!=_normOpts.end())
      ymax =_logYScale?50:min(ymax,(float)1.5);
    // if(_normOpts.find("dif")!=_normOpts.end())
    //   ymax /= _hMC->GetBinWidth(0); //temporary fix

    float ymin = 10000000000.;
    for(size_t ih=0;ih<_nhmc;ih++) {
      //FIXME skip signals not summed
      string nh = (string)( _hClones[ih]->GetName());
      if( !(!_sSignal && nh.find("sig")!=(size_t)-1) ) {
	float ym = HistoUtils::getHistoYlow(_hClones[ih],_xmin,_xmax);
	if(ym<ymin)
	  ymin = ym;
      }
    }
        
    if(!_mcOnly) {
      float ydmin = HistoUtils::getHistoYlowWithError(hData,_xmin,_xmax);
      float dY = (!_hMC)?0.:HistoUtils::getMaxDiff(hData, _hMC,_xmin,_xmax);

      int nd = floor(log(ydmin)/log(10) );
      
      if( floor(log(dY)/log(10)) > 2 ) { //large excess in data

	int n = floor(log(ymin)/log(10) );
	int yrm = ymin/pow(10.,n);
	ymin = ((yrm==1)?(8*pow(10., n-1 )):((yrm-1)*pow(10., n )));
      }
      else { //only one decade below data
	int yrm = ydmin/pow(10.,nd); //thick in decade below the point
	ymin = (yrm==1)?(8*pow(10., nd-2 )):(yrm* pow(10., nd-1 )); 
      }
    }
    else {
      int n = floor(log(ymin)/log(10) );
      int yrm = ymin/pow(10.,n);
      ymin = ((yrm==1)?(8*pow(10., n-1 )):((yrm-1)*pow(10., n )));
    }
    
    float ym = _logYScale?ymin:0.1;
    if(_normOpts.find("norm")!=_normOpts.end() || 
       _normOpts.find("uni")!=_normOpts.end() )
      ym = ymin;
    _empty->GetYaxis()->SetRangeUser( ym, ymax );
    if(_userYScale)
      _empty->GetYaxis()->SetRangeUser( _ymin, _ymax );
    
    _empty->GetYaxis()->SetTitle((_ytitle+addbin).c_str());
  
    if(_normOpts.find("norm")!=_normOpts.end())
      _empty->GetYaxis()->SetTitle("1/N#times#partialN/#partialO  ");
  }
  else {
    _empty->GetYaxis()->SetRangeUser( _ymin, _ymax );
    //_empty->GetYaxis()->SetTitle((theobs->titleY).c_str());
  }

  _empty->GetYaxis()->SetNdivisions(_Ydiv[0],_Ydiv[1],_Ydiv[2]);
  

  //Fix the labels and title positions and sizes
  // _empty->GetXaxis()->SetTitleOffset( 1.05 ); //1.05
  // //_empty->GetXaxis()->SetTitleSize( 0.06 );
  // _empty->GetXaxis()->SetTitleSize( 38/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC() ) );
  // _empty->GetYaxis()->SetTitleOffset( _rmLabel?1000.:1.50 );
  // _empty->GetYaxis()->SetTitleSize( _rmLabel?0.:38/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC() ) );
  // //_empty->GetXaxis()->SetLabelOffset( 0.007 );
  // _empty->GetXaxis()->SetLabelOffset( 5/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC() ) );
  // _empty->GetXaxis()->SetLabelSize( 31/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC() ) );
  // _empty->GetYaxis()->SetLabelOffset( _rmLabel?1000.:(5/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC()) ) );
  // _empty->GetYaxis()->SetLabelSize( _rmLabel?0.:(31/(_pads[0][0]->GetWh()*_pads[0][0]->GetAbsHNDC()) ) );
  //============================================

  if(_showRatio)
    _empty->GetXaxis()->SetTitleOffset( 1.05 ); //1.05
  else
    _empty->GetXaxis()->SetTitleOffset( 0.90 ); //1.05  
  _empty->GetXaxis()->SetTitleSize( 0.06 );
  _empty->GetYaxis()->SetTitleOffset( 1.50 );
  _empty->GetYaxis()->SetTitleSize( 0.06 );
  _empty->GetXaxis()->SetLabelOffset( 0.007 );
  _empty->GetXaxis()->SetLabelSize( 0.05 );
  _empty->GetYaxis()->SetLabelOffset( 0.007 );
  _empty->GetYaxis()->SetLabelSize( 0.05 );

  
  // 2D histo
  if(_is2D)
    HistoUtils::copyStyle(_empty,_hClones[nsig],true);

  for(size_t ih=0;ih<_nhmc;ih++) {
    delete hTmps[ih];
  }
  
  delete hData;

}

void 
Display::saveDMCRWeight(string fname, string hname) {

  TFile* file=new TFile(fname.c_str(),"RECREATE");

  TH1F* ratio  = (TH1F*)_hData->Clone(hname.c_str() );
  ratio->Divide(_hMC);

  ratio->Write();
  file->Write();
  file->Close();

  delete ratio;
  delete file;

}

void 
Display::drawDataMCRatio() {
  
  //The bidon histo
  TH1* emptyHisto=(TH1*)_hMC->Clone();
  emptyHisto->Reset("ICEM");

  // TH1F* ratio = (TH1F*)_hData->Clone();
  

  // ratio->Divide(_hMC);
  
  TGraphAsymmErrors* ratio = HistoUtils::ratioHistoToGraph( _hData, _hMC );
  ratio->SetName( ("ratio") );
 
  for(int ib=0;ib<emptyHisto->GetNbinsX()+2;ib++)
    { emptyHisto->SetBinContent(ib,-1000);   }

  if(_rmLabel)  {
    emptyHisto->GetYaxis()->SetTitleOffset(1000);
    ratio->GetYaxis()->SetTitleOffset(1000);
  }
 
  if(!_overflow && !_underflow) {
    emptyHisto->GetXaxis()->SetRangeUser(_xmin, _xmax-0.0001 );
    ratio->GetXaxis()->SetRangeUser(_xmin,_xmax-0.0001 );
  }
  else {
    if(_overflow) {
      emptyHisto->GetXaxis()->SetRangeUser(_xmin,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      ratio->GetXaxis()->SetRangeUser(_xmin,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      
      if(_underflow) {
	emptyHisto->GetXaxis()->SetRangeUser(_xmin ,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
	ratio->GetXaxis()->SetRangeUser(_xmin ,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      }
    }
    if(_underflow) {
      emptyHisto->GetXaxis()->SetRangeUser(_xmin ,_xmax-0.0001);
      ratio->GetXaxis()->SetRangeUser(_xmin ,_xmax-0.0001);
    }
  }

  //if systematics exists, draw them!
  bool isTeV = _xtitle.find("TeV")!=(size_t)-1;
 
  vector<TPolyLine*> sysBand;
  if(_addSyst) {

    int style[4]={0,0,3002,3001};
    int col[4]={kRed+2,kViolet+2,kAzure+2,kGreen+1};
    int lStyle[4]={1,2,2,2};

    for(int iu=0;iu<min(4,(int)(_mcUncert.size()));iu++) { 
      int ii=_mcUncert.size()-iu-1;

      double x,y,yl,yh;
  
      vector<float> xs;
      vector<float> yls;
      vector<float> yhs;
 
      for(int ib=0;ib<_mcUncert[ii]->GetN();ib++) {
	_mcUncert[ii]->GetPoint(ib,x,y);
	yl = _mcUncert[ii]->GetErrorYlow(ib);
	yh = _mcUncert[ii]->GetErrorYhigh(ib);
    
	if(x<_xmin-emptyHisto->GetBinWidth(1)/2. || x>_xmax+emptyHisto->GetBinWidth(1)/2. ) continue;
      
	xs.push_back(x);
	if(y!=0) {
	  yls.push_back(yl/y);
	  yhs.push_back(yh/y);
	}
	else {
	  yls.push_back(0);
	  yhs.push_back(0);
	}
	//cout<<x<<"   "<<y<<"  "<<yl<<"   "<<yh<<"   "<<yl/y<<"   "<<yh/y<<endl;
      }
      //    cout<<"===="<<_xmax<<endl;
      sysBand.push_back( HistoUtils::GetSystBand(xs,yls,yhs,_xmin,_xmax) );
      if(_uncDet && _mcUncert.size()>1 ) {
	int sSy = _mcUncert.size();
	
	sysBand.back()->SetLineColor(col[iu+4-sSy]);
	sysBand.back()->SetLineStyle(lStyle[iu+4-sSy]);
	sysBand.back()->SetLineWidth(2);
	sysBand.back()->SetFillColor(col[iu+4-sSy]);
	sysBand.back()->SetFillStyle(style[ iu+4-sSy ]);
      } 
    }
  }//addsyst  

  emptyHisto->GetYaxis()->SetRangeUser( 0.4, 1.6);// 1-rangemax, 1+rangemax );
  emptyHisto->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  emptyHisto->GetYaxis()->SetNdivisions(3,_Ydiv[1],_Ydiv[2]);
  emptyHisto->GetXaxis()->SetTitle( (_xtitle+" ").c_str() );
  emptyHisto->GetYaxis()->SetTitle( "Data/MC" );
  emptyHisto->GetXaxis()->SetTitleSize(0.20);
  emptyHisto->GetXaxis()->SetTitleOffset(0.80); //0.85
  emptyHisto->GetXaxis()->SetLabelOffset(0.007);
  emptyHisto->GetXaxis()->SetLabelSize(0.165);
  emptyHisto->GetYaxis()->SetLabelSize(0.14);
  emptyHisto->GetYaxis()->SetLabelOffset(0.011);
  emptyHisto->GetYaxis()->SetTitleSize(0.15);
  emptyHisto->GetYaxis()->SetTitleOffset(0.54);
  emptyHisto->GetXaxis()->SetTickLength(0.09);
  emptyHisto->GetYaxis()->SetTickLength(0.05);
  
  ratio->GetYaxis()->SetRangeUser(  0.4, 1.6);//1-rangemax, 1+rangemax );
  ratio->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  ratio->GetYaxis()->SetNdivisions(3,_Ydiv[1],_Ydiv[2]);
  ratio->GetXaxis()->SetTitle( (_xtitle+"_").c_str() );
  ratio->GetYaxis()->SetTitle( "Data/MC" );
  ratio->GetXaxis()->SetTitleSize(0.20);
  ratio->GetXaxis()->SetTitleOffset(0.83);
  ratio->GetXaxis()->SetLabelSize(0.165);
  ratio->GetYaxis()->SetLabelSize(0.14);
  ratio->GetYaxis()->SetLabelOffset(0.015);
  ratio->GetYaxis()->SetTitleSize(0.15);
  ratio->GetYaxis()->SetTitleOffset(0.54);
  ratio->GetXaxis()->SetTickLength(0.09);
  ratio->GetYaxis()->SetTickLength(0.05);
 
  if(_rmLabel)  {
    ratio->GetYaxis()->SetLabelOffset(1000);
    emptyHisto->GetYaxis()->SetLabelOffset(1000);
  }

  emptyHisto->Draw();
  if(_addSyst) {
    if(!_uncDet) sysBand.back()->Draw("F");
    else {
      for(int iu=(int)sysBand.size()-1;iu>=0;iu--) {
	int sSy = _mcUncert.size();
	sysBand[iu]->Draw((iu+4-sSy)<2?"l":"F");
      }
    }
  }
  //sysBand->Draw("F");
  
  ratio->Draw("p same");
  //isTeV?_xmin:(_xmin/1000),1,isTeV?_xmax:(_xmax/1000),1);
  TLine* line=new TLine(_xmin,1,_xmax,1);
  line->SetLineColor(kRed+1);
  if(_uncDet) line->SetLineColor(kGreen+2);
  line->SetLineStyle(7);
  line->SetLineWidth(2);
  line->Draw("same");
  
}

void
Display::ratioObservables(vector<const hObs*> theObs) {

  softReset();
  
  //protection against 2D histos
  if(theObs[0]->htype.find("2D")!=(size_t)-1 || 
     theObs[1]->htype.find("2D")!=(size_t)-1) {
    cout<<" 2D histograms not supported for ratio"<<endl;
    return;
  }
  
  //protection against difference binnings
  if( theObs[0]->nBX != theObs[1]->nBX ) {
    cout<<" Error, binning between observables is different "<<endl;
    return;
  }
  
  //and get the canvas ready
  _nvars=1;
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();
  
  //now prepare the two distributions (for simulation and data)
  prepareHistograms( theObs[0] );
  
  TH1* numHMc = (TH1*)_hMC->Clone();
  TH1* numHD(0);
  if(!_mcOnly)
    numHD = (TH1*)_hData->Clone();
  vector<TH1*> numHClones;
  for(size_t ii=0;ii<_hClones.size();ii++)
    numHClones.push_back( (TH1*)_hClones[ii]->Clone() );

  string ytitle = _hMC->GetTitle();

  prepareHistograms( theObs[1] );
  ytitle += " / ";
  ytitle += _hMC->GetTitle();

  // compute the ratios ===================
  TGraphAsymmErrors* ratioData(0);
  if(!_mcOnly) {
    ratioData = HistoUtils::ratioHistoToGraph(numHD, _hData,"nP");
    ratioData->SetName("ratioData");
  }

  TGraphAsymmErrors* ratioMC = HistoUtils::ratioHistoToGraph(numHMc, _hMC,"");
  ratioMC->SetName("ratioMC");
  
  vector<TGraphAsymmErrors*> ratioClones;
  for(size_t ii=0;ii<_hClones.size();ii++) {
    ratioClones.push_back( HistoUtils::ratioHistoToGraph(numHClones[ii], _hClones[ii] ,"") );

    _itCol = _colors.find( _names[_nhmc-ii-1] );

    ratioClones.back()->SetMarkerColor( _itCol->second );
    ratioClones.back()->SetLineColor( _itCol->second );
    ratioClones.back()->SetMarkerStyle(21+ii);
  }

  //Cosmetics =============================
  if(!_mcOnly) {
    ratioData->SetMarkerColor(1);
    ratioData->SetLineColor(1);
    ratioData->SetMarkerStyle(20);
  }    

  ratioMC->SetMarkerColor(kOrange+7);
  ratioMC->SetLineColor(kOrange+7);
  ratioMC->SetMarkerStyle(24);
  
  //now draw! =======================
  _empty->GetYaxis()->SetTitle( ytitle.c_str() );//_ytitle.c_str() );
  for(int i=1;i<_empty->GetNbinsX()+1;i++) 
    _empty->SetBinContent(i,-1000);
  
  //with the good range
  float ymin=0, ymax=2;
 
  // float xmin = max( (double)_xmin, _empty->GetXaxis()->GetXmin() );
  // float xmax = min( (double)_xmax, _empty->GetXaxis()->GetXmax() );

  if(_normOpts.find("norm")==_normOpts.end()) { 
    ymin = HistoUtils::getGraphYlowWithError(ratioClones[0], _xmin, _xmax );
    ymax = HistoUtils::getGraphYhighWithError(ratioClones[0], _xmin, _xmax );
    for(size_t ii=1;ii<ratioClones.size();ii++) {
      ymin = min( ymin, HistoUtils::getGraphYlowWithError(ratioClones[ii], _xmin, _xmax ) );
      ymax = max( ymax, HistoUtils::getGraphYhighWithError(ratioClones[ii], _xmin, _xmax) );
    }
  }
  else {
    ymin = HistoUtils::getGraphYlowWithError(ratioMC, _xmin, _xmax );
    ymax = HistoUtils::getGraphYhighWithError(ratioMC, _xmin, _xmax );
  }
  
  if(!_mcOnly) {
    ymin = min( HistoUtils::getGraphYlowWithError(ratioData, _xmin, _xmax ), ymin );
    ymax = max( HistoUtils::getGraphYhighWithError(ratioData, _xmin, _xmax ), ymax );
  }
  
  _empty->GetYaxis()->SetRangeUser(ymin*0.9, ymax*1.1);

  _empty->DrawCopy();


  TLine* line=new TLine(_xmin,1,_xmax,1);
  line->SetLineColor(kGray+2);
  line->SetLineWidth(2);
  line->SetLineStyle(7);
  line->Draw("same");

  if(!_mcOnly) ratioData->Draw("p");
  
  if(_normOpts.find("norm")==_normOpts.end())
    ratioMC->Draw("p");
  else {
    for(size_t ii=0;ii<ratioClones.size();ii++)
      ratioClones[ii]->Draw("p");
  }
  
  //FIXME
  _leg=NULL;
  
}

void
Display::saveHistos(string o1, const hObs* theObs) {
  
  TFile* oFile=new TFile((o1+".root").c_str(), "RECREATE");
  for(size_t ih=0;ih<_nhmc;ih++) {
    _hClones[ih]->Write();
  }
  if(!_mcOnly) {
    _hData->Write();
  }

  oFile->Close();


}

//
// Specific function!!!!
//
void
Display::saveHistosSpecLQ(string o1) {

  map<string,string> cNames;
  cNames[ "t#bar{t}" ]="h_ttbar";
  cNames[ "VV" ]="h_ewk";
  //cNames[ "" ]="h_sig";
  cNames[ "fake #tau" ]="h_wjets";
  cNames[ "Z+jets" ]="h_zjets";
  cNames[ "Z(ll/#tau#tau)+jets" ]="h_zjets";
  cNames[ "single-t" ]="h_singlet";
  
  vector<string> sigs;
  for(size_t ih=0;ih<_nhmc;ih++) {
    string nh = (string)( _hClones[ih]->GetName());
    if(  nh.find("sig")!=(size_t)-1) {
      size_t p1=nh.find(" ") ;
      size_t p2=nh.find("GeV");
      string e = nh.substr(p1+1, p2-2-p1);
      cout<<" signal "<<e<<endl;
      sigs.push_back(e);
    }
  }

  FILE *test_;
  test_=fopen( o1.c_str() , "r" );
  if( test_==0 ) {
    TString command_ = TString("mkdir -p ") + o1; 
    system( command_.Data());
  }
  else
    fclose( test_ );

  for(size_t is=0;is<sigs.size();is++) {
    
    TFile* oFile=new TFile((o1+"/elTau_M"+sigs[is]+".root").c_str(), "RECREATE");
    for(size_t ih=0;ih<_nhmc;ih++) {
      TH1F* hMC=(TH1F*)_hClonesNoStack[ih]->Clone();
      string nh = (string)( hMC->GetName());
      if(nh.find("sig")!=(size_t)-1) {
	if(nh.find(sigs[is])!=(size_t)-1) {
	  hMC->SetName("h_sig");
	  hMC->SetTitle("h_sig");
	  hMC->Write();
	}
      }
      else {
	string name=hMC->GetName();
	size_t p1=name.find("_");
	name = name.substr(p1+1,name.size()-p1-1);
	
	map<string,string>::iterator it;
	it = cNames.find(name);
	if(it==cNames.end()) continue;
	  
	hMC->SetName( (cNames[ name ]).c_str() );
	hMC->SetTitle( (cNames[ name ]).c_str() );
	hMC->Write();
      }
    }
    if(!_mcOnly) {
      TH1F* hD=(TH1F*)_hData->Clone();
      hD->SetName("h_data");
      hD->SetTitle("h_data");
      hD->Write();
    }
    
    oFile->Close();
    
  }

}


void
Display::showSignificance(const hObs* theObs) {
  
  softReset();

  _sSignal=false;

  _xmax = theObs->binsX.back();
  if(_is2D)
    _ymax = theObs->binsY.back();
  _xmin = theObs->binsX[0];
  if(_is2D)
    _ymin = theObs->binsY[0];

  //  //first clean objects
  // for(size_t ix=0;ix<_pads.size();ix++)
  //   for(size_t iy=0;iy<_pads[ix].size();iy++) {
  //     delete _pads[ix][iy];
  //   }  

  // _pads.clear();
  
  // delete _leg;
  // delete _c;
  
  //and get the canvas ready
  

  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();

  //now prepare the two distributions (for background and signal)
  prepareHistograms( theObs );
  

  vector<size_t> sigs;
  for(size_t i=0;i<_nhmc;i++) {
    string nh = (string)( _hClones[i]->GetName());
    if( !_sSignal && nh.find("sig")!=(size_t)-1) 
      sigs.push_back(i);
  }

  if(_is1D) {

    TH1F* numHB = (TH1F*)_hMC->Clone();
    TH1F* numHS = (TH1F*)_hClones[ sigs.back() ]->Clone();

    TH1F* sig = HistoUtils::significance( numHS, numHB);
    HistoUtils::copyStyle( _empty, (TH1*&)sig, true );
    sig->GetYaxis()->SetTitle("S/#sqrt{B} ");
    sig->Draw();
  }
  if(_is2D) {

    TH2F* numHB = (TH2F*)_hClones[ sigs.size() ]->Clone();
    TH2F* numHS = (TH2F*)_hClones[ sigs.back() ]->Clone();
    
    // cout<<numHB->Integral()<<"   "<<numHS->Integral()<<endl;

    TH2F* sig = HistoUtils::significance(numHS, numHB);
    HistoUtils::copyStyle( (TH1*)numHB, (TH1*&)sig, true );
    
    sig->Draw("colz");
  }
  
  _sSignal=true;
}

void
Display::drawEfficiency(const hObs* theObs) {


  softReset();
  
  //protection against 2D histos
  if(theObs->htype.find("2D")!=(size_t)-1) {
    cout<<" 2D histograms not supported for efficiencies"<<endl;
    return;
  }
  

  //and get the canvas ready
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();

  //prepare the distributions
  prepareHistograms( theObs );

  //compute efficiencies
  vector<TGraphAsymmErrors*> effs(_nhmc,NULL);
  for(size_t i=0;i<_nhmc;i++) {
    effs[i] = HistoUtils::efficiency(_hClonesNoStack[i]);
  }
  effs.push_back( HistoUtils::efficiency( _hMC ) );
  if(!_mcOnly)
    effs.push_back( HistoUtils::efficiency( _hData ) );
  
  //cosmetics
  for(size_t i=0;i<_nhmc;i++) {
    effs[i]->SetLineColor( _colors.find( _names[_nhmc-i-1] )->second );
    effs[i]->SetMarkerColor( _colors.find( _names[_nhmc-i-1] )->second );
    effs[i]->SetLineWidth(2);
    effs[i]->SetMarkerStyle(1);
  }

  effs[_nhmc]->SetLineColor(kGray+2);
  effs[_nhmc]->SetMarkerColor(kGray+2);
  effs[_nhmc]->SetLineWidth(2);
  effs[_nhmc]->SetMarkerStyle(1);

  if(!_mcOnly) {
    effs.back()->SetLineColor(kBlack);
    effs.back()->SetMarkerColor(kBlack);
    effs.back()->SetLineWidth(2);
    effs.back()->SetMarkerStyle(1);
  }

  //now draw! =======================
  _empty->GetYaxis()->SetTitle( "#varepsilon  " );
  for(int i=1;i<_empty->GetNbinsX()+1;i++) 
    _empty->SetBinContent(i,-1000);
  _empty->GetYaxis()->SetRangeUser(0., 1.05);
  _empty->DrawCopy();

  for(size_t i=0;i<effs.size();i++)
    effs[i]->Draw("l");

}

void
Display::drawROCCurves(const hObs* theObs) {


  softReset();
  
  //protection against 2D histos
  if(theObs->htype.find("2D")!=(size_t)-1) {
    cout<<" 2D histograms not supported for ROC curves"<<endl;
    return;
  }
  

  //and get the canvas ready
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();

  //prepare the distributions
  prepareHistograms( theObs );
 
  for(size_t i=0;i<_nhmc;i++) {
    string nh = (string)( _hClonesNoStack[i]->GetName());
    if(nh.find("sig")!=(size_t)-1) 
      _hSigClones.push_back( (TH1*)_hClonesNoStack[i]->Clone() );
  }

  if(_hSigClones.size()==0) {cout<<" No Signal to use, abort ROC curve drawing"<<endl; return;}
   
  //compute ROC curves
  vector<TGraphAsymmErrors*> rocCurves(_hSigClones.size(),NULL);
  for(size_t is=0;is<_hSigClones.size();is++) {
    rocCurves[is] = HistoUtils::curveROC(_hSigClones[is],_hMC);
    rocCurves[is]->SetLineColor( _hSigClones[is]->GetLineColor() );
    rocCurves[is]->SetMarkerColor( _hSigClones[is]->GetLineColor() );
    rocCurves[is]->SetLineWidth(2);
  }

  // for(size_t ii=0;ii<_hSigClones.size();ii++)
  //   delete _hSigClones[ii];
  // _hSigClones.clear();


  TH1F* tmp=new TH1F("dummy","dummy",51,0,1.05);
  TH1* empty=(TH1*)tmp->Clone();
  HistoUtils::copyStyle(_empty,empty,true);
  empty->GetYaxis()->SetRangeUser(0,1.05);
  empty->GetXaxis()->SetTitle(" #varepsilon signal  ");
  empty->GetYaxis()->SetTitle(" #varepsilon bkg ");

  empty->Draw();
  for(size_t is=0;is<rocCurves.size();is++) {
    rocCurves[is]->Draw("l");
  }
  
}


void
Display::compaROCCurves(vector<const hObs*> obss) {
  
  softReset();
  
  //and get the canvas ready
  int tmpNvar= _nvars;
  _nvars=1;
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();
  _nvars=tmpNvar;

  //compute ROC curves
  vector<TGraphAsymmErrors*> rocCurves;
  
  //prepare the distributions
  for(size_t io=0;io<obss.size();io++) {
    refreshHistos();
    //protection against 2D histos
    if(obss[io]->htype.find("2D")!=(size_t)-1) {
      cout<<" 2D histograms not supported for ROC curves"<<endl;
      return;
    }

    prepareHistograms( obss[io] );
    
    for(size_t i=0;i<_nhmc;i++) {
      string nh = (string)( _hClonesNoStack[i]->GetName());
      if(nh.find("sig")!=(size_t)-1) {
	_hSigClones.push_back( (TH1*)_hClonesNoStack[i]->Clone() );
      }
    }
    _nhsig=_hSigClones.size();
    
    if(io==0)
      if(_hSigClones.size()==0) {cout<<" No Signal to use, abort ROC curve drawing"<<endl; return;}

    rocCurves.resize( obss.size()*_nhsig );
      
    //cout<<_nhsig<<endl;
    for(size_t is=0;is<_nhsig;is++) {
      //cout<<_hMC<<"   "<<_hSigClones[is]<<endl;
      rocCurves[io*_nhsig +is] = HistoUtils::curveROC(_hSigClones[is],_hMC);
      rocCurves[io*_nhsig +is]->SetLineColor( _hSigClones[is]->GetLineColor() );
      rocCurves[io*_nhsig +is]->SetMarkerColor( _hSigClones[is]->GetLineColor() );
      rocCurves[io*_nhsig +is]->SetLineWidth(2);
      rocCurves[io*_nhsig +is]->SetLineStyle(io+1);
    }

    // for(size_t ii=0;ii<_hSigClones.size();ii++)
    //   delete _hSigClones[ii];
    // _hSigClones.clear();
    
  }

  TH1F* tmp=new TH1F("dummy","dummy",51,0,1.05);
  TH1* empty=(TH1*)tmp->Clone();
  HistoUtils::copyStyle(_empty,empty,true);
  empty->GetYaxis()->SetRangeUser(0,1.05);
  empty->GetXaxis()->SetTitle(" #varepsilon signal  ");
  empty->GetYaxis()->SetTitle(" #varepsilon bkg ");

  empty->Draw();
  for(size_t is=0;is<rocCurves.size();is++) {
    rocCurves[is]->Draw("l");
  }

  _nhsig=0;
}


void
Display::residualData(const hObs* theObs) {

  softReset();
  
  //protection against 2D histos
  if(theObs->htype.find("2D")!=(size_t)-1) {
    cout<<" 2D histograms not supported for ratio"<<endl;
    return;
  }
  

  //protection against no data
  if(_mcOnly) {
    cout<<" Warning, no data, cannot draw the residuals"<<endl;
    return;
  }

  
  //and get the canvas ready
  
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();
  
  //now prepare the two distributions (for simulation and data)
  prepareHistograms( theObs );
  
  TH1* numHMc = (TH1*)_hMC->Clone();
  TH1* numHD = (TH1*)_hData->Clone();
  
  // compute the residuals ===================
  TGraphAsymmErrors* residuals = HistoUtils::residualGraph( numHD, numHMc, "nP" );
  residuals->SetName("residuals");
  
  //Cosmetics =============================
  residuals->SetMarkerColor(1);
  residuals->SetLineColor(1);
  residuals->SetMarkerStyle(20);
    

  //now draw! =======================
  //_empty = (TH1*)_hMC->Clone(); _empty->Reset("ICEM");
  _empty->GetYaxis()->SetTitle( _ytitle.c_str() );
  for(int i=1;i<_empty->GetNbinsX()+1;i++) 
    _empty->SetBinContent(i,-1000);
  
  //with the good range
  float ymin=0, ymax=2;
 
  // float xmin = max( (double)_xmin, _empty->GetXaxis()->GetXmin() );
  // float xmax = min( (double)_xmax, _empty->GetXaxis()->GetXmax() );

 
  ymin = min(ymin, HistoUtils::getGraphYlowWithError(residuals, _xmin, _xmax ) );
  ymax = HistoUtils::getGraphYhighWithError(residuals, _xmin, _xmax );
    
  
  _empty->GetYaxis()->SetRangeUser(ymin*0.9, ymax*1.1);

  _empty->DrawCopy();
  

  //systematic uncertainties =============
  if(_addSyst && _hMC) {
    computeSystematics(_isProf);
   
    TGraphAsymmErrors* mcUnc = (TGraphAsymmErrors*) _mcUncert.back()->Clone();
      
    double x,y;
    for(int i=0;i<mcUnc->GetN();i++) {
      mcUnc->GetPoint(i,x,y);
      mcUnc->SetPoint(i,x,1);
    }
	
    mcUnc->SetName("uncertainties");
    mcUnc->SetTitle("uncertainties");
    
    mcUnc->SetMarkerSize(0); 
    mcUnc->SetMarkerStyle(1);
    mcUnc->SetMarkerColor(1);
    mcUnc->SetFillStyle(3001);
    mcUnc->SetFillColor(kGray+1);
    TGraphAsymmErrors* tmpUnc=(TGraphAsymmErrors*)mcUnc->Clone();
    tmpUnc->SetName("uncertainties");
    tmpUnc->SetTitle("uncertainties");
    tmpUnc->Draw("p E2");
    
    //mcUnc->DrawClone("p E2");
      
  }
  


  TLine* line=new TLine(_xmin,1,_xmax,1);
  line->SetLineColor(kGray+2);
  line->SetLineWidth(2);
  line->SetLineStyle(7);
  line->Draw("same");

  residuals->Draw("p");
  
  //FIXME
  _leg=NULL;
  
}

void
Display::drawCumulativeHistos(const hObs* theObs ) {
 
  softReset();
  _cumulative =true;
  
  vector<const hObs*> hv;
  hv.push_back(theObs);
  plotDistributions(hv);
  _cumulative =false;

}

void
Display::drawStatistics( vector<pair<string,vector<vector<float> > > > vals) {
  softReset();


  vector<string> cNames;
  vector<TH1*> hMC;
  TH1F* hData;

  for(size_t ic=0;ic<vals.size();ic++) {
    cNames.push_back( vals[ic].first );
  }
  
  for(size_t i=0;i<vals[0].second.size()-2; i++ ) {
    TH1F* tmp = new TH1F(_names[i].c_str(),_names[i].c_str(),cNames.size(),0,cNames.size());
    _itCol = _colors.find( _names[i] );
    tmp->SetFillColor(_itCol->second);
    tmp->SetLineColor(_itCol->second);
    tmp->SetLineWidth(2);
    
    hMC.push_back( (TH1*)tmp );
  }
  hData = new TH1F("statData","statData",cNames.size(),0,cNames.size());
  TH1F* hMCt = new TH1F("statMC","statMC",cNames.size(),0,cNames.size());
  TGraphAsymmErrors* mcUncert = new TGraphAsymmErrors(cNames.size() );
  
  //now fill the plots
  for(size_t ic=0;ic<vals.size();ic++) {

    for(size_t id=0;id<vals[ic].second.size();id++) {
     
      if(id==0) {//vals[ic].second.size()-2) { //MC total
	hMCt->SetBinContent( ic+1, vals[ic].second[id][0] );
	hMCt->SetBinError( ic+1, vals[ic].second[id][1] );
	mcUncert->SetPoint( ic, ic+0.5 , vals[ic].second[id][0] );
	mcUncert->SetPointError( ic, 0.25,0.25, vals[ic].second[id][2], vals[ic].second[id][3] );
      }
      else if(id==vals[ic].second.size()-1) { //data
	hData->SetBinContent( ic+1, vals[ic].second[id][0] );
      }
      else {
	//_itW = _gWeights.find( _names[_nhmc-id-1] );
	
	float sum = vals[ic].second[id][0];
	//cout<<id<<" sum "<<sum<<endl;
	for(size_t ii=1;ii<id;ii++) {
	  sum += vals[ic].second[ii][0];
	  //cout<<" + "<<vals[ic].second[ii][0]<<" = "<<sum<<endl;
	}
	//cout<<" total : "<<sum<<endl;
	hMC[id-1]->SetBinContent( ic+1, sum );
	//	hMC[id]->SetBinError( ic, vals[ic].second[id][1] );
      }

    }//ds
  }//cuts
  
  //and get the canvas ready
  
  _pads = preparePads();
  _pads[0][0]->Draw();
  _pads[0][0]->cd();
  
  TGraphAsymmErrors* gData= HistoUtils::convertHistoToGraph(hData,1,
							    _normOpts.find("dif")!=_normOpts.end());

  //error correction
  // if(_normOpts.find("dif")!=_normOpts.end()) {
  //   for(int ip=0;ip<gData->GetN();ip++) {
  //     float eyl = gData->GetErrorYlow(ip);
  //     float eyh = gData->GetErrorYhigh(ip);
  //     gData->SetPointError(0,0,eyl,eyh);
  //   }
  // }
  

  TH1F* emptyH = (TH1F*)hData->Clone();
  emptyH->Reset("ICEM");
  
  emptyH->SetFillColor(0);

  

  float ymin = min( (float)0.1, HistoUtils::getHistoYlowWithError(hMCt, _xmin, _xmax ) );
  float ymax = HistoUtils::getHistoYhighWithError(hMCt, _xmin, _xmax );
  
  ymin = min( HistoUtils::getGraphYlowWithError(gData, _xmin, _xmax ), ymin );
  ymax = max( HistoUtils::getGraphYhighWithError(gData, _xmin, _xmax ), ymax );
  // if(_logYScale)
  //   ymax*= 10;
  // else
  ymax*=1.3;

  emptyH->GetYaxis()->SetRangeUser(ymin,ymax);
  emptyH->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  emptyH->GetYaxis()->SetNdivisions(_Ydiv[0],_Ydiv[1],_Ydiv[2]);
  emptyH->GetYaxis()->SetTitle(_ytitle.c_str());
  for(size_t ib=0;ib<cNames.size();ib++) {
    emptyH->GetXaxis()->SetBinLabel(ib+1, cNames[ib].c_str() );
  }
  emptyH->Draw();
  
  for(size_t ih=0;ih<hMC.size();ih++) {
    hMC[_nhmc-ih-1]->Draw("same hist");
  }

  hMCt->SetLineWidth(2);
  hMCt->SetLineColor(kBlack);
  hMCt->SetFillStyle(0);
  hMCt->Draw("same hist");
  gData->Draw("p");

  cmsPrel();

  // _hCoords.push_back( HistoUtils::getHistoUpperCoordinates(hMCt) );
  // _hCoords.push_back( HistoUtils::getHistoUpperCoordinates(hData) );

  //copy the graphs for legend purposes ... ugly...
  _hClones = hMC;
  _hMC = hMCt;
  _hData = hData;
  _mcUncert[0] = mcUncert;
  
  _pads[0][0]->Update();
  
  adjustLegend(0);
  if(_leg) {
    TLegend* tmpleg=(TLegend*)_leg->Clone();
    tmpleg->Draw("same");
    _pads[0][0]->RedrawAxis();
  }
  
}

void
Display::configureDisplay(string YTitle, double rangeY[2], 
			  double rangeX[2], bool logYscale,
			  int Xdiv[3], int Ydiv[3], 
			  int gbin, int bckbin, 
			  bool OverFlowBin, bool UnderFlowBin,
			  bool ShowDMCRatio, bool ShowGrid, bool staking,
			  bool AddSystematics, bool mcStatSyst,
			  float MarkerSize, float LineWidth, bool sSignal,
			  bool mcOnly, bool cmsPrel, bool uncDet ) {

  _ytitle = YTitle;
  _ymin = rangeY[0];
  _ymax = rangeY[1];
  _xmin = rangeX[0];
  _xmax = rangeX[1];

  _userYScale=false;
  if(_ymin != 0 && _ymax != _ymin )
    _userYScale=true;

  _xCoordSave[0] = _xmin;
  _xCoordSave[1] = _xmax;


  _logYScale = logYscale;
  for(int i=0;i<3;i++) {
    _Xdiv[i] = Xdiv[i];
    _Ydiv[i] = Ydiv[i];
  }
  _gBin = gbin;

  _binningSave = _gBin;

  _gBckBin = bckbin;
  _overflow = OverFlowBin;
  _underflow = UnderFlowBin;
  _showRatio = ShowDMCRatio;
  _showGrid = ShowGrid;
  _noStack = 1-staking;
  _addSyst = AddSystematics;
  _mcSyst= mcStatSyst;
  _mSize = MarkerSize;
  _wLine = LineWidth;

  _sSignal = sSignal;

  _mcOnly = mcOnly;
  _prel = cmsPrel;

  _uncDet = uncDet;
}


void
Display::checkData() {
  //cout<<" checking data "<<_mcOnly<<"   "<<_lockData<<endl;
  //FIXME
  if(_mcOnly) _mcOnly=false;
  // else _mcOnly=false;
  _lockData=true;
}

void
Display::isNoData() {
  if(!_lockData)
    if(!_mcOnly) _mcOnly=true;

  _dOnly=false;
}



void
Display::computeSystematics(bool isProf, bool cumul) {

  for(size_t ii=0;ii<_mcUncert.size();ii++)
    delete _mcUncert[ii];
  _mcUncert.clear();


  systM systs = _csystM[0];
  systM systsUp = _csystM[1];
  systM systsDo = _csystM[2];
   
  //protection against no uncertainty on the plot
  if( (systs.size() + systsUp.size() + _mcSyst) == 0) {_addSyst=false; return;}

  //First, rebin the histograms, they are already copied ==============
  for(itSystM itS=systs.begin();itS!=systs.end();itS++) {
    ((*itS).second)->Rebin(_gBin);
  }
  for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++) {
    ((*itS).second)->Rebin(_gBin);
  }
  for(itSystM itS=systsDo.begin();itS!=systsDo.end();itS++) {
    ((*itS).second)->Rebin(_gBin);
  }
  //==========================================
  
  //overflow/underfow bins ===================
  if(_overflow) {
    for(itSystM itS=systs.begin();itS!=systs.end();itS++) {
      HistoUtils::addOverflowBin( (*itS).second, _xmax);
    }
    for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++) {
      HistoUtils::addOverflowBin( (*itS).second, _xmax);
    }
    for(itSystM itS=systsDo.begin();itS!=systsDo.end();itS++) {
      HistoUtils::addOverflowBin( (*itS).second, _xmax);
    }
  }
  if(_underflow) {
    for(itSystM itS=systs.begin();itS!=systs.end();itS++) {
      HistoUtils::addUnderflowBin( (*itS).second, _xmin);
    }
    for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++) {
      HistoUtils::addUnderflowBin( (*itS).second, _xmin);
    }
    for(itSystM itS=systsDo.begin();itS!=systsDo.end();itS++) {
      HistoUtils::addUnderflowBin( (*itS).second, _xmin);
    }
  }
  //=========================================

  //Check the normalization  =====
  // for(itSystM itS=systs.begin();itS!=systs.end();itS++)
  //   ((*itS).second)->Scale(  _hMC->Integral(0,100000)/((*itS).second)->Integral(0,100000) );
  // for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++) 
  //   ((*itS).second)->Scale( _hMC->Integral(0,100000)/((*itS).second)->Integral(0,100000)  );
  // for(itSystM itS=systsDo.begin();itS!=systsDo.end();itS++)
  //   ((*itS).second)->Scale(  _hMC->Integral(0,100000)/((*itS).second)->Integral(0,100000) );
  // =============================

  //differential normalization ==============
  // if(_normOpts.find("dif")!=_normOpts.end()) {
  //   for(itSystM itS=systs.begin();itS!=systs.end();itS++)
  //     HistoUtils::changeToDiffHisto( (*itS).second );
  //   for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++)
  //     HistoUtils::changeToDiffHisto( (*itS).second );
  //   for(itSystM itS=systsDo.begin();itS!=systsDo.end();itS++)
  //     HistoUtils::changeToDiffHisto( (*itS).second );
  // }
  // ========================================

  //how many uncertainty sources? 
  if(systs.size() + systsUp.size() > 4) _uncDet=false; //protection
  int nUncSrc = _uncDet?(systs.size() + systsUp.size() + _mcSyst):1;
 
  vector<TGraphAsymmErrors*> unc(nUncSrc,NULL);
  _uncNames.resize(nUncSrc);
 
  _uncNames[0]="uncertainties";
  for(int iu=0;iu<nUncSrc;iu++) {
    unc[iu] = new TGraphAsymmErrors(_hMC->GetNbinsX()+2);
      
    unc[0]->SetMarkerSize(0); 
    unc[0]->SetMarkerStyle(1);
    unc[0]->SetMarkerColor(1);
    unc[0]->SetFillColor(kGray+1);
    unc[0]->SetLineColor(kGray+1);
    unc[0]->SetLineStyle(3);
    unc[0]->SetFillStyle(3001);
  
  }

  bool isDif=_normOpts.find("dif")!=_normOpts.end();

  for(int ib=0;ib<_hMC->GetNbinsX()+2;ib++) { 
    
    vector<float> systU(nUncSrc,0);
    vector<float> systD(nUncSrc,0);

    //first, mcStatSyst
    size_t nu=0;
    if(_mcSyst) {

      TH1* tmp=(TH1*)_hMC->Clone();
      tmp->Reset("ICEM");
      float eyMC=0;
      float eyDD=0;
      float y=0;
      for(size_t ids=0;ids<_nhmc;ids++) {
	TH1* htmp=(TH1*) _hClonesNoStack[ids]->Clone();
	//	HistoUtils::changeToDiffHisto(htmp);
	
	y += htmp->GetBinContent(ib);
	
	if(((string)htmp->GetName()).find("_DD")==(size_t)-1)
	  eyMC += pow( htmp->GetBinError(ib),2)
	          *(isDif?pow(htmp->GetBinWidth(ib),2):1.);
	else
	  eyDD += htmp->GetBinError(ib)*(isDif?htmp->GetBinWidth(ib):1.);
	delete htmp;
      }
      tmp->SetBinContent(ib,y);
      tmp->SetBinError(ib, sqrt(eyMC) + eyDD );
      // if(ib==4)
      // 	cout<<tmp->GetBinContent(ib)<<"  "<<tmp->GetBinError(ib)<<"   "
      // 	    <<_hMC->GetBinContent(ib)<<"  "<<_hMC->GetBinError(ib)<<endl;
      float s = tmp->GetBinError(ib); 
      if(_normOpts.find("dif")==_normOpts.end()) {
	systU[0] +=s*s;
	systD[0] +=s*s;
      }
      else {
	systU[0] +=pow(s/_hMC->GetBinWidth(ib), 2);
	systD[0] +=pow(s/_hMC->GetBinWidth(ib), 2);
      }
    
      if(ib==0 && _uncDet) {
	_uncNames[0] = "MC statistics";
      }
      if(_uncDet) nu++;
      delete tmp;
    }
    
    //secondly symetric systs
    
    for(itSystM itS=systs.begin();itS!=systs.end();itS++)  {
      
      float s = fabs( ((*itS).second)->GetBinContent(ib) - _hMC->GetBinContent(ib) );
      for(size_t iv=nu;iv<(cumul?systU.size():(nu+1));iv++) {
	systU[iv] +=s*s;
	systD[iv] +=s*s;
	if(ib==0 && _uncDet) {
	  //cout<<iv<<"   "<<(*itS).first<<endl;
	  _uncNames[iv] = (*itS).first;
	}
      }
      if(_uncDet) nu++;
    } //sym
    

    //now asymetric systs
    for(itSystM itS=systsUp.begin();itS!=systsUp.end();itS++)  {
      
      string n =  (*itS).first.substr( 0, (*itS).first.size() -2 );
      float sU = ((*itS).second)->GetBinContent(ib) - _hMC->GetBinContent(ib);
      float sD = (systsDo[ n+"Do" ])->GetBinContent(ib) - _hMC->GetBinContent(ib);
      
      for(size_t iv=nu;iv<(cumul?systU.size():(nu+1));iv++) {
	
	if(ib==4 && iv==nu)
	  cout<<n<<" ini -> "<<systU[nu]<<"  "<<systD[nu]<<" ===>  "<<sU<<"   "<<sD<<endl;

	if( sU*sD > 0) { //same sign errors
	  systU[iv] +=sU>=0?(sU>sD?(sU*sU):(sD*sD)):0;
	  systD[iv] +=sU<0?(sU<sD?(sU*sU):(sD*sD)):0;
	}
	else { //opposite sign errors
	  systU[iv] +=sU>=0?(sU*sU):(sD*sD);
	  systD[iv] +=sU<0?(sU*sU):(sD*sD);
	}
	
	if(ib==0 && _uncDet) {
	  _uncNames[iv] = (*itS).first;
	}
      }

      if(_uncDet) nu++;
    } //asym
    
    for(int ih=0;ih<nUncSrc;ih++) {
      if(ib==0 && _uncDet) {
       	cout<<" Uncertainty detail : "<<ih<<"   "<< _uncNames[ih]<<endl;
	unc[ih]->SetName(_uncNames[ih].c_str() );
	unc[ih]->SetTitle(_uncNames[ih].c_str() );
	
      }
      unc[ih]->SetPoint(ib, _hMC->GetXaxis()->GetBinCenter(ib), _hMC->GetBinContent(ib) );
      
      float width=_hMC->GetXaxis()->GetBinWidth(ib);
      
      unc[ih]->SetPointEXlow(ib, width/2. );
      unc[ih]->SetPointEXhigh(ib, width/2. );
      // if(_normOpts.find("dif")!=_normOpts.end()) {
      // 	unc[ih]->SetPointEYhigh(ib, sqrt(systU[ih])/width );
      // 	unc[ih]->SetPointEYlow(ib, sqrt(systD[ih])/width );
      // }
      // else {
	unc[ih]->SetPointEYhigh(ib, sqrt(systU[ih]) );
	unc[ih]->SetPointEYlow(ib, sqrt(systD[ih]) );
	//}

    }    

  }//bin

  _mcUncert = unc;
}

void
Display::setSystematicsUnc( vector<vector<systM> > systs ) {
  _systMUnc = systs;
}


void
Display::drawDetailSystematics(bool cumul) {
  //turn on detail
  _uncDet=true;
  _csystM = _systMUnc[0];
  
  preparePadsForSystDetail();
  
  _cSyst->Draw();
  _cSyst->cd();
  _padleft->Draw();
  _padleft->cd();

  computeSystematics(_isProf, cumul);

  TGraphAsymmErrors* ratio = HistoUtils::ratioHistoToGraph( _hData, _hMC );
  
  TH1F* emptyHisto = (TH1F*)_hMC->Clone();
  emptyHisto->Reset("ICEM");
  emptyHisto->SetName("empty");
  emptyHisto->SetTitle("empty");
  
  for(int ib=0;ib<emptyHisto->GetNbinsX()+2;ib++)
    { emptyHisto->SetBinContent(ib,-1000);   }

  if(_rmLabel)  {
    emptyHisto->GetYaxis()->SetTitleOffset(1000);
    ratio->GetYaxis()->SetTitleOffset(1000);
  }
 
  if(!_overflow && !_underflow) {
    emptyHisto->GetXaxis()->SetRangeUser(_xmin, _xmax-0.0001 );
    ratio->GetXaxis()->SetRangeUser(_xmin,_xmax-0.0001 );
  }
  else {
    if(_overflow) {
      emptyHisto->GetXaxis()->SetRangeUser(_xmin,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      ratio->GetXaxis()->SetRangeUser(_xmin,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      
      if(_underflow) {
	emptyHisto->GetXaxis()->SetRangeUser(_xmin ,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
	ratio->GetXaxis()->SetRangeUser(_xmin ,_xmax - _hData->GetBinWidth(1)/1.5 -0.00001);
      }
    }
    if(_underflow) {
      emptyHisto->GetXaxis()->SetRangeUser(_xmin ,_xmax-0.0001);
      ratio->GetXaxis()->SetRangeUser(_xmin ,_xmax-0.0001);
    }
  }
  
  int style[4]={0,0,3002,3001};
  int col[4]={kViolet+2,kOrange+7,kAzure+2,kGreen+1};
  int lStyle[4]={1,2,2,2};

  bool isTeV = _xtitle.find("TeV")!=(size_t)-1;

  string Xtitle = _xtitle;

  float ym=0,yM=0;

  vector<TPolyLine*> sysBand(0,NULL);
  for(int iu=0;iu<min(4,(int)(_mcUncert.size()));iu++) { 
    int ii=_mcUncert.size()-iu-1;
    
    double x,y/*,xl,xh*/,yl,yh;
      
    vector<float> xs;
    vector<float> yls;
    vector<float> yhs;
      
    for(int ib=0;ib<_mcUncert[ ii ]->GetN();ib++) {
      _mcUncert[ ii ]->GetPoint(ib,x,y);
      //   MCUncertRatio->SetPoint(ib, x, 1 );

      yl = _mcUncert[ ii ]->GetErrorYlow(ib);
      yh = _mcUncert[ ii ]->GetErrorYhigh(ib);
  
      if(x<_xmin-emptyHisto->GetXaxis()->GetBinWidth(1)/2. ||
	 x>_xmax+emptyHisto->GetXaxis()->GetBinWidth(1)/2. ) continue;
      
      xs.push_back(x);
      if(y!=0) {
	yls.push_back(yl/y);
	yhs.push_back(yh/y);

	if(ym> (yl/y ) ) ym = yl/y;
	if(yM< (yh/y ) ) yM = yh/y;

      }
      else {
	yls.push_back(0);
	yhs.push_back(0);
      }
    }
 
    sysBand.push_back( HistoUtils::GetSystBand(xs,yls,yhs,_xmin/(isTeV?1000:1),_xmax/(isTeV?1000:1)) );
    int sSy = _mcUncert.size();
    sysBand.back()->SetLineColor(col[iu+4-sSy]);
    sysBand.back()->SetLineStyle(lStyle[iu+4-sSy]);
    sysBand.back()->SetLineWidth(2);
    sysBand.back()->SetFillColor(col[iu+4-sSy]);
    sysBand.back()->SetFillStyle(style[ iu+4-sSy ]);
  }
  // cout<<ym<<"   "<<yM<<endl;
  if(1-ym > yM-1 ) yM = 1-ym +1;  
  else ym = 1-yM;  

  //cout<<ym<<"   "<<yM<<endl;

  emptyHisto->GetYaxis()->SetRangeUser( 0.3, 1.7);// 1-rangemax, 1+rangemax );
  emptyHisto->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  emptyHisto->GetYaxis()->SetNdivisions(3,_Ydiv[1],_Ydiv[2]);
  emptyHisto->GetXaxis()->SetTitle( Xtitle.c_str() );
  emptyHisto->GetYaxis()->SetTitle( "Data/MC" );
  emptyHisto->GetXaxis()->SetTitleSize(0.11);
  emptyHisto->GetXaxis()->SetTitleOffset(0.70);
  emptyHisto->GetXaxis()->SetLabelSize(0.09);
  emptyHisto->GetYaxis()->SetLabelSize(0.09);
  emptyHisto->GetYaxis()->SetLabelOffset(0.011);
  emptyHisto->GetYaxis()->SetTitleSize(0.11);
  emptyHisto->GetYaxis()->SetTitleOffset(0.40);
  emptyHisto->GetXaxis()->SetTickLength(0.09);
  emptyHisto->GetYaxis()->SetTickLength(0.05);
  emptyHisto->GetXaxis()->SetLabelOffset(0.011);
  
  ratio->GetYaxis()->SetRangeUser(  0.3, 1.7);//1-rangemax, 1+rangemax );
  ratio->GetXaxis()->SetNdivisions(_Xdiv[0],_Xdiv[1],_Xdiv[2]);
  ratio->GetYaxis()->SetNdivisions(3,_Ydiv[1],_Ydiv[2]);
  ratio->GetXaxis()->SetTitle( Xtitle.c_str() );
  ratio->GetYaxis()->SetTitle( "Data/MC" );
  ratio->GetXaxis()->SetTitleSize(0.12);
  ratio->GetXaxis()->SetTitleOffset(0.72);
  ratio->GetXaxis()->SetLabelSize(0.09);
  ratio->GetYaxis()->SetLabelSize(0.09);
  ratio->GetYaxis()->SetLabelOffset(0.011);
  ratio->GetYaxis()->SetTitleSize(0.11);
  ratio->GetYaxis()->SetTitleOffset(0.54);
  ratio->GetXaxis()->SetTickLength(0.09);
  ratio->GetYaxis()->SetTickLength(0.05);

  emptyHisto->Draw();

  for(int iu=(int)sysBand.size()-1;iu>=0;iu--) {
    int sSy = _mcUncert.size();
    sysBand[iu]->Draw(iu+4-sSy<2?"l":"F");
  }


  // TLine* line=new TLine(isTeV?_xmin:(_xmin/1000),1,isTeV?_xmax:(_xmax/1000),1);
  TLine* line=new TLine(_xmin,1,_xmax,1);
  line->SetLineColor(kRed+1);
  line->SetLineStyle(7);
  line->SetLineWidth(2);

  line->Draw("same");

  ratio->Draw("p same");
  //turn off detail
  _uncDet=false;

  _cSyst->cd();
  _padright->Draw();
  _padright->cd();

  TLegend* legSyst=new TLegend(0.11,0.11,0.89,0.89);
  legSyst->SetFillColor(0);
  legSyst->SetShadowColor(0);
  
  legSyst->AddEntry(ratio,"data/MC","pl");
  
  //cout<<sysBand.size()<<endl;
  int iui=0;
  for(int iu=0;iu<(int)sysBand.size();iu++) {
    if(cumul) iui=sysBand.size()-iu-1;
    else iui=iu;

    string name=_uncNames[sysBand.size()-iui-1];
    size_t p1=name.find("Unc");
    size_t p2=name.find("Do");
    size_t p3=name.find("Up");
    //cout<<name<<" -> ";
    if(p2==(size_t)-1 && p3==(size_t)-1)
      name=name.substr(p1+3,name.size()-1);
    else if(p2!=(size_t)-1) 
      name=name.substr(p1+3,p2-p1-3);
    else
      name=name.substr(p1+3,p3-p1-3);
    //cout<<name<<endl;
    legSyst->AddEntry(sysBand[iui],name.c_str(),(iui<2)?"l":"f");
  }    
  legSyst->Draw();

}


void
Display::getIntegral(float x1, float x2, float y1, float y2) {
  printInteg(x1,x2,y1,y2);
}


void
Display::printInteg(float x1, float x2, float y1, float y2) {

  vector<double> errors(_nhmc+2,0);
  
  if(!_is2D) {
    if(!_mcOnly && !_dOnly && _normOpts.find("norm")==_normOpts.end()) {
      cout<<" Integral : data -> "
	  << _hData->IntegralAndError(_hData->GetXaxis()->FindBin(x1), _hData->GetXaxis()->FindBin(x2), (errors.back()) );
      cout<<" +- "<<errors.back()
	  <<" // MC-> "
	  <<_hMC->IntegralAndError(_hMC->GetXaxis()->FindBin(x1), _hMC->GetXaxis()->FindBin(x2), (errors[_nhmc+1]) );
      cout<<" +- "<<errors[_nhmc+1]<<endl;
    }
    else if(_mcOnly && _hMC) {
      cout<<" Integral : MC -> "<<_hMC->IntegralAndError(_hMC->GetXaxis()->FindBin(x1), _hMC->GetXaxis()->FindBin(x2), errors[_nhmc+1]);
      cout<<" +- "<<errors[_nhmc+1]<<endl;
    }
    else if(_dOnly) { 
      cout<<" Integral : data -> "<<_hData->IntegralAndError(_hData->GetXaxis()->FindBin(x1), _hData->GetXaxis()->FindBin(x2), errors.back());
      cout<<" +- "<<errors.back()<<endl;
    }
 
    if(!_dOnly) {
      float tmp=0;
      double tmp2=0;
      for(size_t ih=0;ih<_nhmc;ih++) {
	if(_names[ih].find("sig")==(size_t)-1) {
	  cout<<"\t"<<_names[ih]<<" : "
	      <<_hClones[_nhmc-ih-1]->IntegralAndError(_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x1),_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x2), errors[ih])-tmp;
	  cout<<" +- "<<sqrt(errors[ih]*errors[ih]-tmp2*tmp2)<<endl;
	  tmp = _hClones[_nhmc-ih-1]->IntegralAndError(_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x1),_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x2), tmp2);
	}
	else {
	  cout<<"\t"<<_names[ih]<<" : "
	      <<_hClones[_nhmc-ih-1]->IntegralAndError(_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x1),_hClones[_nhmc-ih-1]->GetXaxis()->FindBin(x2), errors[ih]);
	  cout<<" +- "<<errors[ih]<<endl;
	}
      }
    }
  }

}

//old CMS preliminary

// void Display::cmsPrel() {
//   TLatex latex;
//   latex.SetNDC();
//   latex.SetTextSize(_showRatio?0.05:0.04);
//   latex.SetName("lumiText");

//   if (_lumi > 0.) {
//     latex.SetTextAlign(11); // align left, right=11
    
//     // if(_lumi > 1000 )
//     //   latex.DrawLatex(0.48,0.87,Form("e#tau, %.1f fb^{-1}  at  #sqrt{s} = 8 TeV",_lumi/1000.));
//     // else
//     //   latex.DrawLatex(0.48,0.87,Form("e#tau, %.0f pb^{-1}  at  #sqrt{s} = 8 TeV",_lumi));

//     if(_lumi > 1000 )
//       latex.DrawLatex(0.54,0.87,Form(" %.1f fb^{-1}  at  #sqrt{s} = 8 TeV",_lumi/1000.));
//     else
//       latex.DrawLatex(0.54,0.87,Form(" %.0f pb^{-1}  at  #sqrt{s} = 8 TeV",_lumi));
//   }
//   latex.SetTextAlign(11); // align left
  
//   if(!_mcOnly) {
//     if(_prel)
//       latex.DrawLatex(0.54,0.96,"CMS preliminary 2012");
//     else
//       latex.DrawLatex(0.75,0.96,"CMS 2012");
//   }
//   else
//     latex.DrawLatex(0.555,0.96,"CMS simulation 2012");

// }

//new CMS preliminary (thanks gautier..)

void Display::cmsPrel() {
  TLatex latex;
  

  //_showRatio?0.05:0.04;
  float t = _pads[0][0]->GetTopMargin();
  float tmpTextSize=0.75*t;
  latex.SetTextSize(tmpTextSize);
  latex.SetNDC();
  float textSize=latex.GetTextSize();


  if(_showRatio) {
    latex.SetName("lumiText");
    latex.SetTextFont(42);

    if (_lumi > 0.) {
      latex.SetTextAlign(31); // align left, right=31
      latex.SetTextSize(textSize*0.6/0.75);
      if(_lumi > 1000 )
	latex.DrawLatex(0.965,0.96,Form(" %.1f fb^{-1} (8 TeV)",_lumi/1000.));
      else
	latex.DrawLatex(0.965,0.96,Form(" %.0f pb^{-1} (8 TeV)",_lumi));
    }
 
    latex.SetTextAlign(11); // align left
    latex.SetTextFont(61);
    latex.SetTextSize(textSize);
    latex.DrawLatex(0.853,0.875,"CMS");
    
    latex.SetTextFont(52);
    latex.SetTextSize(textSize*0.76);
    if(!_mcOnly) {
      if(_prel)
	latex.DrawLatex(0.80,0.84,"Preliminary");
    }
    else
      latex.DrawLatex(0.81,0.84,"Simulation");

  }
  else {


   latex.SetName("lumiText");
    latex.SetTextFont(42);

    if (_lumi > 0.) {
      latex.SetTextAlign(31); // align left, right=31
      latex.SetTextSize(textSize*0.6/0.75);
      if(_lumi > 1000 )
	latex.DrawLatex(0.922,0.96,Form(" %.1f fb^{-1} (8 TeV)",_lumi/1000.));
      else
	latex.DrawLatex(0.922,0.96,Form(" %.0f pb^{-1} (8 TeV)",_lumi));
    }
 
    latex.SetTextAlign(11); // align left
    latex.SetTextFont(61);
    latex.SetTextSize(textSize);
    latex.DrawLatex(0.807,0.875,"CMS");
    
    latex.SetTextFont(52);
    latex.SetTextSize(textSize*0.76);
    if(!_mcOnly) {
      if(_prel)
	latex.DrawLatex(0.759,0.84,"Preliminary");
    }
    else
      latex.DrawLatex(0.749,0.84,"Simulation");

  }

}



void
Display::addText(float x, float y, float s, string text) {
  _pads[0][0]->cd();

  TLatex t;
  t.SetNDC();
  t.SetTextSize(s);
  t.SetName("addtext");
  t.DrawLatex(x,y,text.c_str() );

}

void
Display::adjustLegend(int iobs, bool skipCoords) {
  float xd,xu,yd,yu,f=1.;
  
  if(!skipCoords) {

    if(_normOpts.find("norm")!=_normOpts.end() ) { // normalized to one, all plots needed
      for(size_t ih=0;ih<_nhmc;ih++) {
	_hCoords.push_back( HistoUtils::getHistoUpperCoordinates(_hClones[ih]) );
      }
    }
    else {
      if(!_dOnly)
	_hCoords.push_back( HistoUtils::getHistoUpperCoordinates(_hMC) );
      //and the signals if not summed...
      for(size_t ih=0;ih<_nhmc;ih++) {
	string nh = (string)( _hClones[ih]->GetName());
	if( !_sSignal && nh.find("sig")!=(size_t)-1)
	  _hCoords.push_back( HistoUtils::getHistoUpperCoordinates(_hClones[ih]) );
      }
    }  
    
    if(_addSyst) {
      _hCoords.push_back( HistoUtils::getGraphUpperCoordinatesWithError(_mcUncert[0]) );
    }


    if(!_mcOnly) {
      float dNorm = 1.;
      if(_normOpts.find("norm")!=_normOpts.end()) 
	dNorm=_hData->Integral(0,10000000);
      if(_normOpts.find("uni")!=_normOpts.end()) 
	dNorm=1.;

      if(_normOpts.find("dif")!=_normOpts.end()) //temporary fix
	_hCoords.push_back( HistoUtils::getHistoUpperCoordinatesWithError(_hData, dNorm) );
    }

  }

  getLegendCoordinate(_hMC,xd,yd,xu,yu,f,iobs);
  //cout<<" leg "<<xd<<"  "<<yd<<"  "<<xu<<"  "<<yu<<"  "<<f<<endl;
  _leg = new TLegend(xd,yd,xu,yu);
  _leg->SetName("legend");
  
  _leg->SetTextSize(0.039*f);
  //_leg->SetTextSize(0.04 );
  _leg->SetShadowColor(0);
  _leg->SetLineColor(1);
  _leg->SetFillColor(0);

  map<string,size_t> sigs;

  string legOpt="pl";
  if(_normOpts.find("norm")!=_normOpts.end()) legOpt="l";

  if(!_mcOnly)
    _leg->AddEntry(_gData,"data", legOpt.c_str() );
  if( _is1D) {//!(_isProf && _is1D) ) {
    for(size_t i=0;i<_nhmc;i++) {
      
      string nh = (string)( _hClones[i]->GetName());
      if( nh.find("sig")==(size_t)-1) {
	_leg->AddEntry(_hClones[i],_names[_nhmc-i-1].c_str(),"f");
      }
      else {
	string na = _names[_nhmc-i-1];
	size_t b= _names[_nhmc-i-1].find("sig");
	na.erase(b,3);
	sigs[ na ] = i ;
      }
    }
  }
  
  if(_addSyst && !_is2D) {
    _leg->AddEntry(_mcUncert[0]->Clone(),"uncertainties","f");
  }
  
  //and signals at the end
  for(map<string,size_t>::const_iterator it=sigs.begin();
      it!=sigs.end();it++) {
    _leg->AddEntry(_hClones[it->second],it->first.c_str(),_sSignal?"f":"l");
  }

}

void
Display::getLegendCoordinate(TH1*h, float& pxd, float& pyd, float& pxu, float& pyu,float& fo, int iobs) {

  //default values
  pxd = 0.5;
  pyd = 0.5;
  pxu = 0.7;
  pyu = 0.8;
  fo = 0.9;
  
  int nObj = min(_names.size()+(int)(!_mcOnly)+(int)_addSyst, (size_t)17);
  float Dy = 0.051;  
  float dy = nObj*Dy;
  
  float dx=0.291;
  
  vector<float> out(4,0);

  float xu,yu,xd,yd;

  float tmpf=0;
  float di=0.01;
  float x_off=1.;
  float y_off=1.;
  int nt=100;
  
  float Xtu=_pads[0][iobs]->GetUxmax();
  float Xtd=_pads[0][iobs]->GetUxmin();
  float Ytu=_pads[0][iobs]->GetUymax();
  float Ytd=_pads[0][iobs]->GetUymin();
  
  float Xof=0.01;
  float Yof=0.035;
  
  _xdlim=(_pads[0][iobs]->XtoPixel( Xtd ))/(float)_wpad + Xof;
  _ydlim=1-(_pads[0][iobs]->YtoPixel( Ytd ))/(float)_hpad + Yof;
  _xulim=(_pads[0][iobs]->XtoPixel( Xtu ))/(float)_wpad - Xof;
  _yulim=1-(_pads[0][iobs]->YtoPixel( Ytu ))/(float)_hpad - Yof;
  
  // cout<<
  // cout<<Xtd<<"   "<<Ytd<<"  "<<Xtu<<"   "<<Ytu<<endl;
  // cout<<_xdlim<<"  "<<_ydlim<<"  "<< _xulim<<"   "<<_yulim<<endl;

  //LumiPad =======================================================================
  float xlp,ylp;
  float xLm=_xulim,yLm=_yulim;
  TList *ListOfObject = _pads[0][iobs]->GetListOfPrimitives();
  if (ListOfObject) {
    TIter next(ListOfObject);
    TObject *ThisObject;
    
    while ( (ThisObject=(TObject*)next()) ) {
     
      TString ClassName  = ThisObject->ClassName();
      
      if(ClassName=="TLatex") {
	xlp = ((TLatex*)ThisObject)->GetX();
	ylp = ((TLatex*)ThisObject)->GetY()-0.01;
	if(xlp>_xdlim && xlp<_xulim && ylp>_ydlim && ylp<_yulim) {
	  xLm=xlp-0.01;
	  yLm=ylp-0.01;
	}
      }
    }
  }
  //LumiPad =======================================================================

  int n=0;
  while( n<nt*nt) {
   
    if(_is1D) {
      
      xu = x_off-(n%nt)*di;
      yu = y_off-(n/nt)*di;
      xd = xu-dx;
      yd = yu-dy;
      
      n++;
      //cout<<xd<<"  "<<yd<<"   "<<xu<<"  "<<yu<<endl;
      if(xd<_xdlim || xu>_xulim || yd<_ydlim || yu>_yulim) continue;
      if(yu>yLm && xu>xLm ) continue;
      float f=1.;
      bool abortPos=false;
      
      for(size_t id=0;id<_hCoords.size();id++) {
	if(yu < graphVal(xu,id,iobs) || yu < graphVal(xd,id,iobs) ) {
	  abortPos=true; break;}
	graphConstraint(id,iobs, xd, xu, yd, yu,f, dx,dy );
      }
      if(abortPos) continue;
      //cout<<" !!!->  "<<xd<<"  "<<xu<<"   "<<yd<<"   "<<yu<<"   "<<f<<endl;
      if(f==1) { //found a definitive position
	pxu = xu;
	pxd = xd;
	pyu = yu;
	pyd = yd;
	fo =f;
	return;
      }
      if(f>tmpf) {
	//cout<<"   "<<xd<<"  "<<xu<<"   "<<yd<<"   "<<yu<<"   "<<f<<endl;
	pxu = xu;
	pxd = xd;
	pyu = yu;
	pyd = yd;
	fo=f;
	tmpf=f;
      }

    }//if is1D
    
  }//position tries
  //cout<<" --->  "<<xd<<"  "<<xu<<"   "<<yd<<"   "<<yu<<"   "<<fo<<endl;
  return;
  
}


float
Display::graphVal(float x,int ih, int iobs) {
  
  float X = _pads[0][iobs]->PixeltoX( x*_wpad  );

  size_t bin = StatUtils::findBin<float>(X, _hCoords[ih][0] );
  //cout<<x<<"   "<<X<<"   "<<bin<<"   "<<_xdlim<<"  "<<endl;
  float yt=_logYScale?(log(_hCoords[ih][1][bin])/log(10)):_hCoords[ih][1][bin];
  
  float y = 1 - (_pads[0][iobs]->YtoPixel( yt ))/(float)_hpad;
 
  return y;
}

void
Display::graphConstraint(size_t ih, int iobs, float& xd, float& xu, float& yd, float& yu,float& f, float dx, float dy) {

  bool lCorIP= (yd < graphVal(xd,ih,iobs));
  bool rCorIP= (yd < graphVal(xu,ih,iobs));

  while( lCorIP || rCorIP ) { //while one of the legend corner is in the plot
	
    f -= 0.05;
    if(f<=0) break;
    
    if(lCorIP && rCorIP) { //both corners in the plot
      if(xd>0.5) { //move the left corner up if legend on the right
	xd = xu-dx*f;
	yd = yu-dy*f;
      }
      else { //move the right corner up if legend on the left
	xu = xd+dx*f;
	yd = yu-dy*f;
      }
    }
    else if(lCorIP) { //right corner in the plot
      //move the right corner up
      xu = xd+dx*f;
      yd = yu-dy*f;
    }
    else if(rCorIP) { //left corner in the plot
      //move the left corner up 
      xd = xu-dx*f;
      yd = yu-dy*f;
    }
    
    lCorIP= (yd < graphVal(xu,ih,iobs));
    rCorIP= (yd < graphVal(xd,ih,iobs));

  }

  //now check the lower band
  
  bool lBandIP=false;
 
  float dX=xu-xd;
  for(int i=0;i<100;i++) {
    if(yd < graphVal(xd + i*dX/100. ,ih,iobs) ) {
      lBandIP=true;
      break;
    }
  }

  while( lBandIP) {

    f -= 0.05;
    if(f<=0) break;

    if(xd>0.5) { //move the left corner up if legend on the right
      xd = xu-dx*f;
      yd = yu-dy*f;
    }
    else { //move the right corner up if legend on the left
      xu = xd+dx*f;
      yd = yu-dy*f;
    }
    lBandIP =false;
    dX=xu-xd;
    for(int i=0;i<100;i++) {
      if(yd < graphVal(xd + i*dX/100. ,ih,iobs) ) {
	lBandIP=true;
	break;
      }
    }
  }
  
}

