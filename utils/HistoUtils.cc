#include "utils/HistoUtils.hh"
#include "utils/StatUtils.hh"

using namespace std;

ClassImp(HistoUtils)

TH1F* 
HistoUtils::convertGraphToHisto(TGraph* graph, int NbinX, double mX, double MX,
				string name) {
  
  TH1F* tmp=new TH1F(name.c_str(),name.c_str(),NbinX,mX,MX);
  double x,y;
  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
 
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}

TH1F* 
HistoUtils::convertGraphToHisto(TGraph* graph, int NbinX, double bins[],
				string name) {
  
  TH1F* tmp=new TH1F(name.c_str(),name.c_str(),NbinX,bins);
  double x,y;
  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
 
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}


TH2F* 
HistoUtils::convertGraphToHisto(TGraph* graph, int NbinX, double mX, double MX,
				int NbinY, double mY, double MY,string name) {
  
  TH2F* tmp=new TH2F(name.c_str(),name.c_str(),NbinX,mX,MX,NbinY,mY,MY);
  double x,y;
  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
 
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}


TH2F* 
HistoUtils::convertGraphToHisto(TGraph* graph, int NbinX, double BinX[],
				int NbinY, double BinY[],string name) {

  TH2F* tmp=new TH2F(name.c_str(),name.c_str(),NbinX,BinX,NbinY,BinY);
  double x,y;
  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}

TProfile* 
HistoUtils::convertGraphToProfile(TGraph* graph, int NbinX, double mX, 
				  double MX,string name,string opt) {

  TProfile* tmp=new TProfile(name.c_str(),name.c_str(),NbinX,mX,MX);
  tmp->SetErrorOption(opt.c_str());
  double x,y;

  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}

TProfile* 
HistoUtils::convertGraphToProfile(TGraph* graph, int NbinX, double BinX[]
				  ,string name,string opt) {

  TProfile* tmp=new TProfile(name.c_str(),name.c_str(),NbinX,BinX);
  tmp->SetErrorOption(opt.c_str());
  double x,y;

  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    if(x!=0 && y!=0) {
      tmp->Fill(x,y);
    }
  }
  return tmp;
}


vector<TGraph*> 
HistoUtils::convertGraphToErrorProfile(TGraph* graph, int NbinX, 
				       double mX, double MX ) {

  double pas = (MX-mX)/NbinX;
  TGraph* tmp=new TGraph(NbinX);
  TGraph* tmpEm=new TGraph(NbinX);
  TGraph* tmpEM=new TGraph(NbinX);

  double x,y;
  vector<TH1F*> tmp1h;
  for(int i=0;i<NbinX;i++) {
    TH1F* tmpf = new TH1F((TString)i, "", 2000, -100,100);
    tmp1h.push_back(tmpf);
  }

  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    for(int k=0;k<NbinX;k++) {
      if(x!=0 && y!=0) {
	
	if(x>(k*pas) && x<( (k+1)*pas) )
	  tmp1h[k]->Fill(y);
      }
    }
  }
  
  double Em,EM,M;
  for(int i=0;i<NbinX;i++) {
    StatUtils::ComputeStatFromHisto(tmp1h[i], Em , EM, M);
    tmp->SetPoint(i,i*pas+pas/2.,M);
    tmpEm->SetPoint(i,i*pas+pas/2.,Em);
    tmpEM->SetPoint(i,i*pas+pas/2.,EM);
    cout<<i<<" Mean "<<M<<" ; Emin "<<Em<<" ; Emax "<<EM<<endl;
    delete tmp1h[i];
 
  }
  
  vector<TGraph*> Graphs;
  Graphs.push_back(tmp);
  Graphs.push_back(tmpEm);
  Graphs.push_back(tmpEM);
 
  return Graphs;
}


vector<TGraph*> 
HistoUtils::convertGraphToErrorProfile(TGraph* graph, int NbinX, int BinVar[]) {

  TGraph* tmp=new TGraph(NbinX);
  TGraph* tmpEm=new TGraph(NbinX);
  TGraph* tmpEM=new TGraph(NbinX);
  double x,y;
  vector<TH1F*> tmp1h;
  for(int i=0;i<NbinX;i++) {
    TH1F* tmpf = new TH1F((TString)i, "", 2000, -100,100);
    tmp1h.push_back(tmpf);
  }

  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    for(int k=0;k<NbinX;k++) {
      if(x!=0 && y!=0) {
	
	if(x>=BinVar[k] && x<BinVar[k+1] )
	  tmp1h[k]->Fill(y);
      }
    }
  }
  
  double Em,EM,M;
  for(int i=0;i<NbinX;i++) {

    double pas = (BinVar[i+1]-BinVar[i]);
    StatUtils::ComputeStatFromHisto(tmp1h[i], Em , EM, M);
    if(i==0) {
      tmp->SetPoint(i,BinVar[i],M);
      tmpEm->SetPoint(i,BinVar[i],Em);
      tmpEM->SetPoint(i,BinVar[i],EM);
    }
    else {
      tmp->SetPoint(i,BinVar[i]+pas/2.,M);
      tmpEm->SetPoint(i,BinVar[i]+pas/2.,Em);
      tmpEM->SetPoint(i,BinVar[i]+pas/2.,EM);
    }
    cout<<i<<" Mean "<<M<<" ; Emin "<<Em<<" ; Emax "<<EM<<endl;
    delete tmp1h[i];
    // cout<<tmp->GetBinContent(i)<<endl;
  }
  
  
  vector<TGraph*> Graphs;
  Graphs.push_back(tmp);
  Graphs.push_back(tmpEm);
  Graphs.push_back(tmpEM);
 
  return Graphs;
}


