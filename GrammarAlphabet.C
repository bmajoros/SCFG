/****************************************************************
 GrammarAlphabet.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include "GrammarAlphabet.H"
using namespace std;
using namespace BOOM;

GrammarAlphabet::GrammarAlphabet()
{
  // ctor
}



void GrammarAlphabet::deleteAll()
{
  for(Vector<GrammarSymbol*>::iterator cur=symbols.begin(), end=symbols.end() ;
      cur!=end ; ++cur)
    delete *cur;
}



void GrammarAlphabet::addSymbol(GrammarSymbol *symbol)
{
  symbols.push_back(symbol);
}



int GrammarAlphabet::size() const
{
  return symbols.size();
}



GrammarSymbol *GrammarAlphabet::operator[](int i)
{
  return symbols[i];
}



GrammarSymbol *GrammarAlphabet::lookupLexeme(const BOOM::String &lexeme)
{
  if(symbolIndex.isDefined(lexeme)) return symbolIndex[lexeme];
  else return NULL;
}



void GrammarAlphabet::deleteIthEntry(int i)
{
  delete symbols[i];
  symbols[i]=NULL;
}



void GrammarAlphabet::compact()
{
  BOOM::Vector<GrammarSymbol*> A;
  for(BOOM::Vector<GrammarSymbol*>::iterator cur=symbols.begin(),
	end=symbols.end() ; cur!=end ; ++cur)
    if(*cur) A.push_back(*cur);
  symbols=A;
}


GrammarSymbol *GrammarAlphabet::findOrCreate(const String &lexeme,
					     SymbolType t)
{
  if(symbolIndex.isDefined(lexeme)) return symbolIndex[lexeme];
  GrammarSymbol *s=new GrammarSymbol(t,lexeme);
  symbols.push_back(s);
  symbolIndex[lexeme]=s;
  return s;
}



void GrammarAlphabet::printOn(ostream &os) const
{
  for(BOOM::Vector<GrammarSymbol*>::const_iterator cur=symbols.begin(),
	end=symbols.end() ; cur!=end ; ++cur)
    os<<(*cur)->getLexeme()<<endl;
}



ostream &operator<<(ostream &os,const GrammarAlphabet &alpha)
{
  alpha.printOn(os);
  return os;
}



