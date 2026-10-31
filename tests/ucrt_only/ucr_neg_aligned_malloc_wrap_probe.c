/* TC
 * id:    ucr_neg_aligned_malloc_wrap_probe
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc
 * note:  PROBE: SIZE_MAX and SIZE_MAX-2 requests (size+alignment padding wraps in a naive implementation) return NULL. Real CRT should range-check; unverified.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_size;
static void try_req(size_t s)
{
    void *p; g_size = s;
    p = _aligned_malloc(g_size, 16);
    CHECK(p == NULL);
    if (p) _aligned_free(p);
}
TC_CASE(size_max) { try_req((size_t)-1); }
TC_CASE(padding_overflow) { try_req((size_t)-1 - 2); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(size_max); TC_RUN(padding_overflow); return tc_end(); }
