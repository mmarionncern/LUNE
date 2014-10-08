#include "modules/METFunctions.hh"

using namespace std;

#include "TFile.h"

ClassImp(METFunctions)

METFunctions::METFunctions() {

}

METFunctions::METFunctions(string flav, std::vector<std::string> uncSrc):
  _c(0) {
  
  _flav= flav;
  _detail=false;

  _uncNames = uncSrc;

  // _uncNames.push_back("EESUp");
  // _uncNames.push_back("EESDo");
  // _uncNames.push_back("MESUp");
  // _uncNames.push_back("MESDo");
  // _uncNames.push_back("PESUp");
  // _uncNames.push_back("PESDo");
  // _uncNames.push_back("UESUp");
  // _uncNames.push_back("UESDo");
  // _uncNames.push_back("JESUp");
  // _uncNames.push_back("JESDo");
  // _uncNames.push_back("JERUp");
  // _uncNames.push_back("JERDo");

  
  for(size_t iu=0;iu<_uncNames.size();iu++) {
    _itt.upa_mc[ _uncNames[iu] ] = -1;
    _itt.upe_mc[ _uncNames[iu] ] = -1;
    _itt.urpa_mc[ _uncNames[iu] ] = -1;
    _itt.umx_mc[ _uncNames[iu] ] = -1;
    _itt.umy_mc[ _uncNames[iu] ] = -1;
      
    _itt.upa_d[ _uncNames[iu] ] = -1;
    _itt.upe_d[ _uncNames[iu] ] = -1;
    _itt.urpa_d[ _uncNames[iu] ] = -1;
    _itt.umx_d[ _uncNames[iu] ] = -1;
    _itt.umy_d[ _uncNames[iu] ] = -1;
  }
  
  _dataT = new TTree("treeData","treeData");
  _dataT->SetDirectory( 0 );
 
  _dataT->Branch("qT",&_itt.qt_d);
  _dataT->Branch("para",&_itt.upara_d);
  _dataT->Branch("perp",&_itt.uperp_d);
  _dataT->Branch("redupara",&_itt.redupara_d);
  _dataT->Branch("metx",&_itt.metx_d);
  _dataT->Branch("mety",&_itt.mety_d);
  _dataT->Branch("nVtx",&_itt.nVtx_d);
  _dataT->Branch("sumEt",&_itt.sumEt_d);
  _dataT->Branch("weight",&_itt.w_d);
  
  // for(size_t iu=0;iu<_uncNames.size();iu++) {
  //   _dataT->Branch( ("upara"+_uncNames[iu]).c_str() ,&_itt.upa_d[ _uncNames[iu] ]);
  //   _dataT->Branch( ("uperp"+_uncNames[iu]).c_str() ,&_itt.upe_d[ _uncNames[iu] ]);
  //   _dataT->Branch( ("uredupara"+_uncNames[iu]).c_str() ,&_itt.urpa_d[ _uncNames[iu] ]);
  //   _dataT->Branch( ("umetx"+_uncNames[iu]).c_str() ,&_itt.umx_d[ _uncNames[iu] ]);
  //   _dataT->Branch( ("umety"+_uncNames[iu]).c_str() ,&_itt.umy_d[ _uncNames[iu] ]);
   
  // }
  
  _mcT = new TTree("treeMC","treeMC");
  _mcT->SetDirectory( 0 );
  _mcT->SetMaxVirtualSize(2000000000);

  _mcT->Branch("qT",&_itt.qt_mc);
  _mcT->Branch("para",&_itt.upara_mc);
  _mcT->Branch("perp",&_itt.uperp_mc);
  _mcT->Branch("redupara",&_itt.redupara_mc);
  _mcT->Branch("metx",&_itt.metx_mc);
  _mcT->Branch("mety",&_itt.mety_mc);
  _mcT->Branch("nVtx",&_itt.nVtx_mc);
  _mcT->Branch("sumEt",&_itt.sumEt_mc);
  _mcT->Branch("weight",&_itt.w_mc);
  _mcT->Branch("qTweight",&_itt.wqT_mc);

  
  for(size_t iu=0;iu<_uncNames.size();iu++) {
    // _mcT->Branch( ("upara"+_uncNames[iu]).c_str() ,&_itt.upa_mc[ _uncNames[iu] ]);
    // _mcT->Branch( ("uperp"+_uncNames[iu]).c_str() ,&_itt.upe_mc[ _uncNames[iu] ]);
    // _mcT->Branch( ("uredupara"+_uncNames[iu]).c_str() ,&_itt.urpa_mc[ _uncNames[iu] ]);
    // _mcT->Branch( ("umetx"+_uncNames[iu]).c_str() ,&_itt.umx_mc[ _uncNames[iu] ]);
    // _mcT->Branch( ("umety"+_uncNames[iu]).c_str() ,&_itt.umy_mc[ _uncNames[iu] ]);
    

  _mcT->Branch( ("upara"+_uncNames[iu]).c_str() ,&_itt.upa_mc[ _uncNames[iu] ]);
  _mcT->Branch( ("uperp"+_uncNames[iu]).c_str() ,&_itt.upe_mc[ _uncNames[iu] ]);
 _mcT->Branch( ("uredupara"+_uncNames[iu]).c_str() ,&_itt.urpa_mc[ _uncNames[iu] ]);
 _mcT->Branch( ("umetx"+_uncNames[iu]).c_str() ,&_itt.umx_mc[ _uncNames[iu] ]);
 _mcT->Branch( ("umety"+_uncNames[iu]).c_str() ,&_itt.umy_mc[ _uncNames[iu] ]);
    


  }

  _isPara=false;
  
}

METFunctions::~METFunctions() {

}



