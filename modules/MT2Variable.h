//ROOT libs
#include <TH1.h>
#include <TF1.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TMath.h>
#include <Minuit2/FCNBase.h>

//C++ libs
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <TLorentzVector.h>
#include <TVector2.h>

using namespace std;
double Chi2(TLorentzVector _el1,TLorentzVector _el2,TLorentzVector _nu1,TLorentzVector _nu2);

class KineFit: public ROOT::Minuit2::FCNBase {
   
  public: 
   
  KineFit();
  ~KineFit();
  
  double operator()(const vector<double>& param) const;   
  double Up() const { return _fUp; }
  void initialize(TLorentzVector el1, TLorentzVector el2, TVector2 met);

 private: 

  TLorentzVector _el1;
  TLorentzVector _el2;
  TVector2 _met;
  
  // float _px1,_px2,_py1,_py2,_pz1,_pz2;

  double _fUp;

};


KineFit::KineFit()
{

  cout << "SimFit::Preparing the FCNBase object to give to Minuit" << endl;

 
  _fUp = 0.5;
  
} 

KineFit::~KineFit() {}

void KineFit::initialize(TLorentzVector mVa, TLorentzVector mVb, TVector2 met) {

  _mVa = mVa;
  _mVb = mVb;
  _met = met;

  cout<<" Parameters loaded "<<endl;

}

double KineFit::operator()( const std::vector<double>& param ) const { //machin à minimiser 

  //==================== simul fit
  float _px1 = param[0]; //nu1 px
  float _py1 = param[1]; //nu1 py 
  
  float _px2 = _met.Px() - _px1;
  float _py2 = _met.Py() - _py1;

  TVector2 _nu1(_px1,_py1);
  TVector2 _nu2(_px2,_py2);
  
  double chi2 = Chi2(_mVa,_mVb,_nu1,_nu2);
  
  return chi2;
  
}

double Chi2(TLorentzVector _mVa,TLorentzVector _mVb,TVector2 _nu1,TVector2 _nu2){
  
  float _mTa = sqrt(2*_mVa.Pt()*_nu1.Mod()*(1-cos(KineUtils::dPhi(_mVa.Phi(), _nu1.Phi() ))) );
  float _mTb = sqrt(2*_mVb.Pt()*_nu2.Mod()*(1-cos(KineUtils::dPhi(_mVb.Phi(), _nu2.Phi() ))) );
    cout<<"==========> "<<_mTa<<"  "<<_mTb<<endl;
 
    return max(_mTa,_mTb);
}







