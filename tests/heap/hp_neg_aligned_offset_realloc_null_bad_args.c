/* TC
 * id:    hp_neg_aligned_offset_realloc_null_bad_args
 * area:  heap
 * funcs: _aligned_offset_realloc _aligned_free
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_realloc (NULL block behaves like _aligned_offset_malloc; alignment power of 2, offset < size; EINVAL)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_64 = 64, v_3 = 3, v_16 = 16, v_0 = 0, v_100 = 100;

static void expect_einval(size_t sz, size_t al, size_t off)
{
    void *r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _aligned_offset_realloc(NULL, sz, al, off);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (r) _aligned_free(r);
}
TC_CASE(bad_alignment) { expect_einval(v_64, v_3, v_0); }
TC_CASE(offset_eq_size) { expect_einval(v_64, v_16, v_64); }
TC_CASE(offset_gt_size) { expect_einval(v_64, v_16, v_100); }
int main(void)
{
    tc_begin();
    TC_RUN(bad_alignment); TC_RUN(offset_eq_size); TC_RUN(offset_gt_size);
    return tc_end();
}
