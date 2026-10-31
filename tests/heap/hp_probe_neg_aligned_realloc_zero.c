/* TC
 * id:    hp_probe_neg_aligned_realloc_zero
 * area:  heap
 * funcs: _aligned_realloc _aligned_malloc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_realloc; cf. realloc(p,0)
 * note:  PROBING that _aligned_realloc(p,0,a) frees the block and returns NULL without invoking the invalid parameter handler. p must not be touched afterwards.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(zero_size)
{
    volatile size_t a = 16, z = 0; void *p, *q;
    tc_iph_install();
    p = _aligned_malloc(48, a);
    CHECK(p != NULL);
    if (!p) return;
    tc_iph_count = 0;
    q = _aligned_realloc(p, z, a);
    CHECK(q == NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    /* p was freed: do not touch it */
}
int main(void)
{
    tc_begin();
    TC_RUN(zero_size);
    return tc_end();
}
