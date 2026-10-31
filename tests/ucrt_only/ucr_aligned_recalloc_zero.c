/* TC
 * id:    ucr_aligned_recalloc_zero
 * area:  ucrt_only
 * funcs: _aligned_recalloc _aligned_free
 * procs: 
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_recalloc
 * note:  zero-fill of the grown tail moved to ucr_aligned_recalloc_grow_zero_probe
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_n, g_s, g_a = 16;
static int allzero(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i]) return 0; return 1; }
TC_CASE(from_null)
{
    unsigned char *p; g_n = 4; g_s = 8; p = _aligned_recalloc(NULL, g_n, g_s, g_a);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 16, 0); CHECK(allzero(p, 32));
    _aligned_free(p);
}
TC_CASE(grow)
{
    unsigned char *p, *q; size_t i;
    g_n = 4; g_s = 8; p = _aligned_recalloc(NULL, g_n, g_s, g_a); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 32; i++) p[i] = 0xAB;
    g_n = 16; q = _aligned_recalloc(p, g_n, g_s, g_a);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 16, 0);
    for (i = 0; i < 32; i++) if (q[i] != 0xAB) { CHECK(!"old data lost"); break; }
    _aligned_free(q);
}
TC_CASE(product)
{
    unsigned char *p; g_n = 10; g_s = 13; p = _aligned_recalloc(NULL, g_n, g_s, 64);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 64, 0); CHECK(allzero(p, 130)); p[129] = 1;
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(from_null); TC_RUN(grow); TC_RUN(product); return tc_end(); }
