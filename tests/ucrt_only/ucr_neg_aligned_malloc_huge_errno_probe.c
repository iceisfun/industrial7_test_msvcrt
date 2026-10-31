/* TC
 * id:    ucr_neg_aligned_malloc_huge_errno_probe
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc
 * note:  PROBE: errno and invalid-parameter count after impossible requests. Guess: ENOMEM, no iph call.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_size;
static void try_req(size_t s, int *iph)
{
    void *p; int b = tc_iph_count; g_size = s; errno = 0;
    p = _aligned_malloc(g_size, 16);
    CHECK(p == NULL);
    if (p) _aligned_free(p);
    *iph = tc_iph_count - b;
}
TC_CASE(errno_16tib) { int n; if (sizeof(size_t) > 4) TC_HAZARD("1<<46 aligned_malloc stalls Wine"); try_req(((size_t)1 << (sizeof(size_t) > 4 ? 46 : 31)), &n); CHECK_EQ_I(errno, ENOMEM); }
TC_CASE(errno_size_max) { int n; try_req((size_t)-1, &n); CHECK_EQ_I(errno, ENOMEM); }
TC_CASE(iph_count) { int n; if (sizeof(size_t) > 4) TC_HAZARD("1<<46 aligned_malloc stalls Wine"); try_req(((size_t)1 << (sizeof(size_t) > 4 ? 46 : 31)), &n); CHECK_EQ_I(n, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(errno_16tib); TC_RUN(errno_size_max); TC_RUN(iph_count); return tc_end(); }
