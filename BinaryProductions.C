/****************************************************************
 BinaryProductions.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BinaryProductions.H"
using namespace std;
using namespace BOOM;

const HASH_SIZE=499;


BinaryProductions::BinaryProductions()
{
  // ctor
}



BinaryProductions::BinaryProductions(SCFG &G,Alphabet &alpha)
{
  init(G,alpha);
}



void BinaryProductions::initNonTermMap(HashMap<GrammarSymbol,int> &
				       nonterminalMap,SCFG &G)
{
  GrammarAlphabet &nonterminals=G.getNonterminals();
  numNonterminals=nonterminals.size();
  for(int i=0 ; i<numNonterminals ; ++i)
    nonterminalMap[*nonterminals[i]]=i;
}



void BinaryProductions::init(SCFG &G,Alphabet &alpha)
{
  terminals=alpha;
  HashMap<GrammarSymbol,int> nonterminalMap(HASH_SIZE);
  initNonTermMap(nonterminalMap,G);
  startSymbol=nonterminalMap[*G.getStart()];
  processGrammar(G,nonterminalMap);
}



void BinaryProductions::processGrammar(SCFG &G,HashMap<GrammarSymbol,int> 
				       &nonterminalMap)
{
  const int numTerms=terminals.size();
  terminalProbs.resize(numTerms);
  Vector<Production*> &rawProds=G.getProductions();
  Vector<BinaryProduction> V;
  for(Vector<Production*>::iterator cur=rawProds.begin(), end=rawProds.end() ;
      cur!=end ; ++cur) {
    Production &rawProd=**cur;
    GrammarSymbol *lhs=rawProd.getLHS();
    const int lhsID=nonterminalMap[*lhs];
    Vector<GrammarSymbol*> &rhs=rawProd.getRHS();
    const float P=rawProd.getProbability();
    if(rhs.size()==1) {
      int term=terminals.lookup(rhs[0]->getLexeme()[0]);
      NonterminalProb ntProb(lhsID,P);
      terminalProbs[term].push_back(ntProb);
    }
    else {
      if(rhs.size()!=2) throw "Grammar is not in CNF";
      const int rhs1=nonterminalMap[*rhs[0]];
      const int rhs2=nonterminalMap[*rhs[1]];
      productions.push_back(BinaryProduction(lhsID,rhs1,rhs2,P));
    }
  }
}



int BinaryProductions::numProductions() const
{
  return productions.size();
}



BinaryProduction &BinaryProductions::production(int i)
{
  return productions[i];
}



Vector<NonterminalProb> &BinaryProductions::terminal(int terminalID)
{
  return terminalProbs[terminalID];
}



Vector<BinaryProduction>::const_iterator BinaryProductions::begin() const
{
  return productions.begin();
}



Vector<BinaryProduction>::const_iterator BinaryProductions::end() const
{
  return productions.end();
}



int BinaryProductions::getNumNonterminals() const
{
  return numNonterminals;
}



int BinaryProductions::getStartSymbol() const
{
  return startSymbol;
}



Alphabet &BinaryProductions::getTerminals()
{
  return terminals;
}