TGraphAsymmErrors*
HistoUtils::graphReduction(TGraph* graph, int NbinX, double mX, double MX,
			   string name,string opt) {

  double pas = (MX-mX)/NbinX;

  TGraphAsymmErrors* tmp=new TGraphAsymmErrors(NbinX);

  double x,y;

  vector<TH1F*> tmp1h;
  vector<TH1F*> tmp2h;
  for(int i=0;i<NbinX;i++) {
    TString s= "_"+name;
    s +=i;
    TH1F* tmpf = new TH1F(s, "", 2000, -100,100);
    tmp1h.push_back(tmpf);
    TH1F* tmpf2 = new TH1F(s+"_", "", 2000, -100,100);
    tmp2h.push_back(tmpf2);
  }
 
  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    for(int k=0;k<NbinX;k++) {
      if(x!=0 && y!=0) {
	
	if(x>=(k*pas+mX) && x<( (k+1)*pas+mX) ) {
	  tmp1h[k]->Fill(y);
	  tmp2h[k]->Fill(x);
	}
      }
    }
  }
     
  double Em,EM,M;
  double Em2,EM2,M2;
  for(int i=0;i<NbinX;i++) {
    StatUtils::ComputeStatFromHisto(tmp1h[i], Em , EM, M);
    StatUtils::ComputeStatFromHisto(tmp2h[i], Em2 , EM2, M2);

    if(opt!="rms") {
      tmp->SetPoint(i,M2,M);
      if(opt=="")
	tmp->SetPointError(i,fabs(Em2-M2),fabs(EM2-M2),fabs(Em-M)/tmp1h[i]->GetEntries(),fabs(EM-M)/tmp1h[i]->GetEntries() );
      else
	tmp->SetPointError(i,fabs(Em2-M2),fabs(EM2-M2),fabs(Em-M),fabs(EM-M) );
    }
    else {
      tmp->SetPoint(i,M2,fabs(EM-M) );
    }
    
    delete tmp1h[i];
    delete tmp2h[i];
  }
  
  return tmp;
  
}



TGraphAsymmErrors* 
HistoUtils::graphReduction(TGraph* graph, int NbinX, int BinVar[],
			   string name,string opt) {

  TGraphAsymmErrors* tmp=new TGraphAsymmErrors(NbinX);
 
  double x,y;

  vector<TH1F*> tmp1h;
  vector<TH1F*> tmp2h;
  for(int i=0;i<NbinX;i++) {
    TString s= "_"+name;
    s +=i;
    TH1F* tmpf = new TH1F(s, "", 2000, -100,100);
    tmp1h.push_back(tmpf);
    TH1F* tmpf2 = new TH1F(s+"_", "", 2000, -100,100);
    tmp2h.push_back(tmpf2);
  }

  for(int i=0;i<graph->GetN();i++) {
    
    graph->GetPoint(i,x,y);
    
    for(int k=0;k<NbinX;k++) {
      if(x!=0 && y!=0) {
	
	if(x>=BinVar[k] && x<BinVar[k+1] ) {
	  tmp1h[k]->Fill(y);
	  tmp2h[k]->Fill(x);
	}
      }
    }
  }
  
  double Em,EM,M;
  double Em2,EM2,M2;
  for(int i=0;i<NbinX;i++) {

    StatUtils::ComputeStatFromHisto(tmp1h[i], Em , EM, M);
    StatUtils::ComputeStatFromHisto(tmp2h[i], Em2 , EM2, M2);

    if(opt=="")
      {
	Em /= tmp1h[i]->GetEntries();
	EM /= tmp1h[i]->GetEntries(); 
      }

    if(i==0) {
      tmp->SetPoint(i,M2,M);
      tmp->SetPointError(i,Em2,EM2,Em,EM);
    }
    else {
      tmp->SetPoint(i,M2,M);
      tmp->SetPointError(i,Em2,EM2,Em,EM);
    }
   
    delete tmp1h[i];
    delete tmp2h[i];  
  }  
  return tmp;
}

