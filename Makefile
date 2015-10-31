CXXFLAGS = -g -std=c++14
LDFLAGS = -lgit2

all : zapgit

zapgit : zapgit.cc zg_util.h
	$(LINK.cc) $< $(LOADLIBES) $(LDLIBS) -o $@
