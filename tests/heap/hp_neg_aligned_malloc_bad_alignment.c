/* TC
 * id:    hp_neg_aligned_malloc_bad_alignment
 * area:  heap
 * funcs: _aligned_malloc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc: alignment must be an integer power of 2; else EINVAL / invalid parameter
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(bad_alignments)
{
    static const size_t al[] = { 3, 6, 12, 24, 100 };
    int i;
    CHECK(tc_iph_install());
    for (i = 0; i < 5; i++) {
        volatile size_t a = al[i], sz = 64; void *p;
        tc_iph_count = i; errno = 0;
        p = _aligned_malloc(sz, a);
        CHECK(p == NULL);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, i + 1);
        if (p) _aligned_free(p);
    }
}
TC_CASE(size_zero)
{
    volatile size_t a = 3, sz = 0; void *p;
    tc_iph_install();
    tc_iph_count = 0; errno = 0;
    p = _aligned_malloc(sz, a);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(bad_alignments); TC_RUN(size_zero);
    return tc_end();
}
