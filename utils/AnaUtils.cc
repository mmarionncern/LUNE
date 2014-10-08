#include "utils/AnaUtils.hh"

using namespace std;

ClassImp(AnaUtils)

AnaUtils::AnaUtils() {

  _status = 0;
 
  //MC initialization
  map<string,EffST> tmp;
  _effMap[ "MC" ] = tmp;
  _dsNames.push_back("MC");

  _invCut=false;
  _skipCut=false;
  
  _uncSrc="";

}

AnaUtils::~AnaUtils() {
}

bool AnaUtils::cbool(bool skip, bool bname) {

  if(!skip)
    return true;
  else{
    if(bname)
      return false;
    else return true;
  }
  
}

string 
AnaUtils::invCut(string type) {
  if(type=="<") {
    return ">=";
  }
  else if(type=="<=") {
    return ">";
  }
  else if( type==">") {
    return "<=";
  }
  else if( type==">=") {
    return "<";
  }
  else if( type=="=") {
    return "!=";
  }
  else if(type=="!=") {
    return "=";
  }
  else if(type=="[]") {
    return "]![";
  }
  else if(type=="][") {
    return "[!]";
  }
  else if(type=="[!]") {
    return "][";
  }
  else if(type=="]![") {
    return "[]";
  }
  else {
    cout<<" Warning cut "<<endl;
    return "=";
  }
} 



void AnaUtils::setEfficiency(string ds, string cName, float w, bool acc ) {

  if(_status) return;


  _itEMap=_effMap.find( ds );
  if(_itEMap==_effMap.end()) {
    // eIMap tmp;
    // _effMap[ ds ] = tmp;
    _dsNames.push_back(ds);
  }
  
  //  Acceptance
  if(_useAccForEff)
    if(!_inAcc) w=0;

  
  _itMap=_effMap[ ds ].find( cName );
   if(_itMap==_effMap[ ds ].end() ) {
    EffST tmp;
    tmp.eff = 0;
    tmp.systp = 0; tmp.systm = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    
    
    _effMap[ ds ][ cName ] = tmp;

    bool exists=false;
    for(size_t ic=0;ic<_effNames.size();ic++)
      if(cName == _effNames[ic]) exists=true;
    if(!exists)
      _effNames.push_back( cName );
  }
  
  if(_itMap==_effMap[ "MC" ].end() ) {
    EffST tmp;
    tmp.eff = 0;
    tmp.systp = 0; tmp.systm = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    

    _effMap[ "MC" ][ cName ] = tmp;
  }

  setNumbers(ds, cName, w, acc);


}


void AnaUtils::setSystematics(string ds, string cName, string sName, bool up, bool down, float w) {

  if(_status) return;

  bool accept[2] = {false,false};
  accept[0] = up;
  accept[1] = down;
  
  float val=w;

  // _effMap[ ds ][ cName ].systp+=(accept[0]?val:0);
  // _effMap[ ds ][ cName ].systm+=(accept[1]?val:0);

  _effMap[ ds ][ cName ].systsU[ sName ] +=(accept[0]?val:0);
  _effMap[ ds ][ cName ].systsD[ sName ] +=(accept[1]?val:0);
  
  if(ds!="data" && ds!="Data") {
    // _effMap[ "MC" ][ cName ].systp+=(accept[0]?val:0);
    // _effMap[ "MC" ][ cName ].systm+=(accept[1]?val:0);
    _effMap[ "MC" ][ cName ].systsU[ sName ] +=(accept[0]?val:0);
    _effMap[ "MC" ][ cName ].systsD[ sName ] +=(accept[1]?val:0);
  }

}

