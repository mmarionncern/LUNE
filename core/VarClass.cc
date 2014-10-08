#include "VarClass.hh"

using namespace std;


// string 
// VarClass::objectType[VarClass::kNTypes] {
//   "kInt",
//   "kUInt",
//   "kULong",
//   "kDouble",
//   "kFloat",
//   "kString",
//     "kBool",
//   // "kVInt",
//   // "kVUInt",
//   // "kVULong",
//   // "kVDouble",
//   // "kVFloat",
//   // "kVString",
//   //"kVBool"
// };




ClassImp(VarClass)

VarClass::VarClass() {
  _init=true;
   
  _su = new SystUtils();
}



void VarClass::reset() {

  varmVI.clear();
  varmVUI.clear();
  varmVUL.clear();
  varmVD.clear();
  varmVF.clear();
  varmVB.clear();
  varmVS.clear();
  
  varmI.clear();
  varmUI.clear();
  varmUL.clear();
  varmS.clear();
  varmB.clear();
  varmD.clear();
  varmF.clear();
  
  varmTB.clear();
  
  varTypes.clear();

  _varnames.clear();

  _init=true;


  uncmVI.clear();
  uncmVUI.clear();
  uncmVUL.clear();
  uncmVD.clear();
  uncmVF.clear();
  
  uncmI.clear();
  uncmUI.clear();
  uncmUL.clear();
  uncmD.clear();
  uncmF.clear();
  


}


VarClass::~VarClass() {
 

}

bool
VarClass::isUsefulVar(string name) {
  
  for(size_t i=0;i<_varnames.size();i++) {
    if(name == _varnames[i] )
      {return true;}
  }
  
  return false;

}

void
VarClass::initVar(string name) {
 
  for(size_t i=0;i<_varnames.size();i++) {
    if(name == _varnames[i] )
      {return;}
  }
  _varnames.push_back(name);
  return;
}

int
VarClass::getI(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }
  
  itI = varmI.find( name );
  if(itI == varmI.end() ) {
    itVI = varmVI.find( name );
    
    if(itVI == varmVI.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVI).second))[idx];
    }
  }
  else {
    return (*itI).second;
  }
  return 0;
}

unsigned int
VarClass::getUI(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }
  
  itUI = varmUI.find( name );
  if(itUI == varmUI.end() ) {
    itVUI = varmVUI.find( name );
    
    if(itVUI == varmVUI.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVUI).second))[idx];
    }
  }
  else {
    return (*itUI).second;
  }
  return 0;
}

unsigned long
VarClass::getUL(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }
  
  itUL = varmUL.find( name );
  if(itUL == varmUL.end() ) {
    itVUL = varmVUL.find( name );
    
    if(itVUL == varmVUL.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVUL).second))[idx];
    }
  }
  else {
    return (*itUL).second;
  }
  return 0;
}

bool
VarClass::getB(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }

  itB = varmB.find( name );
  if(itB == varmB.end() ) {
    itVB = varmVB.find( name );
    
    if(itVB == varmVB.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVB).second))[idx];
    }
  }
  else {
    return (*itB).second;
  }
  return 0;
}

double
VarClass::getD(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }
  
  itD = varmD.find( name );
  if(itD == varmD.end() ) {
    itVD = varmVD.find( name );
    
    if(itVD == varmVD.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVD).second))[idx];
    }
  }
  else {
    return (*itD).second;
  }
  return 0;
}

float
VarClass::getF(string name, int idx) {

  if(_init) {
    initVar(name);
    return 1;
  }
  // //cout<<" variable : "<<name<<" :  ";
  // itVt = varTypes.find( name );
  // if(itVt->second != VarClass::kFloat ) {
  //   cout<<" error, no such variable "<<name<<endl;
  // }
  // else {
  //   //cout<<varmVF[ name ]<<"   "<<varmVF[ name ]->size()<<endl;
  //   return (*varmVF[ name ])[ idx];
  // }
  
  // return 0;


  itF = varmF.find( name );
  if(itF == varmF.end() ) {
    itVF = varmVF.find( name );
    
    if(itVF == varmVF.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVF).second))[idx];
    }
  }
  else {
    return (*itF).second;
  }

  return 0;
}

