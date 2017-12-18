CXX ?= g++
CPPFLAGS = -I include
CXXFLAGS = -Wall -Wextra -std=c++11

OBJS = src/main.o

PROG = iplayer

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG)
