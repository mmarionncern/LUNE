#include "modules/MT2Functions.hh"

using namespace std;
 
ClassImp(MT2Functions)

double
MT2(const double *xx ) {

  float _px1 = xx[0]; //nu1 px
  float _py1 = xx[1]; //nu1 py 
  
  float _px2 = xx[2] - _px1;
  float _py2 = xx[3] - _py1;

  TVector2 _nu1(_px1,_py1);
  TVector2 _nu2(_px2,_py2);
  
  TVector2 _mVa( xx[4], xx[5]);
  TVector2 _mVb( xx[6], xx[7]);

  float _mTa = sqrt(2*_mVa.Mod()*_nu1.Mod()*(1-cos(KineUtils::dPhi(_mVa.Phi(), _nu1.Phi() ))) );
  float _mTb = sqrt(2*_mVb.Mod()*_nu2.Mod()*(1-cos(KineUtils::dPhi(_mVb.Phi(), _nu2.Phi() ))) );
  
  // cout<<_px1<<"   "<<_py1<<" // "<<xx[2]<<"   "<<xx[3]<<endl;
  // cout<<_mVa.Mod()<<"   "<<_mVa.Phi()<<"    "<<_mVb.Mod()<<"   "<<_mVb.Phi()<<endl;
  // cout<<_nu1.Mod()<<"   "<<_nu1.Phi()<<"    "<<_nu2.Mod()<<"   "<<_nu2.Phi()<<endl;
  // cout<<"==========> "<<_mTa<<"  "<<_mTb<<endl;
 
    return max(_mTa,_mTb);
}


MT2Functions::MT2Functions() {
}

MT2Functions::~MT2Functions() {
}


float
MT2Functions::mT2(TLorentzVector mVa,TLorentzVector mVb,
				    TVector2 met) {


  

  // Choose method upon creation between:
  // kMigrad, kSimplex, kCombined, 
  // kScan, kFumili
  ROOT::Minuit2::Minuit2Minimizer min ( ROOT::Minuit2::kMigrad );
 
   min.SetMaxFunctionCalls(100000);
   min.SetMaxIterations(100000);
   min.SetTolerance(0.01);
 
   ROOT::Math::Functor f(&MT2,8); 
   min.SetFunction(f);
 
   // Set the free variables to be minimized!
   min.SetVariable(0,"nu1_x", 0, 0.1);
   min.SetVariable(1,"nu1_y", 0, 0.1);
   min.SetFixedVariable(2,"met_x", met.X());
   min.SetFixedVariable(3,"met_y", met.Y());
   min.SetFixedVariable(4,"mVa_x", mVa.Px());
   min.SetFixedVariable(5,"mVa_y", mVa.Py());
   min.SetFixedVariable(6,"mVb_x", mVb.Px());
   min.SetFixedVariable(7,"mVb_y", mVb.Py());
  
   min.Minimize(); 
 
   const double *xs = min.X();
   // cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): " 
   // 	<< MT2(xs) << endl;
 
   return MT2(xs);
}
