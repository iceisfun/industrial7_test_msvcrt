/* TC
 * id:    hp_neg_aligned_realloc_bad_alignment
 * area:  heap
 * funcs: _aligned_realloc _aligned_malloc _aligned_free
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_realloc: alignment must be a power of 2
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(bad_align_keeps_block)
{
    volatile size_t a = 16, sz = 32, nsz = 64, bad = 3; unsigned char *p, *q; int i;
    CHECK(tc_iph_install());
    p = _aligned_malloc(sz, a);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 1);
    tc_iph_count = 0; errno = 0;
    q = _aligned_realloc(p, nsz, bad);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (q) { _aligned_free(q); return; }
    for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], i + 1);   /* original untouched */
    _aligned_free(p);
}
TC_CASE(null_bad_align)
{
    volatile size_t bad = 5; void *q;
    tc_iph_install();
    tc_iph_count = 0; errno = 0;
    q = _aligned_realloc(NULL, 16, bad);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (q) _aligned_free(q);
}
int main(void)
{
    tc_begin();
    TC_RUN(bad_align_keeps_block); TC_RUN(null_bad_align);
    return tc_end();
}
