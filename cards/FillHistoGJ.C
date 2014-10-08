{
  if(Recompute) {
    METAnalysis an;
  
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();

    an.isNonTreeAnalysis();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string file="~/Documents/CMS/METStudies/Brian/QCDSyst/PlotsMatthieu/Plots_pf/c_PhotonMET_pfuparPlusqT_qT100.root";
  
  string observable="hname";
  
  bool mcOnly = false;
  string Norm="";
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  string xTitle="u_{||} [GeV] ";//"#slash{E}_{T} [GeV] "; //PF #slash{E}_{T} [GeV] 
  int binning=0;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={5,10000000};
  double rangeX[2]={-200.,800};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=true;
  bool overFlowBin=true;
  bool underFlowBin=true;
  bool showDMCRatio=true;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;
  bool stacking=true;

  bool cmsPrel=true;

  float xt=0.22;//0.65 for MET, 0.61 for mva/noPu, 0.55 for mvaU 0.22 for recoil;
  float yt=0.59;//0.45 for MET, 0.59 for upara / 0.57 for uperp
  float st=0.033; //0.039, 0.036 for noPU/mva; recoil with gamma -> 0.033
  string addText="PF #slash{E}_{T}, q_{T}> 100 GeV";//"PF MVA Unity #slash{E}_{T}, q_{T}> 100 GeV";

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=true;
  string systSources="";

  bool uncDet=false;
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=19712; //pb-1 19470
  
  //===============================================================
  // Samples **************************  samples
  if( an.isInitStatus() ) {
    an.anConf.addSample( "EWK",                  "EWK", kOrange+7 );
    an.anConf.addSample( "DiG" ,        "#gamma#gamma", kMagenta+3 );
    an.anConf.addSample( "QCD" ,                 "QCD", kViolet-5 );
    an.anConf.addSample( "GamPlusJet",   "#gamma+jets", kOrange-2 );
    
    an.anConf.addSample( "Data"   , "data", 0 );

  }
 //===============================================================

 //*********************************************************************²
 //Execution macro ******************************************************
 
 //Configuration ================
 if( an.isInitStatus() ) {
   
   //locking
   bool * rtmp= const_cast<bool*> pr2;
   *rtmp = false;

 }
 
 an.dp.setObservables( observable );
 an.ah.configure( file, observable, xTitle);

 //plotting ================
 an.dp.setLumi( lumi );
 an.dp.setNormalization( Norm );
 an.dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
			yDiv, binning, addBinBkg, overFlowBin,
			underFlowBin, showDMCRatio, stacking, addSystematics,
			mcStatSyst, markerSize, lineWidth,summedSignal, 
			mcOnly,cmsPrel, uncDet);

 an.doAnalysis();

 an.dp.addText(xt,yt,st,addText);

}