string
VarClass::getS(string name, int idx) {

  if(_init) {
    initVar(name);
    return "";
  }
  
  itS = varmS.find( name );
  if(itS == varmS.end() ) {
    itVS = varmVS.find( name );
    
    if(itVS == varmVS.end() ) {
      cout<<" error, no such variable "<<name<<endl;
    }
    else {
      return (*((*itVS).second))[idx];
    }
  }
  else {
    return (*itS).second;
  }

  return "";

}

unsigned int
VarClass::getSize(string name) {

 if(_init) {
    initVar(name);
    return 1;
  }
  

  itVS = varmVS.find( name );
  if(itVS != varmVS.end() ) {
    return (*itVS).second->size();
  }
  itVD = varmVD.find( name );
  if(itVD != varmVD.end() ) {
    return (*itVD).second->size();
  }
  itVI = varmVI.find( name );
  if(itVI != varmVI.end() ) {
    return (*itVI).second->size();
  }
  itVF = varmVF.find( name );
  if(itVF != varmVF.end() ) {
    return (*itVF).second->size();
  }
  itVUI = varmVUI.find( name );
  if(itVUI != varmVUI.end() ) {
    return (*itVUI).second->size();
  }
  itVUL = varmVUL.find( name );
  if(itVUL != varmVUL.end() ) {
    return (*itVUL).second->size();
  }
  else{ 
    cout<<"Error for var "<<name<<endl;
    return  0;
  }
}


void 
VarClass::buildTree(TTree* tree, bool bypass) {

  TObjArray* branchs =  tree->GetListOfBranches();
  string name;
  
  EDataType t;
  TClass* cc;
  string type;
  
  for(int ib=0;ib<branchs->GetEntries();ib++) {
    name = (string)( ((*branchs)[ib])->GetName());
    ((TBranchSTL*)((*branchs)[ib]))->GetExpectedType(cc,t);
    if(t==-1)
      type = (string)(cc->GetName());
    
    //  cout<<name<<"   "<<type<<endl;

    //by default, status disabled
    if(!bypass)
      tree->SetBranchStatus( name.c_str() , 0);
    
    if( isUsefulVar(name )) {
       //Status enabled
      tree->SetBranchStatus( name.c_str() , 1);
      
      //Register branch
      registerBranch(tree, name, type, t );
      
    }
 
  }

}


