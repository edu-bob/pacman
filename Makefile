DLBG=./dlbg
TAR=KeyBoard5.01.c Makefile Makefile.m4 bg.h bitgraph.c \
	bithdwr.h blt.c board.c compat.h fonts fonts.c \
	fruit.c host.c host.h main.c monsters.c pacman.5.01 \
	pacman.5.02 pacman.c pacman.h prf.c restart sched.c \
	sounds.c style.h README 

all:	pacman.5.01 pacman.5.02

pacman.5.01:Makefile.5.01
	make semiclean
	make -f Makefile.5.01

pacman.5.02:Makefile.5.02
	make semiclean
	make -f Makefile.5.02

Makefile.5.02:Makefile.m4
	(echo 'define(MAJOR,5)';echo 'define(MINOR,02)')|cat - Makefile.m4|m4 > Makefile.5.02

Makefile.5.01:Makefile.m4
	(echo 'define(MAJOR,5)';echo 'define(MINOR,01)')|cat - Makefile.m4|m4 > Makefile.5.01

semiclean:
	rm -f *.o

clean:
	rm -f a.out *.o core main ,* .,* Makefile.5.*
	cd fonts; make clean

tar:
	@tar cf - ${TAR} 
