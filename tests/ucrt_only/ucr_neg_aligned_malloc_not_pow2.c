/* TC
 * id:    ucr_neg_aligned_malloc_not_pow2
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc (invalid alignment: invalid parameter handler, EINVAL, NULL)
 * note:  alignment must be a power of 2
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_align, g_size;
static void try_align(size_t a)
{
    void *p; int before = tc_iph_count;
    g_align = a; g_size = 64; errno = 0;
    p = _aligned_malloc(g_size, g_align);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
    if (p) _aligned_free(p);
}
TC_CASE(align3)  { try_align(3); }
TC_CASE(align24) { try_align(24); }
TC_CASE(align48) { try_align(48); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(align3); TC_RUN(align24); TC_RUN(align48);
    return tc_end();
}
