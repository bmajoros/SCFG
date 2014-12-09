/****************************************************************
 GrammarSymbol.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include "GrammarSymbol.H"
#include "BOOM/Constants.H"
using namespace std;
using namespace BOOM;


GrammarSymbol::GrammarSymbol(SymbolType t,const BOOM::String &lexeme)
  : type(t), lexeme(lexeme)
{
  // ctor
}



GrammarSymbol::GrammarSymbol()
{
  // ctor
}



BOOM::String &GrammarSymbol::getLexeme()
{
    return lexeme;
}



SymbolType &GrammarSymbol::getType()
{
    return type;
}



bool GrammarSymbol::isTerminal() const
{
    return type==TERMINAL_SYMBOL;
}



bool GrammarSymbol::isNonterminal() const
{
    return type==NONTERMINAL_SYMBOL;
}



unsigned GrammarSymbol::hash() const
{
  //unsigned u=reinterpret_cast<unsigned>(this);
  //return u%UINT_MAX;
  return lexeme.hash();
}



bool GrammarSymbol::operator==(const GrammarSymbol &other)
{
  //return this==&other;
  return type==other.type && lexeme==other.lexeme;
}



void GrammarSymbol::printOn(ostream &os) const
{
  os<<lexeme;
}



ostream &operator<<(ostream &os,const GrammarSymbol &S)
{
  S.printOn(os);
  return os;
}


