/* TC
 * id:    ucr_neg_aligned_recalloc_overflow_errno_probe
 * area:  ucrt_only
 * funcs: _aligned_recalloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_recalloc
 * note:  PROBE: NULL return, then errno after num*size overflow is ENOMEM (guess).
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_n, g_s;
TC_CASE(errno_overflow)
{
    void *p; g_n = (size_t)-1 / 2; g_s = 4; errno = 0;
    p = _aligned_recalloc(NULL, g_n, g_s, 16);
    CHECK(p == NULL); if (p) _aligned_free(p);
}
TC_CASE(errno_overflow_enomem)
{
    void *p; g_n = (size_t)-1 / 2; g_s = 4; errno = 0;
    p = _aligned_recalloc(NULL, g_n, g_s, 16);
    CHECK_EQ_I(errno, ENOMEM); if (p) _aligned_free(p);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(errno_overflow); TC_RUN(errno_overflow_enomem); return tc_end(); }
