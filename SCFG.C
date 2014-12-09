/****************************************************************
 SCFG.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <fstream>
#include "BOOM/GenericScanner.H"
#include "BOOM/GenericTokenStream.H"
#include "SCFG.H"
#include "SCFG_Parser.H"
using namespace std;
using namespace BOOM;

SCFG::SCFG()
{
  // ctor
}



SCFG::SCFG(const String &filename)
{
  load(filename);
}



GrammarSymbol *SCFG::getStart()
{
  return start;
}



void SCFG::setStart(GrammarSymbol *s)
{
  start=s;
}



void SCFG::deleteAll()
{
  terminals.deleteAll();
  nonterminals.deleteAll();
  for(Vector<Production*>::iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur)
    delete *cur;
}



void SCFG::load(const String &filename)
{
  ifstream is(filename.c_str());
  if(!is.good()) throw String("Error opening file ")+filename;
  GenericScanner scanner(is);
  GenericTokenStream tokenStream(&scanner);
  SCFG_Parser parser(tokenStream);
  productions=parser.getProductions();
  terminals=parser.getTerminals();
  nonterminals=parser.getNonterminals();
  if(terminals.size()<1) throw String("no terminal symbols found in grammar");
  if(nonterminals.size()<1) 
    throw String("no nonterminal symbols found in grammar");
  start=parser.getStartSymbol();
}



GrammarAlphabet &SCFG::getTerminals()
{
  return terminals;
}



GrammarAlphabet &SCFG::getNonterminals()
{
  return nonterminals;
}



Vector<Production*> &SCFG::getProductions()
{
  return productions;
}



void SCFG::printOn(ostream &os) const
{
  os<<"start "<<start->getLexeme()<<endl;
  for(Vector<Production*>::const_iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur)
    os<<**cur<<endl;
}



ostream &operator<<(ostream &os,const SCFG &scfg)
{
  scfg.printOn(os);
  return os;
}



ProductionSet *SCFG::getProductionSet()
{
  ProductionSet *S=new ProductionSet;
  for(Vector<Production*>::iterator cur=productions.begin(),
	end=productions.end() ; cur!=end ; ++cur)
    S->add(*cur);
  S->reindex();
  return S;
}


void SCFG::normalize()
{
  ProductionSet *S=getProductionSet();
  S->normalize(nonterminals);
  delete S;
}


