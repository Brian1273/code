
/* Copyright (c) Brian Dunn 2016  */
/* ilog2() implementations with microbenchmark framework. */
/* Bart Massey code converted for use */

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

/*
 * A block of random values for ilog2() to repeatedly
 * operate on. Size chosen to be about 1/4 the L1 d-cache
 * size on my machine.
 */
#define BLOCKSIZE 1000
uint64_t randoms[BLOCKSIZE];

/*
 * XXX Because the ilog2 routine wants to be inlined in the
 * loop, we need to expand each ilog2 routine in its own
 * driver. The ## is all about CPP token-pasting.
 */
#define DRIVER(NAME) \
  int drive_##NAME(int n) {                                  \
    int i, j;                                                \
    int result = 0;                                          \
    for (j = 0; j < n; j++)                                  \
	for (i = 0; i < BLOCKSIZE; i++)                      \
	    result += ilog2_##NAME(randoms[i] ^ result);     \
    return result;                                           \
  }

/* Baseline: naive right-to-left scan. */
static inline
int ilog2_naive_r(uint64_t x) {
	int result;
	assert(x > 0);
	for (result = -1; x > 0; result++)
		x >>= 1;
	return result;
}
DRIVER(naive_r)
/* **********************************************************/
/* **********************************************************/

/* Naive left-to-right scan. */
static inline
int ilog2_naive_l(uint64_t x) {
	/* XXX Insert your code here. */
	int result ;	/*Starts at bit 63*/
	uint64_t mask = 0x8000000000000000;	/*Mask for the bit[63] */
	assert(x > 0);		/*Undefined for 0*/
	for (result = 63; !(x & mask); result--) {	/*Start loop, Checks each bit from left-to-right.If bit is set then result is returned*/
		mask >>= 1;	/*If bit not set, then shift mask right 1 bit*/
				/*Change result to reflect next lower bit, then return to check new bit*/
	}
	return result;
}
DRIVER(naive_l)
/* **********************************************************/
/* **********************************************************/
/* binary search */
static inline
int ilog2_bs(uint64_t x) {
	int index = 32;
	unsigned int stride = 16;
	if (x < 2) {
		/* XXX Avoid infinite loop. */
		assert(x == 1);
		return 0;
	}
	while (1) {
		uint64_t posn = 1L << index;
		uint64_t mask = ~(posn - 1);
		if (posn > x)
			index -= stride;
		else if (posn < (x & mask))
			index += stride;
		else
			return index;
		assert(stride > 0);
		stride >>= 1;
	}
}
DRIVER(bs)

/*
 * Description of a particular implementation
 * in both standalone and driver form.
 * The divisor is for scaling the number of
 * calls so that terrible implementations don't
 * take forever.
 */
struct driver {
	char *name;
	int (*f)(uint64_t);
	int (*blockf)(int);
	uint64_t divisor;
};

/* X86-64-specific assembly code. */
#ifdef X86_64

/* naive assembly loop */
static inline
int ilog2_asmloop(uint64_t x)
{
	int result = -1;
	assert(x > 0);
	asm("\n"
	    "._L%=:\n\t"
	        "incl %[result]\n\t"
	        "shrq %[x]\n\t"
	        "jnz ._L%=\n"
            /*
             * XXX x needs to be read-write so it
             * gets clobbered. Adding it to clobbers
             * is not sufficient, because buggy compiler.
             */
	    : [result] "+r" (result), [x] "+r" (x)
	    : 
	    : "cc");
	return result;
}
DRIVER(asmloop)

/* **********************************************************/
/* **********************************************************/
/* x86 bsr instruction */
static inline
int ilog2_x86(uint64_t x)
{
	/* XXX Insert your code here. */
	uint64_t result = -1;	/*Initialize result, Type must match for BSR*/	
        assert(x > 0);		/*Undefined for 0*/
/*Bit Scan Right takes x as an arguement and returns ilog2 in result*/
        asm ("bsr %[x], %[result]"	/*start inline assembly,Bit Scan Right */
		: [result] "=r" (result)	/*Result is output variable, compiler determines register */
		: [x] "mr" (x));		/*x is first(only)arguement, it resides in memory, */
						/*No clobbered registers, flag only for 0 which never occurs so no "cc"*/	
	return result;
}
DRIVER(x86)
/* **********************************************************/
/* **********************************************************/
#endif

/* **********************************************************/
/* **********************************************************/
/* compiler __builtin_clzll() */
static inline
int ilog2_cc(uint64_t x)
{
	/* XXX Insert your code here. */
/* __builtin_clzll(x) takes the input parameter as a number for
 which the the count of leading zero’s is to be determined. 
It returns the count of leading zero’s as expected.*/
	assert (x > 0);		/*Undefined for 0*/
	int result = (63 - __builtin_clzll(x));		/*Start at 63 and subtract leading zeros*/
	return result;
}
DRIVER(cc)
/* **********************************************************/
/* **********************************************************/
 /* List of all drivers to be run. The top one will be used
 * as the oracle during random testing.
 */
