/****************************************************************
 DerivesEpsilon.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_DerivesEpsilon_H
#define INCL_DerivesEpsilon_H
#include <iostream>
#include "SCFG.H"
#include "ProductionSet.H"
using namespace std;
using namespace BOOM;


class DerivesEpsilon {
  HashMap<GrammarSymbol,float> *table;
  SCFG &G;
  GrammarAlphabet &nonterminals;
  ProductionSet &productions;
  void init();
  float iterate();
public:
  DerivesEpsilon(SCFG &,float tolerance=1e-8);
  virtual ~DerivesEpsilon(); // deletes table!!!!
  float operator[](GrammarSymbol *);
  void dump();
};

#endif