void
AnaUtils::getYieldSysts(string ds, string lvl) {

  EffST eST = _effMap[ ds ][ lvl ];


  float central = eST.sumw;
  
  float totUp=0,totDown=0;
  float dU,dD;
  
  map<string,float> rU;
  map<string,float> rD;

  for(map<string,float>::const_iterator it=eST.systsU.begin();
      it != eST.systsU.end(); it++ ) {
    
    string n =  (*it).first;
    dU = it->second - central; //temporary variation variation
    dD = eST.systsD[ n ] - central;
    
    if( dU*dD > 0) { //same sign errors
      rU[ n ] = dU>=0?(dU>dD?(dU*dU):(dD*dD)):0;
      rD[ n ] = dU<0?(dU<dD?(dU*dU):(dD*dD)):0;
      
      totUp += rU[ n ];
      totDown += rD[ n ];
    }
    else { //opposite sign errors
      rU[ n ] = dU>=0?(dU*dU):(dD*dD);
      rD[ n ] = dU<0?(dU*dU):(dD*dD);
      totUp +=rU[ n ];
      totDown +=rD[ n ];
    }

  }

  totUp = sqrt(totUp);
  totDown = sqrt(totDown);

  for(map<string,float>::const_iterator it= rU.begin();it!=rU.end();it++) {
    rU[ it->first ] = sqrt(rU[ it->first ]);
    rD[ it->first ] = sqrt(rD[ it->first ]);    
  }

  cout<<setprecision(2)<<fixed;
  cout<<" ************************************************************* "<<endl;
  cout<<" Detail of uncertainties on ("<<ds<<") yield, at the ("
      <<lvl<<") selection level "<<endl;

  

  cout<<setw(30)<<"   uncertainty source\t"<<"\t"<<" yield "<<endl;
  cout<<setw(30)<<"   statistical\t"<<"\t"<<central<<" +- "<<sqrt(eST.sumw2)
      <<" ( "<<sqrt(eST.sumw2)*100./central<<" %)"<<endl;
  
  for(map<string,float>::const_iterator it= rU.begin();it!=rU.end();it++) {
    string name="   "+it->first+"\t";
    if( fabs((it->second)+(rD[ it->first]) )>0.01*central ) { //asymetric
      cout<<setw(30)<<name<<"\t"<<" + "<<(it->second)*100/central
	  <<" - "<<rD[ it->first]*100/central<<" %   (+"
	  <<it->second<<" -"<<rD[ it->first]<<" events)"<<endl;
    }
    else { //symetric
      cout<<setw(30)<<name<<"\t"<<" +- "<<max((it->second),rD[ it->first])*100/central
	  <<" % (+"<<it->second<<" -"<<rD[ it->first]<<" events)"<<endl;
    }

  }
  cout<<"------------------------------------- "<<endl;
  if(fabs(totUp-totDown)>0.01*central) { //asymetric
    cout<<setw(30)<<"   total"<<"\t"<<" + "<<totUp*100/central
	<<" - "<<totDown*100/central<<" %   (+"
	<<totUp<<" -"<<totDown<<" events)"<<endl; 
  }
  else { //symetric
    cout<<setw(30)<<"   total systematics"<<"\t"<<" +- "<<max(totUp,totDown)*100/central
	<<" % (+"<<totUp<<" -"<<totDown<<" events)"<<endl;
  }


}

