/****************************************************************
 SCFG.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_SCFG_H
#define INCL_SCFG_H
#include <iostream>
#include "BOOM/Vector.H"
#include "GrammarAlphabet.H"
#include "Production.H"
#include "ProductionSet.H"
using namespace std;
using namespace BOOM;


class SCFG {
  GrammarAlphabet terminals, nonterminals;
  GrammarSymbol *start;
  Vector<Production*> productions;
public:
  SCFG();
  SCFG(const String &filename);
  void deleteAll();
  void load(const String &filename);
  GrammarAlphabet &getTerminals();
  GrammarAlphabet &getNonterminals();
  Vector<Production*> &getProductions();
  GrammarSymbol *getStart();
  void setStart(GrammarSymbol *);
  void printOn(ostream &) const;
  ProductionSet *getProductionSet();
  void normalize();
};

ostream &operator<<(ostream &,const SCFG &);

#endif

