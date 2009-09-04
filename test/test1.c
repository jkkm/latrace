#if 0

#  Copyright (C) 2009 Akos Pasztory <akos.pasztory@gmail.com>
#
#  This file is part of the latrace.
#
#  The latrace is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  The latrace is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the latrace (file COPYING).  If not, see 
#  <http://www.gnu.org/licenses/>.

b="${0%.c}";
cc -Wall -g -shared -fPIC -DTHE_SO -DTHE_CONFIG $0 -o "$b.so";
cc -Wall -g -DTHE_APP -DTHE_CONFIG -Wl,-rpath . "$b.so" $0 -o "$b.bin";
cpp -P -DTHE_CONFIG $0 > "$b.conf";
latrace -a "$b.conf" -D "./$b.bin" | less;
exit 0;
#endif
#if defined(THE_APP) || defined(THE_SO)
#include <stdio.h>

typedef unsigned long long u_llong;
typedef long long llong;

#endif
#ifdef THE_CONFIG

struct st1 {
	char a; char b;
};
struct st2 {
	int a; int b; int c; int d;
};
struct st3 {
	int a; int b; int c; int d; int e; int f;
};
struct st4 {
	int x; char y; int z;
};
struct st5 {
	short a; int b; char c; int d; short e; char f; int g; char h; char i; short j; int k;
};
struct st6 {
	char a; double b; short c; float d;
};
struct st7 {
	double a;
};
struct st8 {
	double a; char b; int c;
};

extern void a1(char x);
void a2(short x);
void a3(int x);
void a4(llong x);

void b1(char x, int y);
void b2(int x, char y);
void b3(int x, int y, int z, int q, int xx, int yy);

struct st3 c1(int x);
const struct st3 *c2(int x, char y, double z);
char c3(int x, double y, char z);
struct st1 c4(int x, double y);
struct st7 c5(double x, double y);
struct st8 c6(float x, double y);
struct st4 c7(char x, long b);

u_llong d1(void);
void d2(u_llong x);
void d3(int a, u_llong x);
void d4(struct st3 x);
void d5(char a, int y, struct st3 x);
double d6(char a, int y);

int l1(int x);
int l2(int x, int y);
int l3(int x, int y, int z);
int l4(int x, int y, int z, int q);
int l5(int x, int y, int z, int q, int xx);
int l6(int x, int y, int z, int q, int xx, int yy);
int l7(int x, int y, int z, int q, int xx, int yy, int zz);
int l8(int x, int y, int z, int q, int xx, int yy, int zz, int qq);

void s1(struct st1 x);
void s1p(struct st1 *x);
void s2(struct st2 x);
void s3(struct st3 x);
void s4(struct st4 x);
void s5(struct st5 x);
void s6(struct st6 x);

struct st1 r1(void);
struct st3 r3(void);
struct st4 r4(void);

void q1(int x, struct st1 y);
void q2(int x, char y, struct st1 z);
void q3(int x, int y, struct st2 z);
void q4(int x, int y, struct st4 z);
void q5(int x, int y, char z, char q, struct st4 w);

int real1(const char *w);
void real2(void);

#endif
#if defined(THE_APP) || defined(THE_SO)

static struct st1 sx = {'a', 'b'};
static struct st2 sy = {10, 20, 30, 40};
static struct st3 sz = {10, 20, 30, 40, 50, 60};
static struct st4 sq = {10, 'x', 30};
static struct st5 sa = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static struct st6 sb = {1, 2, 3, 4};
#endif
#ifdef THE_SO

void real2(void) { puts("i am beta"); }
int real1(const char *w) { puts(w); real2(); return 3; }

u_llong d1(void) { return 1LL + (1LL << 32); }
void d2(u_llong x) { x;}
void d3(int a, u_llong x) { a+x;}
void d4(struct st3 x) { x; }
void d5(char c, int y, struct st3 x) { x; }
double d6(char a, int y) { return 1.0; }

struct st3 c1(int x)
{
	static struct st3 e = {5,6,7,8,9,10};
	return e;
}

const struct st3 *c2(int x, char y, double z)
{
	static struct st3 e = {5,6,7,8,9,10};
	return &e;
}

char c3(int x, double y, char z)
{
	return z+1;
}

struct st1 c4(int x, double y)
{
	struct st1 p = { 'x', 'y' };
	return p;
}

struct st7 c5(double x, double y)
{
	struct st7 p = { 1 };
	return p;
}

struct st8 c6(float x, double y)
{
	struct st8 p = { 1, 'w', 0x10 };
	return p;
}

struct st4 c7(char x, long b)
{
	struct st4 p = { 1, 'a', 2 };
	return p;
}

int l1(int x) { return 2*x; }
int l2(int x, int y) { return 3*y; }
int l3(int x, int y, int z) { return x+y+z; }
int l4(int x, int y, int z, int q) { return x+y+z; }
int l5(int x, int y, int z, int q, int xx) { return x+y+z; }
int l6(int x, int y, int z, int q, int xx, int yy) { return x+y+z; }
int l7(int x, int y, int z, int q, int xx, int yy, int zz) { return x+y+z; }
int l8(int x, int y, int z, int q, int xx, int yy, int zz, int qq) { return x+y+z; }

void a1(char x) {}
void a2(short x) {}
void a3(int x) {}
void a4(llong x) {}

void b1(char x, int y) {}
void b2(int x, char y) {}
void b3(int x, int y, int z, int q, int xx, int yy) {}

void s1(struct st1 x) {}
void s1p(struct st1 *x) {}
void s2(struct st2 x) {}
void s3(struct st3 x) {}
void s4(struct st4 x) {}
void s5(struct st5 x) {}
void s6(struct st6 x) {}

struct st1 r1(void) { return sx; }
struct st3 r3(void) { return sz; }
struct st4 r4(void) { return sq; }

void q1(int x, struct st1 y) {}
void q2(int x, char y, struct st1 z) {}
void q3(int x, int y, struct st2 z) {}
void q4(int x, int y, struct st4 z) {}
void q5(int x, int y, char z, char q, struct st4 w) {}


#endif
#ifdef THE_APP
int main(void)
{
	static const struct st3 a = {1, 2, 3, 4, 5, 6};

	l1(100);
	l2(100, 200);
	l3(100, 200, 300);
	l4(100, 200, 300, 400);
	l5(100, 200, 300, 400, 500);
	l6(100, 200, 300, 400, 500, 600);
	l7(100, 200, 300, 400, 500, 600, 700);
	l8(100, 200, 300, 400, 500, 600, 700, 800);

	d1();
	d2(43);
	d3(100, 200);
	d4(a);
	d5('a', 43, a);
	d6('a', 1);

	c1(44);
	c2(100, 'a', 3.4);
	c3(200, 2.777, 'q');
	c4(200, 1);
	c5(1.1, 2.2);
	c6(1.23, 45.6);
	c7('z', 0x200);

	a1('a');
	a2(10);
	a3(20);
	a4(102030405060LL);

	b1('a', 20);
	b2(30, 'b');
	b3(10, 20, 30, 40, 50, 60);

	s1(sx);
	s1p(&sx);
	s2(sy);
	s3(sz);
	s4(sq);
	s5(sa);
	s6(sb);

	r1();
	r3();
	r4();

	q1(200, sx);
	q2(300, 't', sx);
	q3(400, 410, sy);
	q4(500, 510, sq);
	q5(600, 610, 'z', 'q', sq);

	real1("fresh air");
	real2();

	return 0;
}
#endif