void 
VarClass::registerBranch(TTree* tree, string name, string type, EDataType t) {

  //vector or container first
  if(t==-1) {

    if(type=="vector<int>") { //vector<int>

      if( varmVI.find(name) !=varmVI.end() ) {
	cout<<" Warning, "<<name<<" already registered"<<endl;
	return;
      }
      varmVI[ name ] =NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVI[ name ]) );

    }
    if(type=="vector<unsigned int>") { //vector<unsigned int>

      if( varmVUI.find(name) !=varmVUI.end() ) {
	cout<<" Warning, "<<name<<" already registered"<<endl;
	return;
      }
      varmVUI[ name ] =NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVUI[ name ]) );

    }
    if(type=="vector<unsigned long>") { //vector<unsigned int>

      if( varmVUL.find(name) !=varmVUL.end() ) {
	cout<<" Warning, "<<name<<" already registered"<<endl;
	return;
      }
      varmVUL[ name ] =NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVUL[ name ]) );

    }
    if(type=="vector<float>") { //vector<float>

      if( varmVF.find(name) !=varmVF.end() ) {
        cout<<" Warning, "<<name<<" already registered"<<endl;
        return;
      }
      varmVF[ name ] =NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVF[ name ]) );

     }
    // if(type=="vector<float>") { //vector<float>

    //   if( varTypes.find(name) != varTypes.end() ) {
    // 	cout<<" Warning, "<<name<<" already registered"<<endl;
    // 	return;
    //   }
    //   varTypes[ name ] = VarClass::kFloat;
    //   varmVF[ name ] =NULL;
    //   tree->SetBranchAddress( name.c_str() , &(varmVF[ name ]) );

    // }
    if(type=="vector<double>") { //vector<double
     
      if( varmVD.find(name) !=varmVD.end() ) {
	cout<<" Warning, "<<name<<" already registered"<<endl;
	return;
     }
     varmVD[ name ] =NULL;
     tree->SetBranchAddress( name.c_str() , &(varmVD[ name ]) );

    }
   if(type=="vector<bool>") { //vector<bool>
     
     if( varmVB.find(name) !=varmVB.end() ) {
       cout<<" Warning, "<<name<<" already registered"<<endl;
       return;
     }
     varmVB[ name ] =NULL;
     tree->SetBranchAddress( name.c_str() , &(varmVB[ name ]) );
     
    }
   if(type=="string") { //string
     
     if( varmS.find(name) !=varmS.end() ) {
       cout<<" Warning, "<<name<<" already registered"<<endl;
       return;
     }
     varmS[ name ] ="";
     tree->SetBranchAddress( name.c_str() , &(varmS[ name ]) );
     
   }
  if(type=="vector<string>") { //vector<string>
    
    if( varmVS.find(name) !=varmVS.end() ) {
       cout<<" Warning, "<<name<<" already registered"<<endl;
       return;
     }
    varmVS[ name ] =NULL;
    tree->SetBranchAddress( name.c_str() , &(varmVS[ name ]) );
     
   }
   if(type=="TBits") { //TBits
     
     if( varmTB.find(name) !=varmTB.end() ) {
       cout<<" Warning, "<<name<<" already registered"<<endl;
       return;
     }
     varmTB[ name ] =NULL;
     tree->SetBranchAddress( name.c_str() , &(varmTB[ name ]) );
   }

  }
  else if(t==3) { //int
    
    if( varmI.find(name) !=varmI.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmI[ name ] =0;
    tree->SetBranchAddress( name.c_str() , &(varmI[ name ]) );

  }
 else if(t==13) { //unsigned int
    
    if( varmUI.find(name) !=varmUI.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmUI[ name ] =0;
    tree->SetBranchAddress( name.c_str() , &(varmUI[ name ]) );

  }
 else if(t==14) { //unsigned long
    
   if( varmUL.find(name) !=varmUL.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmUL[ name ] =0;
    tree->SetBranchAddress( name.c_str() , &(varmUL[ name ]) );

  }
  else if(t==5) { //float
   
    if( varmF.find(name) !=varmF.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmF[ name ] =0.;
    tree->SetBranchAddress( name.c_str() , &(varmF[ name ]) );

  }
 // else if(t==5) { //float
   
 //   if( varTypes.find(name) != varTypes.end() ) {
 //     cout<<" Warning, "<<name<<" already registered"<<endl;
 //     return;
 //   }
  
 //   varTypes[ name ] = VarClass::kFloat;
 //   varmVF[ name ] =new vector<float>(1,0); 
 //   //cout<<" registered-> "<<name<<"  "<<varmVF[ name ]<<endl;
 //   tree->SetBranchAddress( name.c_str() , &((*varmVF[ name ])[0]) );

 // }
 else if(t==18) { //bool
   
    if( varmB.find(name) !=varmB.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmB[ name ] =0;
    tree->SetBranchAddress( name.c_str() , &(varmB[ name ]) );

  }
  else if(t==8) { //double

    if( varmD.find(name) !=varmD.end() ) {
      cout<<" Warning, "<<name<<" already registered"<<endl;
      return;
    }
    varmD[ name ] =0.;
    tree->SetBranchAddress( name.c_str() , &(varmD[ name ]) );

  }

}


bool
VarClass::tryType(string name, string type) {
  
  if(_init) {
    initVar(name);
    return 0;
  }
  
  if(type=="S") {
    itS = varmS.find( name );
    if(itS != varmS.end() ) {
      return true;
    }
    return false;
  }
  if(type=="B") {
    itB = varmB.find( name );
    if(itB != varmB.end() ) {
      return true;
    }
    return false;
  }
  if(type=="UI") {
    itUI = varmUI.find( name );
    if(itUI != varmUI.end() ) {
      return true;
    }
    return false;
   }
  if(type=="UL") {
    itUL = varmUL.find( name );
    if(itUL != varmUL.end() ) {
      return true;
    }
    return false;
  }
  if(type=="I") {
    itI = varmI.find( name );
    if(itI != varmI.end() ) {
      return true;
    }
    return false;
  }
  if(type=="D") {
    itD = varmD.find( name );
    if(itD != varmD.end() ) {
      return true;
    }
    return false;
  }
  if(type=="F") {
    itF = varmF.find( name );
    if(itF != varmF.end() ) {
      return true;
    }
    return false;
  }

  //Vectors

 if(type=="VS") {
    itVS = varmVS.find( name );
    if(itVS != varmVS.end() ) {
      return true;
    }
    return false;
  }
  if(type=="VB") {
    itVB = varmVB.find( name );
    if(itVB != varmVB.end() ) {
      return true;
    }
    return false;
  }
  if(type=="VUI") {
    itVUI = varmVUI.find( name );
    if(itVUI != varmVUI.end() ) {
      return true;
    }
    return false;
   }
 if(type=="VUL") {
    itVUL = varmVUL.find( name );
    if(itVUL != varmVUL.end() ) {
       return true;
    }
    return false;
   }
  if(type=="VI") {
    itVI = varmVI.find( name );
    if(itVI != varmVI.end() ) {
      return true;
    }
    return false;
  }
  if(type=="VD") {
    itVD = varmVD.find( name );
    if(itVD != varmVD.end() ) {
      return true;
    }
    return false;
  }
  if(type=="VF") {
    itVF = varmVF.find( name );
    if(itVF != varmVF.end() ) {
      return true;
    }
    return false;
  }

 
  { 
    cout<<"Error for var "<<name<<endl;
    return  0;
  }


}

vector<int>
VarClass::getVI(string name) {

  vector<int> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVI = varmVI.find( name );
    
  if(itVI == varmVI.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVI).second));
  }
  
  return tmp;
}

vector<unsigned int>
VarClass::getVUI(string name) {

 vector<unsigned int> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVUI = varmVUI.find( name );
    
  if(itVUI == varmVUI.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVUI).second));
  }
  
   return tmp;
}

