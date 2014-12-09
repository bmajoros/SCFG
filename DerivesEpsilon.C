/****************************************************************
 DerivesEpsilon.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <math.h>
#include <iostream>
#include "DerivesEpsilon.H"
using namespace std;
using namespace BOOM;


const int HASH_SIZE=499;


DerivesEpsilon::DerivesEpsilon(SCFG &G,float tolerance)
  : table(NULL), G(G), nonterminals(G.getNonterminals()),
    productions(*G.getProductionSet())
{
  init();
  while(1) {
    float change=iterate();
    if(change<tolerance) break;
  }
}



DerivesEpsilon::~DerivesEpsilon()
{
  delete table;
  delete &productions;
}


void DerivesEpsilon::init()
{
  table=new HashMap<GrammarSymbol,float>(HASH_SIZE);
  int n=nonterminals.size();
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    if(!s) continue;
    (*table)[*s]=0;
    Vector<Production*> &prods=productions.lookup(s);
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
	cur!=end ; ++cur)
      if((*cur)->isNull()) (*table)[*s]+=(*cur)->getProbability();
  }
}



float DerivesEpsilon::iterate()
{
  HashMap<GrammarSymbol,float> *newTable=
    new HashMap<GrammarSymbol,float>(HASH_SIZE);
  int n=nonterminals.size();
  float maxDiff=0;
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    if(!s) continue;
    (*newTable)[*s]=0;
    Vector<Production*> &prods=productions.lookup(s);
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ;
	cur!=end ; ++cur) {
      Vector<GrammarSymbol*> &rhs=(*cur)->getRHS();
      float prodProb=(*cur)->getProbability();
      for(Vector<GrammarSymbol*>::iterator cur=rhs.begin(), end=rhs.end() ;
	  cur!=end ; ++cur)
	prodProb*=(*table)[**cur];
      (*newTable)[*s]+=prodProb;
    }
    float diff=fabs((*newTable)[*s]-(*table)[*s]);
    if(diff>maxDiff) maxDiff=diff;
  }
  delete table;
  table=newTable;
  return maxDiff;
}



float DerivesEpsilon::operator[](GrammarSymbol *s)
{
  if(table->isDefined(*s)) return (*table)[*s];
  return 0;
}



void DerivesEpsilon::dump()
{
  int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i)
    cout<<nonterminals[i]->getLexeme()<<"\t"<<(*table)[*nonterminals[i]]
  	<<endl;
}

