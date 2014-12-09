/****************************************************************
 SententialForm.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_SententialForm_H
#define INCL_SententialForm_H
#include <iostream>
#include "BOOM/Vector.H"
#include "GrammarSymbol.H"
#include "Production.H"
using namespace std;
using namespace BOOM;

class SententialForm {
  Vector<GrammarSymbol*> V;
public:
  SententialForm();
  void append(GrammarSymbol *);
  int length();
  GrammarSymbol *operator[](int);
  bool isFinal() const; // contains no nonterminals
  int leftmostNonterminal() const;
  void rewrite(int pos,Production *);
  void printOn(ostream &os) const;
};

ostream &operator<<(ostream &os,const SententialForm &);

#endif

