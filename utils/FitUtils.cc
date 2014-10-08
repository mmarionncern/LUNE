#include "utils/FitUtils.cc"


RooGaussian* 
FitUtils::gausPdf(string id, vector<vector<float> > iVals, RooRealVar x) {
  
  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(),iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* s=new RooRealVar(("#sigma"+id).c_str(),("#sigma"+id).c_str(),iVals[1][0],iVals[1][1],iVals[1][2]);

  RooGaussian* gaus=new RooGaussian(("gaus"+id).c_str(),("gaus"+id).c_str(),x,*m,*s);
  
  return gaus;
}


RooExponential* 
FitUtils::expoPdf(string id, vector<vector<float> > iVals, RooRealVar x) {
  
  RooRealVar* t=new RooRealVar(("#tau"+id).c_str(),("#tau"+id).c_str(), iVals[0][0],iVals[0][1],iVals[0][2]);

  RooExponential* expo=new RooExponential(("expo"+id).c_str(),
					  ("expo"+id).c_str(),
					  x,*t);

  return expo;
}


RooVoigtian* 
FitUtils::voigPdf(string id, vector<vector<float> > iVals, RooRealVar x) {
  
  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(), iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* w=new RooRealVar(("w"+id).c_str(),("w"+id).c_str(), iVals[1][0],iVals[1][1],iVals[1][2]);
  RooRealVar* s=new RooRealVar(("#sigma"+id).c_str(),("#sigma"+id).c_str(), iVals[2][0],iVals[2][1],iVals[2][2]);
  
  RooVoigtian* voig=new RooVoigtian(("voig"+id).c_str(),
				    ("voig"+id).c_str(),
				    x,*m,*w,*s);

  return voig;
}


RooPolynomial* 
FitUtils::polyPdf(string id, vector<vector<float> > iVals, RooRealVar x) {
  
  RooArgList listcoef( ("coefs"+id).c_str() );
  for(size_t ic=0;ic<iVals.size();ic++) {
    ostringstream os; os<<ic;
    string cname="c"+os.str()+id;

    RooRealVar* c=new RooRealVar(cname.c_str(),cname.c_str(), iVals[ic][0],iVals[ic][1],iVals[ic][2]);
    listcoef.add( *c );
  }
  
  RooPolynomial* poly=new RooPolynomial(("poly"+id).c_str(),
					("poly"+id).c_str(),
					x,listcoef);

  return poly;
}


RooCBShape*
FitUtils::cbPdf(string id, vector<vector<float> > iVals, RooRealVar x) {

  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(), iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* m0=new RooRealVar(("#mu0"+id).c_str(),("#mu0"+id).c_str(), iVals[1][0],iVals[1][1],iVals[1][2]);
  RooRealVar* s=new RooRealVar(("#sigma"+id).c_str(),("#sigma"+id).c_str(), iVals[2][0],iVals[2][1],iVals[2][2]);
  RooRealVar* a=new RooRealVar(("#alpha"+id).c_str(),("#alpha"+id).c_str(), iVals[3][0],iVals[3][1],iVals[3][2]);
  
  RooCBShape* cbsh=new RooCBSShape(("cbsh"+id).c_str(),
				   ("cbsh"+id).c_str(),
				   x,*m,*m0,*s,*a);

  return cbsh;
}


RooBreitWigner*
FitUtils::bwPdf(string id, vector<vector<float> > iVals, RooRealVar x) {

  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(), iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* w=new RooRealVar(("w"+id).c_str(),("w"+id).c_str(), iVals[1][0],iVals[1][1],iVals[1][2]);
  
  RooBreitWigner* bwsh=new RooBreitWigner(("bwsh"+id).c_str(),
					  ("bwsh"+id).c_str(),
					  x,*m,*w);

  return bwsh;
}


RooNovosibirsk*
FitUtils::novoPdf(string id, vector<vector<float> > iVals, RooRealVar x) {

  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(), iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* s=new RooRealVar(("#sigma"+id).c_str(),("#sigma"+id).c_str(), iVals[1][0],iVals[1][1],iVals[1][2]);
  RooRealVar* t=new RooRealVar(("#tau"+id).c_str(),("#tau"+id).c_str(), iVals[2][0],iVals[2][1],iVals[2][2]);
  
  RooNovosibirsk* novo=new RooNovosibirsk(("novo"+id).c_str(),
					  ("novo"+id).c_str(),
					  x,*m,*s,*t);
  
  return novo;
}


RooLandau* 
FitUtils::landPdf(string id, vector<vector<float> > iVals, RooRealVar x) {
  
  RooRealVar* m=new RooRealVar(("#mu"+id).c_str(),("#mu"+id).c_str(),iVals[0][0],iVals[0][1],iVals[0][2]);
  RooRealVar* s=new RooRealVar(("#sigma"+id).c_str(),("#sigma"+id).c_str(),iVals[1][0],iVals[1][1],iVals[1][2]);

  RooLandau* land=new RooLandau(("land"+id).c_str(),("land"+id).c_str(),x,*m,*s);
  
  return land;
}
