/****************************************************************
 likelihood.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BOOM/String.H"
#include "BOOM/CommandLine.H"
#include "BOOM/FastaReader.H"
#include "BOOM/Alphabet.H"
#include "BOOM/Exceptions.H"
#include "BOOM/Sequence.H"
#include "BinaryProductions.H"
#include "SCFG.H"
#include "Inside.H"
using namespace std;
using namespace BOOM;


class Application {
  Alphabet alphabet, bgAlphabet;
  BinaryProductions binaryProductions, bgProds;
  void getAlphabet(SCFG &,Alphabet &);
  float applyWindows(Sequence &,BinaryProductions &,int windowSize);
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
  CommandLine cmd(argc,argv,"b:w:");
  if(cmd.numArgs()!=2)
    throw String("\n\
likelihood [options] <grammar.cnf> <seq.fasta>\n\
   -b filename : compute likelihood ratios using bg model from file\n\
   -w windowsize : use nonoverlapping windows\n\
");
  const String grammarFile=cmd.arg(0);
  const String fastaFile=cmd.arg(1);
  const bool wantRatios=cmd.option('b');
  const bool wantWindows=cmd.option('w');
  int windowSize=wantWindows ? cmd.optParm('w').asInt() : 0;

  // Load and recode grammar using binary productions for fast indexing
  SCFG G(grammarFile);
  getAlphabet(G,alphabet);
  binaryProductions.init(G,alphabet);
  if(wantRatios) {
    SCFG bg(cmd.optParm('b'));
    getAlphabet(bg,bgAlphabet);
    bgProds.init(bg,bgAlphabet);
  }

  // Process sequence file
  FastaReader reader(fastaFile,alphabet);
  String def, seqStr;
  while(reader.nextSequence(def,seqStr)) {
    const int L=seqStr.length();
    if(L==0) throw "empty sequence encountered";
    Sequence seq(seqStr,alphabet);
    float lik=applyWindows(seq,binaryProductions,windowSize);
    if(wantRatios) lik-=applyWindows(seq,bgProds,windowSize);
    cout<<lik<<endl;
  }

  return 0;
}



float Application::applyWindows(Sequence &seq,BinaryProductions &prods,
				int windowSize)
{
  float lik=0;
  Sequence subseq;
  int L=seq.getLength();
  if(windowSize<1) windowSize=L;
  for(int begin=0 ; begin<L ; begin+=windowSize) {
    int end=begin+windowSize;
    if(end>=L) end=L;
    int len=end-begin;
    seq.getSubsequence(begin,len,subseq);
    Inside inside(subseq,prods);
    lik+=inside.likelihood(); 
  }
  return lik;
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
