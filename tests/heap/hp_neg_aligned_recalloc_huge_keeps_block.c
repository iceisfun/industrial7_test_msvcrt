/* TC
 * id:    hp_neg_aligned_recalloc_huge_keeps_block
 * area:  heap
 * funcs: _aligned_recalloc _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_recalloc (NULL when memory insufficient; original block not freed). errno is probed elsewhere.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_one = 1, v_huge = SIZE_MAX / 2, v_32 = 32, v_16 = 16;

TC_CASE(huge_keeps_block)
{
    unsigned char *p, *r; int i;
    p = (unsigned char *)_aligned_malloc(v_32, v_32);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(0x70 + i);
    errno = 0;
    r = (unsigned char *)_aligned_recalloc(p, v_one, v_huge, v_32);
    CHECK(r == NULL);
    if (r) { _aligned_free(r); return; }
    CHECK_EQ_U((uintptr_t)p % 32, 0);
    for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], 0x70 + i);
    _aligned_free(p);
}
TC_CASE(null_huge)
{
    void *r;
    errno = 0;
    r = _aligned_recalloc(NULL, v_one, v_huge, v_16);
    CHECK(r == NULL);
    if (r) _aligned_free(r);
}
int main(void)
{
    tc_begin();
    TC_RUN(huge_keeps_block); TC_RUN(null_huge);
    return tc_end();
}
