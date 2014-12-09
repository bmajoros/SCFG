/****************************************************************
 SCFG_Parser.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "SCFG_Parser.H"
using namespace std;
using namespace BOOM;


const int TOK_START=TOK_KEYWORD+0;



SyntaxError::SyntaxError(const char *msg,int line)
  : RootException("")
{
  setMessage(String("Syntax error: ")+msg+", line "+line);
}



SyntaxError::SyntaxError(const String &msg,int line)
  : RootException("")
{
  setMessage(String("Syntax error: ")+msg+", line "+line);
}



SCFG_Parser::SCFG_Parser(GenericTokenStream &tokenStream)
  : tokenStream(tokenStream)
{
  tokenStream.getScanner().registerKeyword("start",TOK_START);
  parse_Program();
}



GrammarAlphabet &SCFG_Parser::getTerminals()
{
  return terminals;
}



GrammarAlphabet &SCFG_Parser::getNonterminals()
{
  return nonterminals;
}



Vector<Production*> &SCFG_Parser::getProductions()
{
  return productions;
}



void SCFG_Parser::match(GenericTokenType tokenType)
{
  GenericToken *token=tokenStream.nextToken();
  if(token->getTokenType()!=tokenType)
    throw SyntaxError(String("expecting ")+toString(tokenType)+
		      ", near "+token->getLexeme(),token->getLineNum());
  delete token;
}



GrammarSymbol *SCFG_Parser::getStartSymbol()
{
  return start;
}



void SCFG_Parser::parse_Program()
{
  //  Program ::= Start Productions

  parse_Start();
  parse_Productions();
}



void SCFG_Parser::parse_Start()
{
  //  Start ::= TOK_START  TOK_IDENT

  match(TOK_START);
  GenericToken *token=tokenStream.nextToken();
  if(token->getTokenType()!=TOK_IDENT)
    throw SyntaxError(String("Expecting identifier after \"start\""),
		      token->getLineNum());
  String ident=token->getLexeme();
  delete token;
  start=nonterminals.findOrCreate(ident,NONTERMINAL_SYMBOL);
}



void SCFG_Parser::parse_Productions()
{
  //  Productions ::= Production Productions
  //  Productions ::= 

  while(tokenStream.peekTokenType()!=TOK_EOF) {
    Production *prod=parse_Production();
    productions.push_back(prod);
  }
}



Production *SCFG_Parser::parse_Production()
{
  //  Production ::= TOK_IDENT TOK_ARROW Rhs Prob

  GenericToken *token=tokenStream.nextToken();
  if(token->getTokenType()!=TOK_IDENT) 
    throw SyntaxError(String("expecting identifier at ")+token->getLexeme(),
		      token->getLineNum());
  match(TOK_ARROW);
  Vector<GenericToken> *tokens=parse_Rhs();
  const float P=parse_Prob();

  Production *prod=new Production;
  GrammarSymbol *lhs=
    nonterminals.findOrCreate(token->getLexeme(),NONTERMINAL_SYMBOL);
  delete token;
  prod->setLHS(lhs);
  Vector<GrammarSymbol*> &rhs=prod->getRHS();
  for(Vector<GenericToken>::iterator cur=tokens->begin(), end=tokens->end() ;
      cur!=end ; ++cur) {
    GenericToken &tok=*cur;
    if(tok.getTokenType()==TOK_IDENT) {
      GrammarSymbol *sym=
	nonterminals.findOrCreate(tok.getLexeme(),NONTERMINAL_SYMBOL);
      rhs.push_back(sym);
    }
    else if(tok.getTokenType()==TOK_CHAR_LIT) {
      GrammarSymbol *sym=
	terminals.findOrCreate(tok.getLexeme(),TERMINAL_SYMBOL);
      rhs.push_back(sym);
    }
    else throw SyntaxError(String("near ")+tok.getLexeme(),tok.getLineNum());
  }
  delete tokens;
  prod->setProbability(P);
  return prod;
}



Vector<GenericToken> *SCFG_Parser::parse_Rhs()
{
  //  Rhs ::= Elem Rhs
  //  Rhs ::= 

  Vector<GenericToken> *rhs=new Vector<GenericToken>;
  while(tokenStream.peekTokenType()!=TOK_LEFT_BRACE) {
    GenericToken *tok=tokenStream.nextToken();
    GenericTokenType tt=tok->getTokenType();
    if(tt!=TOK_CHAR_LIT && tt!=TOK_IDENT)
      throw SyntaxError(String("expecting identifier or character literal "
			       "near ")+tok->getLexeme(),
			tok->getLineNum());
    rhs->push_back(*tok);
  }
  return rhs;
}



float SCFG_Parser::parse_Prob()
{
  // Prob ::= TOK_LEFT_BRACE TOK_FLOAT_LIT TOK_RIGHT_BRACE

  match(TOK_LEFT_BRACE);
  GenericToken *token=tokenStream.nextToken();
  GenericTokenType tt=token->getTokenType();
  if(tt!=TOK_FLOAT_LIT && tt!=TOK_INT_LIT) 
    throw SyntaxError(token->getLexeme(),token->getLineNum());
  float value=token->getLexeme().asFloat();
  delete token;
  match(TOK_RIGHT_BRACE);
  return value;
}