void METFunctions::fillRFTree(float qT, float upara, float uperp, float redupara,
			      int nVtx, float sumEt,float metx, float mety,
			      map<string, float> upa,
			      map<string, float> upe,map<string, float> urpa,
			      map<string, float> umx,map<string, float> umy,
			      float w, float wqt, bool isData) {

  if(!isData) { //MC
    
    _itt.qt_mc = qT;
    _itt.upara_mc = upara;
    _itt.uperp_mc = uperp;
    _itt.redupara_mc = upara+qT;
    _itt.metx_mc = metx;
    _itt.mety_mc = mety;
    _itt.nVtx_mc = nVtx;
    _itt.sumEt_mc = sumEt;
    _itt.w_mc = w;
    _itt.wqT_mc = wqt;

    for(size_t iu=0;iu<_uncNames.size();iu++) {
      _itt.upa_mc[ _uncNames[iu] ] = upa[ _uncNames[iu] ];

      // _itt.upa_mc[ _uncNames[iu] ] = (short)(upa[ _uncNames[iu] ]*10)
      // 	                           + ((short)(upe[ _uncNames[iu] ]*10))*65536;

      _itt.upe_mc[ _uncNames[iu] ] = upe[ _uncNames[iu] ];
      _itt.urpa_mc[ _uncNames[iu] ] = urpa[ _uncNames[iu] ];
      _itt.umx_mc[ _uncNames[iu] ] = umx[ _uncNames[iu] ];
      _itt.umy_mc[ _uncNames[iu] ] = umy[ _uncNames[iu] ];
    }
    
    _mcT->Fill();
  }
  else { //data
    
    _itt.qt_d = qT;
    _itt.upara_d = upara;
    _itt.uperp_d = uperp;
    _itt.redupara_d = upara+qT;
    _itt.metx_d = metx;
    _itt.mety_d = mety;
    _itt.nVtx_d = nVtx;
    _itt.sumEt_d = sumEt;
    _itt.w_d = wqt; //w
    
    // for(size_t iu=0;iu<_uncNames.size();iu++) {
      
    //   _itt.upa_d[ _uncNames[iu] ] = (short)(upa[ _uncNames[iu] ]*10)
    // 	                          + ((short)(upe[ _uncNames[iu] ]*10))*65536;
	
    //   _itt.upe_d[ _uncNames[iu] ] = upe[ _uncNames[iu] ];
    //   _itt.urpa_d[ _uncNames[iu] ] = urpa[ _uncNames[iu] ];
    //   _itt.umx_d[ _uncNames[iu] ] = umx[ _uncNames[iu] ];
    //   _itt.umy_d[ _uncNames[iu] ] = umy[ _uncNames[iu] ];
    // }
  
    _dataT->Fill();
  }

}


void METFunctions::configureBinning(vector<double> binQt, vector<double> binSEt/*, int* binNVtx*/){

  _binsQt = binQt;
  _binsSEt = binSEt;
  //_binsNVtx = binNVtx;

}

void METFunctions::configureMETTool(std::string comp, bool fNV, bool fSEt, bool fit, bool qTCut, bool unc){

  _comp = comp;
  _vsNPV = fNV;
  _vsSEt = fSEt;
  _fit = fit;
  _qTCut = qTCut;
  _unc = unc;

}

  
void METFunctions::computeResponse(){

}

void METFunctions::computeResolution(){

}

void METFunctions::computeResponse(bool unc, bool fit){
  reset();
  _resp=true;
  _vsNPV=false;
  _vsSEt=false;
  configureMETTool("para",0,0,fit,0,unc);

  compute();
  draw();
  
}

void METFunctions::computeResolution(std::string comp, bool fNV, bool fSEt, bool fit, bool qTCut, bool unc){
  reset();
  _resp=false;
  _isPara=(comp=="para");
  configureMETTool(comp,fNV,fSEt,fit,qTCut,unc);
  
  compute();
  draw();

}

void 
METFunctions::checkPoint(bool resp, std::string comp, float val, bool isData, bool fNV, bool fSEt, bool fit, bool qTCut) {

  _resp=resp;
  _isPara=(comp=="para");
  configureMETTool(comp,fNV,fSEt,fit,qTCut,true);
  
  singleFit(isData, val);

}


double METFunctions::FWHM(double sigma, double gamma){

  double f_g = 2*sigma*sqrt(2*log(2));
  double f_l = 2* gamma;

  return 0.5346*2*gamma+sqrt(0.2166*f_l*f_l + f_g*f_g);
}

double METFunctions::FWHMError( double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg){
  double ef_g =  2*esigma*sqrt(2*log(2));
  double ef_l = 2* egamma;
  
  double p1 = ef_l*ef_l*Vgg;
  double p2 = ef_g*ef_l*Vsg; //identical (should be)
  double p3 = ef_g*ef_l*Vgs;
  double p4 = ef_g*ef_g*Vss;
  
  return sqrt(p1 + p2 + p3 + p4);
}

void METFunctions::reset() {

  // delete _dataT;
  // delete _mcT;
  // delete _binsQt;
  // delete _binsSEt;
  // delete _binsNVtx;

  for(size_t i=0;i<_graphs.size();i++)
    delete _graphs[i];
  _graphs.clear();

 
  for(size_t i=0;i<_pads.size();i++)
    delete _pads[i];
  _pads.clear();
  delete _c;
}