vector<TH1F*>
HistoUtils::histoReduction(TH1F* source,TH1F* MC) {

  float pas = source->GetBinWidth(1);
  
  vector<vector<float> > coord;
  vector<float> tmpc(2,0);
  vector<float > errors;

  vector<vector<float> > coord2;
  vector<float > errors2;

  for(int i=1;i<source->GetNbinsX()+1;i++) {

    if(source->GetBinContent(i)!=0) {
      //Data
      tmpc[0] = source->GetBinCenter(i);
      tmpc[1] = source->GetBinContent(i);
      coord.push_back(tmpc);
      errors.push_back( source->GetBinError(i) );

      //MC
      tmpc[0] = MC->GetBinCenter(i);
      tmpc[1] = MC->GetBinContent(i);
      coord2.push_back(tmpc);
      errors2.push_back( MC->GetBinError(i) );
    }
  }

  int NBin = (int)((coord[ coord.size()-1 ][0]-coord[0][0])/pas +1);
  
  TH1F* dataClean = new TH1F("d2","",NBin,coord[0][0],coord[ coord.size()-1 ][0]);
  TH1F* MCClean = new TH1F("d2","",NBin,coord[0][0],coord[ coord.size()-1 ][0]);

  for(int unsigned i=0;i<coord.size();i++) {
    dataClean->SetBinContent( (int)(coord[i][0]/pas)+1 , coord[i][1] );
    MCClean->SetBinContent( (int)(coord2[i][0]/pas)+1 , coord2[i][1] );
    
  }
 for(int unsigned i=0;i<coord.size();i++) {
   dataClean->SetBinError( (int)(coord[i][0]/pas)+1 , errors[i] );
   MCClean->SetBinError( (int)(coord2[i][0]/pas)+1 , errors2[i] );
 }

  vector<TH1F*> vtmp;
  vtmp.push_back(dataClean);
  vtmp.push_back(MCClean);
  return vtmp;
}


vector<TH2F*> HistoUtils::stack2DHistos(vector<TH2F*> Histos2d, vector<string> datanames, map<string,float> weights) {

  vector<TH2F*> StackedHistos;
  
  //Initialisation
  for(int unsigned i=0;i<datanames.size();i++) {
    TH2F* tmp = (TH2F*)Histos2d[i]->Clone();
    tmp->Scale(weights[ datanames[i] ]);
    StackedHistos.push_back(tmp);
  }

  //Stacking
  for(int unsigned i=0;i<datanames.size();i++) {
    for(int unsigned j=1;j<datanames.size();j++) {
      if(j>i) {
	StackedHistos[i]->Add( Histos2d[j], weights[ datanames[j] ]);
      }
    }
  }

  return StackedHistos;
}

void 
HistoUtils::rebin1DHisto(TH1*& h, int binning) {
  
  TH1* tmp=(TH1*)h->Clone();
  tmp->Rebin(binning);
  tmp->Scale(1./binning);

  for(int ib=0;ib<h->GetNbinsX()+2;ib++) {
    float x = h->GetXaxis()->GetBinCenter(ib);
    float val=tmp->GetBinContent( h->GetXaxis()->FindBin(x ));
    float err = tmp->GetBinError( h->GetXaxis()->FindBin(x ));
    h->SetBinContent(ib, val );
    h->SetBinError(ib, err );
  }//bins

  delete tmp;
}


void 
HistoUtils::addOverflowBin(TH1*& h, float Max) {

  TH1* tmp = (TH1*)h->Clone();
  tmp->Reset("ICEM");

  float intMax= h->GetXaxis()->GetXmax();
  int ovbin = h->FindBin(min(Max, intMax ) );
  double err=0;
  
  tmp->SetBinContent(ovbin-1, h->IntegralAndError(ovbin, 10000000., err) );
  tmp->SetBinError(ovbin-1, err );

  h->Add(tmp);

  //remove the ov bin for integral purposes
  for(int ib=ovbin;ib<h->GetNbinsX()+2;ib++) {
    h->SetBinContent(ib,0);
    h->SetBinError(ib,0);
  }

  delete tmp;
}

void 
HistoUtils::addUnderflowBin(TH1*& h, float Min) {

  TH1* tmp = (TH1*)h->Clone();
  tmp->Reset("ICEM");

  float intMin= h->GetXaxis()->GetXmin();
  int udbin = h->FindBin(max(Min, intMin ) );
  double err=0;
  //cout<<Min<<"  "<<intMin<<"  "<<udbin<<endl;
  tmp->SetBinContent(udbin, h->IntegralAndError(0, udbin-1, err) );
  tmp->SetBinError(udbin, err );
  h->Add(tmp);
  
  //remove the ud bin for integral purposes
  for(int ib=0;ib<udbin;ib++) {
    h->SetBinContent(ib,0);
    h->SetBinError(ib,0);
  }

  delete tmp;
}

