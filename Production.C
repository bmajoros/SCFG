/****************************************************************
 Production.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include "Production.H"
using namespace std;
using namespace BOOM;

Production::Production()
{
  // ctor
}



GrammarSymbol *Production::getLHS() const
{
  return lhs;
}



void Production::setLHS(GrammarSymbol *s)
{
  lhs=s;
}



Vector<GrammarSymbol*> &Production::getRHS()
{
  return rhs;
}



float Production::getProbability() const
{
  return P;
}



void Production::setProbability(float p)
{
  P=p;
}



int Production::rhsLength() const
{
  return rhs.size();
}



bool Production::isInCNF() const
{
  switch(rhs.size()) {
  case 1:
    return rhs[0]->isTerminal();
  case 2:
    return rhs[0]->isNonterminal() && rhs[1]->isNonterminal();
  default:
    return false;
  }
}




void Production::printOn(ostream &os) const
{
  os<<lhs->getLexeme()<<" -> ";
  const int n=rhsLength();
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=rhs[i];
    if(s->getType()==NONTERMINAL_SYMBOL) os<<s->getLexeme()<<" ";
    else os<<"'"<<s->getLexeme()<<"' ";
  }
  os<<"{ "<<P<<" }";
}



ostream &operator<<(ostream &os,const Production &prod)
{
  prod.printOn(os);
  return os;
}



bool Production::isNull() const
{
  return rhs.isEmpty();
}



bool Production::isTerminal() const
{
  if(isNull()) return false;
  for(Vector<GrammarSymbol*>::const_iterator cur=rhs.begin(), end=rhs.end() ;
      cur!=end ; ++cur) 
    if((*cur)->isNonterminal()) return false;
  return true;
}



bool Production::isTerminalOrNull() const
{
  for(Vector<GrammarSymbol*>::const_iterator cur=rhs.begin(), end=rhs.end() ;
      cur!=end ; ++cur) 
    if((*cur)->isNonterminal()) return false;
  return true;
}



bool Production::containsAnyTerminals() const
{
  for(Vector<GrammarSymbol*>::const_iterator cur=rhs.begin(), end=rhs.end() ;
      cur!=end ; ++cur) 
    if((*cur)->isTerminal()) return true;
  return false;
}



bool Production::containsAnyNonterminals() const
{
  for(Vector<GrammarSymbol*>::const_iterator cur=rhs.begin(), end=rhs.end() ;
      cur!=end ; ++cur) 
    if((*cur)->isNonterminal()) return true;
  return false;
}



bool Production::operator==(const Production &other) const
{
  if(lhs!=other.lhs) return false;
  if(P!=other.P) return false;
  if(rhs!=other.rhs) return false;
}


