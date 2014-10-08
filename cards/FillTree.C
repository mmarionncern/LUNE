{
  if(Recompute) {
    TestAnalysis an;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string dir="LQ2012test";
  string tName="rootTupleTree/tree";
  string hName="rootTupleTree/nEventProc";
 
  bool mcOnly = false;
  bool runFilter=false;
  int runNumber=139466;
 
  if(an.isInitStatus()) {
    an.anConf.configureNames( dir, tName, hName );
    an.anConf.configureData(runFilter, runNumber, mcOnly);
  }
  
  bool skim;

  //observables ********************** 
  //string obs[6]={"","","","","",""};
  an.dp.setObservables("pt_l1");
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=1;
  int addBinBkg=2; //BinB = binning*AddBin
  double rangeY[2]={0.1,200000};
  double rangeX[2]={0,200};
  int xDiv[3]={5,6,0};
  int yDiv[3]={5,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=false;
  bool underFlowBin=false;
  bool showDMCRatio=true;
  bool addSystematics=true;
  float markerSize=0.8;
  float lineWidth=1;

  bool cmsPrel=false;

  //Analyse parameters ****************** Analyze parameters
  bool N1Plot=false;
  bool InvCut=false;
  string Nm1Var = "";

  string Norm="dnorm";
  
 //Lumis( or XSections ) pb-1 & KFactors ************************************
 float lumi=11930; //pb-1

 bool useXS=true;

 map<string,float> LumisXS;
 //via XSect
 LumisXS [ "ZJets_2l" ] = 3503.71;
 LumisXS [ "WJets_ln" ] = 36257.2;
 LumisXS [ "TTbarJets" ] = 234;
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
 LumisXS [ "LQ1000_tb" ] = 1.;

 map<string,float> KFactors;
 KFactors [ "ZJets_2l" ] = 1.;

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
 KFactors [ "LQ1000_tb" ] = 1.;
 
 if( an.isInitStatus() )
   an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

 //===============================================================
 // Samples **************************  samples
 if( an.isInitStatus() ) {
   an.anConf.addSample( "LQ1000_tb" ,     "LQ1000",      kRed+1 );
   an.anConf.addSample( "LQ400_tb"  ,     "data",      kBlue+1 );
 }
 //===============================================================

 //*********************************************************************
 //Execution macro ******************************************************
 
 //Configuration ================
 if( an.isInitStatus() ) {
  
   //an.anConf.configureNames( dir, tName, hName );
   //an.anConf.configureData(runFilter, runNumber, mcOnly);
   an.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );
   
   an.configureAnalysis(skim);

   //locking
   bool * rtmp= const_cast<bool*> pr2;
   *rtmp = false;

 }
 
 //plotting ================
 an.dp.setLumi( lumi );
 an.dp.setNormalization( Norm );
 an.dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
			yDiv, binning, addBinBkg, overFlowBin,
			underFlowBin, showDMCRatio, addSystematics,
			markerSize, lineWidth, mcOnly,cmsPrel);

 an.doAnalysis();

}
