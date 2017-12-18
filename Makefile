CXX ?= g++
CPPFLAGS = -I include
CXXFLAGS = -Wall -Wextra -std=c++11 -g

OBJS = src/song.o \
       src/library.o \
       src/playlist.o \
       src/playback.o \
       src/player.o \
       src/command.o \
       src/main.o

LIBS = -lpthread

PROG = iplayer

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^ ${LIBS}

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG)
