#
#
#  Makefile for Pacman
#
define(cat,$1$2$3$4)
define(VERSION, cat(MAJOR,`.',MINOR))
VERS=VERSION
#
# C preprocessor flags...
#
CPPFLAGS=cat(`-DV',MAJOR,`_',MINOR)
ADDR=240000
LDFLAGS=-A ../symbols/bg${VERS}/gsyms -e _main -N -Ttext 
LDLIBES=../libbg/libbg.a
CFLAGS=-O
DLBG=../bin/dlbg

OBJS=main.o pacman.o board.o blt.o prf.o monsters.o\
       sounds.o sched.o fruit.o host.o bitgraph.o
SRCS=main.c pacman.c board.c blt.c monsters.c fruit.c sched.c\
       sounds.c host.c fonts.c prf.c bitgraph.c

all:	pacman.VERSION

pacman.VERSION:$(OBJS) fonts.o 
	$(LD) $(LDFLAGS) ${ADDR} $(OBJS) fonts.o $(LDLIBES) -o pacman
	${DLBG} -l${ADDR} -s${ADDR} pacman > pacman.VERSION

clean:
	rm -f a.out *.o core main ,* .,*
	cd fonts; make clean

.c.o:
	/lib/cpp $(CPPFLAGS) $< | /lib/ccom -m68010 -fsoft|/lib/c2 -10 > $*.s && \
	as -o $@ -10 $*.s ; rm $*.s