void AnaUtils::getNumbers() {
  
  //move MC at the end
  vector<string> dsNames;
  bool hasData = _dsNames.back()=="data" || _dsNames.back()=="Data";
  size_t nds=_dsNames.size();
  
  for(size_t ids=1;ids<(hasData?nds-1:nds);ids++) {
    dsNames.push_back(_dsNames[ids]);
  }
  dsNames.push_back(_dsNames[0]);
  if(hasData)
    dsNames.push_back(_dsNames.back());


  for(size_t ids=0;ids<dsNames.size();ids++) { //datasets

    //need last object for change of weights
    float tmpswtot=-1;
    float tmpNTot=-1;
    
    _itEMap = _effMap.find( dsNames[ ids ] );

    cout<<endl<< "Begin efficiencies for "<<_itEMap->first<<"   "<<_itEMap->second.size()<<"  ********* "<<endl;
    
    string initName;
    for(size_t ic=0;ic<_effNames.size();ic++) { //cut
    
      _itMap = _itEMap->second.find( _effNames[ ic ] );
      if(ic==0) initName = _effNames[ ic ];
      if(_itMap == _itEMap->second.end() ) continue;
    
      //get total value
      float eff = _itMap->second.sumw / _itMap->second.sumwTot;
      float error = StatUtils::BinomError( _itMap->second.NTot, eff);

      //get High and low values for systematics (if available)
      float effL = _itMap->second.systm / _itMap->second.sumwTot;
      float effH = _itMap->second.systp / _itMap->second.sumwTot;

      //fix for change of weight
      //cout<<setprecision(10)<< "----> "<<_itMap->second.sumwTot<<endl;
      if(_itMap->second.sumwTot != tmpswtot && tmpswtot!=-1 ) {
	//cout<<setprecision(10)<<" fix needed!!!! "<<_itMap->second.sumwTot<<"  "<<tmpswtot<<setprecision(3)<<endl;
	eff = _itMap->second.sumw / tmpswtot;
	if(eff>1) eff=1.;
	error = StatUtils::BinomError( tmpNTot, eff);
	effL = _itMap->second.systm /tmpswtot;
	effH = _itMap->second.systp /tmpswtot;
      }
         
      //FIXME -> if no systematic called, let remove them
      if( effL ==0 && effH==0 ) 
	effL = eff;
      if( effH ==0 && effH==0 )
	effH = eff;
      
      float systL = fabs(eff-effH); //inverted for the good way
      float systH = fabs(eff-effL);

      // cout<<systL<<"   "<<systH<<endl;

      //==========================================================
      //cout<<_itMap->second.sumw<<"   "<<_itEMap->second.begin()->second.sumwTot<<endl;
      float globEff = _itMap->second.sumw / _itEMap->second[ initName ].sumwTot;
      
      //FIXME ugly protection against a change of weight
      if(globEff>1) globEff=1;
      
      float globErr = StatUtils::BinomError( _itEMap->second[ initName ].sumwTot, globEff);      
      cout<<setprecision(3)<<fixed;
      cout<<" --> "<<setw(30)<<_itMap->first<<"\t  = "<<eff<<" +- "<<error;
      cout<<" (+ "<<systH<<" - "<<systL;
      cout<<") \t\t "<<_itMap->second.sumw<<"  / "<<((tmpswtot==-1)?_itMap->second.sumwTot:tmpswtot);
      cout<<"  ---> \t = "<<globEff*100<<" +- "<<globErr*100<<endl;
      
      tmpswtot =  _itMap->second.sumw;
      tmpNTot = _itMap->second.N;

    } //cuts

  } //datasets
  
  //Numbers =======================================================
  cout<<endl<<endl;
  //For Latex

  bool header=true;
  //start from one to skip the simulation in a first time
  for(size_t ic=0;ic<_effNames.size();ic++) { //cuts
    
    _itMap = _effMap[ "MC" ].find( _effNames[ ic ] );
    if(_itMap == _effMap[ "MC"].end() ) break; //out of the loop if no MC

    if(header) {
      cout<<" Cut  "<<fixed<<setprecision(2);
      for(size_t ids=0;ids<dsNames.size();ids++) { //_itEMap

	_itEMap = _effMap.find( dsNames[ ids ] );
	// if(_itEMap->first=="data" || _itEMap->first=="Data") 
	//   cout<<" & MC   "<<"  & "<<_itEMap->first<<"   ";
	// else
	  cout<<" & "<<_itEMap->first<<"   ";
      }
      cout<<" \\\\ "<<endl;
      header=false;
    }

    cout<<_itMap->first<<"    ";

    //start from one to skip the simulation summary
    for(size_t ids=0;ids<dsNames.size();ids++) { //datasets _itEMap
      _itEMap = _effMap.find( dsNames[ ids ] );
     
      cout<<" & ";
      
      map<string,EffST>::iterator itm;
     
	{ //simulation detail
	itm=_itEMap->second.find( _itMap->first );
	if(itm==_itEMap->second.end()) 
	  cout<<" - ";
	else {
	  if(itm->second.sumw>0.000001 ) {
	    cout<<itm->second.sumw;
	    cout<<" $\\pm$ "<<sqrt(itm->second.sumw2);
	  }
	  else
	    cout<<" - "; 
	}
	if(_itEMap->first=="data" || _itEMap->first=="Data" ||
	   (!hasData && _itEMap->first=="MC" ) )
	  cout<<" \\\\ "<<endl;
      }
      
    }//datasets
  }//cuts


  cout<<endl<<endl;
  

  //   //For Visu

  header=true;
  //to skip the simulation in a first time
   for(size_t ic=0;ic<_effNames.size();ic++) { //cuts
    
    _itMap = _effMap[ "MC" ].find( _effNames[ ic ] );
    if(_itMap == _effMap["MC"].end() ) break; //out of the loop if no MC

    if(header) {
      cout<<setw(15)<<" Cut                "<<fixed<<setprecision(2)<<"\t";
      for(size_t ids=0;ids<dsNames.size();ids++) {
	_itEMap = _effMap.find( dsNames[ ids ] );
	cout<<setw(20)<<_itEMap->first<<"\t";
      }
      cout<<endl;
      header=false;
    }

    cout<<setw(20)<<_itMap->first<<"\t";

    //to skip the simulation summary
    for(size_t ids=0;ids<dsNames.size();ids++) { //datasets
      
      _itEMap = _effMap.find( dsNames[ ids ] );
      map<string,EffST>::iterator itm;
      	{ //simulation detail
	itm=_itEMap->second.find( _itMap->first );
	if(itm==_itEMap->second.end()) 
	  cout<<setw(20)<<" - ";
	//cout<<" - ";
	else {
	  if(itm->second.sumw>0.000001 ) {
	    
	    ostringstream os,os2;
	    os <<fixed<<setprecision(2)<<itm->second.sumw;
	    os2 <<fixed<<setprecision(2)<<sqrt(itm->second.sumw2);

	    string tmps=os.str()+" +- "+os2.str();
	    cout<<setw(20)<<tmps;
	      // cout<<itm->second.sumw;
	    // cout<<" +- "<<sqrt(itm->second.sumw2);
	  }
	  else
	    cout<<" - "; 
	}
	if(_itEMap->first=="data" || _itEMap->first=="Data" ||
	   (!hasData && _itEMap->first=="MC" ) )
	  cout<<endl;
	else
	cout<<"\t";
      }
      
    }//datasets
  }//cuts


  cout<<endl<<endl;

}

