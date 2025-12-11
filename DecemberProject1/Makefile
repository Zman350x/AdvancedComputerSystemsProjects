.PHONY: all clean

SRCDIR=src
OBJDIR=$(SRCDIR)/obj
INCDIR=include
OUTDIR=output

CXX=g++

CXXFLAGS=-I$(INCDIR) -I/usr/local/include -Wall -std=c++23 -O3 -g

DEPS:=$(wildcard $(INCDIR)/*.h)
SRC:=$(wildcard $(SRCDIR)/*.cpp)
OBJ:=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXE:=$(SRC:$(SRCDIR)/%.cpp=$(OUTDIR)/%)

all: $(EXE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	mkdir -p $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OUTDIR)/%: $(OBJDIR)/%.o
	mkdir -p $(OUTDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ $(EXE)
