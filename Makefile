LIBS = -ldl -lm -lcaenhvwrapper
CFLAGS = -DUNIX -DLINUX -pthread -g
INC = -Iinclude

PROGS=$(patsubst %.c, %, $(wildcard src/*.c))
PROGOBJ=$(patsubst %, %.o, $(PROGS))


all: $(PROGS)

$(PROGS): %: %.o
	gcc $< -o $@ $(CFLAGS) $(LIBS) 

$(PROGOBJ): %.o: %.c
	gcc -c $< -o $@ $(CFLAGS) $(INC)

