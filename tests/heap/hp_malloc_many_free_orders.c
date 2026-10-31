/* TC
 * id:    hp_malloc_many_free_orders
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

#define N 1000
static unsigned char *blk[N];
static size_t bsz(int i) { return 16 + (size_t)(i % 97) * 3; }
static void allocall(void) { int i; for (i = 0; i < N; i++) { vs = bsz(i); blk[i] = (unsigned char *)malloc(vs); if (blk[i]) fillp(blk[i], bsz(i), (unsigned)i); } }
static int allok(void) { int i; for (i = 0; i < N; i++) if (!blk[i]) return 0; return 1; }

TC_CASE(forward) { int i; allocall(); CHECK(allok()); for (i = 0; i < N; i++) if (blk[i]) { CHECK(chkp(blk[i], bsz(i), (unsigned)i)); free(blk[i]); } }
TC_CASE(reverse) { int i; allocall(); CHECK(allok()); for (i = N - 1; i >= 0; i--) if (blk[i]) { CHECK(chkp(blk[i], bsz(i), (unsigned)i)); free(blk[i]); } }
TC_CASE(interleaved)
{
    int i, bad = 0;
    allocall(); CHECK(allok());
    for (i = 0; i < N; i += 2) { free(blk[i]); blk[i] = NULL; }
    for (i = 0; i < N; i += 2) { vs = 40 + (size_t)(i % 13); blk[i] = (unsigned char *)malloc(vs); if (blk[i]) fillp(blk[i], 40 + (size_t)(i % 13), 200 + (unsigned)i); }
    for (i = 0; i < N; i++) {
        if (!blk[i]) { bad++; continue; }
        if (i & 1) { if (!chkp(blk[i], bsz(i), (unsigned)i)) bad++; }
        else if (!chkp(blk[i], 40 + (size_t)(i % 13), 200 + (unsigned)i)) bad++;
    }
    CHECK_EQ_I(bad, 0);
    for (i = 0; i < N; i++) free(blk[i]);
}
int main(void) { tc_begin(); TC_RUN(forward); TC_RUN(reverse); TC_RUN(interleaved); return tc_end(); }
