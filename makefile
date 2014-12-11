CC		= g++
DEBUG		= -g
OPTIMIZE	= -O
CFLAGS		= $(OPTIMIZE) -w -fpermissive -I/gpfs/fs0/data/reddylab/bmajoros/gsl/include -I/usr/include/openmpi-i386
LDFLAGS		= $(DEBUG)
LIBS		= -LBOOM -lBOOM -LGSL -lgsl -lm -lgslcblas
BOOM		= BOOM
OBJ		= obj

all:	BOOM obj sim likelihood train to-cnf

BOOM:
	@echo please install BOOM library first

obj:
	mkdir obj

#---------------------------------------------------------
$(OBJ)/Grammar.o:\
		Grammar.C \
		Grammar.H
	$(CC) $(CFLAGS) -o $(OBJ)/Grammar.o -c \
		Grammar.C
#---------------------------------------------------------
$(OBJ)/GrammarAlphabet.o:\
		GrammarAlphabet.C \
		GrammarAlphabet.H
	$(CC) $(CFLAGS) -o $(OBJ)/GrammarAlphabet.o -c \
		GrammarAlphabet.C
#---------------------------------------------------------
$(OBJ)/GrammarSymbol.o:\
		GrammarSymbol.C \
		GrammarSymbol.H
	$(CC) $(CFLAGS) -o $(OBJ)/GrammarSymbol.o -c \
		GrammarSymbol.C
#---------------------------------------------------------
$(OBJ)/Production.o:\
		Production.C \
		Production.H
	$(CC) $(CFLAGS) -o $(OBJ)/Production.o -c \
		Production.C
#---------------------------------------------------------
$(OBJ)/to-cnf.o:\
		Grammar.H \
		GrammarAlphabet.H \
		GrammarSymbol.H \
		Production.H \
		to-cnf.C
	$(CC) $(CFLAGS) -o $(OBJ)/to-cnf.o -c \
		to-cnf.C
#---------------------------------------------------------
to-cnf: \
		$(OBJ)/ReachableFromStart.o \
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/SententialForm.o \
		$(OBJ)/DerivesEpsilon.o \
		$(OBJ)/Terminates.o \
		$(OBJ)/to-cnf.o
	$(CC) $(LDFLAGS) -o to-cnf \
		$(OBJ)/ReachableFromStart.o \
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/SententialForm.o \
		$(OBJ)/DerivesEpsilon.o \
		$(OBJ)/Terminates.o \
		$(OBJ)/to-cnf.o \
		$(LIBS)
#---------------------------------------------------------
$(OBJ)/SCFG_Parser.o:\
		SCFG_Parser.C\
		SCFG_Parser.H
	$(CC) $(CFLAGS) -o $(OBJ)/SCFG_Parser.o -c \
		SCFG_Parser.C
#---------------------------------------------------------
$(OBJ)/sim.o:\
		sim.C
	$(CC) $(CFLAGS) -o $(OBJ)/sim.o -c \
		sim.C
#---------------------------------------------------------
sim: \
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/SententialForm.o \
		$(OBJ)/sim.o
	$(CC) $(LDFLAGS) -o sim \
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/SententialForm.o \
		$(OBJ)/sim.o \
		$(LIBS)
#--------------------------------------------------------
$(OBJ)/SCFG.o:\
		SCFG.C\
		SCFG.H
	$(CC) $(CFLAGS) -o $(OBJ)/SCFG.o -c \
		SCFG.C
#--------------------------------------------------------
$(OBJ)/ProductionSet.o:\
		ProductionSet.C\
		ProductionSet.H
	$(CC) $(CFLAGS) -o $(OBJ)/ProductionSet.o -c \
		ProductionSet.C
#---------------------------------------------------------
$(OBJ)/SententialForm.o:\
		SententialForm.C\
		SententialForm.H
	$(CC) $(CFLAGS) -o $(OBJ)/SententialForm.o -c \
		SententialForm.C
#---------------------------------------------------------
$(OBJ)/DerivesEpsilon.o:\
		DerivesEpsilon.C\
		DerivesEpsilon.H
	$(CC) $(CFLAGS) -o $(OBJ)/DerivesEpsilon.o -c \
		DerivesEpsilon.C
#---------------------------------------------------------
$(OBJ)/Terminates.o:\
		Terminates.C\
		Terminates.H
	$(CC) $(CFLAGS) -o $(OBJ)/Terminates.o -c \
		Terminates.C
#---------------------------------------------------------
$(OBJ)/ReachableFromStart.o:\
		ReachableFromStart.C\
		ReachableFromStart.H
	$(CC) $(CFLAGS) -o $(OBJ)/ReachableFromStart.o -c \
		ReachableFromStart.C
#---------------------------------------------------------
$(OBJ)/BinaryProduction.o:\
		BinaryProduction.C\
		BinaryProduction.H
	$(CC) $(CFLAGS) -o $(OBJ)/BinaryProduction.o -c \
		BinaryProduction.C
#---------------------------------------------------------
$(OBJ)/likelihood.o:\
		likelihood.C
	$(CC) $(CFLAGS) -o $(OBJ)/likelihood.o -c \
		likelihood.C
#---------------------------------------------------------
likelihood: \
		$(OBJ)/BinaryProductions.o\
		$(OBJ)/Inside.o\
		$(OBJ)/Outside.o \
		$(OBJ)/BinaryProduction.o\
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/likelihood.o
	$(CC) $(LDFLAGS) -o likelihood \
		$(OBJ)/BinaryProductions.o\
		$(OBJ)/Inside.o\
		$(OBJ)/Outside.o \
		$(OBJ)/BinaryProduction.o\
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/likelihood.o \
		$(LIBS)
#--------------------------------------------------------
$(OBJ)/Outside.o:\
		Outside.C\
		Outside.H
	$(CC) $(CFLAGS) -o $(OBJ)/Outside.o -c \
		Outside.C
#--------------------------------------------------------
$(OBJ)/Inside.o:\
		Inside.C\
		Inside.H
	$(CC) $(CFLAGS) -o $(OBJ)/Inside.o -c \
		Inside.C
#---------------------------------------------------------
$(OBJ)/BinaryProductions.o:\
		BinaryProductions.C\
		BinaryProductions.H
	$(CC) $(CFLAGS) -o $(OBJ)/BinaryProductions.o -c \
		BinaryProductions.C
#---------------------------------------------------------
$(OBJ)/train.o:\
		train.C
	$(CC) $(CFLAGS) -o $(OBJ)/train.o -c \
		train.C
#---------------------------------------------------------
train: \
		$(OBJ)/BinaryProductions.o\
		$(OBJ)/Inside.o\
		$(OBJ)/Outside.o \
		$(OBJ)/BinaryProduction.o\
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/train.o
	$(CC) $(LDFLAGS) -o train \
		$(OBJ)/BinaryProductions.o\
		$(OBJ)/Inside.o\
		$(OBJ)/Outside.o \
		$(OBJ)/BinaryProduction.o\
		$(OBJ)/Grammar.o \
		$(OBJ)/GrammarSymbol.o \
		$(OBJ)/GrammarAlphabet.o \
		$(OBJ)/SCFG_Parser.o \
		$(OBJ)/Production.o \
		$(OBJ)/ProductionSet.o \
		$(OBJ)/SCFG.o \
		$(OBJ)/train.o \
		$(LIBS)
#---------------------------------------------------------