vector<unsigned long>
VarClass::getVUL(string name) {

  vector<unsigned long> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVUL = varmVUL.find( name );
    
  if(itVUL == varmVUL.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVUL).second));
  }
  
   return tmp;
}

vector<bool>
VarClass::getVB(string name) {

  vector<bool> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVB = varmVB.find( name );
    
  if(itVB == varmVB.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVB).second));
  }
  
  return tmp;
}

vector<double>
VarClass::getVD(string name) {

  vector<double> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVD = varmVD.find( name );
    
  if(itVD == varmVD.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVD).second));
  }
  
  return tmp;
}

vector<float>
VarClass::getVF(string name) {

  vector<float> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVF = varmVF.find( name );
    
  if(itVF == varmVF.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVF).second));
  }
  
  return tmp;
}


vector<string>
VarClass::getVS(string name) {

  vector<string> tmp;

  if(_init) {
    initVar(name);
    return tmp;
  }
  
  itVS = varmVS.find( name );
    
  if(itVS == varmVS.end() ) {
    cout<<" error, no such variable "<<name<<endl;
  }
  else {
    return (*((*itVS).second));
  }
  
  return tmp;
}



//========================== Modification for systematic uncertainties ======


vector<float>
VarClass::getUnivF(string name) {
  
  //if( tryType(name,"S") )   return convertVal<string>(getS(name));
  //if( tryType(name,"B") )   return convertVal<bool>(getB(name));
  if( tryType(name,"UI") )  return convertVal<unsigned int>(getUI(name));
  if( tryType(name,"UL") )  return convertVal<unsigned long>(getUL(name));
  if( tryType(name,"I") )   return convertVal<int>(getI(name));
  if( tryType(name,"D") )   return convertVal<double>(getD(name));
  if( tryType(name,"F") )   return convertVal<float>(getF(name));
  //if( tryType(name,"VS") )  return convertVal<string>(getS(name));
  //if( tryType(name,"VB") )  return convertVal<bool>(getB(name));
  if( tryType(name,"VUI") ) return convertVal<unsigned int>(getUI(name));
  if( tryType(name,"VUL") ) return convertVal<unsigned long>(getUL(name));
  if( tryType(name,"VI") )  return convertVal<int>(getI(name));
  if( tryType(name,"VD") )  return convertVal<double>(getD(name));
  if( tryType(name,"VF") )  return convertVal<float>(getF(name));

  vector<float> dummy;
  return dummy;
}


