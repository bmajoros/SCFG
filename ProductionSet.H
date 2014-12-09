/****************************************************************
 ProductionSet.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_ProductionSet_H
#define INCL_ProductionSet_H
#include <iostream>
#include "BOOM/Vector.H"
#include "BOOM/HashTable.H"
#include "GrammarSymbol.H"
#include "GrammarAlphabet.H"
#include "Production.H"
using namespace std;
using namespace BOOM;

class ProductionSet {
  Vector<Production*> productions;
  HashMap<GrammarSymbol,Vector<Production*> > index;
public:
  ProductionSet();
  void add(Production *);
  void reindex();
  int size() const;
  Production *operator[](int);
  void removeProduction(int);
  Vector<Production*> &lookup(GrammarSymbol *);
  void printOn(ostream &) const;
  void normalize(GrammarAlphabet &);
};

ostream &operator<<(ostream &,const ProductionSet &);

#endif

