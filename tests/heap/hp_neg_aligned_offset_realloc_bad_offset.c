/* TC
 * id:    hp_neg_aligned_offset_realloc_bad_offset
 * area:  heap
 * funcs: _aligned_offset_realloc _aligned_offset_malloc _aligned_free
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_realloc
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(offset_ge_newsize)
{
    volatile size_t a = 16, off = 4, nsz = 8, badoff = 8; unsigned char *p, *q; int i;
    CHECK(tc_iph_install());
    p = _aligned_offset_malloc(32, a, off);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 1);
    tc_iph_count = 0; errno = 0;
    q = _aligned_offset_realloc(p, nsz, a, badoff);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (q) { _aligned_free(q); return; }
    for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], i + 1);
    _aligned_free(p);
}
TC_CASE(bad_alignment)
{
    volatile size_t a = 16, bad = 7, off = 4; unsigned char *p, *q; int i;
    tc_iph_install();
    p = _aligned_offset_malloc(32, a, off);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 1);
    tc_iph_count = 0; errno = 0;
    q = _aligned_offset_realloc(p, 64, bad, off);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (q) { _aligned_free(q); return; }
    for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], i + 1);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(offset_ge_newsize); TC_RUN(bad_alignment);
    return tc_end();
}
