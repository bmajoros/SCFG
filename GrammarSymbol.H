/****************************************************************
 GrammarSymbol.H
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#ifndef INCL_GrammarSymbol_H
#define INCL_GrammarSymbol_H
#include <iostream>
#include "BOOM/String.H"
using namespace std;
using namespace BOOM;

enum SymbolType {
  TERMINAL_SYMBOL,
  NONTERMINAL_SYMBOL
};

class GrammarSymbol {
public:
  GrammarSymbol(SymbolType,const BOOM::String &lexeme);
  GrammarSymbol();
  String &getLexeme();
  SymbolType &getType();
  bool isTerminal() const;
  bool isNonterminal() const;
  unsigned hash() const; // for use in BOOM::HashTable
  bool operator==(const GrammarSymbol &);
  void printOn(ostream &) const;
private:
  BOOM::String lexeme;
  SymbolType type;
};

ostream &operator<<(ostream &,const GrammarSymbol &);

#endif

