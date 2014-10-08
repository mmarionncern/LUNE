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
  string file="~/Documents/CMS/METStudies/20140413_ZeeSpectra/zmass.root";
  
  string observable="hname";
  
  bool mcOnly = false;
  string Norm="dnorm";
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  string xTitle="M_{ee} [GeV] ";//"#slash{E}_{T} [GeV] "; //PF #slash{E}_{T} [GeV] 
  int binning=0;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={-200.,200};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=true;
  bool overFlowBin=true;
  bool underFlowBin=true;
  bool showDMCRatio=false;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;
  bool stacking=true;

  bool cmsPrel=false;

  float xt=0.23;//0.68 for MET, 0.23 for recoil 
  float yt=0.56;//0.48 for MET, 0.56 for recoil
  float st=0.039;
  string addText="";//PF #slash{E}_{T}

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=false;
  string systSources="";

  bool uncDet=false;
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=19712; //pb-1 19470
  
  //===============================================================
  // Samples **************************  samples
  if( an.isInitStatus() ) {
    an.anConf.addSample( "top",               "top",    kRed+1 );
    an.anConf.addSample( "vv" ,                "VV", kOrange+7 );
    an.anConf.addSample( "zee",   "Z#rightarrow ee", kOrange-2 );
    
    an.anConf.addSample( "data"   , "data", 0 );

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
