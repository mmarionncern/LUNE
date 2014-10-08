#include "core/AccessHisto.hh"

using namespace std;

ClassImp(AccessHisto)


AccessHisto::AccessHisto() {

  _filename="";
  _xtitle="";

}

AccessHisto::~AccessHisto() {



}

void
AccessHisto::configure(string filename, string var, string xtitle) {

  _filename=filename;
  _xtitle=xtitle;
  _var= var;

}

void
AccessHisto::access() {

  if(_filename=="") {
    cout<<" filename not specified, abort "<<endl;
    abort();
  }

  TFile* tmpFile=TFile::Open(_filename.c_str() );
  
  if(!tmpFile) {
    cout<<" no such file : "<<_filename<<" ; abort "<<endl;
    abort();
  }

  //scanning of the file
  TList* list= tmpFile->GetListOfKeys();
  if(list) {

    TIter iter(list);
    TObject *obj(0);
    TKey* key;

    while( (key=(TKey*)iter() ) ) {
      obj = (TObject*)key->ReadObj();
      string className = (string)obj->ClassName();
      if(className.substr(0,3)=="TH1") {
	_histos.push_back( (TH1*)obj );
      }

    }
  }

}


TH1*
AccessHisto::getHistogram(size_t chan) {
  return _histos[ chan ];
}

