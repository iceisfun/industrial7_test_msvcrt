/* TC
 * id:    ucr_aligned_realloc_size0_probe
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_realloc
 * procs: 
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _aligned_realloc
 * note:  PROBE: _aligned_realloc(p,0,16) frees the block and returns NULL (guess). Block is not touched afterwards.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a = 16, g_s;
TC_CASE(size0)
{
    void *p, *q; g_s = 64; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    g_s = 0; errno = 0; q = _aligned_realloc(p, g_s, g_a);
    CHECK(q == NULL);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(size0); return tc_end(); }
