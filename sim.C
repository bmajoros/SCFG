/****************************************************************
 sim.C
 william.majoros@duke.edu

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#include <iostream>
#include "BOOM/String.H"
#include "BOOM/CommandLine.H"
#include "BOOM/Random.H"
#include "SCFG.H"
#include "SententialForm.H"
using namespace std;
using namespace BOOM;


class Application {
  Production *sample(Vector<Production*> &V);
  void emit(SententialForm &);
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
  randomize();
}



int Application::main(int argc,char *argv[])
{
  // Process command line
  CommandLine cmd(argc,argv,"");
  if(cmd.numArgs()!=4)
    throw String("\n\
sim [options ]<infile.scfg> <#seqs> <minlen> <maxlen>\n\
");
  const String infile=cmd.arg(0);
  const int N=cmd.arg(1).asInt();
  const int minLen=cmd.arg(2).asInt();
  const int maxLen=cmd.arg(3).asInt();
  
  // Load grammar
  SCFG G(infile);
  G.normalize();
  ProductionSet *productions=G.getProductionSet();
  //cout<<*productions<<endl;
  //cout<<G<<endl;

  // Simulate
  for(int i=0 ; i<N ; ++i) {
    SententialForm sf;
    sf.append(G.getStart());
    while(!sf.isFinal()) {
      const int pos=sf.leftmostNonterminal();
      GrammarSymbol *S=sf[pos];
      Vector<Production*> &prods=productions->lookup(S);
      if(prods.size()==0) throw "error: no productions for nonterminal!";
      Production *prod=sample(prods);
      sf.rewrite(pos,prod);
      //if(sf.length()>maxLen) throw String("maximum length exceeded");
      if(sf.length()>maxLen) break;
    }
    if(sf.length()<minLen || sf.length()>maxLen) {
      --i;
      continue;
    }
    cout<<">"<<(i+1)<<endl;
    emit(sf);
  }
  delete productions;

  return 0;
}



Production *Application::sample(Vector<Production*> &V)
{
  const float r=Random0to1();
  float sum=0;
  for(Vector<Production*>::iterator cur=V.begin(), end=V.end() ; cur!=end ;
      ++cur) {
    sum+=(*cur)->getProbability();
    if(sum>=r) return *cur;
  }
  return V[V.size()-1];
}



void Application::emit(SententialForm &sf)
{
  const int L=sf.length();
  const int maxLine=60;
  for(int i=0 ; i<L ; ++i) {
    GrammarSymbol *s=sf[i];
    cout<<s->getLexeme();
    if((i+1)%maxLine==0) cout<<endl;
  }
  cout<<endl;
}




