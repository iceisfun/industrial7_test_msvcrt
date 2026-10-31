/* TC
 * id:    ucr_aligned_malloc_zero_probe
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free _aligned_offset_malloc
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _aligned_malloc
 * note:  PROBE: alignment 0 passes the power-of-2 check (0&-1==0) and is bumped to pointer size; size 0 gives a unique non-NULL pointer; offset==size==0 is allowed. All unverified guesses.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a, g_s;
TC_CASE(align0) { void *p; g_s = 100; g_a = 0; tc_iph_install(); p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (p) _aligned_free(p); }
TC_CASE(size0)  { void *p; g_s = 0; g_a = 16; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (p) { CHECK_EQ_U((uintptr_t)p % 16, 0); _aligned_free(p); } }
TC_CASE(offset_size_both0) { void *p; g_s = 0; g_a = 16; p = _aligned_offset_malloc(g_s, g_a, 0); CHECK(p != NULL); if (p) _aligned_free(p); }
int main(void) { tc_begin(); TC_RUN(align0); TC_RUN(size0); TC_RUN(offset_size_both0); return tc_end(); }
