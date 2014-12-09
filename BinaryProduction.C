/****************************************************************
 BinaryProduction.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BinaryProduction.H"
using namespace std;


BinaryProduction::BinaryProduction(int lhs,int rhs0,int rhs1,float P)
  : lhs(lhs), prob(P)
{
  rhs[0]=rhs0;
  rhs[1]=rhs1;
}


