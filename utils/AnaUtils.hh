#ifndef _AnaUtils_
#define _AnaUtils_


#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <set>
#include <math.h>
#include <sstream>
#include <iomanip>

#include "TH1F.h"
#include "TCanvas.h"

#include "utils/StatUtils.hh"
#include "utils/SystUtils.hh"
#include "utils/Format.cc"

using namespace std;

struct EffST{

  float eff;

  //deprecated
  float systp;
  float systm;

  //dev
  map<string,float> systsU;
  map<string,float> systsD;

  float sumw;
  float sumw2;
  float N;

  float sumwTot;
  float sumw2Tot;
  float NTot;
  
};

typedef map<string, map<string,EffST> > eMap;
typedef map<string,EffST> eIMap;
typedef map<string, map<string,EffST> >::iterator itEMap;
typedef map<string,EffST>::iterator itEIMap;

class AnaUtils {

private:

  //Efficiencies
  eMap _effMap;
  itEMap _itEMap;
  itEIMap _itMap;
  
  //Acceptance
  bool _inAcc;
  bool _useAccForEff;

  bool _status;

  bool _invCut;
  bool _skipCut;
  set<string> _nm1Var;
  set<string> _bkgFC;
  
  //internal names...
  vector<string> _dsNames;
  vector<string> _effNames;

  string _uncSrc;
  int _uncDir;

public:
  
  AnaUtils();
  ~AnaUtils();

  void setUncSrc(string uncSrc, int dir) {
    _uncSrc=uncSrc;
    _uncDir=dir;
  };

  template < typename T > inline
  bool makeCut(T value, T valcut, string type, string ds, string cName, float w, T valUp, T valDo,T seccut=0) {

    bool mean = internalMakeCut<T>( value, valcut, type, ds, cName, w, seccut, false);

    bool up=mean,down=mean;
   
    up =  simpleCut<T>( valUp, valcut, type, seccut);
    down =  simpleCut<T>( valDo, valcut, type, seccut);
    
    setSystematics( ds, cName, _uncSrc, up, down, w);
    
    return mean;
  };

  template < typename T > inline
  bool makeCut( T value, T valcut, string type, string ds, string cName, float w, T seccut=0, bool noRegister=false) {
    return internalMakeCut<T>(value, valcut,type,ds,cName,w,seccut,noRegister);
  };

  bool makeCut( bool decision, string ds, string cName, float w,string type="=", bool noRegister=false) {
    return internalMakeCut<bool>( decision, true, type, ds, cName, w, 0, noRegister );
  };


  template < typename T > inline
  bool simpleCut( T value, T cut, string type, T seccut=0 ) {
    if(_status)
      return true;

    return internalMakeCut<T>( value, cut, type, "", "dummy", 0, seccut, true );
  };


  bool simpleCut( bool decision, string type="=") {
    if(_status)
      return true;

    return internalMakeCut<bool>( decision, true, type, "", "dummy", 0, 0, true );
  };

  
  std::string invCut(string s);
  bool cbool(bool skip, bool namevar);


  //Efficiencies and yields
  void setEfficiency(string ds, string cName, float value, bool acc);
  void setSystematics( string ds, string cName, string sName, bool up, bool down, float w);
  void getYieldSysts(string ds, string lvl);

  void getNumbers();
  vector< pair<string, vector<vector<float> > > > retrieveNumbers();
  //void drawNumbers();
  
  void changeInitStatus(bool status) {_status=status;};
  void setSkipCut(vector<string> var, bool invCut); 

  void setDDCuts(vector<string> ddCuts);

  void reset();

  static hObs cloneHObs(const hObs* o1);

private:

  template < typename T > inline
  bool internalMakeCut( T value, T valcut, string type, string ds, string cName, float w, T seccut=0, bool noRegister=false) {

    bool accept;
    
    if(!cbool(_skipCut, _nm1Var.find(cName)!=_nm1Var.end() ) )
      { return true; }

    if( !cbool( _invCut, _nm1Var.find(cName)!=_nm1Var.end() ) ) {
      type = invCut(type); }
 
    if(type=="<") {
      accept = (value < valcut);
    }
    else if(type=="<=") {
      accept = (value <= valcut);
    }
    else if( type==">") {
      accept = (value > valcut);
    }
    else if( type==">=") {
      accept = (value >= valcut);
    }
    else if( type=="=") {
      accept = (value == valcut);
    }
    else if(type=="!=") {
      accept = (value != valcut);
    }
    else if(type=="[]") {
      accept = (value >= valcut && value<= seccut );
    }
    else if(type=="][") {
      accept = (value > valcut && value< seccut );
    }
    else if(type=="[!]") {
      accept = !(value >= valcut && value <= seccut );
    }
    else if(type=="]![") {
      accept = !(value > valcut && value < seccut );
    }
    else {
      accept =false; cout<<" Warning cut "<<type<<endl;
    }  
    
    if(!noRegister || ds.find("GHO")!=(size_t)-1 ) {
      //deprecated to take into account uncertainties
      // setEfficiency(ds, cName, w, accept);
      // setSystematics( ds, cName,0,0, w);
 
      if(_uncSrc=="") {
	setEfficiency(ds, cName, w, accept);
	//setSystematics( ds, cName,_uncSrc,false,false, w);
      }
      else {
	setEfficiency(ds, cName, 0, accept);
	if(_uncDir==SystUtils::kUp)
	  setSystematics( ds, cName,_uncSrc,accept,false, w);
	else if(_uncDir==SystUtils::kDown)
	  setSystematics( ds, cName,_uncSrc,false,accept, w);
      } 
    }
    if(_status || _bkgFC.find(cName)!=_bkgFC.end() ) {
      accept=true;
    }
    return accept;
  };


  void setNumbers(string ds,string cName,float w, bool acc);

  ClassDef(AnaUtils,0)
  
};

#endif
