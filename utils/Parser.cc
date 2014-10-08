#include "utils/Parser.hh"

#include <algorithm>

using namespace std;

ClassImp(Parser)

Parser::Parser() {
}

Parser::Parser(string card) {
  _card =card;

  analyse="";
 
  dir="";
  tName="";
  hName="";

  mcOnly=false;
  runFilter=false;
  runNumber=1;

  skim=false;

  observables.resize(6);

  yTitle="";
  binning=1;
  addBinBkg=1;
  rangeY[0]=0;rangeY[1]=1;
  rangeX[0]=0;rangeX[1]=1;
  xDiv[0]=6; xDiv[1]=5; xDiv[2]=0;
  yDiv[0]=6; yDiv[1]=5; yDiv[2]=0;
  logYScale=false;
  overFlowBin=false;
  underFlowBin=false;
  showDMCRatio=false;
  addSystematics=false;
  markerSize=1.;
  lineWidth=1.;

  N1Cut=false;
  InvCut=false;
  Nm1Var="";
  
  Norm="";
  lumi=1.;
  
  useXS=true;
  
  _init=true;
  parseCard();
 
}

Parser::~Parser() {
}

void
Parser::configAnalysis(UseTree*& ut) {
  
  //cout<<" !!!!!!!!!!!!!!! "<<ut->isInitStatus()<<endl;

   if(!_init) parseCard(); 
    else _init=false;

  if(ut->isInitStatus() ) {
 
    //cout<<analyse<<endl;

    ut->anConf.configureNames( dir, tName, hName );
    ut->anConf.configureData(runFilter, runNumber, mcOnly);
    ut->anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

    for(size_t is=0;is<Samples.size();is++) {
      cout<<Samples[is].name<<endl;
      ut->anConf.addSample( Samples[is].name, Samples[is].ds, Samples[is].col );
    }
    
    ut->configureAnalysis(skim);

  }
  
  ut->dp.setObservables( observables[0], observables[1],
			 observables[2], observables[3],
			 observables[4], observables[5]);

  ut->dp.setLumi( lumi );
  ut->dp.setNormalization( Norm );
  ut->dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
			  yDiv, binning, addBinBkg, overFlowBin,
			  underFlowBin, showDMCRatio, 
			  addSystematics, markerSize, lineWidth,
			  ut->anConf.isNoDataAna() );


}


void
Parser::parseCard() {

  ifstream ifile(_card.c_str() , ios::in );
 
  if(ifile) {
  
    string line;
    while(getline(ifile, line))  
      { 

	if(line.substr(0,1)=="#") continue;//comment
	
	vector<string> fields;
	split( fields, line, "\t", split::no_empties );
	
	if(fields.size()==0) continue;
	
	//find the tag
	size_t p1 = fields[0].find("[");
	size_t p2 = fields[0].find("]");
	
	string tag = fields[0].substr(p1+1, p2-p1-1);

	//clean spaces at the end of the field 1 =========
	spaceCleaning( fields[1] );
	
	if(tag=="analyse") analyse=fields[1];
	if(tag=="dir") dir=fields[1];
	if(tag=="tName") tName=fields[1];
	if(tag=="hName") hName=fields[1];
	if(tag=="mcOnly") mcOnly=(bool)atoi(fields[1].c_str());
	if(tag=="runFilter") runFilter=(bool)atoi(fields[1].c_str());
	if(tag=="runNumber") runNumber=atoi(fields[1].c_str());
	if(tag=="skim") skim=(bool)atoi(fields[1].c_str());
	if(tag=="observables") {
	  for(int io=1;io<min(6,(int)fields.size());io++) {
	    observables[io-1] = fields[io];
	  }
	  for(int io=min(6,(int)fields.size());io<=6;io++) {
	    observables[io-1] = "";
	  }
	}
	if(tag=="yTitle") {
	  yTitle = fields[1];
	}
	if(tag=="binning") binning =atoi(fields[1].c_str()) ;
	if(tag=="addBinBkg") addBinBkg=atoi(fields[1].c_str());
	if(tag=="rangeY") {
	  rangeY[0] = atof(fields[1].c_str());
	  rangeY[1] = atof(fields[2].c_str());
	}
	if(tag=="rangeX") {
	  rangeX[0] =atof(fields[1].c_str());
	  rangeX[1] =atof(fields[2].c_str());
	}
	if(tag=="xDiv") {
	  xDiv[0] = atoi(fields[1].c_str());
	  xDiv[1] = atoi(fields[2].c_str());
	  xDiv[2] = atoi(fields[3].c_str());
	}
	if(tag=="yDiv") {
	  yDiv[0] = atoi(fields[1].c_str());
	  yDiv[1] = atoi(fields[2].c_str());
	  yDiv[2] = atoi(fields[3].c_str());
	}
	if(tag=="logYScale") logYScale=(bool)atoi(fields[1].c_str());
	if(tag=="overFlowBin") overFlowBin=(bool)atoi(fields[1].c_str());
	if(tag=="underFlowBin") underFlowBin=(bool)atoi(fields[1].c_str());
	if(tag=="showDMCRatio") showDMCRatio=(bool)atoi(fields[1].c_str());
	if(tag=="addSystematics") addSystematics=(bool)atoi(fields[1].c_str());
	if(tag=="markerSize") markerSize=atof(fields[1].c_str());
	if(tag=="lineWidth") lineWidth=atof(fields[1].c_str());
	if(tag=="N1Cut")  N1Cut=(bool)atoi(fields[1].c_str());
	if(tag=="InvCut") InvCut=(bool)atoi(fields[1].c_str());
	if(tag=="Nm1Var")  Nm1Var=fields[1];
	if(tag=="Norm") Norm=fields[1];
	if(tag=="lumi") lumi=atof(fields[1].c_str());
	if(tag=="useXS") useXS=(bool)atoi(fields[1].c_str());
	if(tag=="LumisXS") {
	  //cout<<"LS  "<<fields[1]<<"  "<<fields[2]<<endl;
	  LumisXS[ fields[1] ] = atof(fields[2].c_str() );
	}
	if(tag=="KFactors") {
	  //cout<<"KF  "<<fields[1]<<"  "<<fields[2]<<endl;
	  KFactors[ fields[1] ] = atof(fields[2].c_str() );
	}
	if(tag=="Samples") {
	  sam tmp;
	  tmp.name = fields[1];
	  tmp.ds = fields[2];
	  tmp.col = getColor( fields[3].c_str() );
	  Samples.push_back( tmp );
	}

	//cout<<fields[0]<<"  ----> "<<fields[1]<<endl;

      }//while

  }
  else {
    cout<<" Error, no card file called "<<_card<<endl; 
    abort();
  }

  //cout<<" (((( "<<getAnType()<<endl;

}


