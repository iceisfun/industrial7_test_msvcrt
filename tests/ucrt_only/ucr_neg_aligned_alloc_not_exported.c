/* TC
 * id:    ucr_neg_aligned_alloc_not_exported
 * area:  ucrt_only
 * funcs:
 * procs: aligned_alloc _aligned_malloc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "C11 support in the UCRT / C runtime library": aligned_alloc is not implemented; _aligned_malloc is the MS equivalent
 * note:  Microsoft does not provide C11 aligned_alloc in any CRT, ucrtbase included.
 */
#include "tc.h"

TC_CASE(aligned_alloc_absent) { CHECK(tc_crt_proc("aligned_alloc") == NULL); }
TC_CASE(aligned_malloc_present)
{
#if TC_AT_LEAST(80)
    CHECK(tc_crt_proc("_aligned_malloc") != NULL);
#else
    TC_SKIP("_aligned_malloc export in old msvcrt.dll not certain");
#endif
}
int main(void) { tc_begin(); TC_RUN(aligned_alloc_absent); TC_RUN(aligned_malloc_present); return tc_end(); }
