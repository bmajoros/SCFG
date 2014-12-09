/****************************************************************
 Production.H
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#ifndef INCL_Production_H
#define INCL_Production_H
#include <iostream>
#include "GrammarSymbol.H"
#include "BOOM/Vector.H"
using namespace std;
using namespace BOOM;

class Production {
public:
  Production();
  GrammarSymbol *getLHS() const;
  void setLHS(GrammarSymbol *);
  Vector<GrammarSymbol*> &getRHS();
  int rhsLength() const;
  float getProbability() const;
  void setProbability(float);
  bool isInCNF() const;
  void printOn(ostream &) const;
  bool isNull() const;
  bool isTerminal() const;
  bool isTerminalOrNull() const;
  bool containsAnyTerminals() const;
  bool containsAnyNonterminals() const;
  bool operator==(const Production &) const;
private:
  GrammarSymbol *lhs;
  Vector<GrammarSymbol*> rhs;
  float P;
};

ostream &operator<<(ostream &,const Production &);

#endif

