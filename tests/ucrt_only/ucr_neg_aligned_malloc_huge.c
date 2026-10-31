/* TC
 * id:    ucr_neg_aligned_malloc_huge
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc (returns NULL on failure)
 * note:  impossible request (beyond address space) must fail with NULL; errno not asserted here.
 *        SIZE_MAX-style requests (size+padding wraps) moved to ucr_neg_aligned_malloc_wrap_probe
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_align, g_size;
static void try_req(size_t s, size_t a)
{
    void *p; g_size = s; g_align = a;
    p = _aligned_malloc(g_size, g_align);
    CHECK(p == NULL);
    if (p) _aligned_free(p);
}
TC_CASE(size_16tib)
{
#ifdef _WIN64
    TC_HAZARD("1<<46 aligned_malloc stalls Wine");
    try_req((size_t)1 << 46, 16);
#else
    try_req((size_t)0xF0000000u, 16);
#endif
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(size_16tib); return tc_end(); }
