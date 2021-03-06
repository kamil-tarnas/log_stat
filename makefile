appname := log_stat.out

CXX := g++
CXXFLAGS := -Wall -g3
CXXFLAGS := -Wall -std=c++11 -static-libstdc++ -static-libgcc

srcfiles := $(shell find . -maxdepth 2 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	    $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	    rm -f ./.depend
	        $(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	    rm -f $(objects)

dist-clean: clean
	   rm -f *~ .depend

include .depend
