#include "modules/LeptonDisplay.hh"

#include <TEllipse.h>
#include <TStyle.h>

LeptonDisplay::LeptonDisplay() {

  _initCanvas=false;
  gStyle->SetPaintTextFormat("1.1f");
  _evt=-1;
  _lep=1;

}


LeptonDisplay::~LeptonDisplay() {
}

void
LeptonDisplay::fill(float l1eta, float l1phi, float l1charge,
		    vector<float> phPt, vector<float> phEta, vector<float> phPhi,
		    vector<float> chPt, vector<float> chEta, vector<float> chPhi,
		    vector<float> nhPt, vector<float> nhEta, vector<float> nhPhi ) {

  _lEta.push_back(l1eta);
  _lPhi.push_back(l1phi);
  _lCharge.push_back(l1charge);

  _phPt.push_back(phPt);
  _phEta.push_back(phEta);
  _phPhi.push_back(phPhi);

  _chPt.push_back(chPt);
  _chEta.push_back(chEta);
  _chPhi.push_back(chPhi);

  _nhPt.push_back(nhPt);
  _nhEta.push_back(nhEta);
  _nhPhi.push_back(nhPhi);

}


void
LeptonDisplay::drawCanvas() {

  if(!_initCanvas) {
    
    _c = new TCanvas("lepton","lepton",600,640);
    _c->Draw();
    _initCanvas=true;
  }

}

void
LeptonDisplay::nextLepton() {

  if(_lep==1) {_lep=0;_evt+=1;}
  else _lep=1;

  if(_evt*2+_lep>=(int)_lEta.size() )
    cout<<" no more lepton"<<endl;
  else
    showLepton(_evt, _lep);
}


void
LeptonDisplay::previousLepton() {

  if(_lep==0) {_lep=1;_evt-=1;}
  else _lep=1;
  
  if(_evt<=-1)
    cout<<" no previous lepton"<<endl;
  else
    showLepton(_evt, _lep);
}


void
LeptonDisplay::showLepton(int evt, int lep) {

  drawCanvas();

  float l_eta= _lEta[evt*2+lep];
  float l_phi= _lPhi[evt*2+lep];

  delete _h;
  delete _hPh;
  delete _hCh;
  delete _hNh;
  _h = new TH2F("h","h",50,l_eta-0.5,l_eta+0.5,50,l_phi-0.5,l_phi+0.5);
  _h->GetXaxis()->SetTitle("#eta");
  _h->GetYaxis()->SetTitle("#phi");
  _h->SetFillColor(0);
  _h->SetLineColor(0);
  _h->GetXaxis()->SetNdivisions(8,5,0);
  _h->GetYaxis()->SetNdivisions(8,5,0);
  

  _hPh=(TH2F*)_h->Clone("hph");
  _hCh=(TH2F*)_h->Clone("hch");
  _hNh=(TH2F*)_h->Clone("hnh");

  _h->SetBinContent(1,1,0.5);

  cout<<" lepton : "<<evt*2+lep<<" ---> "
      <<_phPt[evt*2+lep].size()<<" / "
      <<_chPt[evt*2+lep].size()<<" / "
      <<_nhPt[evt*2+lep].size()<<endl;

  for(size_t ii=0;ii<_phPt[evt*2+lep].size();ii++) {
    _hPh->Fill( _phEta[evt*2+lep][ii], corPhi(_phPhi[evt*2+lep][ii],l_phi), _phPt[evt*2+lep][ii]);
  }

  for(size_t ii=0;ii<_chPt[evt*2+lep].size();ii++) {
    _hCh->Fill( _chEta[evt*2+lep][ii], corPhi(_chPhi[evt*2+lep][ii],l_phi), _chPt[evt*2+lep][ii]);
  }
  
  for(size_t ii=0;ii<_nhPt[evt*2+lep].size();ii++) {
    _hNh->Fill( _nhEta[evt*2+lep][ii], corPhi(_nhPhi[evt*2+lep][ii],l_phi), _nhPt[evt*2+lep][ii]);
  }
  
  
  //_hPh->SetLineColor(kOrange-3);
  _hPh->SetFillColor(kOrange-4);
  
  //_hCh->SetLineColor(kBlue+1);
  _hCh->SetFillColor(kAzure-4);
  
  //_hNh->SetLineColor(kRed+1);
  _hNh->SetFillColor(kViolet-2);
  
  _c->cd();
  
  _h->Draw("box");
  _hPh->Draw("box same text");
  _hCh->Draw("box same text");
  _hNh->Draw("box same text");
  
  TLine l;
  l.SetLineWidth(2);
  l.SetLineStyle(1);
  l.SetLineColor(kGreen+2);
  l.DrawLine(l_eta,l_phi ,l_eta,l_phi+0.2*_lCharge[evt*2+lep]);

  TLine l1;
  l1.SetLineWidth(1);
  l1.SetLineStyle(7);
  l1.DrawLine(l_eta,l_phi-0.5 ,l_eta,l_phi+0.5);

  TLine l2;
  l2.SetLineWidth(1);
  l2.SetLineStyle(7);
  l2.DrawLine(l_eta-0.5,l_phi ,l_eta+0.5,l_phi);


  TEllipse el1;
  el1.SetLineWidth(1);
  el1.SetFillStyle(0);
  el1.SetLineStyle(7);
  el1.DrawEllipse(l_eta,l_phi,0.1,0.1,0,360,0);

  TEllipse el2;
  el2.SetLineWidth(1);
  el2.SetFillStyle(0);
  el2.SetLineStyle(7);
  el2.DrawEllipse(l_eta,l_phi,0.2,0.2,0,360,0);

  TEllipse el3;
  el3.SetLineWidth(1);
  el3.SetFillStyle(0);
  el3.SetLineStyle(7);
  el3.DrawEllipse(l_eta,l_phi,0.3,0.3,0,360,0);

  TEllipse el4;
  el4.SetLineWidth(1);
  el4.SetFillStyle(0);
  el4.SetLineStyle(7);
  el4.DrawEllipse(l_eta,l_phi,0.4,0.4,0,360,0);
  
  TEllipse el5;
  el5.SetLineWidth(1);
  el5.SetFillStyle(0);
  el5.SetLineStyle(7);
  el5.DrawEllipse(l_eta,l_phi,0.5,0.5,0,360,0);
  
  TMarker m;
  m.SetMarkerStyle(2);
  m.SetMarkerSize(1.1);
  m.DrawMarker(l_eta,l_phi);


}



float 
LeptonDisplay::corPhi(float phic, float phir ) {
  
  if(phir<-TMath::Pi()+0.5 && phic>=0)
    return phic-2*TMath::Pi();
  if(phir>TMath::Pi()-0.5 && phic<=0)
    return phic+2*TMath::Pi();
     
  if(fabs(phic-phir)>TMath::Pi() && phir<0)
    return phic-2*TMath::Pi();
  if(fabs(phic-phir)>TMath::Pi() && phir>0)
    return phic+2*TMath::Pi();

  return phic;
}
