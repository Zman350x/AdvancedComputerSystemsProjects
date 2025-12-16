.PHONY: all clean

SRCDIR=src
OBJDIR=$(SRCDIR)/obj
INCDIR=include
OUTDIR=output

CXX=g++

CXXFLAGS=-I$(INCDIR) -I/usr/local/include -Wall -std=c++23 -O3 -march=native -g

DEPS:=$(wildcard $(INCDIR)/*.h)
SRC:=$(wildcard $(SRCDIR)/*.cpp)
OBJ:=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
# EXE:=$(SRC:$(SRCDIR)/%.cpp=$(OUTDIR)/%)
EXE:=$(OUTDIR)/main

all: $(EXE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	mkdir -p $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OUTDIR)/%: $(OBJ)
	mkdir -p $(OUTDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ $(EXE)
