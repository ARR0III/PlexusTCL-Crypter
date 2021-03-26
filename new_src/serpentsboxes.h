#define RND00(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t05, t06, t07, t08, t09, t11, t12, t13, t14, t15, t17, t01;\
	t01 = b   ^ c  ; \
	t02 = a   | d  ; \
	t03 = a   ^ b  ; \
	z   = t02 ^ t01; \
	t05 = c   | z  ; \
	t06 = a   ^ d  ; \
	t07 = b   | c  ; \
	t08 = d   & t05; \
	t09 = t03 & t07; \
	y   = t09 ^ t08; \
	t11 = t09 & y  ; \
	t12 = c   ^ d  ; \
	t13 = t07 ^ t11; \
	t14 = b   & t06; \
	t15 = t06 ^ t13; \
	w   =     ~ t15; \
	t17 = w   ^ t14; \
	x   = t12 ^ t17; }

#define InvRND00(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t12, t13, t14, t15, t17, t18, t01;\
	t01 = c   ^ d  ; \
	t02 = a   | b  ; \
	t03 = b   | c  ; \
	t04 = c   & t01; \
	t05 = t02 ^ t01; \
	t06 = a   | t04; \
	y   =     ~ t05; \
	t08 = b   ^ d  ; \
	t09 = t03 & t08; \
	t10 = d   | y  ; \
	x   = t09 ^ t06; \
	t12 = a   | t05; \
	t13 = x   ^ t12; \
	t14 = t03 ^ t10; \
	t15 = a   ^ c  ; \
	z   = t14 ^ t13; \
	t17 = t05 & t13; \
	t18 = t14 | t17; \
	w   = t15 ^ t18; }

#define RND01(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t10, t11, t12, t13, t16, t17, t01;\
	t01 = a   | d  ; \
	t02 = c   ^ d  ; \
	t03 =     ~ b  ; \
	t04 = a   ^ c  ; \
	t05 = a   | t03; \
	t06 = d   & t04; \
	t07 = t01 & t02; \
	t08 = b   | t06; \
	y   = t02 ^ t05; \
	t10 = t07 ^ t08; \
	t11 = t01 ^ t10; \
	t12 = y   ^ t11; \
	t13 = b   & d  ; \
	z   =     ~ t10; \
	x   = t13 ^ t12; \
	t16 = t10 | x  ; \
	t17 = t05 & t16; \
	w   = c   ^ t17; }

#define InvRND01(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t10, t11, t14, t15, t17, t01;\
	t01 = a   ^ b  ; \
	t02 = b   | d  ; \
	t03 = a   & c  ; \
	t04 = c   ^ t02; \
	t05 = a   | t04; \
	t06 = t01 & t05; \
	t07 = d   | t03; \
	t08 = b   ^ t06; \
	t09 = t07 ^ t06; \
	t10 = t04 | t03; \
	t11 = d   & t08; \
	y   =     ~ t09; \
	x   = t10 ^ t11; \
	t14 = a   | y  ; \
	t15 = t06 ^ x  ; \
	z   = t01 ^ t04; \
	t17 = c   ^ t15; \
	w   = t14 ^ t17; }

#define RND02(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t05, t06, t07, t08, t09, t10, t12, t13, t14, t01;\
	t01 = a   | c  ; \
	t02 = a   ^ b  ; \
	t03 = d   ^ t01; \
	w   = t02 ^ t03; \
	t05 = c   ^ w  ; \
	t06 = b   ^ t05; \
	t07 = b   | t05; \
	t08 = t01 & t06; \
	t09 = t03 ^ t07; \
	t10 = t02 | t09; \
	x   = t10 ^ t08; \
	t12 = a   | d  ; \
	t13 = t09 ^ x  ; \
	t14 = b   ^ t13; \
	z   =     ~ t09; \
	y   = t12 ^ t14; }

#define InvRND02(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t06, t07, t08, t09, t10, t11, t12, t15, t16, t17, t01;\
	t01 = a   ^ d  ; \
	t02 = c   ^ d  ; \
	t03 = a   & c  ; \
	t04 = b   | t02; \
	w   = t01 ^ t04; \
	t06 = a   | c  ; \
	t07 = d   | w  ; \
	t08 =     ~ d  ; \
	t09 = b   & t06; \
	t10 = t08 | t03; \
	t11 = b   & t07; \
	t12 = t06 & t02; \
	z   = t09 ^ t10; \
	x   = t12 ^ t11; \
	t15 = c   & z  ; \
	t16 = w   ^ x  ; \
	t17 = t10 ^ t15; \
	y   = t16 ^ t17; }