// void
// VarClass::reinitVars( string mvar ) {
//   backPortVar(mvar);
// }

void
VarClass::applySystVar(string name, int dir, string mvar, float mag, string type) {

  backPortVar(mvar);

  //for(size_t iv=0;iv<mvars.size();iv++) {

    //if( tryType(mvar,"S") )    _su->systOp<string>(name, dir, type, varmS[mvar], mag);
    //if( tryType(mvar,"B") )    _su->systOp<bool>(name, dir, type, varmB[mvar], mag);
    if( tryType(mvar,"UI") )   _su->systOp<unsigned int>(name, dir, type, varmUI[mvar], mag);
    if( tryType(mvar,"UL") )   _su->systOp<unsigned long>(name, dir, type, varmUL[mvar], mag);
    if( tryType(mvar,"I") )    _su->systOp<int>(name, dir, type, varmI[mvar], mag);
    if( tryType(mvar,"D") )    _su->systOp<double>(name, dir, type, varmD[mvar], mag);
    if( tryType(mvar,"F") )    _su->systOp<float>(name, dir, type, varmF[mvar], mag);
    //if( tryType(mvar,"VS") )   _su->systOpV<string>(name, dir, type, varmVS[mvar], mag);
    //if( tryType(mvar,"VB") )   _su->systOpV<bool>(name, dir, type, varmVB[mvar], mag);
    if( tryType(mvar,"VUI") )  _su->systOpV<unsigned int>(name, dir, type, varmVUI[mvar], mag);
    if( tryType(mvar,"VUL") )  _su->systOpV<unsigned long>(name, dir, type, varmVUL[mvar], mag);
    if( tryType(mvar,"VI") )   _su->systOpV<int>(name, dir, type, varmVI[mvar], mag);
    if( tryType(mvar,"VD") )   _su->systOpV<double>(name, dir, type, varmVD[mvar], mag);
    if( tryType(mvar,"VF") )   _su->systOpV<float>(name, dir, type, varmVF[mvar], mag);

}




