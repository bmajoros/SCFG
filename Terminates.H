/****************************************************************
 Terminates.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Terminates_H
#define INCL_Terminates_H
#include <iostream>
#include "SCFG.H"
#include "DerivesEpsilon.H"
using namespace std;
using namespace BOOM;

class Terminates {
  SCFG &G;
  GrammarAlphabet &nonterminals;
  ProductionSet &productions;
  HashMap<GrammarSymbol,bool> *table;
  void init();
  bool iterate();
public:
  Terminates(SCFG &);
  ~Terminates();
  bool operator[](GrammarSymbol *);
  void dump();
};

#endif

