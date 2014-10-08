#include "utils/KineUtils.hh"

using namespace std;

float 
KineUtils::dR(float e1, float e2, float p1, float p2, bool conv) {

  float p1r,p2r;
  //Convert phi from ° to rad
  if( conv ) {
    p1r = p1*TMath::Pi()/180;
    p2r = p2*TMath::Pi()/180;
  }
  else {
    p1r= p1;
    p2r= p2;
  }
  float dp2 = pow( dPhi(p1r,p2r) , 2);
  
  float de2 = pow(e1-e2,2);  

  return sqrt(de2 + dp2 );

}

float 
KineUtils::phi( float x, float y )
{
  float phi_ =atan2(y, x);
  return (phi_>=0) ?  phi_ : phi_ + 2*TMath::Pi();
}

float 
KineUtils::dPhi( float phi1, float phi2 )
{
  float phi1_= phi( cos(phi1), sin(phi1) );
  float phi2_= phi( cos(phi2), sin(phi2) );
  float dphi_= phi1_-phi2_;
  if( dphi_> TMath::Pi() ) dphi_-=2*TMath::Pi();
  if( dphi_<-TMath::Pi() ) dphi_+=2*TMath::Pi();

  return dphi_;
}