void
HistoUtils::changeToDiffHisto(TH1*& h) {

  for(int i=0;i<h->GetNbinsX()+2;i++) {
    float sc= h->GetBinWidth(i);
    h->SetBinContent(i, h->GetBinContent(i)/sc );
    h->SetBinError(i, h->GetBinError(i)/sc );
  }

}



TGraphAsymmErrors*
HistoUtils::convertHistoToGraph(TH1* h, float datNorm, bool diff) {

  int Nn0=0;
  vector<double> vY;
  vector<double> vX;
  vector<double > veY;
  vector<double > veX;
  vector<double> tmp(0,2);
  
  for(int ip=0;ip<h->GetNbinsX()+2;ip++) {
    double Y,X,eYl,eYh;
      
    X = h->GetBinCenter(ip);
    Y = h->GetBinContent(ip);

    float w=diff?(1./h->GetBinWidth(ip)):1;
    float we=w;

    if(h->GetBinErrorLow(ip)==h->GetBinErrorUp(ip) ) { //gaussian case
      eYl = StatUtils::ErrorPL(Y);
      eYh = StatUtils::ErrorPH(Y);
      
      //check for potential internal histogram weights, dummy version ==============
      if(fabs(sqrt(h->GetBinContent(ip))-h->GetBinError(ip))/h->GetBinError(ip)>0.05) {
	if(ip==1)
	  cout<<" Treatment of weighted errors activated ====================== "<<endl;
	float cor = h->GetBinError(ip)*h->GetBinError(ip)/h->GetBinContent(ip);
	we *= sqrt(cor); //inverted farther
      }
      //============================================================

    } 
    else { // poisson case
      eYl = h->GetBinErrorLow(ip);
      eYh = h->GetBinErrorUp(ip);
    }
   
    //cout<<ip<<" ===> "<<Y<<" // "<<eYl<<" // "<<eYh<<" !! "<<datNorm<<" / "<<w<<endl;
    if(Y!=0) {
      Nn0++;
      vX.push_back(X);
      veX.push_back( h->GetBinWidth(ip)/2. );
      veX.push_back( h->GetBinWidth(ip)/2. );
      // if(diff) {
      // 	vY.push_back(Y*datNorm/w);
      // 	veY.push_back( eYl*datNorm/w );
      // 	veY.push_back( eYh*datNorm/w );
      // }
      // else {
      vY.push_back(Y*datNorm*w);
      veY.push_back( eYl*datNorm*we );
      veY.push_back( eYh*datNorm*we );
	//}
    }
  }

  TGraphAsymmErrors* graph=new TGraphAsymmErrors(Nn0);
  for(int ip=0;ip<Nn0;ip++) {
    graph->SetPoint(ip,vX[ip],vY[ip]);
    graph->SetPointError(ip,0,0/*veX[ip*2],veX[ip*2+1]*/,veY[ip*2],veY[ip*2+1]);
  }

 

  return graph;
}

TGraphAsymmErrors*
HistoUtils::ratioHistoToGraph(TH1* hd, TH1* hmc, string opt) {
  
  int Nn0=0;
  vector<double> vY;
  vector<double> vX;
  vector<double > veY;
  vector<double > veX;
  vector<double> tmp(0,2);
  
  for(int ip=0;ip<hd->GetNbinsX()+2;ip++) {
    double Yd,Xd,eYld,eYhd;
    double Ym, eYlm, eYhm; //Xm,

    Xd = hd->GetBinCenter(ip);
    Yd = hd->GetBinContent(ip);
    if(opt=="nP" || opt=="PP") {
      eYld = StatUtils::ErrorPL(Yd);
      eYhd = StatUtils::ErrorPH(Yd);
    }
    else {
      eYld = hd->GetBinError(ip);
      eYhd = hd->GetBinError(ip);
    }

    //Xm = hmc->GetBinCenter(ip);
    Ym = hmc->GetBinContent(ip);
    if(opt=="dP" || opt=="PP") {
      eYlm = StatUtils::ErrorPL(Ym);
      eYhm = StatUtils::ErrorPH(Ym);
    }
    else {
      eYlm = hmc->GetBinError(ip);
      eYhm = hmc->GetBinError(ip);
    }  

    if(Ym!=0) {
      Nn0++;
      
      vY.push_back(Yd/Ym);
      vX.push_back(Xd);
      veX.push_back( hd->GetBinWidth(ip)/2. );
      veX.push_back( hd->GetBinWidth(ip)/2. );
      veY.push_back( Yd/Ym*sqrt( pow( eYlm/Ym , 2) +  pow( eYld/Yd , 2) ) );
      veY.push_back( Yd/Ym*sqrt( pow( eYhm/Ym , 2) +  pow( eYhd/Yd , 2) ) );
    } 

  }
  TGraphAsymmErrors* graph=new TGraphAsymmErrors(Nn0);
  for(int ip=0;ip<Nn0;ip++) {
    graph->SetPoint(ip,vX[ip],vY[ip]);
    graph->SetPointError(ip,0,0/*veX[ip*2],veX[ip*2+1]*/,veY[ip*2],veY[ip*2+1]);
  }
  
  return graph;
}


