#if V5_01 || V3_10 || V5_02
#define p1(a) ((int)(a)<<16)
#define p2(a, b) (((int)(a)<<16)|(b))
#else
#define p1(a) a
#define p2(a, b) a, b
#endif

#define PsgWrite(a, b) psgwrite(p2(a, b))
#define PsgSweep(a, b, c, d) psgsweep(p2(a, b), p2(c, d))
#define DoHvp(a, b) dohvp(p2(a, b))
#define SetOp(a) setop(p1(a))
#define RastDisplay(a) rastdisplay(a)
#define DoSetBG(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af) dosetbg(p2(a0,a1),p2(a2,a3),p2(a4,a5),p2(a6,a7),p2(a8,a9),p2(aa,ab),p2(ac,ad),p2(ae,af))
#define DoRsetBG(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af) dorsetbg(p2(a0,a1),p2(a2,a3),p2(a4,a5),p2(a6,a7),p2(a8,a9),p2(aa,ab),p2(ac,ad),p2(ae,af))

#if V5_01 || V5_02
#define Getc KbdGetc
#else
#define Getc kbdgetc
#endif
