/* TC
 * id:    hp_neg_aligned_offset_malloc_bad_offset
 * area:  heap
 * funcs: _aligned_offset_malloc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc: offset must be less than size unless both are zero
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>
#include <malloc.h>

TC_CASE(offset_eq_size)
{
    volatile size_t sz = 16, a = 16, off = 16; void *p;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    p = _aligned_offset_malloc(sz, a, off);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
TC_CASE(offset_gt_size)
{
    volatile size_t sz = 16, a = 16, off = 100; void *p;
    tc_iph_install();
    tc_iph_count = 0; errno = 0;
    p = _aligned_offset_malloc(sz, a, off);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
TC_CASE(offset_size_minus_1_ok)
{
    volatile size_t sz = 16, a = 16, off = 15; char *p;
    tc_iph_install();
    tc_iph_count = 0;
    p = _aligned_offset_malloc(sz, a, off);
    CHECK(p != NULL);
    if (p) { CHECK_EQ_U(((uintptr_t)(p + off)) % 16, 0); _aligned_free(p); }
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(zero_zero_ok)
{
    volatile size_t sz = 0, a = 16, off = 0; void *p;
    tc_iph_install();
    tc_iph_count = 0;
    p = _aligned_offset_malloc(sz, a, off);
    CHECK(p != NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    if (p) _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(offset_eq_size); TC_RUN(offset_gt_size); TC_RUN(offset_size_minus_1_ok); TC_RUN(zero_zero_ok);
    return tc_end();
}
