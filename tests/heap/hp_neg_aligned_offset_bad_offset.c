/* TC
 * id:    hp_neg_aligned_offset_bad_offset
 * area:  heap
 * funcs: _aligned_offset_malloc _aligned_free
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc (offset must be less than size, or 0; EINVAL, invalid-parameter handler)
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static volatile size_t v_size, v_off;

TC_CASE(offset_ge_size)
{
    void *p; v_size = 16; v_off = 32; errno = 0; tc_iph_count = 0;
    p = _aligned_offset_malloc(v_size, 16, v_off);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
TC_CASE(bad_alignment)
{
    void *p; v_size = 64; v_off = 0; errno = 0; tc_iph_count = 0;
    p = _aligned_offset_malloc(v_size, 24, v_off);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(offset_ge_size); TC_RUN(bad_alignment); return tc_end(); }