void
Parser::spaceCleaning(string& str) {

  string tmpStr;
  bool init=true;
  for(size_t i=0; i<str.size(); i++) {
    if(str.substr(i,1) != " " || !init) {
      tmpStr += str.substr(i,1);
      init=false;
    }
    if(str.substr(i,1)!=" " && i+3<str.size() && !init) {
      if(str.substr(i+1,1) == " " && str.substr(i+2,1) ==" ")
      init=true;
    }
    if(str.substr(i,1)!=" " && i+2==str.size() && !init) {
      if(str.substr(i+1,1) == " ")
	init=true;
    }
  }
  
  str=tmpStr;
}

int
Parser::getColor(string str) {

  int color=0;
  //default
  color =  atoi(str.c_str());

  size_t p;

  p= str.find("kWhite");
  if(p!=(size_t)-1) {
    color = 0;
    color += getOffset(str, 6);
  }
  p= str.find("kGray");
  if(p!=(size_t)-1) {
    color = 920;
    color += getOffset(str, 5);
  }
  p= str.find("kBlack");
  if(p!=(size_t)-1) {
    color = 1;
    color += getOffset(str, 6);
  }
  p= str.find("kRed");
  if(p!=(size_t)-1) {
    color = 632;
    color += getOffset(str, 4);
  }
  p= str.find("kPink");
  if(p!=(size_t)-1) {
    color = 900;
    color += getOffset(str, 5);
  }
  p= str.find("kMagenta");
  if(p!=(size_t)-1) {
    color = 616;
    color += getOffset(str, 8);
  }
  p= str.find("kViolet");
  if(p!=(size_t)-1) {
    color = 880;
    color += getOffset(str, 7);
  }
  p= str.find("kBlue");
  if(p!=(size_t)-1) {
    color = 600;
    color += getOffset(str, 5);
  }
  p= str.find("kAzure");
  if(p!=(size_t)-1) {
    color = 860;
    color += getOffset(str, 6);
  }
  p= str.find("kCyan");
  if(p!=(size_t)-1) {
    color = 432;
    color += getOffset(str, 5);
  }
  p= str.find("kTeal");
  if(p!=(size_t)-1) {
    color = 840;
    color += getOffset(str, 5);
  }
  p= str.find("kGreen");
  if(p!=(size_t)-1) {
    color = 416;
    color += getOffset(str, 6);
  }
  p= str.find("kSpring");
  if(p!=(size_t)-1) {
    color = 820;
    color += getOffset(str, 7);
  }
  p= str.find("kYellow");
  if(p!=(size_t)-1) {
    color = 400;
    color += getOffset(str, 7);
  }
  p= str.find("kOrange");
  if(p!=(size_t)-1) {
    color = 800;
    color += getOffset(str, 7);
  }
 
  return color;
}


int 
Parser::getOffset(string str,int off) {

  if((size_t)off==str.size()) return 0;

  string op=str.substr(off,1);
  int val = atoi( (str.substr(off+1,1)).c_str() );

  if(op=="+") return val;
  if(op=="-") return val*-1;

  return 0;
}
