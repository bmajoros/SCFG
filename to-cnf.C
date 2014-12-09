/****************************************************************
 to-cnf.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include <fstream>
#include "BOOM/String.H"
#include "BOOM/CommandLine.H"
#include "BOOM/Exceptions.H"
#include "BOOM/SubsetEnumerator.H"
#include "SCFG.H"
#include "DerivesEpsilon.H"
#include "Terminates.H"
#include "ReachableFromStart.H"
using namespace std;
using namespace BOOM;

const int HASH_SIZE=499;

class Application {
  void addNewStart(SCFG &);
  String findUniqueID(GrammarAlphabet &);
  GrammarSymbol *generateUnique(GrammarAlphabet &);
  void eliminateUseless(SCFG &,Terminates &,ReachableFromStart &);
  void factorNullable(SCFG &,DerivesEpsilon &);
  void deleteNullProductions(SCFG &);
  void delSymsWithNoProds(SCFG &);
  void replaceUnitProductions(SCFG &);
  void expandUnitProduction(SCFG &,Production &unit);
  int countNT(Vector<GrammarSymbol*> &);
  void getNullableIndices(Vector<int> &indices,Vector<GrammarSymbol*> &rhs,
			  DerivesEpsilon &);
  void factorLong(SCFG &);
  void factorTerminals(SCFG &);
  void simplify(SCFG &G);
  bool identical(Vector<Production*> &xProds,Vector<Production*> &yProds);
public:
  Application();
  int main(int argc,char *argv[]);
};


int main(int argc,char *argv[]) {
  try
    {
      Application app;
      return app.main(argc,argv);
    }
  catch(RootException &e)
    {
      cerr<<e.getMessage()<<endl;
    }
  catch(const char *p)
    {
      cerr << p << endl;
    }
  catch(const string &msg)
    {
      cerr << msg.c_str() << endl;
    }
  catch(const exception &e)
    {
      cerr << "STL exception caught in main:\n" << e.what() << endl;
    }
  catch(...)
    {
      cerr << "Unknown exception caught in main" << endl;
    }
  return -1;
}



Application::Application()
{
  // ctor
}



int Application::main(int argc,char *argv[])
{
  // Process command line
  CommandLine cmd(argc,argv,"");
  if(cmd.numArgs()!=1)
    throw String("to-cnf <infile>");
  const String infile=cmd.arg(0);

  // Load grammar
  SCFG G(infile);
  G.normalize();

  // **************************************************************
  // Step 1: Add a new start symbol
  addNewStart(G);

  // **************************************************************
  // Step 2: Compute P(derives eps) and P(derives a nonempty terminal string)
  const float tolerance=1e-8;
  ReachableFromStart reachable(G);
  DerivesEpsilon nullable(G,tolerance);
  Terminates terminates(G);

  // **************************************************************
  // Step 3: Eliminate null productions: X->epsilon (...combinatorial...)
  deleteNullProductions(G);
  factorNullable(G,nullable);

  // **************************************************************
  // Step 4: Eliminate unit productions: X->Y
  replaceUnitProductions(G);

  // **************************************************************
  // Step 5: Eliminate symbols that do NOT derive a nonempty terminal string
  eliminateUseless(G,terminates,reachable);

  // **************************************************************
  // Step 6: Factor terminals: A->cBe becomes A->CBE, C->c, E->e
  factorTerminals(G);

  // **************************************************************
  // Step 7: Factor long RHS's: A->XYZ becomes A->XB, B->YZ
  factorLong(G);

  // **************************************************************
  // Step 8: Simplify
  simplify(G);

  G.normalize();
  cout<<G<<endl;

  return 0;
}



String Application::findUniqueID(GrammarAlphabet &alpha)
{
  String S="S";
  int z=0;
  while(1) {
    if(!alpha.lookupLexeme(S)) return S;
    ++z;
    S=String("S")+z;
  }
}




void Application::addNewStart(SCFG &G)
{
  GrammarAlphabet &alpha=G.getNonterminals();
  String lexeme=findUniqueID(alpha);
  GrammarSymbol *newStart=alpha.findOrCreate(lexeme,NONTERMINAL_SYMBOL);
  GrammarSymbol *oldStart=G.getStart();
  Production *prod=new Production;
  prod->setLHS(newStart);
  prod->getRHS().push_back(oldStart);
  prod->setProbability(1);
  G.getProductions().push_back(prod);
  G.setStart(newStart);
}



void Application::eliminateUseless(SCFG &G,Terminates &terminates,
				   ReachableFromStart &reachable)
{
  // Delete any production having a useless symbol on the LHS or RHS
  Vector<Production*> &productions=G.getProductions();
  int numProds=productions.size();
  for(int i=0 ; i<numProds ; ++i) {
    Production *prod=productions[i];
    bool prodOK=true;
    GrammarSymbol *lhs=prod->getLHS();
    if(!terminates[lhs] || !reachable[lhs]) prodOK=false;
    else {
      Vector<GrammarSymbol*> &rhs=prod->getRHS();
      for(Vector<GrammarSymbol*>::iterator cur=rhs.begin(), end=rhs.end() ;
	  cur!=end ; ++cur)
	if((*cur)->isNonterminal() && 
	   (!terminates[*cur] || !reachable[*cur]))
	  { prodOK=false; break; }
    }
    if(!prodOK) { delete prod; productions.cut(i); --i; --numProds; }
  }	

  // Delete useless symbols from the alphabet
  GrammarAlphabet &nonterminals=G.getNonterminals();
  int N=nonterminals.size();
  for(int i=0 ; i<N ; ++i) {
    if(!terminates[nonterminals[i]] || !reachable[nonterminals[i]]) 
      nonterminals.deleteIthEntry(i);
  }	
  nonterminals.compact();
}



void Application::deleteNullProductions(SCFG &G)
{
  Vector<Production*> &productions=G.getProductions();
  int numProds=productions.size();
  for(int i=0 ; i<numProds ; ++i) {
    Production *prod=productions[i];
    if(prod->isNull()) { delete prod; productions.cut(i); --i; --numProds; }
  }	
}



void Application::delSymsWithNoProds(SCFG &G)
{
  ProductionSet &productions=*G.getProductionSet();
  GrammarAlphabet &nonterminals=G.getNonterminals();
  int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i) {
    GrammarSymbol *s=nonterminals[i];
    if(!s) continue;
    if(productions.lookup(s).size()==0) 
      nonterminals.deleteIthEntry(i);
  }
  delete &productions;
  nonterminals.compact();
}



void Application::replaceUnitProductions(SCFG &G)
{
  while(1) {
    bool changes=false;
    Vector<Production*> &productions=G.getProductions();
    int numProds=productions.size();
    for(int i=0 ; i<numProds ; ++i) {
      Production *prod=productions[i];
      Vector<GrammarSymbol*> &rhs=prod->getRHS();
      if(rhs.size()==1 && rhs[0]->isNonterminal()) {
	expandUnitProduction(G,*prod);
	delete prod;
	productions.cut(i);
	--i; --numProds;
	changes=true;
      }
    }
    if(!changes) break;
  }
}



void Application::expandUnitProduction(SCFG &G,Production &unit)
{
  const float unitProb=unit.getProbability();
  GrammarSymbol *lhs=unit.getLHS(), *rhs=unit.getRHS()[0];
  ProductionSet *prodSet=G.getProductionSet();
  Vector<Production*> &rhsProds=prodSet->lookup(rhs);
  for(Vector<Production*>::iterator cur=rhsProds.begin(), end=rhsProds.end() ;
      cur!=end ; ++cur) {
    Production *newProd=new Production;
    newProd->setLHS(lhs);
    newProd->getRHS()=(*cur)->getRHS();
    const float rhsProb=(*cur)->getProbability();
    newProd->setProbability(unitProb*rhsProb);
    G.getProductions().push_back(newProd);
  }
  delete prodSet;
}



int Application::countNT(Vector<GrammarSymbol*> &V)
{
  int c=0;
  for(Vector<GrammarSymbol*>::iterator cur=V.begin(), end=V.end() ;
      cur!=end ; ++cur)
    if((*cur)->isNonterminal()) ++c;
  return c;
}



void Application::getNullableIndices(Vector<int> &indices,
				     Vector<GrammarSymbol*> &rhs,
				     DerivesEpsilon &nullable)
{
  int L=rhs.size();
  for(int i=0 ; i<L ; ++i) {
    GrammarSymbol *s=rhs[i];
    if(s->isNonterminal() && nullable[s]>0) 
      indices.push_back(i);
  }
}



void Application::factorNullable(SCFG &G,DerivesEpsilon &nullable)
{
  Vector<Production*> &productions=G.getProductions(), newProds;
  int numProd=productions.size();
  for(int i=0 ; i<numProd ; ++i) {
    Production *prod=productions[i];
    Vector<GrammarSymbol*> &rhs=prod->getRHS();
    Vector<int> indices;
    getNullableIndices(indices,rhs,nullable);
    const int numNullable=indices.size();
    if(!numNullable) continue;
    SubsetEnumerator iter(numNullable);
    Set<int> subset;
    while(iter.getNext(subset)) {
      Set<int> keep;
      for(Set<int>::iterator cur=subset.begin(), end=subset.end() ; 
	  cur!=end ; ++cur) keep.insert(indices[*cur]);
      Production *newProd=new Production;
      float P=prod->getProbability();
      newProd->setLHS(prod->getLHS());
      Vector<GrammarSymbol*> rhs, &oldRhs=prod->getRHS();
      int L=oldRhs.size();
      for(int i=0 ; i<L ; ++i) {
	GrammarSymbol *s=oldRhs[i];
	if(s->isTerminal() || nullable[s]==0) rhs.push_back(s);
	else if(keep.isMember(i)) { rhs.push_back(s); P*=(1-nullable[s]); }
	else P*=nullable[s];
      }
      if(rhs.isEmpty() || P==0) { delete newProd; continue; }
      newProd->getRHS()=rhs;
      newProd->setProbability(P);
      newProds.push_back(newProd);
    }
    delete prod;
    productions.cut(i);
    --i; --numProd;
  }	
  productions.append(newProds);
}



void Application::factorLong(SCFG &G)
{
  // Factor long RHS's: A->CDE becomes A->CF, F->DE
  // Precondition: any RHS>1 contains only nonterminals

  GrammarAlphabet &nonterminals=G.getNonterminals();
  Vector<Production*> &productions=G.getProductions(), newProds;
  int numProds=productions.size();
  for(int i=0 ; i<numProds ; ++i) {
    Production *prod=productions[i];
    Vector<GrammarSymbol*> &rhs=prod->getRHS();
    const int L=rhs.size();
    if(L<3) continue;
    GrammarSymbol *lhs=prod->getLHS();
    for(int j=0 ; j<L-1 ; ++j) {
      Production *newProd=new Production;
      newProd->setLHS(lhs);
      newProd->setProbability(j==0 ? prod->getProbability() : 1.0);
      Vector<GrammarSymbol*> &newRHS=newProd->getRHS();
      newRHS.push_back(rhs[j]);
      GrammarSymbol *nextS=j<L-2 ? generateUnique(nonterminals) : rhs[L-1];
      newRHS.push_back(nextS);
      lhs=nextS;
      newProds.push_back(newProd);
    }
    delete prod;
    productions.cut(i);
    --i; --numProds;
  }
  productions.append(newProds);
}



void Application::factorTerminals(SCFG &G)
{
  // Factor terminals: A->cB becomes A->CB, C->c

  Vector<Production*> &productions=G.getProductions(), newProds;
  GrammarAlphabet &nonterminals=G.getNonterminals(), 
    &terminals=G.getTerminals();
  HashMap<GrammarSymbol,GrammarSymbol*> alias(HASH_SIZE);
  const int T=terminals.size();
  for(int i=0 ; i<T ; ++i) {
    GrammarSymbol *term=terminals[i], *newNT=generateUnique(nonterminals);
    alias[*term]=newNT;
    Production *prod=new Production;
    prod->setProbability(1);
    prod->setLHS(newNT);
    prod->getRHS().push_back(term);
    newProds.push_back(prod);
  }
  int numProds=productions.size();
  for(int i=0 ; i<numProds ; ++i) {
    Production *prod=productions[i];
    Vector<GrammarSymbol*> &rhs=prod->getRHS();
    const int L=rhs.size();
    if(L==1) continue;
    for(int j=0 ; j<L ; ++j) {
      GrammarSymbol *s=rhs[j];
      if(s->isTerminal()) rhs[j]=alias[*s];
    }
  }
  productions.append(newProds);
}



GrammarSymbol *Application::generateUnique(GrammarAlphabet &alpha)
{
  String lexeme=findUniqueID(alpha);
  return alpha.findOrCreate(lexeme,NONTERMINAL_SYMBOL);
}



void Application::simplify(SCFG &G)
{
  HashMap<GrammarSymbol,GrammarSymbol*> alias(HASH_SIZE);
  GrammarAlphabet &nonterminals=G.getNonterminals();
  ProductionSet &prodSet=*G.getProductionSet();
  int numNT=nonterminals.size();
  for(int i=0 ; i<numNT ; ++i) {
    GrammarSymbol *X=nonterminals[i];
    if(alias.isDefined(*X)) continue;
    Vector<Production*> &xProds=prodSet.lookup(X);
    for(int j=i+1 ; j<numNT ; ++j) {
      GrammarSymbol *Y=nonterminals[j];
      Vector<Production*> &yProds=prodSet.lookup(Y);
      if(identical(xProds,yProds)) alias[*Y]=X;
    }
  }
  delete &prodSet;
  Vector<Production*> &productions=G.getProductions();
  int numProds=productions.size();
  for(int i=0 ; i<numProds ; ++i) {
    Production *prod=productions[i];
    if(alias.isDefined(*prod->getLHS())) {
      delete prod;
      productions.cut(i);
      --i; --numProds;
      continue;
    }
    Vector<GrammarSymbol*> &rhs=prod->getRHS();
    int L=rhs.size();
    for(int i=0 ; i<L ; ++i)
      if(alias.isDefined(*rhs[i]))
	rhs[i]=alias[*rhs[i]];
  }
  for(int i=0 ; i<numNT ; ++i) {
    GrammarSymbol *X=nonterminals[i];
    if(alias.isDefined(*X)) nonterminals.deleteIthEntry(i);
  }
  nonterminals.compact();
}



bool Application::identical(Vector<Production*> &xProds,
			    Vector<Production*> &yProds)
{
  int numX=xProds.size(), numY=yProds.size();
  if(numX!=numY) return false;
  for(int i=0 ; i<numX ; ++i) {
    Production *xProd=xProds[i];
    float P=xProd->getProbability();
    Vector<GrammarSymbol*> &rhs=xProd->getRHS();
    bool found=false;
    for(int j=0 ; j<numY ; ++j) {
      Production *yProd=yProds[j];
      if(yProd->getProbability()==P && yProd->getRHS()==rhs) found=true;
    }
    if(!found) return false;
  }
  return true;
}