#define RND03(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t10, t11, t13, t14, t15, t01;\
	t01 = a   ^ c  ; \
	t02 = a   | d  ; \
	t03 = a   & d  ; \
	t04 = t01 & t02; \
	t05 = b   | t03; \
	t06 = a   & b  ; \
	t07 = d   ^ t04; \
	t08 = c   | t06; \
	t09 = b   ^ t07; \
	t10 = d   & t05; \
	t11 = t02 ^ t10; \
	z   = t08 ^ t09; \
	t13 = d   | z  ; \
	t14 = a   | t07; \
	t15 = b   & t13; \
	y   = t08 ^ t11; \
	w   = t14 ^ t15; \
	x   = t05 ^ t04; }

#define InvRND03(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t09, t11, t12, t13, t14, t16, t01;\
	t01 = c   | d  ; \
	t02 = a   | d  ; \
	t03 = c   ^ t02; \
	t04 = b   ^ t02; \
	t05 = a   ^ d  ; \
	t06 = t04 & t03; \
	t07 = b   & t01; \
	y   = t05 ^ t06; \
	t09 = a   ^ t03; \
	w   = t07 ^ t03; \
	t11 = w   | t05; \
	t12 = t09 & t11; \
	t13 = a   & y  ; \
	t14 = t01 ^ t05; \
	x   = b   ^ t12; \
	t16 = b   | t13; \
	z   = t14 ^ t16; }

#define RND04(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t11, t12, t13, t14, t15, t16, t01;\
	t01 = a   | b  ; \
	t02 = b   | c  ; \
	t03 = a   ^ t02; \
	t04 = b   ^ d  ; \
	t05 = d   | t03; \
	t06 = d   & t01; \
	z   = t03 ^ t06; \
	t08 = z   & t04; \
	t09 = t04 & t05; \
	t10 = c   ^ t06; \
	t11 = b   & c  ; \
	t12 = t04 ^ t08; \
	t13 = t11 | t03; \
	t14 = t10 ^ t09; \
	t15 = a   & t05; \
	t16 = t11 | t12; \
	y   = t13 ^ t08; \
	x   = t15 ^ t16; \
	w   =     ~ t14; }

#define InvRND04(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t09, t10, t11, t12, t13, t15, t01;\
	t01 = b   | d  ; \
	t02 = c   | d  ; \
	t03 = a   & t01; \
	t04 = b   ^ t02; \
	t05 = c   ^ d  ; \
	t06 =     ~ t03; \
	t07 = a   & t04; \
	x   = t05 ^ t07; \
	t09 = x   | t06; \
	t10 = a   ^ t07; \
	t11 = t01 ^ t09; \
	t12 = d   ^ t04; \
	t13 = c   | t10; \
	z   = t03 ^ t12; \
	t15 = a   ^ t04; \
	y   = t11 ^ t13; \
	w   = t15 ^ t09; }

#define RND05(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t11, t12, t13, t14, t01;\
	t01 = b   ^ d  ; \
	t02 = b   | d  ; \
	t03 = a   & t01; \
	t04 = c   ^ t02; \
	t05 = t03 ^ t04; \
	w   =     ~ t05; \
	t07 = a   ^ t01; \
	t08 = d   | w  ; \
	t09 = b   | t05; \
	t10 = d   ^ t08; \
	t11 = b   | t07; \
	t12 = t03 | w  ; \
	t13 = t07 | t10; \
	t14 = t01 ^ t11; \
	y   = t09 ^ t13; \
	x   = t07 ^ t08; \
	z   = t12 ^ t14; }

#define InvRND05(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t12, t13, t15, t16, t01;\
	t01 = a   & d  ; \
	t02 = c   ^ t01; \
	t03 = a   ^ d  ; \
	t04 = b   & t02; \
	t05 = a   & c  ; \
	w   = t03 ^ t04; \
	t07 = a   & w  ; \
	t08 = t01 ^ w  ; \
	t09 = b   | t05; \
	t10 =     ~ b  ; \
	x   = t08 ^ t09; \
	t12 = t10 | t07; \
	t13 = w   | x  ; \
	z   = t02 ^ t12; \
	t15 = t02 ^ t13; \
	t16 = b   ^ d  ; \
	y   = t16 ^ t15; }

#define RND06(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t11, t12, t13, t15, t17, t18, t01;\
	t01 = a   & d  ; \
	t02 = b   ^ c  ; \
	t03 = a   ^ d  ; \
	t04 = t01 ^ t02; \
	t05 = b   | c  ; \
	x   =     ~ t04; \
	t07 = t03 & t05; \
	t08 = b   & x  ; \
	t09 = a   | c  ; \
	t10 = t07 ^ t08; \
	t11 = b   | d  ; \
	t12 = c   ^ t11; \
	t13 = t09 ^ t10; \
	y   =     ~ t13; \
	t15 = x   & t03; \
	z   = t12 ^ t07; \
	t17 = a   ^ b  ; \
	t18 = y   ^ t15; \
	w   = t17 ^ t18; }