TGraphAsymmErrors*
HistoUtils::residualGraph(TH1* hd, TH1* hmc, string opt) {
  
  int Nn0=0;
  vector<double> vY;
  vector<double> vX;
  vector<double > veY;
  vector<double > veX;
  vector<double> tmp(0,2);
  
  for(int ip=0;ip<hd->GetNbinsX()+2;ip++) {
    double Yd,Xd,eYld,eYhd;
    double Ym, eYlm=0, eYhm=0; //Xm,

    Xd = hd->GetBinCenter(ip);
    Yd = hd->GetBinContent(ip);
    if(opt=="nP" || opt=="PP") {
      eYld = StatUtils::ErrorPL(Yd);
      eYhd = StatUtils::ErrorPH(Yd);
    }
    else {
      eYld = hd->GetBinError(ip);
      eYhd = hd->GetBinError(ip);
    }

    //Xm = hmc->GetBinCenter(ip);
    Ym = hmc->GetBinContent(ip);
    // if(opt=="dP" || opt=="PP") {
    //   eYlm = StatUtils::ErrorPL(Ym);
    //   eYhm = StatUtils::ErrorPH(Ym);
    // }
    // else {
    //   eYlm = hmc->GetBinError(ip);
    //   eYhm = hmc->GetBinError(ip);
    // }  
    

    if(Ym!=0) {
      Nn0++;
      
      vY.push_back(Yd-Ym);
      vX.push_back(Xd);
      veX.push_back( hd->GetBinWidth(ip)/2. );
      veX.push_back( hd->GetBinWidth(ip)/2. );
      veY.push_back(  eYld );
      veY.push_back(  eYhd );
    } 

  }
  TGraphAsymmErrors* graph=new TGraphAsymmErrors(Nn0);
  for(int ip=0;ip<Nn0;ip++) {
    graph->SetPoint(ip,vX[ip],vY[ip]);
    graph->SetPointError(ip,0,0/*veX[ip*2],veX[ip*2+1]*/,veY[ip*2],veY[ip*2+1]);
  }
  
  return graph;
}



void 
HistoUtils::cleanGraph(TGraph*& g) {


}



TPolyLine* 
HistoUtils::GetSystBand(vector<float> xs, vector<float> yl, vector<float> yh, float xmin, float xmax) {
  

  int nbin = xs.size()-1;

  vector< float > vecx_(8*nbin,-100000);
  vector< float > vecyl_(8*nbin,-100000);
  vector< float > vecy_(8*nbin,-100000);
  //float lsyst;
  
  bool findM=false,findm=false;

  for( int ibin=0; ibin<nbin; ibin++ )
    {
      vecx_[4*ibin]   = (xs[ibin]*3+xs[ibin+1])/4.;
      vecx_[4*ibin+1]   = (xs[ibin]+xs[ibin+1])/2.;
      vecx_[4*ibin+2]   = (xs[ibin]+xs[ibin+1]*3)/4.;
      vecx_[4*ibin+3]   = xs[ibin+1];
      
      if(findM) {
	vecx_[ibin] = vecx_[ibin-1];
	findM=false;
      }
      if(findm && ibin!=0) {
	vecx_[ibin] = vecx_[0];
	findm=false;
      }

      vecy_[4*ibin]   = yh[ibin];
      vecy_[4*ibin+1]  = (yh[ibin]+yh[ibin+1])/2.;
      vecy_[4*ibin+2]  = yh[ibin+1];
      vecy_[4*ibin+3]  = yh[ibin+1];
      vecyl_[4*ibin]  = yl[ibin];
      vecyl_[4*ibin+1]  = (yl[ibin]+yl[ibin+1])/2.;
      vecyl_[4*ibin+2]  = yl[ibin+1];
      vecyl_[4*ibin+3]  = yl[ibin+1];
    
      vecx_[8*nbin-4*ibin-1] = vecx_[4*ibin];
      vecx_[8*nbin-4*ibin-2] = vecx_[4*ibin+1];
      vecx_[8*nbin-4*ibin-3] = vecx_[4*ibin+2];
      vecx_[8*nbin-4*ibin-4] = vecx_[4*ibin+3];
      vecy_[8*nbin-4*ibin-1] = -vecyl_[4*ibin];
      vecy_[8*nbin-4*ibin-2] = -vecyl_[4*ibin+1];
      vecy_[8*nbin-4*ibin-3] = -vecyl_[4*ibin+2];
      vecy_[8*nbin-4*ibin-4] = -vecyl_[4*ibin+3];
    }

  int n=0;
  for( size_t ipt=0; ipt<vecx_.size(); ipt++ )
    {
      if(vecx_[ipt]>=xmin && vecx_[ipt]<=xmax)
	n++;
    }
  TPolyLine* band = new TPolyLine(n); 
  int cnt=0;

  for( size_t ipt=0; ipt<vecx_.size(); ipt++ )
    {
      if(vecx_[ipt]>=xmin && vecx_[ipt]<=xmax) {
	band->SetPoint(cnt, vecx_[ipt], (1+vecy_[ipt])>1.6?1.6:((1+vecy_[ipt])<0.4?0.4:1+vecy_[ipt]) );
	cnt++;
      }
    }

  band->SetLineColor(kGray+1);
  band->SetFillColor(kGray+1);
  band->SetFillStyle(3002);

  return band;
}


