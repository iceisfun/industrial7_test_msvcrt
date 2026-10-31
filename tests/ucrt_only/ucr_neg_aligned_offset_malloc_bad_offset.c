/* TC
 * id:    ucr_neg_aligned_offset_malloc_bad_offset
 * area:  ucrt_only
 * funcs: _aligned_offset_malloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc (offset must be < size unless both are 0; alignment power of 2)
 * note:  invalid parameter handler, EINVAL, NULL
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a, g_s, g_o;
static void bad(size_t s, size_t a, size_t o)
{
    void *p; int b = tc_iph_count; g_s = s; g_a = a; g_o = o; errno = 0;
    p = _aligned_offset_malloc(g_s, g_a, g_o);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - b, 1);
    if (p) _aligned_free(p);
}
TC_CASE(offset_eq_size)  { bad(16, 16, 16); }
TC_CASE(offset_gt_size)  { bad(16, 16, 100); }
TC_CASE(bad_alignment)   { bad(64, 3, 0); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 1; TC_RUN(offset_eq_size); TC_RUN(offset_gt_size); TC_RUN(bad_alignment); return tc_end(); }
