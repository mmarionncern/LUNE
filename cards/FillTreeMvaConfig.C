{
  if(Recompute) {
    MVAConfigAnalysis an;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string dir="test";  //53XABCDv3, v4 for noPU
  string tName="TT2Hadron/tree";
  string hName="";
 
  bool mcOnly = false;
  bool runFilter=false;
  int runNumber=139466;
 
  if(an.isInitStatus()) {
    an.anConf.configureNames( dir, tName, hName );
    an.anConf.configureData(runFilter, runNumber, mcOnly);
  }
  
  bool skim=false;
  an.noFullSkim();
  
  //an.launchTest(100000);

  //observables ********************** 
  //string obs[6]={"","","","","",""};
  an.dp.setObservables("MET");//STVsMassFinal
  
  //Binning & title ************************* Binning & titre
  string yTitle="number ofzjmlgljzlg events";
  int binning=1;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={-500.10.};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=true;
  bool underFlowBin=true;
  bool showDMCRatio=true;
  bool showGrid=true;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;
  bool stacking=true;

  bool cmsPrel=true;

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=false;
  string systSources="";

  bool uncDet=false;

  //Analyse parameters ****************** Analyze parameters
  bool InvCut=true;
  vector<string> Nm1Var; 
  Nm1Var.push_back( "" ); 

  string Norm="norm";
  
  //an.resoStudy("pfT1"); //mva, mvaU, noPU, Calo or pfT1
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=19712; //pb-1 19470

  bool useXS=true;

  map<string,float> LumisXS;
  //via XSect
 
  map<string,float> KFactors;
 
 
  if( an.isInitStatus() )
    an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  if( an.isInitStatus() ) {
    // an.anConf.addSample( "allLep/WJetsToEleNu",    "pfMET",    kBlack );
    // an.anConf.addSample( "allLep/WJetsToEleNu",   "mvaMETall",    kRed+1 );
    // an.anConf.addSample( "lep/WJetsToEleNu",      "mvaMET",    kOrange-3 );
    // an.anConf.addSample( "allLep/WJetsToEleNu",  "noPuMETall",    kBlue+1 );
    // an.anConf.addSample( "lep/WJetsToEleNu",     "noPuMET",    kViolet+3 );

    // an.anConf.addSample( "allLep/ZZ2El2Nu_v2",    "pfMET",    kBlack );
    // an.anConf.addSample( "allLep/ZZ2El2Nu_v2",   "mvaMETall",    kRed+1 );
    // an.anConf.addSample( "lep/ZZTo2El2Nu_v2",      "mvaMET",    kOrange-3 );
    // an.anConf.addSample( "allLep/ZZ2El2Nu_v2",  "noPuMETall",    kBlue+1 );
    // an.anConf.addSample( "lep/ZZTo2El2Nu_v2",     "noPuMET",    kViolet+3 );

    // an.anConf.addSample( "allLep/monoJet_Gstar_v2",    "pfMET",    kBlack );
    // an.anConf.addSample( "allLep/monoJet_Gstar_v2",   "mvaMETall",    kRed+1 );
    // an.anConf.addSample( "lep/monoJet_Gstar_v2",      "mvaMET",    kOrange-3 );
    // an.anConf.addSample( "allLep/monoJet_Gstar_v2",  "noPuMETall",    kBlue+1 );
    // an.anConf.addSample( "lep/monoJet_Gstar_v2",     "noPuMET",    kViolet+3 );

    an.anConf.addSample( "allLep/TTJetsHardron_v1",    "pfMET",    kBlack );
    an.anConf.addSample( "allLep/TTJetsHardron_v1",   "mvaMETall",    kRed+1 );
    //an.anConf.addSample( "lep/monoJet_Gstar_v2",      "mvaMET",    kOrange-3 );
    an.anConf.addSample( "allLep/TTJetsHardron_v1",  "noPuMETall",    kBlue+1 );
    //an.anConf.addSample( "lep/monoJet_Gstar_v2",     "noPuMET",    kViolet+3 );
  }
  //===============================================================

  //*********************************************************************²
  //Execution macro ******************************************************
 
  //Configuration ================
  if( an.isInitStatus() ) {
  
    an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );
   
    an.configureAnalysis(skim,InvCut, Nm1Var);

    //locking
    bool * rtmp= const_cast<bool*> pr2;
    *rtmp = false;
  }
 
  //plotting ================
  an.dp.setLumi( lumi );
  an.dp.setNormalization( Norm );
  an.dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
			 yDiv, binning, addBinBkg, overFlowBin,
			 underFlowBin, showDMCRatio, showGrid, stacking,
			 addSystematics, mcStatSyst, markerSize, lineWidth,
			 summedSignal, mcOnly,cmsPrel, uncDet);

  an.doAnalysis();

}
