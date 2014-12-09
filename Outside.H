/****************************************************************
 Outside.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Outside_H
#define INCL_Outside_H
#include <iostream>
#include "BOOM/TriangMatrix.H"
#include "BOOM/Sequence.H"
#include "Inside.H"
using namespace std;
using namespace BOOM;

typedef Array1D<float> Cell; // indexed by nonterminal ID

class Outside {
  Inside &alpha;
  TriangMatrix<Cell> B;
  Sequence &S;
  BinaryProductions &productions;
  int numNonterminals;
  int numProds;
  int L;
  void initCorner();
  void fillMatrix();
  void sumRight(Array1D< Vector<float> > &logProbs,int i,int k,int j);
  void sumLeft(Array1D< Vector<float> > &logProbs,int i,int k,int j);
public:
  Outside(Sequence &,BinaryProductions &,Inside &);
  float likelihood() const;
  float operator()(int i,int j,int nonterminal) const;
};

#endif

