/****************************************************************
 train.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <math.h>
#include <iostream>
#include "BOOM/String.H"
#include "BOOM/CommandLine.H"
#include "BOOM/FastaReader.H"
#include "BOOM/Alphabet.H"
#include "BOOM/Sequence.H"
#include "BOOM/Array1D.H"
#include "BOOM/Array2D.H"
#include "BOOM/SumLogProbs.H"
#include "BOOM/Exceptions.H"
#include "BinaryProductions.H"
#include "SCFG.H"
#include "Inside.H"
#include "Outside.H"
using namespace std;
using namespace BOOM;


class Application {
  Alphabet alphabet;
  BinaryProductions binaryProductions;
  void getAlphabet(SCFG &,Alphabet &);
  float update(SCFG &,Vector<Sequence> &);
  void output(SCFG &);
public:
  Application();
  int main(int argc,char *argv[]);
};


int main(int argc,char *argv[])
  {
    try
      {
	Application app;
	return app.main(argc,argv);
      }
    catch(const RootException &e) {
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
  if(cmd.numArgs()!=3)
    throw String("\n\
train [options] <grammar.cnf> <seq.fasta> <max-iter>\n\
");
  const String grammarFile=cmd.arg(0);
  const String fastaFile=cmd.arg(1);
  const int maxIter=cmd.arg(2).asInt();

  // Load and recode grammar using binary productions for fast indexing
  SCFG G(grammarFile);
  getAlphabet(G,alphabet);
  binaryProductions.init(G,alphabet);

  // Read sequences
  FastaReader reader(fastaFile,alphabet);
  Vector<Sequence> seqs;
  String def, seqStr;
  while(reader.nextSequence(def,seqStr)) {
    Sequence seq(seqStr,alphabet);
    seqs.push_back(seq);
  }

  // Do N iterations of training
  for(int i=0 ; i<maxIter ; ++i) {
    cerr<<"iteration #"<<(i+1)<<"\t"; cerr.flush();
    float lik=update(G,seqs);
    cerr<<lik<<endl;
  }

  // Output trained grammar
  output(G);

  return 0;
}



void Application::getAlphabet(SCFG &G,Alphabet &alphabet)
{
  GrammarAlphabet &terminals=G.getTerminals();
  const int N=terminals.size();
  for(int i=0 ; i<N ; ++i) {
    GrammarSymbol *s=terminals[i];
    const String &lexeme=s->getLexeme();
    if(lexeme.length()!=1) 
      throw lexeme+" has improper length for a terminal";
    alphabet.add(lexeme[0]);
  }
}



float Application::update(SCFG &G,Vector<Sequence> &seqs)
{
  const int numProds=binaryProductions.numProductions();
  const int numTerminals=binaryProductions.getTerminals().size();
  const int numNonterminals=binaryProductions.getNumNonterminals();
  float lik=0;
  Array1D< Vector<float> > g_ntLogProbs(numNonterminals);    // A
  Array1D< Vector<float> > g_binaryLogProbs(numProds);       // A -> BC
  Array2D< Vector<float> > g_unaryLogProbs(numTerminals,numNonterminals);//A->a
  for(Vector<Sequence>::iterator cur=seqs.begin(), end=seqs.end() ;      
      cur!=end ; ++cur) {
    Sequence &S=*cur;
    const int L=S.getLength();
    Inside inside(S,binaryProductions);
    Outside outside(S,binaryProductions,inside);
    float insideLik=inside.likelihood();
    lik+=insideLik;

    Array1D< Vector<float> > ntLogProbs(numNonterminals);    // A
    Array1D< Vector<float> > binaryLogProbs(numProds);       // A -> BC
    Array2D< Vector<float> > unaryLogProbs(numTerminals,numNonterminals);//A->a

    // First, compute denominators
    for(int nt=0 ; nt<numNonterminals ; ++nt)
      for(int i=0 ; i<=L-1 ; ++i)
	for(int j=i ; j<=L-1 ; ++j) {
	  const float sum=inside(i,j,nt)+outside(i,j,nt);
	  if(isFinite(sum)) ntLogProbs[nt].push_back(sum);}

    // Now compute numerators
    for(int i=0 ; i<=L-2 ; ++i)
      for(int j=i+1 ; j<=L-1 ; ++j) // ORIGINAL
	//for(int j=i ; j<=L-1 ; ++j)
	for(int k=i ; k<=j-1 ; ++k) // ORIGINAL
	  //for(int k=i ; k<=j-1 ; ++k) // ### j-1?
	  for(int l=0 ; l<numProds ; ++l) { // A -> BC
	    const BinaryProduction &prod=binaryProductions.production(l);
	    const float sum=outside(i,j,prod.LHS())+log(prod.P())+
	      inside(i,k,prod.RHS()[0])+inside(k+1,j,prod.RHS()[1]);
	    if(isFinite(sum)) binaryLogProbs[l].push_back(sum);}
    for(int i=0 ; i<=L-1 ; ++i) { // A -> a
      Symbol x=S[i];
      Vector<NonterminalProb> &ntProbs=binaryProductions.terminal(int(x));
      for(Vector<NonterminalProb>::iterator cur=ntProbs.begin(), end=
	    ntProbs.end() ; cur!=end ; ++cur) {
	const NonterminalProb &ntProb=*cur;
	const float sum=outside(i,i,ntProb.nonterminal)+log(ntProb.P);
	if(isFinite(sum)) 
	  unaryLogProbs[int(x)][ntProb.nonterminal].push_back(sum);}}

    // Compress arrays
    for(int i=0 ; i<numNonterminals ; ++i)
      g_ntLogProbs[i].push_back(sumLogProbs(ntLogProbs[i])-insideLik);
    for(int i=0 ; i<numProds ; ++i)
      g_binaryLogProbs[i].push_back(sumLogProbs(binaryLogProbs[i])-insideLik);
    for(int i=0 ; i<numTerminals ; ++i)
      for(int j=0 ; j<numNonterminals ; ++j)
	if(!unaryLogProbs[i][j].isEmpty())
	  g_unaryLogProbs[i][j].
	    push_back(sumLogProbs(unaryLogProbs[i][j])-insideLik);
  }

  // Update grammar parameters
  Array1D<float> denom(numNonterminals);
  for(int nt=0 ; nt<numNonterminals ; ++nt)
    denom[nt]=sumLogProbs(g_ntLogProbs[nt]);
  for(int l=0 ; l<numProds ; ++l) { // A -> BC
    BinaryProduction &prod=binaryProductions.production(l);
    const float numer=sumLogProbs(g_binaryLogProbs[l]);
    const float newP=exp(numer-denom[prod.LHS()]);
    prod.setP(newP);}
  for(int x=0 ; x<numTerminals ; ++x) { // A -> x
    Vector<NonterminalProb> &ntProbs=binaryProductions.terminal(x);
    for(Vector<NonterminalProb>::iterator cur=ntProbs.begin(), end=
	  ntProbs.end() ; cur!=end ; ++cur) {
      NonterminalProb &ntProb=*cur;
      const float numer=sumLogProbs(g_unaryLogProbs[x][ntProb.nonterminal]);
      ntProb.P=exp(numer-denom[ntProb.nonterminal]);}
  }

  return lik;
}



void Application::output(SCFG &G)
{
  GrammarAlphabet &terminals=G.getTerminals();
  GrammarAlphabet &nonterminals=G.getNonterminals();
  const int startID=binaryProductions.getStartSymbol();
  String startName=nonterminals[startID]->getLexeme();
  cout<<"start "<<startName<<endl;
  for(Vector<BinaryProduction>::const_iterator cur=binaryProductions.begin(), 
	end=binaryProductions.end() ; cur!=end ; ++cur) {
    const BinaryProduction &prod=*cur;
    String lhs=nonterminals[prod.LHS()]->getLexeme();
    String rhs0=nonterminals[prod.RHS()[0]]->getLexeme();
    String rhs1=nonterminals[prod.RHS()[1]]->getLexeme();
    float P=prod.P();
    if(P>0) cout<<lhs<<" -> "<<rhs0<<" "<<rhs1<<" {"<<P<<"}"<<endl;
    else cerr<<"DELETED: "<<lhs<<" -> "<<rhs0<<" "<<rhs1<<" {"<<P<<"}"<<endl;
  }
  const int numTerminals=terminals.size();
  for(int i=0 ; i<numTerminals ; ++i) {
    String rhs=terminals[i]->getLexeme();
    Vector<NonterminalProb> &V=binaryProductions.terminal(i);
    for(Vector<NonterminalProb>::iterator cur=V.begin(), end=V.end() ;
	cur!=end ; ++cur) {
      NonterminalProb ntp=*cur;
      String lhs=nonterminals[ntp.nonterminal]->getLexeme();
      float P=ntp.P;
      if(P>0) cout<<lhs<<" -> '"<<rhs<<"' {"<<P<<"}"<<endl;
      else cerr<<"DELETED: "<<lhs<<" -> '"<<rhs<<"' {"<<P<<"}"<<endl;
    }
  }
}


