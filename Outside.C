/****************************************************************
 Outside.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BOOM/Constants.H"
#include "BOOM/SumLogProbs.H"
#include "Outside.H"
using namespace std;
using namespace BOOM;

Outside::Outside(Sequence &S,BinaryProductions &prods,Inside &alpha)
  : B(S.getLength()), productions(prods), numProds(prods.numProductions()),
    L(S.getLength()), numNonterminals(prods.getNumNonterminals()),
    S(S), alpha(alpha)
{
  initCorner();
  fillMatrix();
}



void Outside::initCorner()
{
  int start=productions.getStartSymbol();
  Cell &corner=B(0,L-1);
  corner.resize(numNonterminals);
  corner.setAllTo(NEGATIVE_INFINITY);
  corner[start]=0; // log(1)
}



void Outside::fillMatrix()
{
  for(int i=0 ; i<=L-1 ; ++i)
    for(int j=L-1 ; j>=i ; --j) {
      if(i==0 && j==L-1) continue;
      Cell &cell=B(i,j);
      cell.resize(numNonterminals);
      cell.setAllTo(NEGATIVE_INFINITY);
      Array1D< Vector<float> > logProbs(numNonterminals);
      for(int k=j+1 ; k<=L-1 ; ++k) sumRight(logProbs,i,k,j);
      for(int k=0 ; k<=i-1 ; ++k) sumLeft(logProbs,i,k,j);
      for(int m=0 ; m<numNonterminals ; ++m)
	cell[m]=sumLogProbs(logProbs[m]);
    }
}



void Outside::sumRight(Array1D< Vector<float> > &logProbs,
		       int i,int k,int j)
{
  Outside &beta=*this;
  const int numProds=productions.numProductions();
  for(Vector<BinaryProduction>::const_iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur) {
    const BinaryProduction &prod=*cur;
    const int lhs=prod.LHS(), rhs0=prod.RHS()[0], rhs1=prod.RHS()[1];
    const float P=prod.P();
    const float sum=log(P)+alpha(j+1,k,rhs1)+beta(i,k,lhs);
    logProbs[rhs0].push_back(sum);
  }
}



void Outside::sumLeft(Array1D< Vector<float> > &logProbs,
		       int i,int k,int j)
{
  Outside &beta=*this;
  const int numProds=productions.numProductions();
  for(Vector<BinaryProduction>::const_iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur) {
    const BinaryProduction &prod=*cur;
    const int lhs=prod.LHS(), rhs0=prod.RHS()[0], rhs1=prod.RHS()[1];
    const float P=prod.P();
    const float sum=log(P)+alpha(k,i-1,rhs0)+beta(k,j,lhs);
    logProbs[rhs1].push_back(sum);
  }
}



float Outside::likelihood() const
{
  const Outside &beta=*this;
  Vector<float> logProbs;
  for(int X=0 ; X<numNonterminals ; ++X) {
    float value=beta(0,0,X)+alpha(0,0,X);
    if(isFinite(value)) logProbs.push_back(value);
  }
  float lik=sumLogProbs(logProbs);
  return lik;
}



float Outside::operator()(int i,int j,int nonterminal) const
{
  return B(i,j)[nonterminal];
}



