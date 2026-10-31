/* TC
 * id:    ucr_neg_aligned_recalloc_overflow
 * area:  ucrt_only
 * funcs: _aligned_recalloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_recalloc (NULL on failure; invalid alignment: invalid parameter, EINVAL)
 * note:  only the invalid-alignment path is asserted; the num*size overflow request is in
 *        ucr_neg_aligned_recalloc_overflow_errno_probe (undocumented)
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_n, g_s, g_a;
TC_CASE(bad_align)
{
    void *p; int b = tc_iph_count; g_n = 4; g_s = 8; g_a = 3; errno = 0;
    p = _aligned_recalloc(NULL, g_n, g_s, g_a);
    CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - b, 1);
    if (p) _aligned_free(p);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 1; TC_RUN(bad_align); return tc_end(); }