struct driver drivers[] = {
    {"ilog2_naive_r", ilog2_naive_r, drive_naive_r, 8},
#ifdef X86_64
    {"ilog2_asmloop", ilog2_asmloop, drive_asmloop, 8},
#endif
    {"ilog2_bs", ilog2_bs, drive_bs, 2},
    {"ilog2_cc", ilog2_cc, drive_cc, 1},
#ifdef X86_64
    {"ilog2_x86", ilog2_x86, drive_x86, 1},
#endif
    {"ilog2_naive_l", ilog2_naive_l, drive_naive_l, 1},
    {0, 0, 0}
};

/*
 * Generate a 64-bit random value in a fairly expensive
 * but probably high-quality way.
 */
static inline
uint64_t random64(void) {
	extern long random();
	uint64_t result = random() >> 16;
	int i;
	for (i = 0; i < 64; i += 16)
		result ^= ((uint64_t) random()) << i;
	return result;
}

/* Fill up the block of random data for later use. */
void init_randoms(void) {
	int i;
	for (i = 0; i < BLOCKSIZE; i++) {
		uint64_t randval = random64();
		if (randval == 0)
			randval = ~0;
		randoms[i] = randval;
	}
}

/* Explicit timeval difference. */
int64_t elapsed_msecs(struct timeval *start,
                      struct timeval *end) {
	int64_t diff = (end->tv_sec - start->tv_sec) * 1000L;
	int64_t endu = end->tv_usec / 1000L;
	int64_t startu = start->tv_usec / 1000L;
	return diff + endu - startu;
}

/* Hand-built testcase structure. */
struct testcase {
	uint64_t input;
	int output;
};

/* Test cases built manually. */
struct testcase testcases[] = {
	{0x00000001, 0},
	{0x00000003, 1},
	{0x00000080, 7},
	{0x000000f0, 7},
	{0x00008000, 15},
	{0x0000f000, 15},
	{0x00800000, 23},
	{0x00f00000, 23},
	{0x80000000, 31},
	{0xf0000000, 31},
	{0xff000000, 31},
	{0x000000ff, 7},
	{0x01fe0000, 24},
	{0xea9031e8, 31},
	{0x2e8eb2b2, 29},
	{0x5b8be5b7, 30},
	{0x9b8be5b7, 31},
	{0x5b8be5b700000000L, 62},
	{0x9b8be5b700000000L, 63},
	{~0, 63},
	{0, 0}
};

/*
 * Run both manual and random tests of all functions.
 * Any function that fails a test will be marked
 * invalid and not used subsequently.
 */
void run_tests(struct driver *d) {
	struct testcase *t;
	int nt = 1;
	int i;
	for (t = testcases; t->input != 0; t++) {
		int output = (d->f)(t->input);
		if (output != t->output) {
			printf("%s failed case %d: "
			       "%lx -> %d != %d: abandoning\n",
			       d->name, nt, t->input, output, t->output);
			d->blockf = 0;
			/*
			 * Since the first implementation will be used as
			 * an oracle for random testing, it must be OK.
			 */
			assert (drivers[0].blockf);
			return;
		}
		nt++;
	}
	for (i = 0; i < BLOCKSIZE; i++) {
		uint64_t input = randoms[i];
		int output = (d->f)(input);
		int target = (drivers->f)(input);
		if (output != target) {
			printf("%s failed random test %d: "
			       "%lx -> %d != %d: abandoning\n",
			       d->name, i, input, output, target);
			d->blockf = 0;
			/*
			 * Since the first implementation will be used as
			 * an oracle for random testing, it must be OK.
			 */
			assert (drivers[0].blockf);
			return;
		}
	}
}


/* Run the given driver on the block of randoms. */
void run_driver(struct driver *d, int n) {
	volatile int result = 0;
	struct timeval start, end;
	int64_t elapsed;
	uint64_t real_n = n / d->divisor;
	/* preheat */
	result += d->blockf(5000 / d->divisor);
	assert(gettimeofday(&start, 0) != -1);
	result += d->blockf(real_n);
	assert(gettimeofday(&end, 0) != -1);
	elapsed = elapsed_msecs(&start, &end);
	printf("%s: %g iters in %ld msecs for %0.2f nsecs/iter\n",
	       d->name, (double)real_n * BLOCKSIZE, elapsed,
	       elapsed * d->divisor * 1.0e6 / BLOCKSIZE / n);
}

/* First test every driver, then time it. */
void run_all(int n) {
	struct driver *d;
	for (d = drivers; d->name; d++)
		run_tests(d);
	for (d = drivers; d->name; d++)
		if (d->blockf)
			run_driver(d, n);
}

int main(int argc, char **argv) {
#ifdef SHOW_RANDOMS
	int i;
#endif
	extern long atoi();
	int n = atoi(argv[1]);
	init_randoms();
#ifdef SHOW_RANDOMS
	for (i = 0; i < 10; i++)
		printf("%lx\n", randoms[i]);
#endif
	run_all(n);
	return 0;
}
