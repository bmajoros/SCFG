/****************************************************************
 ReachableFromStart.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "ReachableFromStart.H"
using namespace std;
using namespace BOOM;

const int HASH_SIZE=499;

ReachableFromStart::ReachableFromStart(SCFG &G)
  : G(G), productions(*G.getProductionSet()), start(G.getStart()),
    nonterminals(G.getNonterminals())
{
  init();
  while(iterate());
}



ReachableFromStart::~ReachableFromStart()
{
  delete table;
  delete &productions;
}



bool ReachableFromStart::operator[](GrammarSymbol *s)
{
  return (*table)[*s];
}



void ReachableFromStart::init()
{
  table=new HashMap<GrammarSymbol,bool>(HASH_SIZE);
  const int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i) (*table)[*nonterminals[i]]=false;
  Vector<Production*> &prods=productions.lookup(start);
  for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
      cur!=end ; ++cur)
    addRhsToTable((*cur)->getRHS(),table);
  (*table)[*start]=true;
}



bool ReachableFromStart::addRhsToTable(Vector<GrammarSymbol*> &rhs,
				       HashMap<GrammarSymbol,bool> *table)
{
  bool changes=false;
  int L=rhs.size();
  for(int i=0 ; i<L ; ++i) {
    GrammarSymbol *s=rhs[i];
    if(s->isNonterminal())
      if(!(*table)[*s]) { (*table)[*s]=true; changes=true; }
  }
  return changes;
}


bool ReachableFromStart::iterate()
{
  bool changes=false;
  HashMap<GrammarSymbol,bool> *newTable=
    new HashMap<GrammarSymbol,bool>(HASH_SIZE);
  const int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i)
    (*newTable)[*nonterminals[i]]=(*table)[*nonterminals[i]];
  for(int i=0 ; i<numNT ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    if(!(*table)[*s]) continue;
    Vector<Production*> &prods=productions.lookup(s);
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
	cur!=end ; ++cur)
      if(addRhsToTable((*cur)->getRHS(),newTable)) changes=true;
  }
  delete table;
  table=newTable;
  return changes;
}



void ReachableFromStart::printOn(ostream &os) const
{
  const int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i)
    os<<*nonterminals[i]<<"\t"<<(*table)[*nonterminals[i]]<<endl;
}



ostream &operator<<(ostream &os,const ReachableFromStart &r)
{
  r.printOn(os);
  return os;
}




