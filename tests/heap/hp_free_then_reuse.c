/* TC
 * id:    hp_free_then_reuse
 * area:  heap
 * funcs: malloc free calloc
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static volatile size_t v_n = 4096;

TC_CASE(many_cycles)
{
    int i, fails = 0;
    for (i = 0; i < 10000; i++) {
        unsigned char *p = (unsigned char *)malloc(v_n);
        if (!p) { fails++; continue; }
        p[0] = 1; p[4095] = 2;
        free(p);
    }
    CHECK_EQ_I(fails, 0);
}
TC_CASE(calloc_after_dirty_free)
{
    int round, nz = 0, nul = 0;
    for (round = 0; round < 20; round++) {
        size_t i; unsigned char *p = (unsigned char *)malloc(v_n);
        if (!p) { nul++; continue; }
        memset(p, 0xAA, v_n);
        free(p);
        p = (unsigned char *)calloc(1, v_n);
        if (!p) { nul++; continue; }
        for (i = 0; i < v_n; i++) if (p[i]) { nz++; break; }
        memset(p, 0xBB, v_n);
        free(p);
    }
    CHECK_EQ_I(nul, 0);
    CHECK_EQ_I(nz, 0);
}
int main(void) { tc_begin(); TC_RUN(many_cycles); TC_RUN(calloc_after_dirty_free); return tc_end(); }
