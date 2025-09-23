SRCDIR=src
OBJDIR=$(SRCDIR)/obj
LIBDIR=lib
INCDIR=include
OUTDIR=output

CXX=g++
CXXFLAGS=-I$(INCDIR) -I/usr/local/include -Wall -std=c++23 -g

LIBS = -L/usr/local/lib -Wl,-rpath=/usr/local/lib

ifdef DEF
USEDEF=-D $(DEF)
else
USEDEF=
endif

_DEPS=
DEPS=$(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ=main.o
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

EXE=main

all: $(EXE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(USEDEF)

$(EXE): $(OBJ)
	$(CXX) -o $(OUTDIR)/$@ $^ $(CXXFLAGS) $(LIBS) $(USEDEF)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~ $(OUTDIR)/$(EXE)
