#include "src/AnalysisFactory.hh"

#include "src/TestAnalysis.hh"
#include "src/LQTopAnalysis.hh"

ClassImp(AnalysisFactory)

UseTree*
AnalysisFactory::get(string type) {

  if(type == "TestAnalysis") {
    return new TestAnalysis();
  }
  

  
  cout<<" Error, no such analysis "<<endl;
  abort();
  
  return 0;
}
