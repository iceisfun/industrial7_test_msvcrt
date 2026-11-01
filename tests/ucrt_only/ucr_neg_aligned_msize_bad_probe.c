/* TC
 * id:    ucr_neg_aligned_msize_bad_probe
 * area:  ucrt_only
 * funcs: _aligned_msize _aligned_malloc _aligned_free
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _aligned_msize; parameter validation conventions
 * note:  Win10 22H2 x64 (observed, identical in 2 runs): _aligned_msize(NULL,16,0) -> (size_t)-1, EINVAL, one handler call (null_block). A non-power-of-two alignment is NOT
 *        validated (deviates from the MS docs, which require a power of two): no -1, no errno, no handler call; the result is arithmetic on the block: for a block from
 *        _aligned_malloc(32,16): alignment 3 (raised to sizeof(void*)) -> 40, 16 -> 32 (control), 24 -> 24. The values assume x64 HeapAlloc 16-byte block alignment.
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
TC_CASE(align_not_pow2_not_validated)
{
    size_t r; void *p = _aligned_malloc(32, 16);
    CHECK(p != NULL);
    if (!p) return;
    v_align = 16; tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, v_align, 0);
    CHECK_EQ_U(r, 32); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* control: valid alignment */
    v_align = 3; tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, v_align, 0);
    CHECK_EQ_U(r, 40); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* observed: not validated, 3 raised to 8 */
    v_align = 24; tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, v_align, 0);
    CHECK_EQ_U(r, 24); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* observed: used as is */
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_block); TC_RUN(align_not_pow2_not_validated);
    return tc_end();
}
