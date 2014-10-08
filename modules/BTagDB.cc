#include "modules/BTagDB.hh"

// ClassImp(BTagDB)

BTagDB::BTagDB(string disc, string type) {
  _disc = disc;
  _type = type;
}

void
BTagDB::configure(string disc, string type) {
  _disc = disc;
  _type = type;
}

float 
BTagDB::getBTagSF(double pt, double eta, int isb) {

  if(_disc=="CSVL") {
    if(isb==5)
      return btagCSVLSF(pt, 1);
    if(isb==4)
      return btagCSVLSF(pt, 0);
    else
      return mistagCSVLSF(pt, eta);
  }
  else return 0.;
}

float 
BTagDB::getBTagEff(double pt, double eta,double disc, int isb) {
  
  if(_disc=="CSVL") {
    if(isb==5)
      return btagCSVLEFF(disc, 1); //b quark
    if(isb==4)
      return btagCSVLEFF(disc, 0); //c quark
    else
      return mistagCSVLEFF(pt, eta);
  }
  else return 0.;
}

float 
BTagDB::btagCSVLSF(double x, bool isb) {

  //Using payloads for Moriond13
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_Moriond13_presc
  //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_payload_Moriond13.txt
  float ptmin[16] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  float ptmax[16] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  // Tagger: CSVL within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
  float SFb = 0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));
  float SFb_error[16] = {
    0.0484285,
    0.0126178,
    0.0120027,
    0.0141137,
    0.0145441,
    0.0131145,
    0.0168479,
    0.0160836,
    0.0126209,
    0.0136017,
    0.019182,
    0.0198805,
    0.0386531,
    0.0392831,
    0.0481008,
    0.0474291 };
  
  double SFunc = 0.;
  for (int i=0; i<15; ++i){
    if (x > ptmin[i] && x < ptmax[i+1])
      SFunc = SFb_error[i];
  }
  
  double unc = 0;
  
  if (isb) 
    unc = SFunc;
  else 
    unc = 2*SFunc;

  if(_type=="mean")
    return SFb;
  else if(_type=="min")
    return SFb-unc;
  else if(_type=="max")
    return SFb+unc;
  else
    return 0.;
  //  return SFb-unc;
}

float
BTagDB::btagCSVLEFF(double x, int isb) {

  //x == discriminator value
  
  double effb = 1.;
  double effc = 1.;
  //double effb_err_max = 0.;
  //This returns btag efficiency measured in 2011 data ttbar events
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2011_Data_and_MC
  effb = -4.46932526854*x*x*x*x+7.32781975653*x*x*x-3.78459588569*x*x+0.221027515486*x+0.970299300468;

  //Functional form for b/c-tagging efficiency on MC:
  //  effb = 3.90732786802e-06*x*x*x*x +  -0.000239934437355*x*x*x +  0.00664986827287*x*x +  -0.112578996016*x +  1.00775721404;
  effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;

  double eff = 1.;
  if (isb==1)
    eff = effb;
  else
    eff = effc;
  
  return eff;
}

float 
BTagDB::mistagCSVLSF(double x, double y) {
  //x= pT
  //y= eta
  //This function is only for CSVL taggger
  //Does not take into account syst+stat uncertainties on SF

  //Values are available only for jets with pT<800
  if (x>800) x=799;

  
  string meanminmax = "mean"; //_type
  //  TF1 *tmpSFl = NULL;  
  string Atagger = _disc; //tagger+TaggerStrength;
  // string sEtamin = Form("%1.1f",Etamin);
  // string sEtamax = Form("%1.1f",Etamax);
  //  cout << sEtamin << endl;
  //  cout << sEtamax << endl;
  
  // Begin of definition of functions from SF_12ABCD ---------------


  float tmpSFl = 1.;

  if(  fabs(y)>=1.5 && fabs(y)<2.4 ) {
    if (x > 700)
      x = 699;
  }

  if( Atagger == "CSVL" && fabs(y)<0.5 ) {
    if( meanminmax == "mean" ) tmpSFl = ((1.04901+(0.00152181*x))+(-3.43568e-06*(x*x)))+(2.17219e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.973773+(0.00103049*x))+(-2.2277e-06*(x*x)))+(1.37208e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.12424+(0.00201136*x))+(-4.64021e-06*(x*x)))+(2.97219e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && fabs(y)>=0.5 && fabs(y)<1. ) {
    if( meanminmax == "mean" ) tmpSFl = ((0.991915+(0.00172552*x))+(-3.92652e-06*(x*x)))+(2.56816e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.921518+(0.00129098*x))+(-2.86488e-06*(x*x)))+(1.86022e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.06231+(0.00215815*x))+(-4.9844e-06*(x*x)))+(3.27623e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && fabs(y)>=1. && fabs(y)<1.5 ) {
    if( meanminmax == "mean" ) tmpSFl = ((0.962127+(0.00192796*x))+(-4.53385e-06*(x*x)))+(3.0605e-09*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.895419+(0.00153387*x))+(-3.48409e-06*(x*x)))+(2.30899e-09*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.02883+(0.00231985*x))+(-5.57924e-06*(x*x)))+(3.81235e-09*(x*(x*x)));
  }
  if( Atagger == "CSVL" && fabs(y)>=1.5 && fabs(y)<2.4 ){
    if( meanminmax == "mean" ) tmpSFl = ((1.06121+(0.000332747*x))+(-8.81201e-07*(x*x)))+(7.43896e-10*(x*(x*x)));
    if( meanminmax == "min" ) tmpSFl = ((0.983607+(0.000196747*x))+(-3.98327e-07*(x*x)))+(2.95764e-10*(x*(x*x)));
    if( meanminmax == "max" ) tmpSFl = ((1.1388+(0.000468418*x))+(-1.36341e-06*(x*x)))+(1.19256e-09*(x*(x*x)));
  }
   
 return tmpSFl;
}


float 
BTagDB::mistagCSVLEFF(double x, double eta) {

  //These correspond still to the 2011 payloads
  //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C

  //Values are available only for jets with pT<670
  if (x>670) x=669;

  Double_t eff_l = 0.0;
  
  if(fabs(eta) >= 0.0 && fabs(eta) < 0.5) 
    eff_l = 242534*(((1+(0.0182863*x))+(4.50105e-05*(x*x)))/(1+(108569*x)));
  else if (fabs(eta) >= 0.5 && fabs(eta) < 1.0) 
    eff_l = 129.938*(((1+(0.0197657*x))+(4.73472e-05*(x*x)))/(1+(55.2415*x)));
  else if (fabs(eta) >= 1.0 && fabs(eta) < 1.5)
    eff_l = 592.214*(((1+(0.00671207*x))+(6.46109e-05*(x*x)))/(1+(134.318*x)));
  else if (fabs(eta) >= 1.5 && fabs(eta) < 2.4)
    eff_l = 93329*(((1+(0.0219705*x))+(3.76566e-05*(x*x)))/(1+(18245.1*x)));      
  
  return eff_l;

}
