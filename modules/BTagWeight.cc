#include "modules/BTagWeight.hh"

using namespace std; 

//ClassImp(BTagWeight)

bool BTagWeight::filter(std::vector<int> t){
  //  return t[0] >= 2; //If at least two b-tags are required
  return t[0] >= 1; // If at least one b-tag is required
  // return (t >= minTags && t <= maxTags);
}

float BTagWeight::weight(vector<vector<JetInfo> >jets){
  size_t njets=jets.size();
  std::vector<int> comb(jets.size());
  for(size_t i=0;i < jets.size(); i++) comb[i]=0;
  int idx=0;
  int max=taggers+1; //force sorted tagging //1 << taggers;
  float pMC=0;
  float pData=0;
  if(jets.size()==0) return 0.;
  while(comb[jets.size()-1] < max) {
    // std::cout << std::endl << "New comb" << std::endl;
    // for(int i=0;i < jets.size(); i++) {std::cout << comb[i] << " ";}
    
    ////// std::cout << std::endl;
    
    std::vector<int> tags;
    for(int j=0;j<taggers;j++) tags.push_back(0);
    
    float mc=1.;
    float data=1.;
    // loop on jets
    for(size_t j=0;j<njets;j++) {
      // std::cout << std::endl << "Jet" << j ;
      // if none tagged, take the 1-eff SF for the loosest:
      float tagMc = 1.-jets[j][0].eff;
      float tagData = 1.-jets[j][0].eff*jets[j][0].sf;
      //if at least one tagged take the SF for the tightest tagger
      if(comb[j]> 0) {
	int k=comb[j]-1;
	tagMc=jets[j][k].eff;
	tagData=jets[j][k].eff*jets[j][k].sf;
	
	//if at least one tagged take the SF for the tightest tagger
	if(comb[j]< taggers) {
	  int k1=comb[j];
	  tagMc*=1-jets[j][k1].eff/jets[j][k].eff;
	  tagData*=1-jets[j][k1].eff/jets[j][k].eff*jets[j][k1].sf/jets[j][k].sf;
	}
      }
      
      // loop on taggers
      for(int k=0;k< taggers; k++ ) {
	bool tagged = (comb[j] > k) ; ///((comb[j] >> k) & 0x1) == 1;
	  if(tagged) tags[k]++;
      }
      mc*=tagMc;       
      data*=tagData;       
    }
    
    if(filter(tags)){
      //  std::cout << mc << " " << data << " " << data/mc << " " << pData/pMC << endl;
      pMC+=mc;
      pData+=data;
      //n    std::cout << std::endl<< "mc, data,ratioThis,ratioTot " <<  mc << " " << data << " " << data/mc << " " << pData/pMC << endl;
    }
    while (comb[idx] == max -1  && idx+1 < jets.size()) idx++; // find first jets for which we did not already test all configs 
    // next combination
    comb[idx]++;  // test a new config for that jet
    for(int i=0;i<idx;i++) { comb[i]=0; } // reset the tested configs for all previous jets
    idx=0;
  }
  if(pMC==0) return 0; 
  return pData/pMC;
}