void
VarClass::applySystVar(string name, int dir, string mvar, vector<string> vars, string db, string type) {

  backPortVar(mvar);

  vector<vector<float> > vals; //order needed
  for(size_t iv=0;iv<vars.size();iv++) {
    vector<float> p = getUnivF( vars[iv] );
    vals.push_back(p);
  }
  
  //if( tryType(mvar,"S") )   _su->systOp<string>(name, dir, type, varmS[mvar], db, vals);
  //if( tryType(mvar,"B") )   _su->systOp<bool>(name, dir, type, varmB[mvar], db, vals);
  if( tryType(mvar,"UI") )  _su->systOp<unsigned int>(name, dir, type, varmUI[mvar], db, vals);
  if( tryType(mvar,"UL") )  _su->systOp<unsigned long>(name, dir, type, varmUL[mvar], db, vals);
  if( tryType(mvar,"I") )   _su->systOp<int>(name, dir, type, varmI[mvar], db, vals);
  if( tryType(mvar,"D") )   _su->systOp<double>(name, dir, type, varmD[mvar], db, vals);
  if( tryType(mvar,"F") )   _su->systOp<float>(name, dir, type, varmF[mvar], db, vals);
  //if( tryType(mvar,"VS") )  _su->systOpV<string>(name, dir, type, varmVS[mvar], db, vals);
  //if( tryType(mvar,"VB") )  _su->systOpV<bool>(name, dir, type, varmVB[mvar], db, vals);
  if( tryType(mvar,"VUI") ) _su->systOpV<unsigned int>(name, dir, type, varmVUI[mvar], db, vals);
  if( tryType(mvar,"VUL") ) _su->systOpV<unsigned long>(name, dir, type, varmVUL[mvar], db, vals);
  if( tryType(mvar,"VI") )  _su->systOpV<int>(name, dir, type, varmVI[mvar], db, vals);
  if( tryType(mvar,"VD") )  _su->systOpV<double>(name, dir, type, varmVD[mvar], db, vals);
  if( tryType(mvar,"VF") )  _su->systOpV<float>(name, dir, type, varmVF[mvar], db, vals);
  
}


// void
// VarClass::applyWSystVar(string name, int dir, float& w, vector<fliats> vars, string db, string type) {
  
//   vector<vector<float> > vals; //order needed
//   for(size_t iv=0;iv<vars.size();iv++) {
//     vector<float> p = getUnivF( vars[iv] );
//     vals.push_back(p);
//   }

//   _su->systOpW(name, dir, type , w, db, vals);
 
// }

void
VarClass::backPortVar(string mvar) {

  if( tryType(mvar,"UI") ) storeAccess<unsigned int>(mvar, varmUI, uncmUI );
  if( tryType(mvar,"UL") ) storeAccess<unsigned long>(mvar, varmUL, uncmUL );
  if( tryType(mvar,"I") )  storeAccess<int>(mvar, varmI, uncmI );
  if( tryType(mvar,"D") )  storeAccess<double>(mvar, varmD, uncmD );
  if( tryType(mvar,"F") )  storeAccess<float>(mvar, varmF, uncmF );

  if( tryType(mvar,"VUI") ) storeAccessV<unsigned int>(mvar, varmVUI, uncmVUI );
  if( tryType(mvar,"VUL") ) storeAccessV<unsigned long>(mvar, varmVUL, uncmVUL );
  if( tryType(mvar,"VI") )  storeAccessV<int>(mvar, varmVI, uncmVI );
  if( tryType(mvar,"VD") )  storeAccessV<double>(mvar, varmVD, uncmVD );
  if( tryType(mvar,"VF") )  storeAccessV<float>(mvar, varmVF, uncmVF );
  
}

void
VarClass::backPortAllVars() {
  
  multiReinit<unsigned int>( varmUI, uncmUI );
  multiReinit<unsigned long>( varmUL, uncmUL );
  multiReinit<int>( varmI, uncmI );
  multiReinit<double>( varmD, uncmD );
  multiReinit<float>( varmF, uncmF );

  multiReinitV<unsigned int>( varmVUI, uncmVUI );
  multiReinitV<unsigned long>( varmVUL, uncmVUL );
  multiReinitV<int>( varmVI, uncmVI );
  multiReinitV<double>( varmVD, uncmVD );
  multiReinitV<float>( varmVF, uncmVF );
  
}

string 
VarClass::getType(string mvar) {

  if( tryType(mvar,"UI") )  return "UI";
  if( tryType(mvar,"UL") )  return "UL";
  if( tryType(mvar,"I") )   return "I";
  if( tryType(mvar,"D") )   return "D";
  if( tryType(mvar,"F") )   return "F";
  
  if( tryType(mvar,"VUI") )  return "VUI";
  if( tryType(mvar,"VUL") )  return "VUL";
  if( tryType(mvar,"VI") )   return "VI";
  if( tryType(mvar,"VD") )   return "VD";
  if( tryType(mvar,"VF") ) return "VF";

  return "raté";
  
}