void METFunctions::doRooFitJob(bool isData){
  
  bool debug_=false;

  TTree* ZData(0);
  if(isData)
    ZData = _dataT;
  else
    ZData = _mcT;

  // ZData->OptimizeBaskets();
  //ZData->Print();
  //cout<<isData<<" ZData "<<ZData<<"    "<<_dataT<<"   "<<_mcT<<endl;
  //cout<<ZData->GetEntries()<<"   "<<_dataT->GetEntries()<<"   "<<_mcT->GetEntries()<<endl;

  vector<double> bVar;
  int nBin;

  int offset=3;
  int NVMax=36;
  
  if(!_vsNPV) { //all vtx
    offset=0;
    NVMax=1;
    int nB = _binsQt.size();
    cout<<" number of bins "<<nB<<endl;
    bVar.resize(nB);//=new double[nB];
    nBin=nB-1;
    for(int i=0;i<nB;i++) {
      bVar[i]= _binsQt[i];
    }
  }
  
  if(_vsNPV) { //all pts
    nBin=1;
    bVar.push_back(0);
    bVar.push_back(10000);
  }
  
  if(_vsSEt) {
    offset=0;
    NVMax=1;
    //sumEt=true;
    int nB =_binsSEt.size();
    nBin=nB-1;
    bVar.resize(nB);//=new double[nB];
    for(int i=0;i<nB;i++)
      bVar[i]= _binsSEt[i];
  }
  
  vector<vector<float> > cqt(NVMax-offset,vector<float>(nBin,0));
  vector<vector<float> > cqte(NVMax-offset,vector<float>(nBin,0));
  vector<vector<float> > upx(NVMax-offset,vector<float>(nBin,0));
  vector<vector<float> > upxe(NVMax-offset,vector<float>(nBin,0));
  vector<vector<vector<float> > > upxu(NVMax-offset,vector<vector<float> >(nBin,vector<float>(2,0)));
  
  for(int Nvtx=offset;Nvtx<NVMax;Nvtx++) {

    float nVm = (!_vsNPV)?-0.1:(Nvtx - 0.1);
    float nVM = (!_vsNPV)?100.:(Nvtx+0.1);
    
    for(int iqt=0;iqt<nBin;iqt++) {
      cout<<" Start bin "<<iqt<<"   "<<bVar[iqt]<<"    "<<bVar[iqt+1]<<"   "<<Nvtx<<"   with sumET?"<<_vsSEt<<"   "<<_comp<<endl;

      float qtm = (_vsNPV && !_vsSEt)?0.:bVar[iqt];
      float qtM = (_vsNPV && !_vsSEt)?1000.:bVar[iqt+1];

      vector<float> vals=fit(ZData, isData, nVm, nVM, qtm, qtM );

      if(vals.size()==0) continue; //not enough entries to compute the point

      cqt[Nvtx-offset][iqt] = vals[0];
      cqte[Nvtx-offset][iqt] = vals[1];
	
      upx[Nvtx-offset][iqt] = vals[2];
      upxe[Nvtx-offset][iqt] = vals[3];

      if(_unc && !isData) {
	upxu[Nvtx-offset][iqt][0] = vals[4];
	upxu[Nvtx-offset][iqt][1] = vals[5];
      }	  

    }//qt
  }//Nvtx

  //filling output ==============
  _vals [ "centerQt" ]     = cqt;
  _vals [ "centerQtErr" ]  = cqte;
  _vals [ "upara" ]       = upx;
  _vals [ "uparaErr" ] = upxe;

  if(_unc && !isData) {
    _uncs = upxu;
  }

}

void
METFunctions::singleFit(bool isData, float val) {

  TTree* ZData(0);
  if(isData)
    ZData = _dataT;
  else
    ZData = _mcT;

  //bin finding===================
  float qtm, qtM, nVm, nVM;

  if(!_vsNPV) {
    nVm = -0.1;
    nVM = 100;

    if(_vsSEt) {
      for(size_t i=0;i<_binsSEt.size()-1;i++) {
	if(val<_binsSEt[i+1] && val>=_binsSEt[i]) {
	  qtm = _binsSEt[i];
	  qtM = _binsSEt[i+1];
	  break;
	}
      }
    }
    else {
      for(size_t i=0;i<_binsQt.size()-1;i++) {
	if(val<_binsQt[i+1] && val>=_binsQt[i]) {
	  qtm = _binsQt[i];
	  qtM = _binsQt[i+1];
	  break;
	}
      }
    }
  }
  else {
    qtm = 0;
    if(_qTCut) qtm=100;
    qtM = 1000.;
    nVm = val - 0.1;
    nVM = val + 0.1;
  }

  cout<<"Starting fit : [ "<<qtm<<" : "<<qtM<<" ] / [ "<<nVm<<" : "<<nVM<<" ]"<<endl;

  _detail=true; _unc=true;
  vector<float> vals=fit(ZData, isData, nVm, nVM, qtm, qtM );
  _detail=false; _unc=false;

  cout<<" ====== Fit Summary : "<<endl;
  cout<<" X = "<<vals[0]<<" +- "<<vals[1]<<endl;
  cout<<" Y = "<<vals[2]<<" +- "<<vals[3]<<" (+ "<<vals[4]<<" - "<<vals[5]<<" )"<<endl;

}



