/****************************************************************
 ReachableFromStart.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_ReachableFromStart_H
#define INCL_ReachableFromStart_H
#include <iostream>
#include "SCFG.H"
using namespace std;
using namespace BOOM;


class ReachableFromStart {
  SCFG &G;
  GrammarSymbol *start;
  ProductionSet &productions;
  GrammarAlphabet &nonterminals;
  HashMap<GrammarSymbol,bool> *table;
  void init();
  bool iterate();
  bool addRhsToTable(Vector<GrammarSymbol*> &rhs,
		     HashMap<GrammarSymbol,bool> *table);
public:
  ReachableFromStart(SCFG &G);
  ~ReachableFromStart();
  bool operator[](GrammarSymbol *);
  void printOn(ostream &) const;
};

ostream &operator<<(ostream &,const ReachableFromStart &);

#endif

