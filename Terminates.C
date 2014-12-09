/****************************************************************
 Terminates.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Terminates.H"
using namespace std;
using namespace BOOM;


const int HASH_SIZE=499;


Terminates::Terminates(SCFG &G)
  : G(G), nonterminals(G.getNonterminals()), 
    productions(*G.getProductionSet())
{
  init();
  while(iterate());
}



Terminates::~Terminates()
{
  delete table;
  delete &productions;
}



bool Terminates::operator[](GrammarSymbol *s)
{
  if(table->isDefined(*s)) return (*table)[*s];
  return false;
}



void Terminates::init()
{
  table=new HashMap<GrammarSymbol,bool>(HASH_SIZE);
  int n=nonterminals.size();
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    (*table)[*s]=false;
    Vector<Production*> &prods=productions.lookup(s);
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
	cur!=end ; ++cur)
      if((*cur)->isTerminalOrNull()) { (*table)[*s]=true; break; }
  }
}



bool Terminates::iterate()
{
  HashMap<GrammarSymbol,bool> *newTable=
    new HashMap<GrammarSymbol,bool>(HASH_SIZE);
  int n=nonterminals.size();
  bool changes=false;
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    if((*table)[*s]) { (*newTable)[*s]=true; continue; }
    (*newTable)[*s]=false;
    Vector<Production*> &prods=productions.lookup(s);
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
	cur!=end ; ++cur) {
      Production *prod=*cur;
      Vector<GrammarSymbol*> &rhs=prod->getRHS();
      bool thisProd=true;
      for(Vector<GrammarSymbol*>::iterator cur=rhs.begin(), end=rhs.end() ;
	  cur!=end ; ++cur) {
	GrammarSymbol *t=*cur;
	if(t->isNonterminal() && !(*table)[*t]) thisProd=false;
      }
      if(thisProd) { (*newTable)[*s]=true; changes=true; break; }
    }
  }
  delete table;
  table=newTable;
  return changes;
}



void Terminates::dump()
{
  int n=nonterminals.size();
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    cout<<s->getLexeme()<<" : "<<((*table)[*s]?"true":"false")<<endl;
  }
}