vector<float>
METFunctions::fit(TTree* ZData, bool isData, float nVm, float nVM, float qtm, float qtM) {
  
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);
  

  vector<float> vals;
  
  map<string, TH1* > uncHistosU;

  RooRealVar weight("weight","weight",-10,100);
  RooRealVar qTweight("qTweight","qTweight",-10,100);
  RooRealVar Vtxcat("nVtx","nVtx", nVm , nVM );

  string n=_vsSEt?"sumEt":"qT";
  RooRealVar qTcat(n.c_str(),n.c_str(),qtm,qtM);

  float um = -250; //250
  float uM = 250; //250

  int Nvtx=(int)nVM;

  double m,M;//,c;
  if(_comp=="para" && !_vsSEt)
    {
      //c = (bVar[iqt]+bVar[iqt+1])/2.;
      m = -600;//(_vsNPV || _vsSEt)? -800 : -500;
      M = 400;//(_vsNPV || _vsSEt)? 400  : uM;
    }
  else {
    //	c = 0;
    m = um; //FIXME 100 75
    M = uM;
  }
  
  string nv=_vsSEt?((_comp=="para")?"metx":"mety"):_comp;
  RooRealVar ucomp(nv.c_str(),"ucomp",m,M);
  RooRealVar redUpar( "redupara","redUPara", _resp?-1000:um,_resp?1000:uM );
  // RooRealVar metx("metx","metx",um, uM);
  // RooRealVar mety("mety","mety",um, uM);


  //the dataset ====================
  RooDataSet* Dpara;
  if(isData)
    { 
      if(!_qTCut)
	Dpara = new RooDataSet("data","Data",ZData, RooArgSet(Vtxcat,qTcat,ucomp,redUpar) );
      else
	Dpara = new RooDataSet("data","Data",ZData, RooArgSet(Vtxcat,qTcat,ucomp,weight,redUpar),"","weight" );
    }
  else {
    if(!_qTCut)
      Dpara = new RooDataSet("data","Data",ZData, RooArgSet(Vtxcat,qTcat,ucomp,weight,redUpar),"","weight" );
    else
      Dpara = new RooDataSet("data","Data",ZData, RooArgSet(Vtxcat,qTcat,ucomp,weight,redUpar,qTweight),"","qTweight" );
  }

  if(_unc && !isData) {
  
    for(size_t iu=0;iu<_uncNames.size();iu++) {
      // delete uncHistosU[ _uncNames[iu] ];
      // delete uncHistosU[ "central" ];
      RooRealVar uncU(("u"+nv+_uncNames[iu]).c_str(),"ucomp",m,M);//,m,M);
      RooRealVar uncRU(("uredupara"+_uncNames[iu]).c_str(),"redUpara", _resp?-1000:um,_resp?1000:uM );
      // string form=(_comp=="para")?"((@0%65536)<<16)/65536":"(@0/65536)";
      // cout<<form<<endl;
      //RooFormulaVar modUncU(("modu"+nv+_uncNames[iu]).c_str(),(form+"/10.").c_str(),RooArgSet(uncU));
      // RooFormulaVar uncRU(("moduredUpara"+_comp+_uncNames[iu]).c_str(),"@0/10.",RooArgSet(tmpUncU) );

      // RooRealVar uncMX(("umetx"+_uncNames[iu]).c_str(),"umetx",um,uM);
      // RooRealVar uncMY(("umety"+_uncNames[iu]).c_str(),"umety",um,uM);
     
      RooDataSet* uncDS = new RooDataSet("udata","uData",ZData, RooArgSet(Vtxcat,qTcat,uncU,weight,uncRU),"","weight" );
      // cout<<" >>TEST*********************************** "<<_comp<<" ==> "<<uncDS->numEntries()<<endl;
      // uncDS->addColumn(modUncU);
      
      // cout<<" <<TEST*********************************** "<<_comp<<" ==> "<<uncDS->numEntries()<<endl;

      if(_comp=="para" && !_resp && !_vsSEt) {
	  uncHistosU[ _uncNames[iu] ] = uncDS->createHistogram( ("uredupara"+_uncNames[iu]).c_str(), 100 );
	  uncHistosU[ "central" ] = Dpara->createHistogram( "redupara", 100 );
	}
	else {
	  uncHistosU[ _uncNames[iu] ] = uncDS->createHistogram( ("u"+nv+_uncNames[iu]).c_str(), 100 );
	  uncHistosU[ "central" ] = Dpara->createHistogram( (nv).c_str(), 100 );
	}
	
      delete uncDS;
    }//unc loop
  }

  RooRealVar* meanqT = (*Dpara).meanVar(qTcat);
  RooRealVar* rms = _resp?(*Dpara).rmsVar(ucomp):(*Dpara).rmsVar(redUpar);
  RooRealVar* mean = (*Dpara).meanVar(ucomp);
	
	
  double fwhm=-1, efwhm=-1;
  double peak=-1, epeak=-1;

  if(_fit) {
    double meU= 0;//-1* ( bVar[iqt+1] + bVar[iqt] )/2.;
    double mU = -50;//qt -100;
    double MU = 50;//qt +100;
    // double qt= -1* ( bVar[iqt+1] + bVar[iqt] )/2.;
    // double qtm = qt -100;
    // double qtM = qt +100;

    if(_comp=="perp") {
      meU =0;
      mU = -50; //-100 -20
      MU = 50; //100
    }

    if(_resp) {
      meU= -1* ( qtM + qtM )/2.;
      mU = -qtM-20;
      MU = -qtm+20;
    }

    if(Dpara->sumEntries() < 0.001 || Dpara->numEntries() ==0 ) 
      cout<< " No Entry for bin ["<<m<<":"<<M<<"] / ["<<((!_vsNPV)?0:Nvtx)<<":"<<((!_vsNPV)?100.:Nvtx)<<"]"<<endl;
      
    RooRealVar g_w("g_w","width Gaus", 10., 0., 30., "GeV"); //40
    RooRealVar gamma_Z0( "gamma_Z0", "Z0 width",2.3, 0, 30, "GeV" ); //10
    RooRealVar v_m("v_m","v_m",meU ,mU ,MU, "GeV" );
    RooVoigtian* voigt= new RooVoigtian("voigt","Voightian",ucomp, v_m, gamma_Z0, g_w);
    if(_comp=="para" && !_resp && !_vsSEt)
      voigt= new RooVoigtian("voigt","Voightian",redUpar, v_m, gamma_Z0, g_w);
    // if(_vsSEt && _comp=="para")
    //   voigt= new RooVoigtian("voigt","Voightian",metx, v_m, gamma_Z0, g_w);
    // if(_vsSEt && _comp=="perp")
    //   voigt= new RooVoigtian("voigt","Voightian",mety, v_m, gamma_Z0, g_w);

    RooFitResult* result;
    if(Dpara->numEntries() < 50000 ) { //reasonnable number of entries for an unbinned ift

      result = voigt->fitTo( (*Dpara) ,RooFit::SumW2Error(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(-1) );
    }
    else {
      RooDataHist* DparaHist;
      
      if(_comp=="para" && !_resp && !_vsSEt) 
	DparaHist=new RooDataHist("DPHist","DPHist",redUpar, (TH1*)(Dpara->createHistogram( "redupara", 375 ))->Clone() );
      else
	DparaHist=new RooDataHist("DPHist","DPHist",ucomp, (TH1*)(Dpara->createHistogram( nv.c_str(), 250 ))->Clone() );  
      
      
      result = voigt->fitTo( (*DparaHist) ,RooFit::SumW2Error(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(-1) );
      delete DparaHist;
    }

    //Get the FWHM
    double sigma = g_w.getVal();
    double gamma = gamma_Z0.getVal();
    double esigma = g_w.getError();
    double egamma = gamma_Z0.getError();

    double Vsg = result->correlation(g_w,gamma_Z0) ;
    double Vgs = result->correlation(gamma_Z0,g_w) ;
    double Vss = result->correlation(g_w,g_w) ;
    double Vgg = result->correlation(gamma_Z0,gamma_Z0) ;
	 
    fwhm = FWHM( sigma, gamma );
    efwhm = FWHMError( esigma, egamma,
		       Vss, Vsg, Vgs, Vgg);
    peak = v_m.getVal();
    epeak = v_m.getError();

    if(_detail) {
      cout<<" sigma -> "<<sigma<<" +- "<<esigma<<endl;
      cout<<" gamma -> "<<gamma<<" +- "<<egamma<<endl;
      cout<<" peak  -> "<<peak<<" +- "<<epeak<<endl;
    }

    cout<<" fwhm : "<<fwhm/(2.3548)<<" +- "<<efwhm/(2.3548)<<"  //  "<<peak<<" +- "<<epeak<<endl;
    delete voigt;
	 
  }
  //cout<<" sumEntries "<<(*Dpara).sumEntries()<<endl;
  if( (*Dpara).sumEntries()>20) {
	
    vals.push_back( _vsNPV?Nvtx:meanqT->getVal() );
    vals.push_back( _vsNPV?0.:meanqT->getError() );
  
    if(!_fit) {
      if(isData) {
	vals.push_back( _resp?mean->getVal():rms->getVal() );
	vals.push_back( _resp?mean->getError():rms->getError() );
      }
      else {
	vals.push_back( _resp?mean->getVal():rms->getVal() );
	vals.push_back( _resp?mean->getError():rms->getError() );
      }
      if(_detail)
	cout<<"========> "<<mean->getVal()<<"   "<<meanqT->getVal()<<endl;

      if(isnan(rms->getVal())) {
	TH1* ht(0);
	    
	if(_comp=="para" && !_resp)
	  ht = Dpara->createHistogram( "redupara",100);
	else
	  ht = Dpara->createHistogram( (nv).c_str(),100);
	      
	TH1* ht2 = Dpara->createHistogram("qT",100);
	//TFile* tr=new TFile("outputMETTool.root","RECREATE");
	// ht->Write();
	// ht2->Write();
	//tr->Close();
	      
	vals[0] = ht2->GetMean();
	vals[1] = ht2->GetMeanError();

	vals[2] = _resp?ht->GetMean():ht->GetRMS();
	vals[3] = _resp?ht->GetMeanError():ht->GetRMSError();
	delete ht;
	delete ht2;
      }
      if(_detail)
	cout<<" ---> "<<mean->getVal()<<"  "<<mean->getError()<<"  "<<rms->getVal()<<"  "<<Dpara->numEntries()<<"  "<<(*Dpara).sumEntries()<<"   "<<meanqT->getVal()<<endl;
    }
    else {
      vals.push_back( _resp?peak:fwhm/2.3548 );
      vals.push_back( _resp?epeak:efwhm/2.3548 );
    }
	  
    if(_unc && !isData) {
      vector<float> unc = computeUncertainties(uncHistosU);
      vals.push_back(unc[0]);
      vals.push_back(unc[1]);
    }	  
  
  }// number of event > 20
 
  delete Dpara;
  
  for(size_t iu=0;iu<_uncNames.size();iu++) {
    delete uncHistosU[ _uncNames[iu] ];
  }
  delete uncHistosU[ "central" ];
  
  return vals;
}


void METFunctions::convertVals(bool isData){

  int nBin = (_vals[ "centerQt" ])[0].size();
  if(_vsNPV)
    nBin = _vals[ "centerQt" ].size();

  TGraphAsymmErrors* og=new TGraphAsymmErrors();
  TGraphAsymmErrors* ogunc=new TGraphAsymmErrors();
  
  int n=0;
  for(int i=0;i<nBin;i++) {
    //cout<<i<<" !! "<<(_vals[ "upara" ]).size()<<"  "<< (_vals[ "upara" ])[0].size() <<endl;
    float x,y,ex,ey;
    if(!_vsNPV) {
      x = (_vals[ "centerQt" ])[0][i]/(_vsSEt?(1.):1.);
      y = (_vals[ "upara" ])[0][i]/(_resp?(-1*x):1.);
    
      ex = (_vals[ "centerQtErr" ])[0][i]/(_vsSEt?(1.):1.);
      ey = (_vals[ "uparaErr" ])[0][i]/(_resp?x:1.);
    }
    else {
      x = (_vals[ "centerQt" ])[i][0]/(_vsSEt?(1.):1.);
      y = (_vals[ "upara" ])[i][0]/(_resp?x:1.);
      
      ex = (_vals[ "centerQtErr" ])[i][0]/(_vsSEt?(1.):1.);
      ey = (_vals[ "uparaErr" ])[i][0]/(_resp?x:1.);
    }

    if(x!=0) {
      og->SetPoint(n, x, y );
      og->SetPointError(n, ex,ex,ey, ey );
      
      if(_unc && !isData) {
	float uyh,uyl;
	if(!_vsNPV) {
	  uyh = (_uncs)[0][i][0]/(_resp?x:1.);
	  uyl = (_uncs)[0][i][1]/(_resp?x:1.);
	  cout<<i<<"  "<<x<<"   "<<(_uncs)[0][i][0]<<"  ->  "<<uyh<<endl;
	}
	else {
	  uyh = (_uncs)[i][0][0]/(_resp?x:1.);
	  uyl = (_uncs)[i][0][1]/(_resp?x:1.);
	}
	ogunc->SetPoint(n,x,y);
	
	float exl= ex;//(bVar[i+1]-bVar[i])/2.;
	float exh= ex;//((i+1)!=NptBin)?((bVar[i+2]-bVar[i+1])/2.):(400);

	ogunc->SetPointError(n,exl,exh,uyl,uyh);
      }

      n++;
    }
  }

  if(isData) {
    og->SetLineColor(1);
    og->SetMarkerColor(1);
    og->SetMarkerStyle(20);
  }
  else {
    og->SetLineColor(kRed+1);
    og->SetMarkerColor(kRed+1);
    og->SetMarkerStyle(24);
  }
  
  if(isData)
    og->SetName("data");
  else
    og->SetName("mc");

  _graphs.push_back( og );
  if(_unc && !isData)  {
    ogunc->SetName("unc");
    _graphs.push_back( ogunc );
  }

}

void METFunctions::compute(){
  
  doRooFitJob(false);// MC first
  convertVals(false);// 

  _vals.clear();

  doRooFitJob(true);//data next
  convertVals(true);//
  
}

vector<float> METFunctions::computeUncertainties( map<string, TH1*> uncs ){
  
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR) ;
  map<string, float> vals;
  
  TFile* o=new TFile("gloubi.root","recreate");
  o->cd();

  map<string, TH1*>::iterator itVar; 
  for(itVar=uncs.begin();itVar!=uncs.end();itVar++) {
    
    if(_detail)
      itVar->second->Write();

    if(_fit) {
      float um=-40,uM=40;
      float m=0;
      if(_resp) {
	m = itVar->second->GetMean();
	um = itVar->second->GetMean() - itVar->second->GetRMS();
	uM = itVar->second->GetMean() + itVar->second->GetRMS();
      }

      RooRealVar x("x","x",0, _resp?-500:-200,200);
      RooDataHist Hist("Hist","Hist",x, (TH1*)itVar->second->Clone() );
      RooRealVar g_w("g_w","width Gaus", 10., 0., 100., "GeV"); //40
      RooRealVar gamma_Z0( "gamma_Z0_U", "Z0 width",2.3, 0, 100, "GeV" ); //20
      RooRealVar v_m("v_m","v_m",m, um , uM , "GeV" );
      RooVoigtian* voigt=new RooVoigtian("voigt","Voightian",x, v_m, gamma_Z0, g_w);
      
      RooFitResult* result = voigt->fitTo( (Hist) ,RooFit::SumW2Error(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(-1) );
      
      double sigma = g_w.getVal();
      double gamma = gamma_Z0.getVal();
      double fwhm = FWHM( sigma, gamma );
      double val=fwhm/2.3548;
      vals[ itVar->first ] = _resp?v_m.getVal():val;
      
      if(_detail)
	cout<<val<<"   "<<itVar->first<<"   "<<v_m.getVal()<<" /// "<<itVar->second->GetBinContent(0)<<"   "<<itVar->second->GetBinContent(10000)<<"   "<<itVar->second->GetXaxis()->GetBinCenter(0)<<endl;
    }
    else {
      
      if(_detail)
	cout<<itVar->second->GetName()<<"   "<<itVar->first<<" --> "<<itVar->second->GetMean()<<endl;
      vals[ itVar->first ] = _resp?itVar->second->GetMean():itVar->second->GetRMS();
    }
  } //all uncs
  
  if(_detail) {
    o->Write();
    o->Close();
  }

  float central = vals["central"];
  vector<float> totUnc(2,0);
  
  map<string, float>::iterator it2;
  for(map<string, float>::iterator it=vals.begin();
      it!=vals.end();it++) {
    
    if(it->first=="central") continue;
    
    if( (it->first.find("Up")==(size_t)-1 &&  
	 it->first.find("Do")==(size_t)-1 ) 
	//|| it->first=="JERDo" //fixme, this is ugly
	) {
      totUnc[0] += pow(it->second - central, 2);
      totUnc[1] += pow(it->second - central, 2);
      if(_detail)
	cout<<" sym "<<it->second <<"   "<<central<<endl;
    }
    else { //symetrization

      if(it->first.find("Up")!=(size_t)-1) {

	string name= it->first.substr( 0, it->first.size()-2 );
	it2 = vals.find( name+"Do");
	
	float sU=it->second - central;
	float sD=it2->second - central;

	if( sU*sD > 0) { //same sign errors
	  totUnc[0] +=sU>0?(sU>sD?(sU*sU):(sD*sD)):0;
	  totUnc[1] +=sU<=0?(sU<sD?(sU*sU):(sD*sD)):0;
	}
	else { //opposite sign errors
	  totUnc[0] +=sU>0?(sU*sU):(sD*sD);
	  totUnc[1] +=sU<=0?(sU*sU):(sD*sD);
	}
	if(_detail)
	  cout<<" asym "<<central <<"  "<<it->second<<"  "<<it2->second<<" --> "<<sU<<"  "<<sD<<endl;
	// totUnc[0] += sU; pow( (it->second - it2->second)/2., 2);
	// totUnc[1] += sD; pow( (it->second - it2->second)/2., 2);
      } 
    }
    if(_detail)
      cout<<it->first<<"   += ---> "<<sqrt(totUnc[0])<<"   "<<sqrt(totUnc[1])<<endl;

  }
  
  totUnc[0] = sqrt( totUnc[0] );
  totUnc[1] = sqrt( totUnc[1] );

  delete o;

  return totUnc;
}

void METFunctions::preparePad(){
  
  //Prepare pads
  double t = 35;
  double h = 535;
  double he = 5;
  double b = 70;
  double hl = 75;
  double H = t + h + b + hl + he;

  double w = 535;
  double g = 90;
  double d = 25;
  double W = w + g + d;
  
  _c=new TCanvas("c2","c2",W,H);
  TPad* pHigh = new TPad( "phigh", "phigh", 
			  0, (hl+b+he)/H , W/W, 1. );
  pHigh->SetLeftMargin(  g/W );
  pHigh->SetRightMargin( d/W );
  pHigh->SetTopMargin(  t/H );
  pHigh->SetBottomMargin( he/H );
  _c->cd();
  TPad* pLow = new TPad( "plow", "plow", 
			 0, 0 , W/W, (hl+b+he)/H );
  pLow->SetLeftMargin(  g/W );
  pLow->SetRightMargin( d/W );
  pLow->SetTopMargin(  he/H );
  pLow->SetBottomMargin( b/(hl+b+he) );
  
  _c->cd();
  pHigh->Draw();
  _c->cd();
  pLow->Draw();
  _c->cd();

  _pads.push_back(pHigh);
  _pads.push_back(pLow);
  
}


void METFunctions::draw() {


  preparePad();
  getRatios();
  
  //and now we draw everything
  _pads[0]->cd();
  TH1F* histo(0);
  if(!_vsNPV && !_vsSEt)
    histo = new TH1F("h","h",150,0,400);
  if(_vsNPV)
    histo = new TH1F("h","h",80,0,40);
  if(_vsSEt)
    histo = new TH1F("h","h",100,0,_binsSEt.back()/1. );

  histo->GetXaxis()->SetTitle("Z q_{T} [GeV] ");
  if(_vsNPV)
    histo->GetXaxis()->SetTitle("number of vertices ");
  if(_vsSEt) 
    histo->GetXaxis()->SetTitle("#sum E_{T} [TeV] ");

  histo->GetYaxis()->SetTitle("-<u_{||}>/q_{T}  ");
  if(_resp)
    histo->GetYaxis()->SetRangeUser(0.55,1.08);
  else {
    histo->GetYaxis()->SetRangeUser(3,38);

    string title="#sigma(u_{"+(string)(_isPara?"||}":"#perp}  ")+") [GeV]  ";
    histo->GetYaxis()->SetTitle( title.c_str() );
    histo->GetYaxis()->SetTitleOffset(1.04);
  }

  histo->Draw();
  
  if(_resp) {
    TLine* lineR=new TLine(histo->GetXaxis()->GetXmin(),1,histo->GetXaxis()->GetXmax(),1);
    lineR->SetLineColor(kBlue+1);
    lineR->SetLineWidth(2);
    lineR->SetLineStyle(2);
    lineR->Draw();
  }

  //uncertainty
  if(_unc) {
    _graphs[1]->SetMarkerStyle(1);
    _graphs[1]->SetMarkerColor(0);
    _graphs[1]->SetLineColor(0);
    _graphs[1]->SetFillColor(kGray+1);
    _graphs[1]->SetFillStyle(3001);
    _graphs[1]->Draw("P E3 same");
  }
  
  if(!_unc)
    for(size_t i=0;i<2;i++) {
      _graphs[i]->Draw("p");
    }
  else {
    _graphs[0]->Draw("p");
    _graphs[2]->Draw("p");
  }

  TLatex lat;
  lat.SetTextSize(0.04);
  lat.SetNDC();
  lat.DrawLatex(0.597,0.9653,"CMS Preliminary 2012");
  lat.DrawLatex(0.145,0.9598,"19.7 fb^{-1} at #sqrt{s} = 8 TeV");

  _pads[1]->cd();
  TH1F* histo2=(TH1F*)histo->Clone();
  histo2->GetYaxis()->SetRangeUser(0.7,1.3);
  if(_resp)
    histo2->GetYaxis()->SetRangeUser(0.88,1.12);
  histo2->GetYaxis()->SetNdivisions(4,5,0);
  histo2->GetYaxis()->SetLabelSize(0.17);
  histo2->GetYaxis()->SetTitle("Data/MC");
  histo2->GetYaxis()->SetTitleSize(0.19);
  histo2->GetYaxis()->SetTitleOffset(0.32);
  histo2->GetXaxis()->SetTickLength(0.07);
  histo2->GetXaxis()->SetLabelSize(0.19);
  histo2->GetXaxis()->SetTitleSize(0.21);
  histo2->GetXaxis()->SetTitleOffset(0.96);
  histo2->Draw();
  
  // and uncertainties
  if(_unc) {
    TGraphAsymmErrors* ratioUnc=new TGraphAsymmErrors();
    ratioUnc->SetName("ratioUnc");
    double x,y,exl,eyl,exh,eyh;
    double xd,yd;
    int n=0;
    //cout<<_graphs[2]->GetName()<<endl;
    for(int i=0;i<_graphs[1]->GetN();i++) {

      _graphs[1]->GetPoint(i,x,y);
      if(i<_graphs[0]->GetN()) {
	_graphs[0]->GetPoint(i,xd,yd);
      }
    
      exh = _graphs[1]->GetErrorXhigh(i);
      exl = _graphs[1]->GetErrorXlow(i);
      eyh = _graphs[1]->GetErrorYhigh(i);
      eyl = _graphs[1]->GetErrorYlow(i);
      //cout<<x<<"  "<<xd<<"   "<<exh<<"   "<<eyl<<"   "<<y<<endl;
      if( fabs(x-xd)<xd/10. || i==_graphs[1]->GetN()-1 ) {

	ratioUnc->SetPoint(n,x,1.);
	ratioUnc->SetPointError(n,exl,exh,eyl/y*yd/y,eyh/y*yd/y);
	n++;
      }
    }

    ratioUnc->SetMarkerStyle(1);
    ratioUnc->SetMarkerColor(0);
    ratioUnc->SetLineColor(0);
    ratioUnc->SetFillColor(kGray+2);
    ratioUnc->SetFillStyle(3002);
    ratioUnc->Draw("p E3 same");
  }  

  TLine* line=new TLine(histo2->GetXaxis()->GetXmin(),1,histo2->GetXaxis()->GetXmax(),1);
  line->SetLineColor(kGray+2);
  line->SetLineWidth(2);
  line->SetLineStyle(2);
  line->Draw();
  
  //the ratio
  cout<<" graph size "<<_graphs.size()<<endl;
  double x, y;
  for(int i=0;i<_graphs.back()->GetN();i++) {
    _graphs.back()->GetPoint(i,x,y);
    //cout<<x<<"  "<<y<<endl;
  }
  _graphs.back()->SetName("ratio");
  _graphs.back()->Draw("p");
 
  
  //and save the plots
  string base = "";
  if(_flav.find("MVA")!=(size_t)-1 || _flav.find("mva")!=(size_t)-1)
    base = "MVA";
  if(_flav.find("MVAU")!=(size_t)-1 || _flav.find("mvaU")!=(size_t)-1)
    base = "MVAUnity";
  if(_flav.find("NoPileUp")!=(size_t)-1 || _flav.find("noPU")!=(size_t)-1)
    base = "NoPU";
  if(_flav.find("Calo")!=(size_t)-1)
    base = "Calo";


  //legend first
  _pads[0]->cd();
  TLegend* leg(0);
  if(_resp)
    leg = new TLegend(0.45,0.30,0.82,0.57);
  else
    leg = new TLegend(0.18,0.60,0.55,0.87);
  
  leg->SetFillColor(0); leg->SetLineColor(0); leg->SetShadowColor(0);
  if(_unc)
    leg->AddEntry(_graphs[2], (base+"PF #slash{E}_{T} data").c_str(), "lp" );
  else
    leg->AddEntry(_graphs[1], (base+"PF #slash{E}_{T} data").c_str(), "lp" );
  leg->AddEntry(_graphs[0], (base+"PF #slash{E}_{T} simulation").c_str(), "lp");
  leg->Draw("same");
   
  if(_unc)
    leg->AddEntry(_graphs[1],"uncertainties","f");
  
  if(base=="MVA") base = "mva";
  if(base=="MVAUnity") base = "mvaU";
  if(base=="NoPU") base = "noPU";
  if(base=="Calo") base = "calo";
  if(base=="") base="pf";
  
  string type;
  if(_resp)
    type="responseVsQt";
  else {
    if(_vsNPV)
      type = _isPara?"rmsUparaVsNPV":"rmsUperpVsNPV";
    if(_vsSEt)
      type = _isPara?"rmsUparaVsSumEt":"rmsUperpVsSumEt";
    if(!_vsNPV && !_vsSEt)
      type = _isPara?"rmsUparaVsQt":"rmsUperpVsQt";
  }


  string name=base+type+(_qTCut?"_W":"")+(_fit?"_doFit":"");
  _c->SaveAs( ("Plots/"+name+".eps").c_str() );
  _c->SaveAs( ("Plots/"+name+".png").c_str() );
  _c->SaveAs( ("Plots/"+name+".pdf").c_str() );
  _c->SaveAs( ("Plots/"+name+".root").c_str() );
  
}



void METFunctions::getRatios() {

  size_t mc = 0;
  size_t d  = (_graphs.size()==2)?1:2;
  //  size_t u  = (_graphs.size()==2)?0:1;
  
  _graphs.push_back( getRatio(_graphs[d], _graphs[mc] ) );

}

TGraphAsymmErrors* METFunctions::getRatio(TGraphAsymmErrors* denom, TGraphAsymmErrors* num){

  //Advanced ratio, check of incompatibility between points

  TGraphAsymmErrors* g=new TGraphAsymmErrors();
  double xd,yd,eyd;
  double xm,ym,eymh, eyml;
  double tmp, tmp2;
  int n=0;
  for(int i=0;i<denom->GetN();i++) {

    denom->GetPoint(i,xd,yd);
    denom->GetPoint( (i+1!=denom->GetN())?i+1:i-1,tmp,tmp2);

    for(int j=0;j<denom->GetN();j++) {

      num->GetPoint(j,xm,ym);
      //cout<<xm<<"  "<<xd<<"   "<<fabs(xm-xd)<<"  "<<fabs(xd-tmp)<<endl;
      if( fabs(xm-xd)> fabs(xd-tmp)/10. ) continue; 
      //cout<<"    "<<i<<"   "<<j<<"   "<<xm<<"   "<<xd<<"  "<<yd<<"   "<<ym<<endl;

      eyd = denom->GetErrorYhigh(i);
      eymh = num->GetErrorYhigh(j);
      eyml = num->GetErrorYlow(j);

      if(ym!=0) {
	g->SetPoint(n, xd,yd/ym );
	g->SetPointError(n, 0,0, (yd/ym)*sqrt( pow(eyd/yd ,2) + pow(eymh/ym ,2) ),(yd/ym)*sqrt( pow(eyd/yd ,2) + pow(eyml/ym ,2) ));
	n++;
      }
    } //points numerator
  }//points denom

  g->SetLineColor( denom->GetLineColor() );
  g->SetMarkerColor( denom->GetMarkerColor() );
  g->SetMarkerStyle( denom->GetMarkerStyle() );

  return g;
}