void
HistoUtils::copyStyle(TH1* href, TH1*& hmod, bool axisOnly) {

  if(!axisOnly) {
    //color
    
    hmod->SetLineColor( href->GetLineColor() );
    hmod->SetMarkerColor( href->GetMarkerColor() );
    hmod->SetFillColor( href->GetFillColor() );

    //style
    hmod->SetLineStyle( href->GetLineStyle() );
    hmod->SetMarkerStyle( href->GetMarkerStyle() );
    hmod->SetFillStyle( href->GetFillStyle() );

    //widths
    hmod->SetLineWidth( href->GetLineWidth() );
    hmod->SetMarkerSize( href->GetMarkerSize() );
  }
  //X title
  hmod->GetXaxis()->SetTitle( href->GetXaxis()->GetTitle() );
  hmod->GetXaxis()->SetTitleOffset( href->GetXaxis()->GetTitleOffset() );
  hmod->GetXaxis()->SetTitleSize( href->GetXaxis()->GetTitleSize() );

  hmod->GetXaxis()->SetNdivisions( href->GetXaxis()->GetNdivisions() );
  hmod->GetXaxis()->SetLabelOffset( href->GetXaxis()->GetLabelOffset() );
  hmod->GetXaxis()->SetLabelSize( href->GetXaxis()->GetLabelSize() );
  
  //y title
  hmod->GetYaxis()->SetTitle( href->GetYaxis()->GetTitle() );
  hmod->GetYaxis()->SetTitleOffset( href->GetYaxis()->GetTitleOffset() );
  hmod->GetYaxis()->SetTitleSize( href->GetYaxis()->GetTitleSize() );

  hmod->GetYaxis()->SetNdivisions( href->GetYaxis()->GetNdivisions() );
  hmod->GetYaxis()->SetLabelOffset( href->GetYaxis()->GetLabelOffset() );
  hmod->GetYaxis()->SetLabelSize( href->GetYaxis()->GetLabelSize() );

  //Axis range
  // hmod->GetXaxis()->SetRangeUser( href->GetXaxis()->GetXmin(), href->GetXaxis()->GetXmax() );
  // hmod->GetYaxis()->SetRangeUser( href->GetYaxis()->GetXmin(), href->GetYaxis()->GetXmax() );

}


void
HistoUtils::manualClone(TH1* href, TH1*& hmod) {

  int nBin=href->GetNbinsX();
  for(int i=0;i<nBin+2;i++) {
    hmod->SetBinContent( i, href->GetBinContent(i) );
    hmod->SetBinError( i, href->GetBinError(i) );
  }

}

void
HistoUtils::manualRebin(TH1*& h, int bin) {

  if(bin==0) return;

  TH1* htmp=(TH1*)h->Clone();
  htmp->Reset("ICEM");

  vector<float> vals(h->GetNbinsX()/bin,0);
  vector<float> errs(h->GetNbinsX()/bin,0);

  if(h->GetNbinsX()%bin!=0) 
    cout<<"Warning, "<<h->GetName()
	<<" cannot be rebinned by "<<bin<<endl;

  for(int ib=1;ib<h->GetNbinsX()+1;ib++) {
    vals[ (ib-1)/bin ] += h->GetBinContent(ib)/bin;
    errs[ (ib-1)/bin ] += h->GetBinError(ib)/bin;
  }
  
  htmp->SetBinContent(0, h->GetBinContent(0) );
  htmp->SetBinContent(h->GetNbinsX()+1,  h->GetBinContent(h->GetNbinsX()+1) );
  for(int ib=1;ib<h->GetNbinsX()+1;ib++) {
    htmp->SetBinContent( ib, vals[ (ib-1)/bin ]);
    htmp->SetBinError( ib, errs[ (ib-1)/bin ]);
  }
  h=(TH1*)htmp;
}


