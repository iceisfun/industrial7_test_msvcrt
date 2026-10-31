/* TC
 * id:    ucr_aligned_recalloc_grow_zero_probe
 * area:  ucrt_only
 * funcs: _aligned_recalloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _aligned_recalloc
 * note:  PROBE: when _aligned_recalloc grows a block, the bytes beyond the old size are zero (relies on the CRT knowing the exact old size).
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_n, g_s, g_a = 16;
TC_CASE(grow_zero)
{
    unsigned char *p, *q; size_t i;
    g_n = 4; g_s = 8; p = _aligned_recalloc(NULL, g_n, g_s, g_a); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 32; i++) p[i] = 0xAB;
    g_n = 16; q = _aligned_recalloc(p, g_n, g_s, g_a);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    for (i = 32; i < 128; i++) if (q[i] != 0) { CHECK(!"tail not zero"); break; }
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(grow_zero); return tc_end(); }
