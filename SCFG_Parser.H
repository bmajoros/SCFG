/****************************************************************
 SCFG_Parser.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_SCFG_Parser_H
#define INCL_SCFG_Parser_H
#include <iostream>
#include "BOOM/GenericTokenStream.H"
#include "BOOM/Alphabet.H"
#include "BOOM/Vector.H"
#include "BOOM/Exceptions.H"
#include "Production.H"
#include "GrammarAlphabet.H"
using namespace std;
using namespace BOOM;

/*****************************************************
                         GRAMMAR

  Program ::= Start Productions

  Start ::= TOK_START  TOK_IDENT

  Productions ::= Production Productions
  Productions ::= 

  Production ::= TOK_IDENT TOK_ARROW Rhs Prob

  Rhs ::= Elem Rhs
  Rhs ::= 

  Prob ::= TOK_LEFT_BRACE TOK_FLOAT_LIT TOK_RIGHT_BRACE

*****************************************************/

struct SyntaxError :public RootException {
  SyntaxError(const char *msg,int line);
  SyntaxError(const String &msg,int line);
};

class SCFG_Parser {
  GrammarAlphabet terminals, nonterminals;
  GrammarSymbol *start;
  GenericTokenStream &tokenStream;
  Vector<Production*> productions;
  void match(GenericTokenType);
  //bool canBeginExpression(TokenType);

  // Parsing procedures
  void parse_Program();
  void parse_Start();
  void parse_Productions();
  Production *parse_Production();
  Vector<GenericToken> *parse_Rhs();
  float parse_Prob();
public:
  SCFG_Parser(GenericTokenStream &);
  GrammarAlphabet &getTerminals();
  GrammarAlphabet &getNonterminals();
  Vector<Production*> &getProductions();
  GrammarSymbol *getStartSymbol();
};


/****************************************************

start S

S -> MARGIN STEM MARGIN {1}

MARGIN -> N MARGIN {0.5}
MARGIN -> {0.5}

STEM -> 'A' STEM 'T' {0.25}
STEM -> 'T' STEM 'A' {0.25}
STEM -> 'C' STEM 'G' {0.2}
STEM -> 'G' STEM 'C' {0.2}
STEM -> LOOP {0.1}

LOOP -> N LOOP {0.7}
LOOP -> N {0.3}

N -> 'A' {0.3}
N -> 'G' {0.3}
N -> 'C' {0.2}
N -> 'T' {0.2}

****************************************************/
#endif