void
HistoUtils::manualCompleteRebin(TH1*& h, int nBin, float xmin, float xmax) {

  TH1F* htmp=new TH1F(h->GetName(),h->GetTitle(), nBin, xmin, xmax);
  htmp->SetLineColor(h->GetLineColor() );
  htmp->SetMarkerColor(h->GetMarkerColor() );
  htmp->SetFillColor(h->GetFillColor() );
  htmp->SetLineWidth(h->GetLineWidth() );


  htmp->Reset("ICEM");

  vector<float> vals(nBin+2,0);
  vector<float> errs(nBin+2,0);

  for(int ib=0;ib<h->GetNbinsX()+2;ib++) {
    int bin=htmp->FindBin( h->GetXaxis()->GetBinCenter(ib) );
    vals[ bin ] += h->GetBinContent(ib);
    errs[ bin ] += h->GetBinError(ib)*h->GetBinError(ib);
  }
  
  for(int ib=0;ib<nBin+2;ib++) {
    htmp->SetBinContent( ib, vals[ib] );
    htmp->SetBinError( ib, sqrt(errs[ib]) );
  }
  h=(TH1*)htmp;
}


vector<vector<float> >
HistoUtils::getHistoUpperCoordinates(TH1* h) {
  
  vector<vector<float> > out(2,vector<float>(0,0));
  //cout<<h<<endl;
  for(int i=0;i<h->GetNbinsX()+2;i++) {
    out[0].push_back( h->GetBinCenter(i) - h->GetBinWidth(i)/2. );
    out[0].push_back( h->GetBinCenter(i) );
    
    out[1].push_back( max(h->GetBinContent(i), h->GetBinContent(i+1) ) );
    out[1].push_back( h->GetBinContent(i) );
    //cout<<i<<"   "<<h->GetBinCenter(i)<<"  "<<h->GetBinContent(i)<<endl;
  }
  
  return out;
}


vector<vector<float> >
HistoUtils::getHistoUpperCoordinatesWithError(TH1* h, float dNorm) {
  
  vector<vector<float> > out(2,vector<float>(0,0));
  //cout<<h<<endl;
  double x,y,eyh;
  for(int i=0;i<h->GetNbinsX()+2;i++) {
    x = h->GetBinCenter(i);
    y = h->GetBinContent(i)/dNorm;
    eyh = StatUtils::ErrorPH(y)/dNorm;
    out[0].push_back( x );
    out[1].push_back( y+eyh );
  }
  
  return out;
}


vector<vector<float> >
HistoUtils::getGraphUpperCoordinatesWithError(TGraphAsymmErrors* g) {
  
  vector<vector<float> > out(2,vector<float>(0,0));
  double x,y,eyh;
  for(int i=0;i<g->GetN();i++) {
    g->GetPoint(i,x,y);
    eyh = g->GetErrorYhigh(i);
    out[0].push_back( x );
    out[1].push_back( y+eyh );
  }
  
  return out;
}



float
HistoUtils::getHistoYlow(TH1* h, float xm, float xM) {
  
  float ylow=100000000000.; 
 
  for(int i=1;i<h->GetNbinsX()+1;i++) {
    if(ylow >h->GetBinContent(i) && h->GetBinContent(i)!=0 && 
       h->GetBinCenter(i)>=xm && h->GetBinCenter(i)<=xM ) {
      ylow = h->GetBinContent(i);
    }
  }
  return ylow;
}


float
HistoUtils::getHistoYlowWithError(TH1* h, float xm, float xM ,string opt) {
  
  float ylow=100000000000.; 
  double y,eyl;
  for(int i=1;i<h->GetNbinsX()+1;i++) {
    y = h->GetBinContent(i);
    if(opt=="P")
      eyl = StatUtils::ErrorPL(y);
    else
      eyl = h->GetBinError(i);

    if(ylow > y+eyl && h->GetBinContent(i)!=0 && 
       h->GetBinCenter(i)>=xm && h->GetBinCenter(i)<=xM ) {
      ylow = y+eyl;
    }
  }
  
  return ylow;
}


float
HistoUtils::getHistoYhighWithError(TH1* h, float xm, float xM, string opt) {
  
  float yhigh=-100000000000.; 
  double y,eyh;
  for(int i=1;i<h->GetNbinsX()+1;i++) {
    y = h->GetBinContent(i);
    if(opt=="P")
      eyh = StatUtils::ErrorPH(y);
    else
      eyh = h->GetBinError(i);
    
    if(yhigh < y+eyh && h->GetBinContent(i)!=0 && 
       h->GetBinCenter(i)>=xm && h->GetBinCenter(i)<=xM ) {
      yhigh = y+eyh;
    }
  }
  
  return yhigh;
}


float
HistoUtils::getGraphYlowWithError(TGraphAsymmErrors* g, float xm, float xM) {
  
  float ylow=100000000000.; 
  double y,eyl,x;
  for(int i=0;i<g->GetN();i++) {
    g->GetPoint(i,x,y);
    eyl = g->GetErrorYlow(i);
  
    if(ylow > y+eyl && y!=0 && x>=xm && x<=xM ) {
      ylow = y+eyl; 
    }
  }
  
  return ylow;
}

