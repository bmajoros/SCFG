/****************************************************************
 Inside.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BOOM/Constants.H"
#include "BOOM/SumLogProbs.H"
#include "Inside.H"
using namespace std;
using namespace BOOM;

Inside::Inside(Sequence &S,BinaryProductions &prods)
  : M(S.getLength()), productions(prods), numProds(prods.numProductions()),
    L(S.getLength()), numNonterminals(prods.getNumNonterminals()),
    S(S)
{
  initDiagonal();
  fillMatrix();
}



void Inside::initDiagonal()
{
  for(int i=0 ; i<L ; ++i) {
    Cell &cell=M(i,i);
    cell.resize(numNonterminals);
    cell.setAllTo(NEGATIVE_INFINITY);
    int term=S[i];
    const Array1D<NonterminalProb> &ntProbs=productions.terminal(term);
    int n=ntProbs.size();
    for(int i=0 ; i<n ; ++i) {
      const NonterminalProb &ntProb=ntProbs[i];
      cell[ntProb.nonterminal]=log(ntProb.P);
    }
  }
}



void Inside::fillMatrix()
{
  const int numProds=productions.numProductions();
  for(int i=L-2 ; i>=0 ; --i)
    for(int j=i+1 ; j<=L-1 ; ++j) {
      Cell &cell=M(i,j);
      Array1D< Vector<float> > logProbs(numNonterminals);
      cell.resize(numNonterminals);
      cell.setAllTo(NEGATIVE_INFINITY);
      for(int k=i ; k<=j-1 ; ++k) {
	Cell &leftCell=M(i,k), &rightCell=M(k+1,j);
	for(Vector<BinaryProduction>::const_iterator cur=productions.begin(),
	      end=productions.end() ; cur!=end ; ++cur) {
	  const BinaryProduction &prod=*cur;
	  const int lhs=prod.LHS(), rhs0=prod.RHS()[0], rhs1=prod.RHS()[1];
	  const float P=prod.P();
	  const float sum=log(P)+leftCell[rhs0]+rightCell[rhs1];
	  logProbs[lhs].push_back(sum);
	}
      }
      for(int m=0 ; m<numNonterminals ; ++m)
	cell[m]=sumLogProbs(logProbs[m]);
    }
}



float Inside::likelihood() const
{
  return M(0,L-1)[productions.getStartSymbol()];
}



float Inside::operator()(int i,int j,int nonterminal) const
{
  return M(i,j)[nonterminal];
}






