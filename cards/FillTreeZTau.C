{
  if(Recompute) {
    ZTauAnalysis an;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string dir="LQ2012test"; //v2
  string tName="rootTupleTree/tree";
  string hName="rootTupleTree/nEventProc";
 
  bool mcOnly = true;
  bool runFilter=false;
  int runNumber=139466;
 
  if(an.isInitStatus()) {
    an.anConf.configureNames( dir, tName, hName );
    an.anConf.configureData(runFilter, runNumber, mcOnly);
  }
  
  bool skim=false;
  an.noFullSkim();

  an.launchTest(100000);

  //observables ********************** 
  //string obs[6]={"","","","","",""};
  an.dp.setObservables("PtVsEtaAccID");//STVsMassFinal
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=5;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,200000};
  double rangeX[2]={-10000.,1000.};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=true;
  bool underFlowBin=true;
  bool showDMCRatio=false;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;

  bool cmsPrel=true;

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=true;
  string systSources="";


  //Analyse parameters ****************** Analyze parameters
  bool InvCut=true;
  vector<string> Nm1Var; 
  Nm1Var.push_back( "" ); 

  string Norm="";
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=19470; //pb-1 19470

  bool useXS=true;

  map<string,float> LumisXS;
  //via XSect

  map<string,float> KFactors;
  
  if( an.isInitStatus() )
    an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  if( an.isInitStatus() ) {
    // an.anConf.addSample( "WW" ,                  "VV",     kRed+1 );
    // an.anConf.addSample( "WZ" ,                  "VV",     kRed+1 );
    // an.anConf.addSample( "ZZ" ,                  "VV",     kRed+1 );
    // an.anConf.addSample( "T_schan",        "single-t",  kOrange+7 );
    // an.anConf.addSample( "T_tchan",        "single-t",  kOrange+7 );
    // an.anConf.addSample( "T_tWchan",       "single-t",  kOrange+7 );
    // an.anConf.addSample( "Tbar_schan",     "single-t",  kOrange+7 );
    // an.anConf.addSample( "Tbar_tchan",     "single-t",  kOrange+7 );
    // an.anConf.addSample( "Tbar_tWchan",    "single-t",  kOrange+7 );
    //an.anConf.addSample( "test" ,     "t#bar{t}",  kOrange-2 );





    //an.anConf.addSample( "WJets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W1Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W2Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W3Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W4Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "ZJets_2l"  ,   "Z(ll)+jets",    kGray+1 );
    an.anConf.addSample( "test"  , "Z(#tau#tau)+jets",   kGray );
    // an.anConf.addSample( "DD opposite charge",  "QCD",  kViolet+1 );
    // an.anConf.addSample( "LQ600_tb"  , "LQ 600 GeV sig",   kBlue+1 );
    // an.anConf.addSample( "LQ1000_tb" ,   "LQ 1 TeV sig",  kGreen+2 );
    
    // an.anConf.addSample( "LQ400_tb" , "LQ 400 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ450_tb" , "LQ 450 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ500_tb" , "LQ 500 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ550_tb" , "LQ 550 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ600_tb" , "LQ 600 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ650_tb" , "LQ 650 GeV sig",    kGreen+2 );
    //an.anConf.addSample( "LQ700_tb" , "LQ 700 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ750_tb" , "LQ 750 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ800_tb" , "LQ 800 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ850_tb" , "LQ 850 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ900_tb" , "LQ 900 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ950_tb" , "LQ 950 GeV sig",    kGreen+2 );
    // an.anConf.addSample( "LQ1000_tb" , "LQ 1 TeV sig",    kGreen+2 );

    // an.anConf.addSample( "SingleEle_A_190456_196531_1"   , "data", 0 );
    // an.anConf.addSample( "SingleEle_A_r_190782_190949_1" , "data", 0 );
    // an.anConf.addSample( "SingleEle_B_190456_196531_1"   , "data", 0 );
    // an.anConf.addSample( "SingleEle_B_190456_196531_2"   , "data", 0 );
    // an.anConf.addSample( "SingleEle_C_1_198022_198523_1" , "data", 0 );
    // an.anConf.addSample( "SingleEle_C_2_190456_208686_1" , "data", 0 );
    // an.anConf.addSample( "SingleEle_C_2_190456_208686_2" , "data", 0 );
    // an.anConf.addSample( "SingleEle_C_2_190456_208686_3" , "data", 0 );
    // an.anConf.addSample( "SingleEle_D_1_190456_208686_1" , "data", 0 );
    // an.anConf.addSample( "SingleEle_D_1_190456_208686_2" , "data", 0 );
    // an.anConf.addSample( "SingleEle_D_1_190456_208686_3" , "data", 0 );
    


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
			underFlowBin, showDMCRatio, addSystematics,mcStatSyst,
			markerSize, lineWidth,summedSignal, mcOnly,cmsPrel);

 an.doAnalysis();

}
