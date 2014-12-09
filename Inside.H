/****************************************************************
 Inside.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Inside_H
#define INCL_Inside_H
#include <iostream>
#include "BOOM/TriangMatrix.H"
#include "BOOM/Sequence.H"
#include "BinaryProductions.H"
using namespace std;
using namespace BOOM;

typedef Array1D<float> Cell; // indexed by nonterminal ID

class Inside {
  TriangMatrix<Cell> M;
  Sequence &S;
  BinaryProductions &productions;
  int numNonterminals;
  int numProds;
  int L;
  void initDiagonal();
  void fillMatrix();
public:
  Inside(Sequence &,BinaryProductions &);
  float likelihood() const;
  float operator()(int i,int j,int nonterminal) const;
};

#endif

