/* TC
 * id:    hp_neg_aligned_realloc_huge_keeps_block
 * area:  heap
 * funcs: _aligned_realloc _aligned_malloc _aligned_free _aligned_msize
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_realloc (NULL on failure, original block not freed)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_h1 = SIZE_MAX / 2, v_al = 64, v_sz = 64;

TC_CASE(huge_keeps_block)
{
    unsigned char *p, *r; int i;
    p = (unsigned char *)_aligned_malloc(v_sz, v_al);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(0x30 + i);
    r = (unsigned char *)_aligned_realloc(p, v_h1, v_al);
    CHECK(r == NULL);
    if (r) { _aligned_free(r); return; }
    CHECK_EQ_U((uintptr_t)p % 64, 0);
    for (i = 0; i < 64; i++) CHECK_EQ_I(p[i], 0x30 + i);
    CHECK(_aligned_msize(p, v_al, 0) >= 64);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(huge_keeps_block);
    return tc_end();
}
