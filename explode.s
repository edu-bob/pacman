	.data
	.comm	_Pacman,56
	.comm	_Fruit,20
	.comm	_Seekprob,4
	.comm	_Seekinc,4
	.comm	_Silent,1
	.comm	_Wave,4
	.comm	_Dotsrem,4
	.comm	_Monseeking,1
	.comm	_Monvalue,4
	.comm	_Pacmen,4
	.comm	_Monbase,4
	.comm	_VolumeControl,4
	.comm	_Wallsoff,1
	.comm	_piece,1920
	.text
LL0:
|#PROC# 04
	LF43	=	32
	LS43	=	8420
	LFF43	=	12
	LSS43	=	0
	LP43	=	16
	.data
	.even
	.globl	_Font
_Font:
	.word	65535
	.word	65535
	.word	65535
	.word	65535
	.text
	.globl	_explode
_explode:
|#PROLOGUE# 0
	link	a6,#-32
	moveml	#8420,sp@
|#PROLOGUE# 1
	movl	a6@(12),d7
	movl	a6@(8),d6
	moveq	#0,d5
LY00001:
	movl	d5,d0
	asll	#3,d0
	movl	d0,d1
	addl	d1,d1
	addl	d1,d0
	addl	#_piece,d0
	movl	d0,a5
L50:
	pea	100
	pea	-100
	jbsr	_grand
	addqw	#8,sp
	movl	d0,a6@(-8)
	pea	100
	pea	-100
	jbsr	_grand
	addqw	#8,sp
	movl	d0,a6@(-12)
	movl	a6@(-8),d0
	movl	d0,d1
	jsr	lmult
	movl	d0,d2
	movl	a6@(-12),d0
	movl	d0,d1
	jsr	lmult
	addl	d0,d2
	cmpl	#10000,d2
	jgt	L50
	addl	d7,a6@(-8)
	addl	d6,a6@(-12)
	movl	a6@(-8),d0
	subl	d7,d0
	movl	d0,a5@
	movl	a6@(-12),d0
	subl	d6,d0
	movl	d0,a5@(4)
	movl	d7,a5@(16)
	movl	d6,a5@(20)
	movl	a5@(16),a5@(8)
	movl	a5@(20),a5@(12)
	addql	#1,d5
	moveq	#80,d2
	cmpl	d2,d5
	jlt	LY00001
	jbsr	_fruitsound
	jbsr	_rsetdead
	moveq	#0,d5
LY00003:
	movl	d5,d0
	asll	#3,d0
	movl	d0,d1
	addl	d1,d1
	addl	d1,d0
	addl	#_piece,d0
	movl	d0,sp@-
	jbsr	_turnon
	addqw	#4,sp
	addql	#1,d5
	moveq	#80,d7
	cmpl	d7,d5
	jlt	LY00003
	clrl	a6@(-4)
LY00005:
	moveq	#0,d5
LY00009:
	movl	d5,d0
	asll	#3,d0
	movl	d0,d1
	addl	d1,d1
	addl	d1,d0
	addl	#_piece,d0
	movl	d0,a5
	tstl	a6@(-4)
	jeq	L64
	pea	a5@
	jbsr	_turnoff
	addqw	#4,sp
L64:
	movl	a5@,d0
	movl	a6@(-4),d1
	jsr	lmult
	moveq	#10,d1
	jsr	ldivt
	addl	a5@(16),d0
	movl	d0,a5@(8)
	movl	a5@(4),d0
	movl	a6@(-4),d1
	jsr	lmult
	moveq	#10,d1
	jsr	ldivt
	addl	a5@(20),d0
	movl	d0,a5@(12)
	pea	a5@
	jbsr	_turnon
	addqw	#4,sp
	addql	#1,d5
	moveq	#80,d7
	cmpl	d7,d5
	jlt	LY00009
	jbsr	_rsetdead
	addql	#1,a6@(-4)
	cmpl	#11,a6@(-4)
	jlt	LY00005
	moveq	#0,d5
LY00007:
	movl	d5,d0
	asll	#3,d0
	movl	d0,d1
	addl	d1,d1
	addl	d1,d0
	addl	#_piece,d0
	movl	d0,sp@-
	jbsr	_turnoff
	addqw	#4,sp
	addql	#1,d5
	moveq	#80,d7
	cmpl	d7,d5
	jlt	LY00007
	moveml	a6@(-32),#8420
	unlk	a6
	rts
|#PROC# 04
	LF70	=	24
	LS70	=	15360
	LFF70	=	8
	LSS70	=	0
	LP70	=	8
	.data
	.text
	.globl	_turnon
_turnon:
|#PROLOGUE# 0
	link	a6,#-24
	moveml	#15360,sp@
|#PROLOGUE# 1
	movl	a6@(8),a5
	movl	a5@(8),d0
	subql	#2,d0
	movl	d0,a6@(-8)
	movl	a5@(12),d0
	addql	#2,d0
	movl	d0,a6@(-4)
	cmpl	#96,a5@(8)
	jge	LE70
	tstl	a5@(8)
	jlt	LE70
	cmpl	#128,a5@(12)
	jge	LE70
	tstl	a5@(12)
	jlt	LE70
	movl	a6@(-8),d0
	addql	#6,d0
	asll	#3,d0
	asll	#7,d0
	movl	#128,d1
	subl	a6@(-4),d1
	subql	#6,d1
	addl	d1,d0
	addl	#2490368,d0
	movl	d0,a4
	lea	a4@(1024),a3
	movl	#_Font,a2
	jra	LY00010
LY00011:
	movb	a2@+,d0
	eorb	d0,a4@
	lea	a4@(128),a4
LY00010:
	cmpl	a3,a4
	jcs	LY00011
LE70:
	moveml	a6@(-24),#15360
	unlk	a6
	rts
|#PROC# 04
	LF76	=	24
	LS76	=	15360
	LFF76	=	8
	LSS76	=	0
	LP76	=	8
	.data
	.text
	.globl	_turnoff
_turnoff:
|#PROLOGUE# 0
	link	a6,#-24
	moveml	#15360,sp@
|#PROLOGUE# 1
	movl	a6@(8),a5
	movl	a5@(8),d0
	subql	#2,d0
	movl	d0,a6@(-8)
	movl	a5@(12),d0
	addql	#2,d0
	movl	d0,a6@(-4)
	cmpl	#96,a5@(8)
	jge	LE76
	tstl	a5@(8)
	jlt	LE76
	cmpl	#128,a5@(12)
	jge	LE76
	tstl	a5@(12)
	jlt	LE76
	movl	a6@(-8),d0
	addql	#6,d0
	asll	#3,d0
	asll	#7,d0
	movl	#128,d1
	subl	a6@(-4),d1
	subql	#6,d1
	addl	d1,d0
	addl	#2490368,d0
	movl	d0,a4
	lea	a4@(1024),a3
	movl	#_Font,a2
	jra	LY00012
LY00013:
	movb	a2@+,d0
	eorb	d0,a4@
	lea	a4@(128),a4
LY00012:
	cmpl	a3,a4
	jcs	LY00013
LE76:
	moveml	a6@(-24),#15360
	unlk	a6
	rts
