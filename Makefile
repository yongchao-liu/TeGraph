
ROOT= $(shell pwd)
SRCDIR=./apps
BINDIR=./bin/apps

CXX?= g++
CXXFLAGS?= -O3 -g -std=c++11 -g -fopenmp -I$(ROOT) 
HEADERS= $(shell find . -name '*.hpp')

# ---- apps ---- #
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
APPS= $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%, $(SOURCES))

# ---- tools ---- #
TSRCDIR=./tools
TBINDIR=./bin/tools

TSOURCES= $(wildcard $(TSRCDIR)/*.cpp)
TOOLS= $(patsubst $(TSRCDIR)/%.cpp, $(TBINDIR)/%, $(TSOURCES))

# ---- compile ---- #
all: default $(APPS) $(TOOLS)

default: 
	mkdir -p $(BINDIR) $(TBINDIR)

$(BINDIR)/% : $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

$(TBINDIR)/% : $(TSRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

# ---- clean ---- #
clean:
	rm -rf $(APPS) $(TOOLS)
