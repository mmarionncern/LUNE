{
  if(Recompute) {
    TTEffAnalysis an;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

    if(unlock) 
      an.unlockStatus();
  }
  else an.refresh();
  
  //general parameters ********************* general parameters
  string dir="LQ2012/ttbar"; //etauv11_1
  string tName="tree";
  string hName="nEventProc";
 
  bool mcOnly = false;
  bool runFilter=false;
  int runNumber=139466;
 
  if(an.isInitStatus()) {
    an.anConf.configureNames( dir, tName, hName );
    an.anConf.configureData(runFilter, runNumber, mcOnly);
  }
  
  bool skim=false;
  an.noFullSkim();

  an.launchTest(200000);

  //observables ********************** 
  //string obs[6]={"","","","","",""};
  an.dp.setObservables("STbjet");//STVsMassFinal
  
  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=1;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={0,1100};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=true;
  bool underFlowBin=false;
  bool showDMCRatio=false;
  bool stacking=true;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;

  bool cmsPrel=true;

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=true;
  string systSources="";


  //Analyse parameters ****************** Analyze parameters
  bool InvCut=false;
  vector<string> Nm1Var; 
  Nm1Var.push_back( "" ); 

  string Norm="";
  //an.dp.setFitStr("eMETTMassPresel::VV-0:t#bar{t}-1:Z+jets-1:W+jets-1");
  
  //oscharge, lqd selection
  an.advOptions(true,false);

  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=19712; //pb-1 19712

  bool useXS=true;

  map<string,float> LumisXS;
  //via XSect
  LumisXS [ "ZJets_2l" ] = 3503.71;
  LumisXS [ "ZJets_2l_nojr" ] = 3503.71;
  LumisXS [ "Z1Jets_2l" ] = 666.3;
  LumisXS [ "Z2Jets_2l" ] = 214.97;
  LumisXS [ "Z3Jets_2l" ] = 60.69;
  LumisXS [ "Z4Jets_2l" ] = 27.36;
  LumisXS [ "WJets_ln" ] = 36257.2;
  LumisXS [ "W1Jets_ln" ] = 6440.58;
  LumisXS [ "W2Jets_ln" ] = 2087.225;
  LumisXS [ "W3Jets_ln" ] = 619.0113;
  LumisXS [ "W4Jets_ln" ] = 255.2378;
  LumisXS [ "TTbarJets" ] = 228.4; //234
  // LumisXS [ "TTbarJets_2l" ] = 26.1975; //234
  LumisXS [ "TTbarJets_2l" ] = 228.4*0.324*0.324; //234
  LumisXS [ "TTbarJets_1l" ] = 100.0;
  LumisXS [ "TTbarJets_had" ] = 104.3;
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
  KFactors [ "ZJets_2l" ] = 1.21;
  KFactors [ "ZJets_2l_nojr" ] = 1.21;
  KFactors [ "WJets_ln" ] = 0.86;
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
    
    an.anConf.addSample( "TTbarJets_2l_noFilter", "ttbar",    kRed+1 );
  
   


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
			underFlowBin, showDMCRatio,stacking, addSystematics,
			mcStatSyst,
			markerSize, lineWidth,summedSignal, mcOnly,cmsPrel);

 an.doAnalysis();

}
