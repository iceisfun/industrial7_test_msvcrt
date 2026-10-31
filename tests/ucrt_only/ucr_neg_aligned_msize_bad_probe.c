/* TC
 * id:    ucr_neg_aligned_msize_bad_probe
 * area:  ucrt_only
 * funcs: _aligned_msize _aligned_malloc _aligned_free
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_msize; parameter validation conventions
 * note:  PROBE: _aligned_msize(NULL,16,0) and a non-power-of-two alignment both return (size_t)-1, set errno=EINVAL and invoke the invalid-parameter handler once. Docs say little about the error return.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static void *volatile v_null;
static volatile size_t v_align;

TC_CASE(null_block)
{
    size_t r; tc_iph_count = 0; errno = 0;
    r = _aligned_msize(v_null, 16, 0);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(align_not_pow2)
{
    size_t r; void *p = _aligned_malloc(32, 16); v_align = 3;
    CHECK(p != NULL);
    tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, v_align, 0);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_block); TC_RUN(align_not_pow2);
    return tc_end();
}
