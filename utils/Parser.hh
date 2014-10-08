#ifndef __parser__
#define __parser__

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstddef>

#include "core/UseTree.hh"

using namespace std;

struct sam{
  string name;
  string ds;
  int col;

};

struct split
{
  enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& split(
  Container&                            result,
  const typename Container::value_type& s,
  const typename Container::value_type& delimiters,
  split::empties_t                      empties = split::empties_ok )
{
  result.clear();
  size_t current;
  size_t next = -1;
  do
  {
    if (empties == split::no_empties)
    {
      next = s.find_first_not_of( delimiters, next + 1 );
      if (next == Container::value_type::npos) break;
      next -= 1;
    }
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
  }
  while (next != Container::value_type::npos);
  return result;
}

class Parser {

public:

  Parser();
  Parser(string card);
  ~Parser();

  void parseCard();
  void configAnalysis(UseTree*& ut);

  string getAnType() {return analyse;};

private:

  void spaceCleaning(string& str);
  int getOffset(string str,int off);
  int getColor(string str);

  bool _init;

  string _card;

  string analyse;
  
  string dir;
  string tName;
  string hName;

  bool mcOnly;
  bool runFilter;
  int runNumber;

  bool skim;

  vector<string> observables;

  string yTitle;
  int binning;
  int addBinBkg;
  double rangeY[2];
  double rangeX[2];
  int xDiv[3];
  int yDiv[3];
  bool logYScale;
  bool overFlowBin;
  bool underFlowBin;
  bool showDMCRatio;
  bool addSystematics;
  float markerSize;
  float lineWidth;

  bool N1Cut;
  bool InvCut;
  string Nm1Var;
  
  string Norm;

  float lumi;
  
  bool useXS;

  map<string,float > LumisXS;
  map<string,float> KFactors;

  vector<sam> Samples;

  ClassDef(Parser,0)
};

#endif
