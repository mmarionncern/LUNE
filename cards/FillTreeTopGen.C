{
  if(Recompute) {
    TopGenAnalysis an;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string dir="LQ2012/v2"; //v2
  string tName="tree";
  string hName="nEventProc";
 
  bool mcOnly = true;
  bool runFilter=false;
  int runNumber=139466;
 
  if(an.isInitStatus()) {
    an.anConf.configureNames( dir, tName, hName );
    an.anConf.configureData(runFilter, runNumber, mcOnly);
  }
  
  bool skim;

  //observables ********************** 
  //string obs[6]={"","","","","",""};
  an.dp.setObservables("emuDPhi","etauDPhi","mutauDPhi");
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=16;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0.1,200000};
  double rangeX[2]={-1000,200};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=false;
  bool underFlowBin=false;
  bool showDMCRatio=true;
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
  LumisXS [ "ZJets_2l" ] = 3503.71;
  LumisXS [ "WJets_ln" ] = 36257.2;
  LumisXS [ "W1Jets_ln" ] = 6440.58;
  LumisXS [ "W2Jets_ln" ] = 2087.225;
  LumisXS [ "W3Jets_ln" ] = 619.0113;
  LumisXS [ "W4Jets_ln" ] = 255.2378;
  LumisXS [ "TTbarJets" ] = 228.4; //234
  LumisXS [ "T_tWchan" ] = 11.1;
  LumisXS [ "T_tchan" ] = 30.7;
  LumisXS [ "T_schan" ] = 1.76;
  LumisXS [ "Tbar_tWchan" ] = 11.1;
  LumisXS [ "Tbar_tchan" ] = 56.4;
  LumisXS [ "Tbar_schan" ] = 3.79;
  LumisXS [ "ZZ_4l" ] = 0.0843;
  LumisXS [ "WZ_3ln" ] = 1.05;
  LumisXS [ "WWJets_2l2n" ] = 5.998;
  LumisXS [ "WW" ] = 55.47;
  LumisXS [ "ZZ_2l2n" ] = 0.33;
  LumisXS [ "ZZ" ] = 8.27;
  LumisXS [ "WZ" ] = 33.59;

  LumisXS [ "LQ400_tb" ] = 0.342;
  LumisXS [ "LQ450_tb" ] = 0.163;
  LumisXS [ "LQ500_tb" ] = 0.082;
  LumisXS [ "LQ550_tb" ] = 0.0431;
  LumisXS [ "LQ600_tb" ] = 0.0235;
  LumisXS [ "LQ650_tb" ] = 0.0132;
  LumisXS [ "LQ700_tb" ] = 0.00761;
  LumisXS [ "LQ750_tb" ] = 0.00448;
  LumisXS [ "LQ800_tb" ] = 0.00269;
  LumisXS [ "LQ850_tb" ] = 0.00164;
  LumisXS [ "LQ900_tb" ] = 0.00101;
  LumisXS [ "LQ950_tb" ] = 0.000634;
  LumisXS [ "LQ1000_tb" ] = 0.000401;

  map<string,float> KFactors;
  KFactors [ "ZJets_2l" ] = 1.;
  KFactors [ "WJets_ln" ] = 1.;
  KFactors [ "W1Jets_ln" ] = 1.;
  KFactors [ "W2Jets_ln" ] = 1.;
  KFactors [ "W3Jets_ln" ] = 1.;
  KFactors [ "W4Jets_ln" ] = 1.;
  KFactors [ "TTbarJets" ] = 1.00;
  KFactors [ "TTbarJets" ] = 1.00;
  KFactors [ "T_tWchan" ] = 1.;
  KFactors [ "T_tchan" ] = 1.;
  KFactors [ "Tbar_tWchan" ] = 1.;
  KFactors [ "Tbar_tchan" ] = 1.;
  KFactors [ "ZZ_4l" ] = 1.;
  KFactors [ "WZ_3ln" ] = 1.;
  KFactors [ "WWJets_2l2n" ] = 1.;
  KFactors [ "ZZ_2l2n" ] = 1.;
  KFactors [ "ZZ" ] = 1.;
  KFactors [ "WZ" ] = 1.;

  KFactors [ "LQ400_tb" ] = 1.;
  KFactors [ "LQ450_tb" ] = 1.;
  KFactors [ "LQ500_tb" ] = 1.;
  KFactors [ "LQ550_tb" ] = 1.;
  KFactors [ "LQ600_tb" ] = 1.;
  KFactors [ "LQ650_tb" ] = 1.;
  KFactors [ "LQ700_tb" ] = 1.;
  KFactors [ "LQ750_tb" ] = 1.;
  KFactors [ "LQ800_tb" ] = 1.;
  KFactors [ "LQ850_tb" ] = 1.;
  KFactors [ "LQ900_tb" ] = 1.;
  KFactors [ "LQ950_tb" ] = 1.;
  KFactors [ "LQ1000_tb" ] = 1.;
 
  if( an.isInitStatus() )
    an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  if( an.isInitStatus() ) {
    an.anConf.addSample( "WW" ,                  "VV",     kRed+1 );
    an.anConf.addSample( "WZ" ,                  "VV",     kRed+1 );
    an.anConf.addSample( "ZZ" ,                  "VV",     kRed+1 );
    an.anConf.addSample( "T_schan",        "single-t",  kOrange+7 );
    an.anConf.addSample( "T_tchan",        "single-t",  kOrange+7 );
    an.anConf.addSample( "T_tWchan",       "single-t",  kOrange+7 );
    an.anConf.addSample( "Tbar_schan",     "single-t",  kOrange+7 );
    an.anConf.addSample( "Tbar_tchan",     "single-t",  kOrange+7 );
    an.anConf.addSample( "Tbar_tWchan",    "single-t",  kOrange+7 );
    an.anConf.addSample( "TTbarJets" ,     "t#bar{t}",  kOrange-2 );
    //an.anConf.addSample( "WJets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W1Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W2Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W3Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "W4Jets_ln"  ,       "W+jets",    kGray+2 );
    // an.anConf.addSample( "ZJets_2l"  ,   "Z(ll)+jets",    kGray+1 );
    // an.anConf.addSample( "ZJets_2l"  , "Z(#tau#tau)+jets",   kGray );
    // an.anConf.addSample( "DD opposite charge",  "QCD",  kViolet+1 );
    // an.anConf.addSample( "LQ600_tb"  , "LQ 600 GeV sig",   kBlue+1 );
    // an.anConf.addSample( "LQ1000_tb" ,   "LQ 1 TeV sig",  kGreen+2 );
    
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