#define InvRND06(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t12, t13, t14, t15, t16, t17, t01;\
	t01 = a   ^ c  ; \
	t02 =     ~ c  ; \
	t03 = b   & t01; \
	t04 = b   | t02; \
	t05 = d   | t03; \
	t06 = b   ^ d  ; \
	t07 = a   & t04; \
	t08 = a   | t02; \
	t09 = t07 ^ t05; \
	x   = t06 ^ t08; \
	w   =     ~ t09; \
	t12 = b   & w  ; \
	t13 = t01 & t05; \
	t14 = t01 ^ t12; \
	t15 = t07 ^ t13; \
	t16 = d   | t02; \
	t17 = a   ^ x  ; \
	z   = t17 ^ t15; \
	y   = t16 ^ t14; }

#define RND07(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t11, t13, t14, t15, t16, t17, t01;\
	t01 = a   & c  ; \
	t02 =     ~ d  ; \
	t03 = a   & t02; \
	t04 = b   | t01; \
	t05 = a   & b  ; \
	t06 = c   ^ t04; \
	z   = t03 ^ t06; \
	t08 = c   | z  ; \
	t09 = d   | t05; \
	t10 = a   ^ t08; \
	t11 = t04 & z  ; \
	x   = t09 ^ t10; \
	t13 = b   ^ x  ; \
	t14 = t01 ^ x  ; \
	t15 = c   ^ t05; \
	t16 = t11 | t13; \
	t17 = t02 | t14; \
	w   = t15 ^ t17; \
	y   = a   ^ t16; }

#define InvRND07(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t06, t07, t08, t09, t10, t11, t13, t14, t15, t16, t01;\
	t01 = a   & b  ; \
	t02 = a   | b  ; \
	t03 = c   | t01; \
	t04 = d   & t02; \
	z   = t03 ^ t04; \
	t06 = b   ^ t04; \
	t07 = d   ^ z  ; \
	t08 =     ~ t07; \
	t09 = t06 | t08; \
	t10 = b   ^ d  ; \
	t11 = a   | d  ; \
	x   = a   ^ t09; \
	t13 = c   ^ t06; \
	t14 = c   & t11; \
	t15 = d   | x  ; \
	t16 = t01 | t10; \
	w   = t13 ^ t15; \
	y   = t14 ^ t16; }

#define RND08(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND09(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND10(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND11(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND12(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND13(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND14(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND15(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)
#define RND16(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND17(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND18(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND19(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND20(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND21(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND22(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND23(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)
#define RND24(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND25(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND26(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND27(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND28(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND29(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND30(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND31(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)

#define InvRND08(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND09(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND10(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND11(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND12(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND13(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND14(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND15(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)
#define InvRND16(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND17(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND18(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND19(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND20(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND21(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND22(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND23(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)
#define InvRND24(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND25(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND26(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND27(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND28(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND29(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND30(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND31(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)

#define ROL(x,n) ((((unsigned long)(x))<<(n))| \
                  (((unsigned long)(x))>>(32-(n))))

#define ROR(x,n) ((((unsigned long)(x))<<(32-(n)))| \
                  (((unsigned long)(x))>>(n)))

#define transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y0 = ROL(x0, 13); \
      y2 = ROL(x2, 3); \
      y1 = x1 ^ y0 ^ y2; \
      y3 = x3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = ROL(y1, 1); \
      y3 = ROL(y3, 7); \
      y0 = y0 ^ y1 ^ y3; \
      y2 = y2 ^ y3 ^ ((unsigned long)y1<<7); \
      y0 = ROL(y0, 5); \
      y2 = ROL(y2, 22)

#define inv_transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y2 = ROR(x2, 22);\
      y0 = ROR(x0, 5); \
      y2 = y2 ^ x3 ^ ((unsigned long)x1<<7); \
      y0 = y0 ^ x1 ^ x3; \
      y3 = ROR(x3, 7); \
      y1 = ROR(x1, 1); \
      y3 = y3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = y1 ^ y0 ^ y2; \
      y2 = ROR(y2, 3); \
      y0 = ROR(y0, 13)

#define keying(x0, x1, x2, x3, subkey) \
                         x0 ^= subkey[0];x1 ^= subkey[1]; \
                         x2 ^= subkey[2];x3 ^= subkey[3]

#define PHI 0x9e3779b9L
