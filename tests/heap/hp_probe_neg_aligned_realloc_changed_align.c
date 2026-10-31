/* TC
 * id:    hp_probe_neg_aligned_realloc_changed_align
 * area:  heap
 * funcs: _aligned_realloc _aligned_malloc _aligned_free
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_realloc
 * note:  PROBING behaviour when the alignment argument of _aligned_realloc differs from the one used at allocation (expect result aligned to the new value, content kept).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(changed)
{
    volatile size_t a1 = 32, a2 = 64; unsigned char *p, *q; int i;
    tc_iph_install();
    p = _aligned_malloc(32, a1);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(0xA0 + i);
    tc_iph_count = 0;
    q = _aligned_realloc(p, 64, a2);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 64, 0);
    for (i = 0; i < 32; i++) CHECK_EQ_I(q[i], 0xA0 + i);
    CHECK_EQ_I(tc_iph_count, 0);
    _aligned_free(q);
}
int main(void)
{
    tc_begin();
    TC_RUN(changed);
    return tc_end();
}