vector< pair<string, vector<vector<float> > > >
AnaUtils::retrieveNumbers() {

  vector< pair<string, vector<vector<float> > > > onums;


 for(size_t ic=0;ic<_effNames.size();ic++) { //cuts
    
   _itMap = _effMap[ "MC" ].find( _effNames[ ic ] );
    
   pair<string, vector<vector<float> > > p;
   vector<vector<float> > v(_dsNames.size(),vector<float>(4,0));
   p.first = _itMap->first;
   p.second = v;

   //to skip the simulation summary
   for(size_t ids=0;ids<_dsNames.size();ids++) { //datasets
      
     _itEMap = _effMap.find( _dsNames[ ids ] );
     map<string,EffST>::iterator itm;
     { //simulation detail
       itm=_itEMap->second.find( _itMap->first );
       if(itm==_itEMap->second.end()) {
	 p.second[ids][0] = 0.; //no data for this point
       }
       else {
	 p.second[ids][0] = itm->second.sumw;
	 p.second[ids][1] = sqrt(itm->second.sumw2);
	 p.second[ids][2] = 0.;
	 p.second[ids][3] = 0.;
       }
     }
     
   }//datasets
   
   onums.push_back( p );
 }//cuts
 
 return onums;
}

void AnaUtils::setSkipCut(vector<string> var, bool invCut) {

  if(!invCut)
    _skipCut=(var.size()!=0);
  if(var.size()!=0)
    _invCut=invCut;
  
  for(size_t i=0;i<var.size();i++)
    _nm1Var.insert(var[i]);
}


void AnaUtils::setNumbers(string ds,string cName,float w, bool acc) {

  _effMap[ ds ][ cName ].NTot +=1;
  _effMap[ ds ][ cName ].sumwTot +=w;
  _effMap[ ds ][ cName ].sumw2Tot +=w*w;

  if(ds!="data" && ds!="Data" && ds.find("sig")==(size_t)-1) {
    _effMap[ "MC" ][ cName ].NTot +=1;
    _effMap[ "MC" ][ cName ].sumwTot +=w;
    _effMap[ "MC" ][ cName ].sumw2Tot +=w*w;
  }

  if(acc) {
    _effMap[ ds ][ cName ].N +=1;
    _effMap[ ds ][ cName ].sumw +=w;
    _effMap[ ds ][ cName ].sumw2 +=w*w;

    if(ds!="data" && ds!="Data" && ds.find("sig")==(size_t)-1) {
      _effMap[ "MC" ][ cName ].N +=1;
      _effMap[ "MC" ][ cName ].sumw +=w;
      _effMap[ "MC" ][ cName ].sumw2 +=w*w;
    }

  }

}


void 
AnaUtils::reset() {

  _effMap.clear();
  _dsNames.clear();
  _effNames.clear();
  
  //MC initialization
  map<string,EffST> tmp;
  _effMap[ "MC" ] = tmp;
  _dsNames.push_back("MC");

}


void
AnaUtils::setDDCuts(vector<string> ddcuts) {
  
  for(size_t i=0;i<ddcuts.size();i++) {
    _bkgFC.insert( ddcuts[i] );
  }

}



hObs 
AnaUtils::cloneHObs(const hObs* o1) {

  hObs o2;
  for(size_t ih=0;ih<o1->hs.size();ih++) {
    o2.hs.push_back((TH1*)o1->hs[ih]->Clone());
  } 
  
  o2.nBX = o1->nBX;
  o2.nBY = o1->nBY;
  o2.binsX = o1->binsX;
  o2.binsY = o1->binsY;
  
  o2.htype = o1->htype;
  o2.titleX = o1->titleX;
  o2.titleY = o1->titleY;
  o2.name = o1->name;
  o2.type = o1->type;
  
  return o2;
}


//====================================================
//====================================================
