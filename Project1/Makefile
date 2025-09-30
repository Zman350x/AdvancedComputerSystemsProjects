.SECONDARY:
SRCDIR=src
OBJDIR_BASE=$(SRCDIR)/obj-baseline
OBJDIR_VECT=$(SRCDIR)/obj-vectorized
ASMDIR_BASE=$(SRCDIR)/asm-baseline
ASMDIR_VECT=$(SRCDIR)/asm-vectorized
INCDIR=include
OUTDIR=output

CXX=g++

BASELINE_CXXFLAGS=-fno-tree-vectorize
VECTORIZED_CXXFLAGS=-ftree-vectorize -march=native
COMMON_CXXFLAGS=-I$(INCDIR) -I/usr/local/include -Wall -std=c++23 -O3
BUILD_FLAGS=-g

ifdef DEF
USEDEF=-D $(DEF)
else
USEDEF=
endif

FILES=experiment1 experiment2

all: \
	$(patsubst %,$(OUTDIR)/%-baseline,$(FILES)) \
	$(patsubst %,$(OUTDIR)/%-vectorized,$(FILES)) \
	$(patsubst %,$(ASMDIR_BASE)/%.s,$(FILES)) \
	$(patsubst %,$(ASMDIR_VECT)/%.s,$(FILES))

$(OBJDIR_BASE)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR_BASE)
	$(CXX) -c -o $@ $< $(COMMON_CXXFLAGS) $(BASELINE_CXXFLAGS) $(BUILD_FLAGS) $(USEDEF)

$(OBJDIR_VECT)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR_VECT)
	$(CXX) -c -o $@ $< $(COMMON_CXXFLAGS) $(VECTORIZED_CXXFLAGS) $(BUILD_FLAGS) $(USEDEF)

$(ASMDIR_BASE)/%.s: $(SRCDIR)/%.cpp
	mkdir -p $(ASMDIR_BASE)
	$(CXX) -S -fverbose-asm -o $@ $< $(COMMON_CXXFLAGS) $(BASELINE_CXXFLAGS) $(USEDEF)

$(ASMDIR_VECT)/%.s: $(SRCDIR)/%.cpp
	mkdir -p $(ASMDIR_VECT)
	$(CXX) -S -fverbose-asm -o $@ $< $(COMMON_CXXFLAGS) $(VECTORIZED_CXXFLAGS) $(USEDEF)

$(OUTDIR)/%-baseline: $(OBJDIR_BASE)/%.o
	mkdir -p $(OUTDIR)
	$(CXX) -o $@ $^ $(COMMON_CXXFLAGS) $(BASELINE_CXXFLAGS) $(BUILD_FLAGS) $(USEDEF)

$(OUTDIR)/%-vectorized: $(OBJDIR_VECT)/%.o
	mkdir -p $(OUTDIR)
	$(CXX) -o $@ $^ $(COMMON_CXXFLAGS) $(VECTORIZED_CXXFLAGS) $(BUILD_FLAGS) $(USEDEF)

.PHONY: clean

clean:
	rm -rf $(OBJDIR_BASE) $(OBJDIR_VECT) $(ASMDIR_BASE) $(ASMDIR_VECT) *~ core $(INCDIR)/*~ $(OUTDIR)/*-baseline $(OUTDIR)/*-vectorized
