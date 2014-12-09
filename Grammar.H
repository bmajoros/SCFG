/****************************************************************
 Grammar.H
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#ifndef INCL_Grammar_H
#define INCL_Grammar_H
#include <iostream>
#include "GrammarAlphabet.H"
#include "Production.H"
#include "BOOM/Vector.H"
#include "BOOM/Map.H"
using namespace std;
using namespace BOOM;

class Grammar {
public:
  Grammar();
  void addProduction(Production *);
  int numProductions() const;
  Production *getIthProduction(int i);
  Vector<Production*> &getProductionsFor(GrammarSymbol *lhs);
  GrammarAlphabet &getTerminals();
  GrammarAlphabet &getNonterminals();
private:
  GrammarAlphabet terminals, nonterminals;
  Vector<Production*> productions;
  Map< GrammarSymbol* , Vector<Production*> > productionsByLHS;
};

#endif

