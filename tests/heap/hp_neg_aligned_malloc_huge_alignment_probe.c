/* TC
 * id:    hp_neg_aligned_malloc_huge_alignment_probe
 * area:  heap
 * funcs: _aligned_malloc _aligned_free
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc (alignment must be a power of 2; no upper bound documented)
 * note:  PROBE: valid power-of-two alignments 1<<20, 1<<30 (and 1<<62 on 64-bit) with a small size. The CRT adds
 *        alignment+overhead to the request, so Windows may return NULL (ENOMEM) or a correctly aligned block. Recorded as
 *        TC INFO lines; asserted: no crash and, if non-NULL, the pointer is aligned and freeable.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_sz = 24;

static void info(const char *k, long long v)
{
    char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}
static void probe(const char *tag, size_t al)
{
    void *p; char k[48]; int n = 0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    p = _aligned_malloc(v_sz, al);
    while (tag[n]) { k[n] = tag[n]; n++; }
    k[n] = '_'; k[n+1] = 'n'; k[n+2] = 'u'; k[n+3] = 'l'; k[n+4] = 'l'; k[n+5] = 0; info(k, p == NULL);
    k[n+1] = 'e'; k[n+2] = 'r'; k[n+3] = 'r'; k[n+4] = 0; info(k, errno);
    k[n+1] = 'i'; k[n+2] = 'p'; k[n+3] = 'h'; k[n+4] = 0; info(k, tc_iph_count);
    if (p) { CHECK_EQ_U((uintptr_t)p % al, 0); _aligned_free(p); }
}
TC_CASE(al_1_20) { probe("a20", (size_t)1 << 20); }
TC_CASE(al_1_30) { probe("a30", (size_t)1 << 30); }
#ifdef _WIN64
TC_CASE(al_1_62) { probe("a62", (size_t)1 << 62); }
#endif
int main(void)
{
    tc_begin();
    TC_RUN(al_1_20); TC_RUN(al_1_30);
#ifdef _WIN64
    TC_RUN(al_1_62);
#endif
    return tc_end();
}