float
HistoUtils::getGraphYhighWithError(TGraphAsymmErrors* g, float xm, float xM) {
  
  float yhigh=-100000000000.; 
  double y,eyh,x;
  for(int i=0;i<g->GetN();i++) {
    g->GetPoint(i,x,y);
    eyh = g->GetErrorYhigh(i);
  
    if(yhigh < y+eyh && y!=0 && x>=xm && x<=xM  ) {
      yhigh = y+eyh; 
    }
  }
  
  return yhigh;
}


float
HistoUtils::getMaxDiff(TH1* hd, TH1* hm, float xm, float xM) {
  
  float dY=0.; 
  for(int i=1;i<hd->GetNbinsX()+1;i++) {
    if(hd->GetBinContent(i)!=0 && hm->GetBinContent(i)!=0 && 
       hd->GetBinCenter(i)>=xm && hd->GetBinCenter(i)<=xM  ) {
      if(dY < (hd->GetBinContent(i)-hm->GetBinContent(i)) )
	dY = hd->GetBinContent(i)-hm->GetBinContent(i);
    }
  }
  
  return dY;
}

int
HistoUtils::autoBinning(float xm, float xM, int nTB, float Xm, float XM) {

  float pas=(XM-Xm)/nTB;
  vector<int> pVals;
  for(int i=1;i<nTB/4.;i++)
    if(nTB%i==0) pVals.push_back(i);
  
  int bB=1; int tmp=nTB;
  for(size_t i=0;i<pVals.size();i++) {
    int nB = (min(xM,XM)-max(xm,Xm))/(pas*pVals[i]);
    if( fabs(nB-50)<tmp ){ 
      bB = pVals[i];
      tmp = fabs(nB-50);
    }
  }

  return bB;
  
}

vector<float>
HistoUtils::getXbinning(TH1* h ) {
  vector<float> bins;
  for(int ib=1;ib<h->GetNbinsX()+2;ib++) {
    bins.push_back( h->GetBinLowEdge(ib) );
  }
  return bins;
}

TH1F*
HistoUtils::significance(TH1F* hs, TH1F* hb) {
  
  TH1F* hsb=(TH1F*)hs->Clone();
  hsb->Reset("ICEM");

  int nT = hs->GetNbinsX()+2;
  for(int ib=0;ib<nT;ib++) {

    float ints = hs->Integral( ib, nT );
    float intb = hb->Integral( ib, nT );
    if(intb!=0)
      hsb->SetBinContent( ib, ints/sqrt(intb) );
    //hsb->SetBinError( ib, ints/sqrt(intb) );
  }
  
  
  return hsb;

}



TH2F*
HistoUtils::significance(TH2F* hs, TH2F* hb) {
  
  TH2F* hsb=(TH2F*)hs->Clone();
  hsb->Reset("ICEM");

  int nTx = hs->GetNbinsX()+2;
  int nTy = hs->GetNbinsY()+2;
  for(int ix=0;ix<nTx;ix++) {
    for(int iy=0;iy<nTy;iy++) {

      float ints = hs->Integral( ix, nTx, iy, nTy );
      float intb = hb->Integral( ix, nTx, iy, nTy );
      if(intb!=0)
	hsb->SetBinContent( ix, iy, ints/sqrt(intb) );

    }
  }

  return hsb;

}


TGraphAsymmErrors*
HistoUtils::efficiency(TH1* h) {

  int nBins= h->GetNbinsX();
  float integ=h->Integral(0,100000); 
 
  TGraphAsymmErrors* g=new TGraphAsymmErrors(nBins);
  
  for(int i=0;i<nBins;i++) {

    float eff=h->Integral(i+1,nBins+1)/integ;
    float err = StatUtils::BinomError(h->GetEntries(), eff );

    if(integ!=0) {
      g->SetPoint(i,h->GetBinCenter(i),eff);
      g->SetPointError(i, h->GetBinWidth(i)/2.,h->GetBinWidth(i)/2.,err, err);	
    }
  }

  return g;
}

TGraphAsymmErrors*
HistoUtils::curveROC(TH1* s, TH1* b) {

  int nBins= s->GetNbinsX();
  float intS=s->Integral(0,100000);
  float intB=b->Integral(0,100000);
  
  TGraphAsymmErrors* g=new TGraphAsymmErrors(nBins);
  for(int i=0;i<nBins;i++) {
    float effS=s->Integral(i+1,nBins+1)/intS;
    float errS = StatUtils::BinomError(s->GetEntries(), effS );

    float effB=b->Integral(i+1,nBins+1)/intB;
    float errB = StatUtils::BinomError(b->GetEntries(), effS );

    if(intS!=0 && intB!=0) {
      g->SetPoint(i,effS,effB);
      g->SetPointError(i,errS,errS,errB, errB);	
    }
  }
  
  return g;
}


