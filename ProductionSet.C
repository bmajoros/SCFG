/****************************************************************
 ProductionSet.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "ProductionSet.H"
using namespace std;
using namespace BOOM;

ProductionSet::ProductionSet()
  : index(101)
{
  // ctor
}



void ProductionSet::add(Production *prod)
{
  productions.push_back(prod);
}



void ProductionSet::reindex()
{
  index.clear();
  for(Vector<Production*>::iterator cur=productions.begin(), 
	end=productions.end() ; cur!=end ; ++cur) {
    Production *prod=*cur;
    GrammarSymbol *lhs=prod->getLHS();
    lookup(lhs).push_back(prod);
  }
}



int ProductionSet::size() const
{
  return productions.size();
}



Production *ProductionSet::operator[](int i)
{
  return productions[i];
}



void ProductionSet::removeProduction(int i)
{
  productions.cut(i);
}



Vector<Production*> &ProductionSet::lookup(GrammarSymbol *s)
{
  if(!index.isDefined(*s)) index[*s]=Vector<Production*>();
  return index[*s];
}



void ProductionSet::printOn(ostream &os) const
{
  for(Vector<Production*>::const_iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur)
    os<<**cur<<endl;
}



ostream &operator<<(ostream &os,const ProductionSet &S)
{
  S.printOn(os);
  return os;
}



void ProductionSet::normalize(GrammarAlphabet &alpha)
{
  int n=alpha.size();
  for(int i=0 ; i<n ; ++i) {
    GrammarSymbol *s=alpha[i];
    Vector<Production*> &prods=lookup(s);
    float sum=0;
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ; 
	cur!=end ; ++cur)
      sum+=(*cur)->getProbability();
    for(Vector<Production*>::iterator cur=prods.begin(), end=prods.end() ; 
	cur!=end ; ++cur)
      (*cur)->setProbability((*cur)->getProbability()/sum);
  }
}

