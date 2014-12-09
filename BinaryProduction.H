/****************************************************************
 BinaryProduction.H
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_BinaryProduction_H
#define INCL_BinaryProduction_H
#include <iostream>
using namespace std;

/****************************************************************
 Represents chomsky productions of the form A->BC for nonterminals
 A, B, C, where the nonterminals are represented as indices into
 a nonterminal alphabet (for fast indexing into DP arrays).
 ****************************************************************/
class BinaryProduction {
  int lhs;
  int rhs[2];
  float prob;
public:
  BinaryProduction() {}
  BinaryProduction(int lhs,int rhs0,int rhs1,float P);
  inline int LHS() const { return lhs; }
  inline const int *RHS() const { return rhs; }
  inline float P() const { return prob; }
  inline void setP(float p) { prob=p; }
};

#endif

