/****************************************************************
 Grammar.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include "Grammar.H"
using namespace std;
using namespace BOOM;

Grammar::Grammar()
{
  // ctor
}



void Grammar::addProduction(Production *prod)
{
    productions.push_back(prod);
}



int Grammar::numProductions() const
{
    return productions.size();
}



Production *Grammar::getIthProduction(int i)
{
    return productions[i];
}



Vector<Production*> &Grammar::getProductionsFor(GrammarSymbol *lhs)
{
    return productionsByLHS[lhs];
}



GrammarAlphabet &Grammar::getTerminals()
{
    return terminals;
}



GrammarAlphabet &Grammar::getNonterminals()
{
    return nonterminals;
}




