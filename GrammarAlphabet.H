/****************************************************************
 GrammarAlphabet.H
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#ifndef INCL_GrammarAlphabet_H
#define INCL_GrammarAlphabet_H
#include <iostream>
#include "BOOM/Map.H"
#include "BOOM/String.H"
#include "BOOM/Vector.H"
#include "GrammarSymbol.H"
using namespace std;
using namespace BOOM;

class GrammarAlphabet {
public:
  GrammarAlphabet();
  void deleteAll();
  void addSymbol(GrammarSymbol *);
  int size() const;
  GrammarSymbol *operator[](int i); // indices are not stable!
  GrammarSymbol *lookupLexeme(const BOOM::String &lexeme);
  GrammarSymbol *findOrCreate(const String &lexeme,SymbolType);
  void deleteIthEntry(int i);
  void compact(); // deletes unused slots; changes indices!
  void printOn(ostream &) const;
private:
  BOOM::Vector<GrammarSymbol*> symbols;
  Map<BOOM::String,GrammarSymbol*> symbolIndex; // lexeme -> GrammarSymbol
};

ostream &operator<<(ostream &,const GrammarAlphabet &);

#endif

