/****************************************************************
 SententialForm.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "SententialForm.H"
using namespace std;
using namespace BOOM;

SententialForm::SententialForm()
{
  // ctor
}



void SententialForm::append(GrammarSymbol *s)
{
  V.push_back(s);
}



int SententialForm::length()
{
  return V.size();
}



bool SententialForm::isFinal() const
{
  for(Vector<GrammarSymbol*>::const_iterator cur=V.begin(), end=V.end() ;
      cur!=end ; ++cur)
    if((*cur)->isNonterminal()) return false;
  return true;
}



void SententialForm::rewrite(int pos,Production *prod)
{
  V.cut(pos);
  V.insertByIndex(prod->getRHS(),pos);
}



void SententialForm::printOn(ostream &os) const
{
  for(Vector<GrammarSymbol*>::const_iterator cur=V.begin(), end=V.end() ;
      cur!=end ; ++cur)
    os<<(*cur)->getLexeme()<<" ";
}



ostream &operator<<(ostream &os,const SententialForm &f)
{
  f.printOn(os);
  return os;
}



int SententialForm::leftmostNonterminal() const
{
  int L=length();
  for(int i=0 ; i<L ; ++i)
    if(V[i]->isNonterminal()) return i;
  throw String("no nonterminals found in SententialForm");
}



GrammarSymbol *SententialForm::operator[](int i)
{
  return V[i];
}


