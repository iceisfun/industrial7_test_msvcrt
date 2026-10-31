/* TC
 * id:    ucr_neg_aligned_realloc_keeps_block_probe
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_realloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_realloc
 * note:  PROBE: after _aligned_realloc fails on invalid alignment the original block is untouched and still valid (standard realloc semantics; not documented for the aligned variant).
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a, g_s;
TC_CASE(keeps)
{
    unsigned char *p, *q; size_t i;
    g_s = 64; g_a = 16; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(i + 1);
    g_s = 256; g_a = 3;
    q = _aligned_realloc(p, g_s, g_a);
    CHECK(q == NULL);
    if (q) { _aligned_free(q); return; }
    for (i = 0; i < 64; i++) if (p[i] != (unsigned char)(i + 1)) { CHECK(!"original content damaged"); break; }
    _aligned_free(p);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(keeps); return tc_end(); }
