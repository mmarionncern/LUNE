#include "utils/StatUtils.hh"

using namespace std;

ClassImp(StatUtils)

float 
StatUtils::BinomError(float Nt, double eff) {
  
  float error=0;
  if(Nt==0) return 1;
  error = sqrt(eff*(1-eff)/Nt) ;
  return error;
}



void 
StatUtils::ComputeStatFromHisto(TH1* histo, double& Em, double& EM, double& M) {

  M = histo->GetMean();
  double integral = histo->Integral(0,histo->GetNbinsX() );
  double pas = histo->GetBinWidth(3);
  double em=0, eM=0;
  bool Limf[2]={false,false};
  for(int i=0;i<histo->GetNbinsX();i++) {
    em = histo->Integral(0,i)/integral;
    eM = histo->Integral(i,histo->GetNbinsX() )/integral;
   
    if(!Limf[0] && em >=0.16)
      {Limf[0]=true;  Em=i*pas-100;}
    if(!Limf[1] && eM <=0.16)
      {Limf[1]=true; EM=i*pas-100;}
  }
}



void
StatUtils::Get90PCInterval(TH1* histo,double &range0,double &range1) {

  double integral = histo->Integral(0, histo->GetNbinsX() +1 );
  double tmp=0;
  int bins[2]={0,10000};
  int dbin=10000;

  for(int ib=0;ib<histo->GetNbinsX() +1;ib++) {

    tmp=0;
    for(int ib2=ib;ib2<histo->GetNbinsX() +1;ib2++) {

      tmp =  histo->Integral(ib,ib2);
      if(tmp/integral>0.95) {
	if(dbin>(ib2-ib) )
	  {
	    dbin = ib2-ib;
	    bins[0] = ib -1;
	    bins[1] = ib2 +1;
	  }
      }
    }
  }
  
  range0 = histo->GetBinCenter(bins[0]);
  range1 = histo->GetBinCenter(bins[1]);
  
}

TH1* 
StatUtils::Get90HistoInterval(TH1* histo) {
  
  double integral = histo->Integral(0, histo->GetNbinsX() +1 );
  double tmp=0;
  int bins[2]={0,10000};
  int dbin=10000;

  for(int ib=0;ib<histo->GetNbinsX() +1;ib++) {

    tmp=0;
    for(int ib2=ib;ib2<histo->GetNbinsX() +1;ib2++) {

      tmp =  histo->Integral(ib,ib2);
      if(tmp/integral>0.95) {
	if(dbin>(ib2-ib) )
	  {
	    dbin = ib2-ib;
	    bins[0] = ib;
	    bins[1] = ib2;
	  }
      }
    }
  }
  TH1* tmph = (TH1*)histo->Clone();
  tmph->Reset("ICEM");
  for(int ib=bins[0];ib<bins[1];ib++)
    tmph->SetBinContent(ib, histo->GetBinContent(ib) );

  return tmph;
}


float StatUtils::Kurt(vector<double> values, double mean, double sigma) {

  if(values.size()<2)
    return 0;

  float kurt=0;
  int N = values.size();
  float mom4=0;

  for(int i=0;i<N;i++) {
    mom4 += pow( values[i] - mean, 4);
  }
  mom4 /= N;
  kurt = (mom4- ( (N-3)/(N-1) * sigma ))/N;

  return kurt;
}


void StatUtils::KolmogorovTest(TH1* MC, TH1* dataH, string obs) {

  double Kolm;
  Kolm= dataH->KolmogorovTest( MC,"UO");
  
  double Chi2norm,Chi2prob;
  Chi2prob = dataH->Chi2Test(MC,"UW P");
  Chi2norm = dataH->Chi2Test(MC,"UW Chi2/NDF");
  
  cout<<obs<<" ---> KS prob = "<<Kolm<<"   ; Chi2 prob = "<< Chi2prob<<"  ; Chi2/NDF = "<<Chi2norm<<" whithout weight "<<dataH->Chi2Test(MC,"P")<<endl;
}



float StatUtils::Poisson(float mc, int dataN) {
  float poisson;
  if(dataN<20) {
    double lnmu= log( pow(mc,(float)dataN)/Fact(dataN) );
    poisson = mc - lnmu;
  }
   else //normal law approx
    poisson = log(sqrt(2*acos(-1)*dataN) ) + pow((dataN- mc),2)/(2*dataN) ;
  return poisson;
}

// float StatUtils::LnL(float si, float bi, int ni,int NData, float a) {

//   double L=0;
//   double lmu = (a*si + (1-a)*bi )*NData;
//   if(ni<20) {
//     double lnmu= log( pow(lmu,(double)ni)/Fact(ni) );
//     L = lmu - lnmu;
//   }
//   else {
//     L = log(sqrt(2*acos(-1)*ni) ) + pow((ni- lmu),2)/(2*ni) ;
//   }
    
//   return L;
// }


float StatUtils::Fact(int x)
{
  if(x==0) return 1;
  float res = 1;
  for(int i = 1; i <= x; i++)
    res *= i;
  return res;
}

void StatUtils::Chi2Test(TH1* MC, TH1* dataH) {
  
  float chi2=0;
  float ni=0;
  float li=0;
  int N=0;
  
  for(int i=0;i<MC->GetNbinsX();i++) {

    ni = dataH->GetBinContent(i);
    li = MC->GetBinContent(i);
    
    if( li!=0 ) 
      {
	chi2 += Poisson(li,ni);
	N++;
      }
    }
  cout<<"Chi2 = "<<chi2<<"   ;  ndf = "<<N<<"  chi2/ndof "<<chi2/(N)<<endl;
}


float 
StatUtils::ErrorPH(int N, int sigma) {

  float alpha;

  switch (sigma)
    {
    case 1 : alpha = 0.3173; break;
    case 2 : alpha = 0.0455; break;
    case 3 : alpha = 0.0027; break;
    case 4 : alpha = 0.000063; break;
    case 5 : alpha = 0.00000057; break;
    default: {cout<<" By default, sigma=1"<<endl; alpha = 0.3173; break;}
    }
  
  float eh =  TMath::ChisquareQuantile(1- alpha/2,2*(N +1))*0.5 - N;

  if(N == 0)
    eh =  TMath::ChisquareQuantile(1- alpha,2*(N +1))*0.5  - N;
  
  return eh;
  
}

float 
StatUtils::ErrorPL(int N, int sigma) {

  float alpha;

  switch (sigma)
    {
    case 1 : alpha = 0.3173; break;
    case 2 : alpha = 0.0455; break;
    case 3 : alpha = 0.0027; break;
    case 4 : alpha = 0.000063; break;
    case 5 : alpha = 0.00000057; break;
    default: {cout<<" By default, sigma=1"<<endl; alpha = 0.3173; break;}
    }
 
  float el = N - TMath::ChisquareQuantile(alpha/2,2*N)*0.5;

  return el;
  
}



void StatUtils::Integral(TH1* mc, TH1* data, double xmin, double xmax) {
  
  int Binm = data->FindBin( xmin );
  int BinM = data->FindBin( xmax );

  double integMC = mc->Integral(Binm,BinM);
  double integdata = data->Integral(Binm,BinM);

  cout<<endl<<" *** Integral "<<xmin<<" -> "<<xmax<<" ---> "<<Binm<<"   "<<BinM<<" : MC -> "<<integMC<<"   ; data ->  "<<integdata<< " +- "<<sqrt(integdata)<<endl;
}

