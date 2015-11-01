CXXFLAGS = -g -std=c++14
LDFLAGS = -lgit2

all : zapgit

zapgit : zapgit.cc git2pp.h
	$(LINK.cc) $< $(LOADLIBES) $(LDLIBS) -o $@
