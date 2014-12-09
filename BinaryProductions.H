/****************************************************************
 BinaryProductions.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_BinaryProductions_H
#define INCL_BinaryProductions_H
#include <iostream>
#include "BOOM/Alphabet.H"
#include "BOOM/Array1D.H"
#include "BOOM/Vector.H"
#include "BinaryProduction.H"
#include "SCFG.H"
using namespace std;
using namespace BOOM;

struct NonterminalProb {
  int nonterminal;
  float P;
  NonterminalProb() {}
  NonterminalProb(int nt,float p) : nonterminal(nt), P(p) {}
};

class BinaryProductions {
  Alphabet terminals;
  int numNonterminals;
  int startSymbol;
  Vector<BinaryProduction> productions; // A->BC
  Array1D< Vector<NonterminalProb> > terminalProbs;// indexed by terminal ID
  void initNonTermMap(HashMap<GrammarSymbol,int> &nonterminalMap,SCFG &);
  void processGrammar(SCFG &G,HashMap<GrammarSymbol,int> &nonterminalMap);
public:
  BinaryProductions();
  BinaryProductions(SCFG &,Alphabet &);
  void init(SCFG &,Alphabet &);
  int numProductions() const;
  int getNumNonterminals() const;
  int getStartSymbol() const;
  BinaryProduction &production(int i);
  Vector<NonterminalProb> &terminal(int terminalID);
  Vector<BinaryProduction>::const_iterator begin() const;
  Vector<BinaryProduction>::const_iterator end() const;
  Alphabet &getTerminals();
};

#endif

